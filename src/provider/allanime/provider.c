#include "provider.h"
#include "core/provider.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <arc/std/errno.h>
#include <arc/std/string.h>
#include <arc/std/vector.h>

/*
 * @NOTE: this file has a fair ammount of temprorary things and is full of memory leaks
*/

ARC_Vector *providerShows;

void HUSBANDO_CoreProvider_CreateAllanimeProvider(HUSBANDO_CoreProvider **provider){
    *provider = (HUSBANDO_CoreProvider *)malloc(sizeof(HUSBANDO_CoreProvider));
    (*provider)->searchFn = HUSBANDO_Allanime_Search;
}

void HUSBANDO_CoreProvider_DestroyAllanimeProvider(HUSBANDO_CoreProvider *provider){
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
    //TODO: call curl again
//    HUSBANDO_Allanime_GetCurlResponse(&curlResponse, url);

    //NOTE: left for testing
//    ARC_String_CreateWithStrlen(&curlResponse, "DATA: {\"data\":{\"shows\":{\"edges\":[{\"_id\":\"F9D256cuz4v3hsyaD\",\"name\":\"Gamers!\",\"availableEpisodes\":{\"sub\":12,\"dub\":12,\"raw\":0},\"__typename\":\"Show\"}]}}}");

    //NOTE: left for testing
    ARC_String_CreateWithStrlen(&curlResponse, "{\"data\":{\"shows\":{\"edges\":[{\"_id\":\"TeoTrxCsN9Y4inSug\",\"name\":\"Kaguya-sama wa Kokurasetai: First Kiss wa Owaranai\",\"availableEpisodes\":{\"sub\":4,\"dub\":4,\"raw\":0},\"__typename\":\"Show\"},{\"_id\":\"Mk8Z4bqjYq9FNeRDS\",\"name\":\"Kaguya-sama wa Kokurasetai: Ultra Romantic\",\"availableEpisodes\":{\"sub\":13,\"dub\":13,\"raw\":0},\"__typename\":\"Show\"},{\"_id\":\"Bf7fBzgwovaGGkp4G\",\"name\":\"Kaguya-sama wa Kokurasetai: Tensai-tachi no Renai Zunousen\",\"availableEpisodes\":{\"sub\":12,\"dub\":12,\"raw\":0},\"__typename\":\"Show\"},{\"_id\":\"QdXgBiBLvsuRZFkTf\",\"name\":\"Kaguya-sama wa Kokurasetai?: Tensai-tachi no Renai Zunousen 2\",\"availableEpisodes\":{\"sub\":12,\"dub\":12,\"raw\":0},\"__typename\":\"Show\"}]}}}");

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

HUSBANDO_CoreProviderEpisode *HUSBANDO_Allanime_GetEpisode(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderShow *show, uint32_t episode){
    //note: this info can be found in https://github.com/pystardust/ani-cli
    const char *queryCString = "query ($showId: String!, $translationType: VaildTranslationTypeEnumType!, $episodeString: String!) {    episode(        showId: $showId        translationType: $translationType        episodeString: $episodeString    ) {        episodeString sourceUrls    }}";
    const char *variablesCString = "{\"showId\":\"%s\",\"translationType\":\"%s\",\"episodeString\":\"%d\"}";

    //get the variables with name in the string
    //note: max value of a uint32_t is 4294967295 which is 10 digits in length, so + 10
    uint64_t variablesLength = strlen(variablesCString) + show->providerId->length + show->language->length + 10;
    char variables[variablesLength + 1];
    sprintf(variables, variablesCString, show->providerId->data, show->language->data, episode);
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

    //stor ids for later
    ARC_Vector *providerIds;
    ARC_Vector_Create(&providerIds);

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
                    ARC_Vector_Add(providerIds, (void *)showId);
                }
                else {
                    ARC_String_Destroy(showId);
                }
            }

//            //cleanup
//            //NOTE: key does not need to be cleaned up here because it is stored in providerShow, providerShow needs to clean it up
            ARC_String_Destroy(tempString);
            ARC_String_Destroy(labelString);
            continue;
        }

        ARC_String_Destroy(labelString);
        ARC_String_Destroy(keyString);
    }

    for(uint32_t i = 0; i < ARC_Vector_Size(providerIds); i++){
        ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerIds, i);
        printf("ID: %s\n", providerId->data);
    }

    ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerIds, 0);

    ARC_String *tempCurlResponse;
    char tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length + 1];
    sprintf(tempUrl, "%s%s", "https://" HUSBANDO_ALLANIME_BASE, providerId->data);
    tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length] = '\0';
    HUSBANDO_Allanime_GetCurlResponse(&tempCurlResponse, tempUrl);
    printf("\n\n%s\n\n", tempCurlResponse->data);

    //cleanup
    if(currentString != NULL){
        ARC_String_Destroy(currentString);
    }
    ARC_String_Destroy(curlResponse);

    return NULL;
}

void temp1(){
    ARC_String *curlResponse;
    ARC_String_CreateWithStrlen(&curlResponse, "{\"data\":{\"episode\":{\"episodeString\":\"1\",\"sourceUrls\":[{\"sourceUrl\":\"--175948514e4c4f57175b54575b5307515c050f5c0a0c0f0b0f0c0e590a0c0b5b0a0c0e5d0f0a0f0a0f0e0f0d0b5b0a010a010e0b0e5a0e0c0f0a0e0f0e5c0f0b0a000f0e0f0c0e010a010f0d0f0a0f0c0e0b0e0f0e5a0e5e0e000e090a000f0e0e5d0f0e0b010e5e0e0a0b5a0c5a0d0a0d0b0b0e0c5a0f5b0c0b0b0f0a080f0a0e5e0f0a0e590e0b0b5a0c5c0e0f0e090f0b0f5e0e0f0a5a0f0d0e0f0e5a0e0f0a5c0f090e0f0a5c0c5c0e010e5c0f0b0f0c0e0f0f0d0e0b0f0a0e0f0e5e0a0b0b0d0c0f0a5c0d0a0e0b0e000f0d0e0f0e5e0a5a0f0a0e0f0e0d0e5d0e5e0a5c0e000e010a5c0d0c0e0b0e000e0f0e5e0a5c0d5b0f0b0e000e010f0b0f0d0e0b0e000a5c0a0b0b0c0b5d0c0a0f0b0e0c0a0b0b0c0b5e0a080f0a0f5e0f0e0e0b0f0d0f0b0e0c0b5a0d0d0d0b0c0c0a080f0d0f0b0e0c0b5a0a080e0d0e010f080e0b0f0c0b5a0d5e0b0c0b5e0b0c0d5b0d5d0c5e0f080e0f0b0c0c080e000e0a0d5d0e590e5d0c590d5d0c000e5d0e0c0d090c0b0f0a0e0a0b0c0c0b0f0a0e0f0b0c0b5e0f0c0e0a0d5d0c5b0e5d0e0d0b0c0d080b0e0d5e0d090e5c0f0a0e0a0c090d080f0b0e0d0b0c0c080f0e0c590d5d0d0c0e5d0d5e0b0c0e5d0f0e0c590d090b0b0f080c590d5d0c5b0e590e0c0e5a0c080f0e0c590d5d0f0e0b0f0e0c0e5a0b5e0b0f0e0d0b0c0d080f0b0c590d090d0c0b0f0d5e0e5e0b0b0f090e0c0e5a0e0d0b5a0a0c0a590a0c0f0d0f0a0f0c0e0b0e0f0e5a0e0b0f0c0c5e0e0a0a0c0b5b0a0c0f080e5e0e0a0e0b0e010f0d0f0a0f0c0e0b0e0f0e5a0e5e0e010a0c0a590a0c0e0a0e0f0f0a0e0b0a0c0b5b0a0c0b0c0b0e0b0c0b0a0a5a0b0e0b080a5a0b0f0b0c0d0a0b0e0b0b0b5b0b0b0b0a0b5b0b0e0b0e0a000b0e0b0e0b0e0d5b0a0c0f5a1e4a5d5e5d4a5d4a05\",\"priority\":7.7,\"sourceName\":\"Luf-mp4\",\"type\":\"iframe\",\"className\":\"\",\"streamerId\":\"allanime\"},{\"sourceUrl\":\"https://embtaku.pro/streaming.php?id=MTU0MzE1&title=Kaguya-sama+wa+Kokurasetai%3A+Tensai-tachi+no+Renai+Zunousen+%28Dub%29&typesub=SUB&sub=&cover=Y292ZXIva2FndXlhLXNhbWEtd2Eta29rdXJhc2V0YWktdGVuc2FpLXRhY2hpLW5vLXJlbmFpLXp1bm91c2VuLWR1Yi5wbmc=\",\"priority\":4,\"sourceName\":\"Vid-mp4\",\"type\":\"iframe\",\"className\":\"\",\"streamerId\":\"allanime\",\"downloads\":{\"sourceName\":\"Gl\",\"downloadUrl\":\"https://embtaku.pro/download?id=MTU0MzE1&title=Kaguya-sama+wa+Kokurasetai%3A+Tensai-tachi+no+Renai+Zunousen+%28Dub%29&typesub=SUB&sub=&cover=Y292ZXIva2FndXlhLXNhbWEtd2Eta29rdXJhc2V0YWktdGVuc2FpLXRhY2hpLW5vLXJlbmFpLXp1bm91c2VuLWR1Yi5wbmc=&sandbox=allow-forms%20allow-scripts%20allow-same-origin%20allow-downloads\"}},{\"sourceUrl\":\"--175948514e4c4f57175b54575b5307515c050f5c0a0c0f0b0f0c0e590a0c0b5b0a0c0a010f080e5e0e0a0e0b0e010f0d0a010c0c0e080b090e080c0c0f5b0e090f090e010f080e0f0c090c090e5c0f0e0b0a0c090a010e0a0f0b0e0c0a010b0f0a0c0a590a0c0f0d0f0a0f0c0e0b0e0f0e5a0e0b0f0c0c5e0e0a0a0c0b5b0a0c0c0a0f0c0e010f0e0e0c0e010f5d0a0c0a590a0c0e0a0e0f0f0a0e0b0a0c0b5b0a0c0b0c0b0e0b0c0b0a0a5a0b0e0b080a5a0b0f0b0c0d0a0b0e0b0b0b5b0b0b0b0a0b5b0b0e0b0e0a000b0e0b0e0b0e0d5b0a0c0a590a0c0f0a0f0c0e0f0e000f0d0e590e0f0f0a0e5e0e010e000d0a0f5e0f0e0e0b0a0c0b5b0a0c0e0a0f0b0e0c0a0c0a590a0c0e5c0e0b0f5e0a0c0b5b0a0c0e0b0f0e0a5a0a010f080e5e0e0a0e0b0e010f0d0a010c0c0e080b090e080c0c0f5b0e090f090e010f080e0f0c090c090e5c0f0e0b0a0c090a010e0a0f0b0e0c0a010b0f0a0c0f5a\",\"priority\":7,\"sourceName\":\"Sak\",\"type\":\"iframe\",\"className\":\"\",\"streamerId\":\"allanime\"},{\"sourceUrl\":\"--504c4c484b0217174c5757544b165e594b4c0c4b485d5d5c164a4b4e4817174e515c5d574b177a5e0f5e7a425f4f574e597f7f53480c7f175c4d5a1709\",\"priority\":7.9,\"sourceName\":\"Yt-mp4\",\"type\":\"player\",\"className\":\"\",\"streamerId\":\"allanime\"},{\"sourceUrl\":\"https://streamsb.net/embed-8ftnbfwxpr9c.html\",\"priority\":5.5,\"sourceName\":\"Ss-Hls\",\"type\":\"iframe\",\"className\":\"text-danger\",\"streamerId\":\"allanime\",\"downloads\":{\"sourceName\":\"StreamSB\",\"downloadUrl\":\"https://streamsb.net/embed-8ftnbfwxpr9c.html&sandbox=allow-forms%20allow-scripts%20allow-same-origin%20allow-downloads\"}},{\"sourceUrl\":\"https://ok.ru/videoembed/2677984594578\",\"priority\":3.5,\"sourceName\":\"Ok\",\"type\":\"iframe\",\"sandbox\":\"allow-forms allow-scripts allow-same-origin\",\"className\":\"text-info\",\"streamerId\":\"allanime\"},{\"sourceUrl\":\"--175948514e4c4f57175b54575b5307515c050f5c0a0c0f0b0f0c0e590a0c0b5b0a0c0b5d0e0a0b0d0b0b0b0d0b5e0d010e0c0b080e0d0b090b080e0d0b080b0a0b0b0b0e0e0a0e0f0b0a0b5d0b0e0b0d0b5d0b0c0e0f0e080b5d0e0d0b5e0b0e0e0c0b0d0b5e0b080b0a0e0b0e0a0e0b0a0e0f590a0e0b0c0b5e0b0c0b0c0b0e0b080e0f0e0b0b0d0b0c0b0a0b0d0b5d0b5d0e0a0b0b0b090b090e0a0e0d0e080e0c0b080e0c0b0a0b5d0e0c0e080b0a0e0d0e0c0e080a0e0f590a0e0e5a0e0b0e0a0e5e0e0f0a010b5d0e0a0b0d0b0b0b0d0b5e0d010e0c0b080e0d0b090b080e0d0b080b0a0b0b0b0e0e0a0e0f0b0a0b5d0b0e0b0d0b5d0b0c0e0f0e080b5d0e0d0b5e0b0e0e0c0b0d0b5e0b080b0a0e0b0e0a0e0b0e080b0e0b0e0b0f0a000e5b0f0e0e090a0e0f590a0e0a590b0f0b0e0b5d0b0e0f0e0a590b0a0b5d0b0e0f0e0a590b090b0c0b0e0f0e0a590a0c0a590a0c0f0d0f0a0f0c0e0b0e0f0e5a0e0b0f0c0c5e0e0a0a0c0b5b0a0c0d090e5e0f5d0a0c0a590a0c0e0a0e0f0f0a0e0b0a0c0b5b0a0c0b0c0b0e0b0c0b0a0a5a0b0e0b080a5a0b0f0b0c0d0a0b0e0b0b0b5b0b0b0b0a0b5b0b0e0b0e0a000b0e0b0e0b0e0d5b0a0c0a590a0c0f0a0f0c0e0f0e000f0d0e590e0f0f0a0e5e0e010e000d0a0f5e0f0e0e0b0a0c0b5b0a0c0e0a0f0b0e0c0a0c0a590a0c0e5c0e0b0f5e0a0c0b5b0a0c0e0b0f0e0a5a0c0c0e080b090e080c0c0f5b0e090f090e010f080e0f0c090c090e5c0f0e0b0a0c090d010b0f0d010e0a0f0b0e0c0a0c0f5a\",\"priority\":8.5,\"sourceName\":\"Default\",\"type\":\"iframe\",\"className\":\"text-info\",\"streamerId\":\"allanime\"},{\"sourceUrl\":\"--175948514e4c4f57175b54575b5307515c050f5c0a0c0f0b0f0c0e590a0c0b5b0a0c0a010f0d0e5e0f0a0e0b0f0d0a010e0f0e000e5e0e5a0e0b0a010d0d0e5d0e0f0f0c0e0b0e0a0a0e0c0a0e010e0d0f0b0e5a0e0b0e000f0a0f0d0a010c0c0e080b090e080c0c0f5b0e090f090e010f080e0f0c090c090e5c0f0e0b0a0c090d010b0f0d010e0a0f0b0e0c0a000e5a0f0e0b0a0a0c0a590a0c0f0d0f0a0f0c0e0b0e0f0e5a0e0b0f0c0c5e0e0a0a0c0b5b0a0c0d0d0e5d0e0f0f0c0e0b0f0e0e010e5e0e000f0a0a0c0a590a0c0e0a0e0f0f0a0e0b0a0c0b5b0a0c0b0c0b0e0b0c0b0a0a5a0b0e0b080a5a0b0f0b0c0d0a0b0e0b0b0b5b0b0b0b0a0b5b0b0e0b0e0a000b0e0b0e0b0e0d5b0a0c0a590a0c0f0a0f0c0e0f0e000f0d0e590e0f0f0a0e5e0e010e000d0a0f5e0f0e0e0b0a0c0b5b0a0c0e0a0f0b0e0c0a0c0a590a0c0e5c0e0b0f5e0a0c0b5b0a0c0e0b0f0e0a5a0c0c0e080b090e080c0c0f5b0e090f090e010f080e0f0c090c090e5c0f0e0b0a0c090d010b0f0d010e0a0f0b0e0c0a0c0f5a\",\"priority\":7.4,\"sourceName\":\"S-mp4\",\"type\":\"iframe\",\"className\":\"\",\"streamerId\":\"allanime\",\"downloads\":{\"sourceName\":\"S-mp4\",\"downloadUrl\":\"https://blog.allanime.day/apivtwo/clock/download?id=7d2473746a243c2429756f7263752967686f6b6329556e6774636226426965736b636872752944603160447c617169706741416d763241593759627364286b7632242a2475727463676b63744f62243c24556e67746376696f6872242a2462677263243c24343634322b36302b37345236333c33323c3636283636365c242a24626971686a696762243c727473637b\"}}]}}}");

    //NOTE: don't need to check ARC string find functions for errors as curlResponse and cstring will never be NULL
    uint64_t startBracketIndex = ARC_String_FindCStringWithStrlen(curlResponse, "[");
    uint64_t endBracketIndex = ARC_String_FindBackCStringWithStrlen(curlResponse, "]");
    if(startBracketIndex == ~(uint64_t)0 || endBracketIndex == ~(uint64_t)0){
        //could not find array in json of ids
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in temp(), couldn't find array in json, so probably malformed data");
        return;
    }

    if(startBracketIndex >= endBracketIndex){
        arc_errno = ARC_ERRNO_DATA;
        ARC_DEBUG_ERR("in temp(), start bracked found after end bracket");
        return;
    }

    //strip the brakets from the json
    ARC_String *currentString;
    ARC_String_CopySubstring(&currentString, curlResponse, startBracketIndex, endBracketIndex - startBracketIndex - 1);

    //stor ids for later
    ARC_Vector *providerIds;
    ARC_Vector_Create(&providerIds);

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
                    ARC_Vector_Add(providerIds, (void *)showId);
                }
                else {
                    ARC_String_Destroy(showId);
                }
            }

//            //cleanup
//            //NOTE: key does not need to be cleaned up here because it is stored in providerShow, providerShow needs to clean it up
            ARC_String_Destroy(tempString);
            ARC_String_Destroy(labelString);
            continue;
        }

        ARC_String_Destroy(labelString);
        ARC_String_Destroy(keyString);
    }

    for(uint32_t i = 0; i < ARC_Vector_Size(providerIds); i++){
        ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerIds, i);
        printf("ID: %s\n", providerId->data);
    }

    ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerIds, 0);

    ARC_String *tempCurlResponse;
    char tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length + 1];
    sprintf(tempUrl, "%s%s", "https://" HUSBANDO_ALLANIME_BASE, providerId->data);
    tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length] = '\0';
    HUSBANDO_Allanime_GetCurlResponse(&tempCurlResponse, tempUrl);
    printf("\n\n%s\n\n", tempCurlResponse->data);

    //cleanup
    if(currentString != NULL){
        ARC_String_Destroy(currentString);
    }
    ARC_String_Destroy(curlResponse);
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
