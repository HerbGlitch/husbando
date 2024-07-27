#include "provider.h"

#include "core/episode.h"
#include "core/provider.h"
#include "core/show.h"
#include <json_object.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
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

    free(provider->data);

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

    //create the return vector
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

    json_object *root = json_tokener_parse(curlResponse->data);
    if(!root){
        return providerShows;
    }

    json_object *data = json_object_object_get(root, "data");
    if(json_object_get_type(data) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return providerShows;
    }

    json_object *shows = json_object_object_get(data, "shows");
    if(json_object_get_type(shows) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return providerShows;
    }

    json_object *edges = json_object_object_get(shows, "edges");
    if(json_object_get_type(shows) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return providerShows;
    }

    uint32_t edgesSize = json_object_array_length(edges);
    if(edgesSize == 0){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return providerShows;
    }

    for(uint32_t index = 0; index < edgesSize; index++){
        json_object *edge = json_object_array_get_idx(edges, index);

        json_object *id = json_object_object_get(edge, "_id");
        if(json_object_get_type(id) == json_type_null){
            //TODO: arc_errno stuffs?
            continue;
        }

        json_object *name = json_object_object_get(edge, "name");
        if(json_object_get_type(name) == json_type_null){
            //TODO: arc_errno stuffs?
            continue;
        }

        json_object *availableEpisodes = json_object_object_get(edge, "availableEpisodes");
        if(json_object_get_type(availableEpisodes) == json_type_null){
            //TODO: arc_errno stuffs?
            continue;
        }

        json_object *sub = json_object_object_get(availableEpisodes, "sub");
        if(json_object_get_type(sub) == json_type_null){
            //TODO: arc_errno stuffs?
            continue;
        }

        json_object *dub = json_object_object_get(availableEpisodes, "dub");
        if(json_object_get_type(dub) == json_type_null){
            //TODO: arc_errno stuffs?
            continue;
        }

        //all data was found, so create and store the show
        HUSBANDO_CoreProviderShow *completeProviderShow = (HUSBANDO_CoreProviderShow *)malloc(sizeof(HUSBANDO_CoreProviderShow));
        *completeProviderShow = providerShow;

        const char *idCString   = json_object_get_string(id);
        const char *nameCString = json_object_get_string(name);

        ARC_String_CreateWithStrlen(&(completeProviderShow->providerId), (char *)idCString);
        ARC_String_CreateWithStrlen(&(completeProviderShow->name)      , (char *)nameCString);

        completeProviderShow->subTotalEpisodes = json_object_get_uint64(sub);
        completeProviderShow->dubTotalEpisodes = json_object_get_uint64(dub);

        ARC_String_Copy(&(completeProviderShow->language), language);
        ARC_Vector_Add(providerShows, (void *)completeProviderShow);
    }

    //cleanup
    json_object_put(root);

    ARC_String_Destroy(curlResponse);

    return providerShows;
}

HUSBANDO_CoreProviderEpisode *HUSBANDO_Allanime_GetEpisode(HUSBANDO_CoreProvider *provider, HUSBANDO_CoreProviderShow *show, uint32_t episodeNumber){
    HUSBANDO_CoreProviderData *providerData = (HUSBANDO_CoreProviderData *)provider->data;

    //note: this info can be found in https://github.com/pystardust/ani-cli
    const char *queryCString = "query ($showId: String!, $translationType: VaildTranslationTypeEnumType!, $episodeString: String!) {    episode(        showId: $showId        translationType: $translationType        episodeString: $episodeString    ) {        episodeString sourceUrls    }}";
    const char *variablesCString = "{\"showId\":\"%s\",\"translationType\":\"%s\",\"episodeString\":\"%d\"}";

//    ARC_String_Destroy(show->providerId);
//    ARC_String_CreateWithStrlen(&(show->providerId), "KB5XDvwPdtLFEkoQZ");

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

    json_object *root;
    root = json_tokener_parse(curlResponse->data);
    ARC_String_Destroy(curlResponse);
    if(!root){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    json_object *data = json_object_object_get(root, "data");
    if(json_object_get_type(data) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    json_object *episodeJson = json_object_object_get(data, "episode");
    if(json_object_get_type(episodeJson) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    json_object *episodeString = json_object_object_get(episodeJson, "episodeString");
    if(json_object_get_type(episodeString) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    json_object *sourceUrls = json_object_object_get(episodeJson, "sourceUrls");
    if(json_object_get_type(sourceUrls) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    uint32_t sourceUrlsSize = json_object_array_length(sourceUrls);
    if(sourceUrlsSize == 0){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    for(uint32_t index = 0; index < sourceUrlsSize; index++){
        json_object *sourceUrl = json_object_array_get_idx(sourceUrls, index);

        json_object *sourceUrlJson = json_object_object_get(sourceUrl, "sourceUrl");
        if(json_object_get_type(sourceUrlJson) == json_type_null){
            continue;
        }

        json_object *sourceName = json_object_object_get(sourceUrl, "sourceName");
        if(json_object_get_type(sourceName) == json_type_null){
            continue;
        }

        json_object *sourceType = json_object_object_get(sourceUrl, "type");
        if(json_object_get_type(sourceType) == json_type_null){
            continue;
        }

        json_object *streamerId = json_object_object_get(sourceUrl, "streamerId");
        if(json_object_get_type(streamerId) == json_type_null){
            continue;
        }

        //TODO: probably want to do something with sourceName, sourceType, and streamerId

        //TODO: might want to check to see if this has a length
        const char *sourceUrlCString = json_object_get_string(sourceUrlJson);

        ARC_String *sourceUrlString;
        if(sourceUrlCString[0] == '-' && sourceUrlCString[1] == '-'){
            ARC_String_Create(&sourceUrlString, (char *)(sourceUrlCString + 2), strlen(sourceUrlCString) - 2);
        }
        else {
            ARC_String_CreateWithStrlen(&sourceUrlString, (char *)sourceUrlCString);
        }

        //substitue and check provider show id
        ARC_String *showId;
        HUSBANDO_Allanime_SubstitueId(&showId, sourceUrlString);
        ARC_String_ReplaceMatchingCStringWithStrlen(&showId, "/clock", "/clock.json");
        ARC_String_Destroy(sourceUrlString);

        //TODO: might want to handle this a bit better
        if(showId->data[0] != '?'){
            //don't need to free if added to the resolution urls
            ARC_Vector_Add(providerData->resolutionUrls, (void *)showId);
            continue;
        }

        ARC_String_Destroy(showId);
    }

    json_object_put(root);

    //TODO: select the id in a better way
    ARC_String *providerId = (ARC_String *)ARC_Vector_Get(providerData->resolutionUrls, 0);

    //get the video url
    ARC_String *episodeUrl;
    char tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length + 1];
    sprintf(tempUrl, "%s%s", "https://" HUSBANDO_ALLANIME_BASE, providerId->data);
    tempUrl[strlen("https://" HUSBANDO_ALLANIME_BASE) + providerId->length] = '\0';
    HUSBANDO_Allanime_GetCurlResponse(&episodeUrl, tempUrl);

    root = json_tokener_parse(episodeUrl->data);
    ARC_String_Destroy(episodeUrl);
    if(!root){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    json_object *links = json_object_object_get(root, "links");
    if(json_object_get_type(links) == json_type_null){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    uint32_t linksSize = json_object_array_length(links);
    if(linksSize == 0){
        //TODO: arc_errno stuffs?
        json_object_put(root);
        return NULL;
    }

    HUSBANDO_CoreProviderEpisode *episode;
    HUSBANDO_CoreProviderEpisode_CreateEmpty(&episode);

    for(uint32_t index = 0; index < linksSize; index++){
        json_object *sourceLink = json_object_array_get_idx(links, index);

        json_object *sourceLinkLink = json_object_object_get(sourceLink, "link");
        if(json_object_get_type(sourceLinkLink) == json_type_null){
            continue;
        }

        json_object *sourceMp4 = json_object_object_get(sourceLink, "mp4");
        if(json_object_get_type(sourceMp4) == json_type_null){
            continue;
        }

        json_object *sourceResolution = json_object_object_get(sourceLink, "resolutionStr");
        if(json_object_get_type(sourceResolution) == json_type_null){
            continue;
        }

        json_object *sourceSrc = json_object_object_get(sourceLink, "src");
        if(json_object_get_type(sourceSrc) != json_type_null){
            //TODO: do smthn with this?
        }

        const char *episodeUrlCString = json_object_get_string(sourceLinkLink);

        ARC_String *episodeUrl;
        ARC_String_CreateWithStrlen(&episodeUrl, (char *)episodeUrlCString);

        ARC_Vector_Add(episode->urls, (void *)episodeUrl);
    }

    //cleanup
    json_object_put(root);

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
