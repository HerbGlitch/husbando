#include "provider.h"

#include "core/provider.h"
#include "core/show.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <arc/std/errno.h>
#include <arc/std/string.h>
#include <arc/std/vector.h>

//private type to store things like privider ids or urls for resolution
typedef struct HUSBANDO_CoreProviderData {
    ARC_Vector *resolutionUrls;
} HUSBANDO_CoreProviderData;

void HUSBANDO_CoreProvider_CreateAllanimeProvider(HUSBANDO_CoreProvider **provider){
    *provider = (HUSBANDO_CoreProvider *)malloc(sizeof(HUSBANDO_CoreProvider));
    (*provider)->searchFn     = HUSBANDO_Allanime_Search;
    (*provider)->getEpisodeFn = HUSBANDO_Allanime_GetEpisode;

    //init the data
    HUSBANDO_CoreProviderData *data = (HUSBANDO_CoreProviderData *)malloc(sizeof(HUSBANDO_CoreProviderData));
    ARC_Vector_Create(&(data->resolutionUrls));

    (*provider)->data = (void *)data;
}

void HUSBANDO_CoreProvider_DestroyAllanimeProvider(HUSBANDO_CoreProvider *provider){
    HUSBANDO_CoreProviderData *data = (HUSBANDO_CoreProviderData *)provider->data;

    for(uint32_t index = 0; index < ARC_Vector_Size(data->resolutionUrls); index++){
        ARC_String_Destroy((ARC_String *)ARC_Vector_Get(data->resolutionUrls, index));
    }
    ARC_Vector_Destroy(data->resolutionUrls);

    free(provider);
}

// this is a private function that takes a curl response and appends it to a string, according to curl size is allways 1 https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
size_t HUSBANDO_Allanime_CurlWriteToString(void *curlData, uint64_t size, uint64_t curlDataSize, ARC_String *string){
    uint64_t newLength = string->length + (size * curlDataSize);
    char *data = (char *)malloc(sizeof(char) * (newLength + 1));

    if(data == NULL){
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(data, string->data, string->length);
    memcpy(data + string->length, curlData, size * curlDataSize);
    data[newLength] = '\0';

    //free the strings data if it exists
    if(string->data != NULL){
        free(string->data);
    }

    string->data   = data;
    string->length = newLength;

    return size * curlDataSize;
}

void HUSBANDO_Allanime_GetCurlResponse(ARC_String **responseString, char *url){
    CURL *curl;
    curl = curl_easy_init();

    if(!curl){
        //TODO: throw and log arc_errno
        *responseString = NULL;
        return;
    }

    //create the string manually cuz right now ARC_String_Create does not support strings with no size. Will fix that hopefully soon
    //TODO: will want to replace this with ARC_String_Create when fixed
    *responseString = (ARC_String *)malloc(sizeof(ARC_String));
    (*responseString)->data   = NULL;
    (*responseString)->length = 0;

    curl_easy_setopt(curl, CURLOPT_REFERER, HUSBANDO_ALLANIME_REFERER);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, HUSBANDO_ALLANIME_USERAGENT);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HUSBANDO_Allanime_CurlWriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, *responseString);
    CURLcode response = curl_easy_perform(curl);

    //TODO: might want curl error code handling
    if(response != CURLE_OK){
        printf("curl code: %d\n", response);
        ARC_String_Destroy(*responseString);
        *responseString = NULL;
    }

    //cleanup
    curl_easy_cleanup(curl);
}

ARC_Vector *HUSBANDO_Allanime_Search(HUSBANDO_CoreProvider *provider, ARC_String *name, ARC_String *language){
    //note: this info can be found in https://github.com/pystardust/ani-cli
    const char *queryCString = "query(        $search: SearchInput        $limit: Int        $page: Int        $translationType: VaildTranslationTypeEnumType        $countryOrigin: VaildCountryOriginEnumType    ) {    shows(        search: $search        limit: $limit        page: $page        translationType: $translationType        countryOrigin: $countryOrigin    ) {        edges {            _id name availableEpisodes __typename       }    }}";
    const char *variablesCString = "{\"search\":{\"allowAdult\":%s,\"allowUnknown\":false,\"query\":\"%s\"},\"limit\":40,\"page\":1,\"translationType\":\"%s\",\"countryOrigin\":\"ALL\"}";

    //get the variables with name in the string
    uint64_t variablesLength = strlen(variablesCString) + strlen("false") + name->length + language->length;
    char variables[variablesLength + 1];
    sprintf(variables, variablesCString, "false", name->data, language->data);
    variables[variablesLength] = '\0';

    //init curl to encode strings
    CURL *curl;
    curl = curl_easy_init();

    //ecode the strings
    char *encodedQuery     = curl_easy_escape(curl, queryCString, 0);
    char *encodedVariables = curl_easy_escape(curl, variables   , 0);

    //cleanup
    curl_easy_cleanup(curl);

    //get the url as a c string
    uint64_t urlLength = strlen(HUSBANDO_ALLANIME_API) + strlen("?query=") + strlen(encodedQuery) + strlen("&variables=") + strlen(encodedVariables);
    char url[urlLength + 1];
    sprintf(url, "%s?query=%s&variables=%s", HUSBANDO_ALLANIME_API, encodedQuery, encodedVariables);
    url[urlLength] = '\0';

    curl_free(encodedQuery);
    curl_free(encodedVariables);

    //call the search function
    ARC_String *curlResponse;
    HUSBANDO_Allanime_GetCurlResponse(&curlResponse, url);

    //NOTE: don't need to check ARC string find functions for errors as curlResponse and cstring will never be NULL
    uint64_t startBracketIndex = ARC_String_FindCStringWithStrlen(curlResponse, "[");
    uint64_t endBracketIndex = ARC_String_FindBackCStringWithStrlen(curlResponse, "]");
    if(startBracketIndex == ~(uint64_t)0 || endBracketIndex == ~(uint64_t)0){
        //could not find array in json of ids
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in HUSBANDO_Allanime_Search(name), couldn't find array in json, so probably malformed data");
        ARC_String_Destroy(curlResponse);
        return NULL;
    }

    if(startBracketIndex >= endBracketIndex){
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in HUSBANDO_Allanime_Search(name), start bracked found after end bracket");
        ARC_String_Destroy(curlResponse);
        return NULL;
    }

    //strip the brakets from the json
    ARC_String *currentString;
    ARC_String_CopySubstring(&currentString, curlResponse, startBracketIndex, endBracketIndex - startBracketIndex - 1);

    ARC_Vector *providerShows;
    ARC_Vector_Create(&providerShows);

    //note the grouping of variables matches the grouping in core/show.h
    HUSBANDO_CoreProviderShow providerShow = {
        NULL, NULL, 0, 0, 0, 0,
        (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 }, (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 }, (ARC_Time){ 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0,
        NULL,
        NULL, NULL, NULL,
        NULL, NULL
    };

    while(currentString != NULL){
        //NOTE: don't need to check ARC string find functions for errors as currentString and cstring will never be NULL
        uint64_t startLabelStringIndex = ARC_String_FindCStringWithStrlen(currentString, "\"");
        uint64_t endLabelStringIndex = ARC_String_FindCStringWithStrlen(currentString, "\":");
        if(startLabelStringIndex == ~(uint64_t)0 || endLabelStringIndex == ~(uint64_t)0){
            break;
        }

        //check if label is like :{ and we want to skip that type of label
        if(currentString->data[endLabelStringIndex + 1] == '{'){
            ARC_String *tempString = currentString;
            ARC_String_CopySubstring(&currentString, tempString, endLabelStringIndex + 2, tempString->length - endLabelStringIndex - 2);
            ARC_String_Destroy(tempString);
            continue;
        }

        //NOTE: don't need to check ARC string find functions for errors as currentString and cstring will never be NULL
        uint64_t startKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, ":");
        uint64_t endKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, ",");
        //this might be the end where there is a } instead of a , so check if that is the case
        if(endKeyStringIndex == ~(uint64_t)0){
            endKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, "}");
        }

        //couldn't find the key so break
        if(startKeyStringIndex == ~(uint64_t)0 || endKeyStringIndex == ~(uint64_t)0){
            break;
        }

        //strip the label
        ARC_String *labelString;
        ARC_String_CopySubstring(&labelString, currentString, startLabelStringIndex, endLabelStringIndex - startLabelStringIndex - 1);

        //strip the key
        ARC_String *keyString;
        ARC_String_CopySubstring(&keyString, currentString, startKeyStringIndex, endKeyStringIndex - startKeyStringIndex - 1);

        //remove the stripped section from the current string
        if(endKeyStringIndex != currentString->length){
            ARC_String *tempString = currentString;
            ARC_String_CopySubstring(&currentString, tempString, endKeyStringIndex, tempString->length - endKeyStringIndex);
            ARC_String_Destroy(tempString);
        }
        else {
            //we hit the end of the string so delete it
            ARC_String_Destroy(currentString);
            currentString = NULL;
        }

        //set the id
        if(ARC_String_EqualsCStringWithStrlen(labelString, "_id")){
            ARC_String *tempString = keyString;
            ARC_String_StripEnds(&keyString, tempString, '"');
            providerShow.providerId = keyString;

            //cleanup
            //NOTE: key does not need to be cleaned up here because it is stored in providerShow, providerShow needs to clean it up
            ARC_String_Destroy(tempString);
            ARC_String_Destroy(labelString);
            continue;
        }

        //set the name
        if(ARC_String_EqualsCStringWithStrlen(labelString, "name")){
            ARC_String *tempString = keyString;
            ARC_String_StripEnds(&keyString, tempString, '"');
            providerShow.name = keyString;

            //cleanup
            //NOTE: key does not need to be cleaned up here because it is stored in providerShow, providerShow needs to clean it up
            ARC_String_Destroy(tempString);
            ARC_String_Destroy(labelString);
            continue;
        }

        if(ARC_String_EqualsCStringWithStrlen(labelString, "sub")){
            providerShow.subTotalEpisodes = (uint32_t)ARC_String_ToUint64_t(keyString);

            //cleanup
            ARC_String_Destroy(labelString);
            ARC_String_Destroy(keyString);
            continue;
        }

        if(ARC_String_EqualsCStringWithStrlen(labelString, "dub")){
            providerShow.dubTotalEpisodes = (uint32_t)ARC_String_ToUint64_t(keyString);

            //cleanup
            ARC_String_Destroy(labelString);
            ARC_String_Destroy(keyString);
            continue;
        }

        //discard raw, IDK what this is
        if(ARC_String_EqualsCStringWithStrlen(labelString, "raw")){
            //cleanup
            ARC_String_Destroy(labelString);
            ARC_String_Destroy(keyString);
            continue;
        }

        //TODO: might want to make sure the type is a show later, for now skip
        //NOTE: this is the last tag so we will store the providerShow details here into the vector
        if(ARC_String_EqualsCStringWithStrlen(labelString, "__typename")){
            //store the complete provider show
            HUSBANDO_CoreProviderShow *completeProviderShow = (HUSBANDO_CoreProviderShow *)malloc(sizeof(HUSBANDO_CoreProviderShow));
            *completeProviderShow = providerShow;
            ARC_String_Copy(&(completeProviderShow->language), language);
            ARC_Vector_Add(providerShows, (void *)completeProviderShow);

            //cleanup
            ARC_String_Destroy(labelString);
            ARC_String_Destroy(keyString);
            continue;
        }

        ARC_String_Destroy(labelString);
        ARC_String_Destroy(keyString);
        break;
    }

    //cleanup
    if(currentString != NULL){
        ARC_String_Destroy(currentString);
    }
    ARC_String_Destroy(curlResponse);

    return providerShows;
}

HUSBANDO_CoreProviderEpisode *HUSBANDO_Allanime_GetEpisode(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderShow *show, uint32_t episodeNumber){
    HUSBANDO_CoreProviderData *providerData = (HUSBANDO_CoreProviderData *)provider->data;

    //note: this info can be found in https://github.com/pystardust/ani-cli
    const char *queryCString = "query ($showId: String!, $translationType: VaildTranslationTypeEnumType!, $episodeString: String!) {    episode(        showId: $showId        translationType: $translationType        episodeString: $episodeString    ) {        episodeString sourceUrls    }}";
    const char *variablesCString = "{\"showId\":\"%s\",\"translationType\":\"%s\",\"episodeString\":\"%d\"}";

    //get the variables with name in the string
    //note: max value of a uint32_t is 4294967295 which is 10 digits in length, so + 10
    uint64_t variablesLength = strlen(variablesCString) + show->providerId->length + show->language->length + 10;
    char variables[variablesLength + 1];
    sprintf(variables, variablesCString, show->providerId->data, show->language->data, episodeNumber);
    variables[variablesLength] = '\0';

    //init curl to encode strings
    CURL *curl;
    curl = curl_easy_init();

    //ecode the strings
    char *encodedQuery     = curl_easy_escape(curl, queryCString, 0);
    char *encodedVariables = curl_easy_escape(curl, variables   , 0);

    //cleanup
    curl_easy_cleanup(curl);

    //get the url as a c string
    uint64_t urlLength = strlen(HUSBANDO_ALLANIME_API) + strlen("?query=") + strlen(encodedQuery) + strlen("&variables=") + strlen(encodedVariables);
    char url[urlLength + 1];
    sprintf(url, "%s?query=%s&variables=%s", HUSBANDO_ALLANIME_API, encodedQuery, encodedVariables);
    url[urlLength] = '\0';

    curl_free(encodedQuery);
    curl_free(encodedVariables);

    //call the search function
    ARC_String *curlResponse;
    HUSBANDO_Allanime_GetCurlResponse(&curlResponse, url);

    //NOTE: don't need to check ARC string find functions for errors as curlResponse and cstring will never be NULL
    uint64_t startBracketIndex = ARC_String_FindCStringWithStrlen(curlResponse, "[");
    uint64_t endBracketIndex = ARC_String_FindBackCStringWithStrlen(curlResponse, "]");
    if(startBracketIndex == ~(uint64_t)0 || endBracketIndex == ~(uint64_t)0){
        //could not find array in json of ids
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in temp(), couldn't find array in json, so probably malformed data");
        return NULL;
    }

    if(startBracketIndex >= endBracketIndex){
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in temp(), start bracked found after end bracket");
        return NULL;
    }

    //strip the brakets from the json
    ARC_String *currentString;
    ARC_String_CopySubstring(&currentString, curlResponse, startBracketIndex, endBracketIndex - startBracketIndex - 1);

    HUSBANDO_CoreProviderEpisode *episode = (HUSBANDO_CoreProviderEpisode *)malloc(sizeof(HUSBANDO_CoreProviderEpisode));
    episode->currentEpisode = episodeNumber;

    while(currentString != NULL){
        //NOTE: don't need to check ARC string find functions for errors as currentString and cstring will never be NULL
        uint64_t startLabelStringIndex = ARC_String_FindCStringWithStrlen(currentString, "\"");
        uint64_t endLabelStringIndex = ARC_String_FindCStringWithStrlen(currentString, "\":");
        if(startLabelStringIndex == ~(uint64_t)0 || endLabelStringIndex == ~(uint64_t)0){
            break;
        }

        //check if label is like :{ and we want to skip that type of label
        if(currentString->data[endLabelStringIndex + 1] == '{'){
            ARC_String *tempString = currentString;
            ARC_String_CopySubstring(&currentString, tempString, endLabelStringIndex + 2, tempString->length - endLabelStringIndex - 2);
            ARC_String_Destroy(tempString);
            continue;
        }

        //NOTE: don't need to check ARC string find functions for errors as currentString and cstring will never be NULL
        uint64_t startKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, ":");
        uint64_t endKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, ",");
        //this might be the end where there is a } instead of a , so check if that is the case
        if(endKeyStringIndex == ~(uint64_t)0){
            endKeyStringIndex = ARC_String_FindCStringWithStrlen(currentString, "}");
        }

        //couldn't find the key so break
        if(startKeyStringIndex == ~(uint64_t)0 || endKeyStringIndex == ~(uint64_t)0){
            break;
        }

        //strip the label
        ARC_String *labelString;
        ARC_String_CopySubstring(&labelString, currentString, startLabelStringIndex, endLabelStringIndex - startLabelStringIndex - 1);

        //strip the key
        ARC_String *keyString;
        ARC_String_CopySubstring(&keyString, currentString, startKeyStringIndex, endKeyStringIndex - startKeyStringIndex - 1);

        //remove the stripped section from the current string
        if(endKeyStringIndex != currentString->length){
            ARC_String *tempString = currentString;
            ARC_String_CopySubstring(&currentString, tempString, endKeyStringIndex, tempString->length - endKeyStringIndex);
            ARC_String_Destroy(tempString);
        }
        else {
            //we hit the end of the string so delete it
            ARC_String_Destroy(currentString);
            currentString = NULL;
        }

        //set the id
        if(ARC_String_EqualsCStringWithStrlen(labelString, "sourceUrl")){
            ARC_String *tempString = keyString;
            ARC_String_StripEnds(&keyString, tempString, '"');

            //two dashes, -- in source url
            if(keyString->length >= 3 && keyString->data[0] == '-' && keyString->data[1] == '-'){
                ARC_String *tempString = keyString;
                ARC_String_CopySubstring(&keyString, tempString, 2, tempString->length - 2);
                ARC_String_Destroy(tempString);

                ARC_String *showId;
                HUSBANDO_Allanime_SubstitueId(&showId, keyString);

                ARC_String_ReplaceMatchingCStringWithStrlen(&showId, "/clock", "/clock.json");

                if(showId->data[0] != '?'){
                    ARC_Vector_Add(providerData->resolutionUrls, (void *)showId);
                }
                else {
                    ARC_String_Destroy(showId);
                }
            }

            //cleanup
            ARC_String_Destroy(tempString);
            ARC_String_Destroy(labelString);
            continue;
        }

        ARC_String_Destroy(labelString);
        ARC_String_Destroy(keyString);
    }

    ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerData->resolutionUrls, 0);

    ARC_String *tempCurlResponse;
    char tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length + 1];
    sprintf(tempUrl, "%s%s", "https://" HUSBANDO_ALLANIME_BASE, providerId->data);
    tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length] = '\0';
    HUSBANDO_Allanime_GetCurlResponse(&tempCurlResponse, tempUrl);

    //strip the url
    ARC_String *episodeUrl;
    uint64_t urlStartIndex = ARC_String_FindCStringWithStrlen(tempCurlResponse, "\"src\":") + strlen("\"src\":");
    ARC_String_CopySubstring(&episodeUrl, tempCurlResponse, urlStartIndex, tempCurlResponse->length - urlStartIndex);

    uint64_t urlEndSize = ARC_String_FindCStringWithStrlen(episodeUrl, "\"");
    ARC_String *tempString = episodeUrl;
    ARC_String_CopySubstring(&episodeUrl, tempString, 0, urlEndSize - 1);
    ARC_String_Destroy(tempString);

    episode->url = episodeUrl;

    ARC_String_Destroy(tempCurlResponse);

    //cleanup
    if(currentString != NULL){
        ARC_String_Destroy(currentString);
    }
    ARC_String_Destroy(curlResponse);

    return episode;
}

//private type used in HUSBANDO_Allanime_SubstitueId for a baisc substitution type
typedef struct HUSBANDO_AllanimeSubstitue {
    char hex[3];
    char substitution;
} HUSBANDO_AllanimeSubstitue;

void HUSBANDO_Allanime_SubstitueId(ARC_String **outputId, ARC_String *inputId){
    //set the substitution rules, based on ani-cli sed "'s/^01$/9/g;s/^08$/0/g;s/^05$/=/g;s/^0a$/2/g;s/^0b$/3/g;s/^0c$/4/g;s/^07$/?/g;s/^00$/8/g;s/^5c$/d/g;s/^0f$/7/g;s/^5e$/f/g;s/^17$/\//g;s/^54$/l/g;s/^09$/1/g;s/^48$/p/g;s/^4f$/w/g;s/^0e$/6/g;s/^5b$/c/g;s/^5d$/e/g;s/^0d$/5/g;s/^53$/k/g;s/^1e$/\&/g;s/^5a$/b/g;s/^59$/a/g;s/^4a$/r/g;s/^4c$/t/g;s/^4e$/v/g;s/^57$/o/g;s/^51$/i/g;'"
    int32_t substitueRulesLength = 29;
    HUSBANDO_AllanimeSubstitue substitueRules[] = {
        {"01", '9'}, {"08", '0'}, {"05", '='}, {"0a", '2'}, {"0b", '3'},
        {"0c", '4'}, {"07", '?'}, {"00", '8'}, {"5c", 'd'}, {"0f", '7'},
        {"5e", 'f'}, {"17", '/'}, {"54", 'l'}, {"09", '1'}, {"48", 'p'},
        {"4f", 'w'}, {"0e", '6'}, {"5b", 'c'}, {"5d", 'e'}, {"0d", '5'},
        {"53", 'k'}, {"1e", '&'}, {"5a", 'b'}, {"59", 'a'}, {"4a", 'r'},
        {"4c", 't'}, {"4e", 'v'}, {"57", 'o'}, {"51", 'i'}
    };

    //create the output string with the substitued size
    *outputId = (ARC_String *)malloc(sizeof(ARC_String));
    (*outputId)->length = (inputId->length / 2);
    (*outputId)->data   = (char*)malloc((*outputId)->length + 1);

    //need an extra value for the end string '\0' for strcmp to work
    char hex[3] = {0};
    for(uint32_t inputIndex = 0, outputIndex = 0; inputIndex < inputId->length; inputIndex += 2, outputIndex++) {
        hex[0] = inputId->data[inputIndex];
        hex[1] = inputId->data[inputIndex + 1];

        //default to '?' if no match is found
        (*outputId)->data[outputIndex] = '?';

        //find matching rule if it exists
        for(int ruleIndex = 0; ruleIndex < substitueRulesLength; ruleIndex++) {
            if(strcmp(hex, substitueRules[ruleIndex].hex) == 0) {
                (*outputId)->data[outputIndex] = substitueRules[ruleIndex].substitution;
                break;
            }
        }
    }

    (*outputId)->data[(*outputId)->length] = '\0';
}
