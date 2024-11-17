
#ifndef _SFG_NETWORKING_H
#define _SFG_NETWORKING_H
#include <stdio.h>
#include <curl/curl.h>

CURL* curl;
CURLcode res;
#define HTTP_RESPONSE_BUFFER_SIZE 5000
#define MAX_URL_SIZE 250
#define MAX_PAYLOAD_SIZE 500

#define LEADERBOARD_URL "https://levelserver.ruyn.co.uk/api/v1/leaderboard"
#define GET_LEADERBOARD_URL "%s/asstring?levelPack=%s&levelNumber=%i&clientId=%s&skip=%i&take=%i"
#define POST_LEADERBOARD_URL "{ \"userName\": \"%s\", \"clientId\" : \"%s\", \"score\" : %i, \"levelPackName\" : \"%s\", \"levelNumber\" : %i }"
void NTW_init()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

void NTW_close()
{
    curl_global_cleanup();
}
#define MAX_SCORE_SIZE 10

static size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t total_size = size * nmemb;  

    if (userdata && total_size < HTTP_RESPONSE_BUFFER_SIZE) {
        memcpy_s(userdata, HTTP_RESPONSE_BUFFER_SIZE, (char*)ptr, total_size);
    }
    else {
        printf("HTTP response size returned was bigger than the allocated buffer\n");
        return 0;  
    }

    return total_size;
}

static void post(char* url, char* data)
{
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return;
    }

    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));

    // TODO: Do properly
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "X-Api-Key: ovOV4mY2jVMMXsjEBkU8VzY7gscieQk9eViDg9G4B3VYwR5+JFihw3LdD74bCYs9");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void postScore(char* userName, char* clientId, char* levelPackName, int score, int levelNumber)
{
    char payload[MAX_PAYLOAD_SIZE] = {0};
    sprintf(&payload, POST_LEADERBOARD_URL, userName, clientId, score, levelPackName, levelNumber);
    
    post(LEADERBOARD_URL, payload);
}

static void Get(char* url, char* responseBuffer)
{   
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    if(responseBuffer)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseBuffer);
    }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "X-Api-Key: ovOV4mY2jVMMXsjEBkU8VzY7gscieQk9eViDg9G4B3VYwR5+JFihw3LdD74bCYs9");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }    

    curl_easy_cleanup(curl);
}


void NTW_GetLeaderboards(char* responseBuffer, char* clientId, char* levelPack, uint8_t levelNumber, int skip, int take) {
    char url[MAX_URL_SIZE];
    snprintf(url, sizeof(url), GET_LEADERBOARD_URL, LEADERBOARD_URL, levelPack , levelNumber, clientId, skip, take);
    Get(url, responseBuffer);
}

void NTW_GetPlayerScore(char* responseBuffer, char* clientId, char* levelPack, uint8_t levelNumber) {        
        char url[MAX_URL_SIZE];
        snprintf(url, sizeof(url), "%s/%s/%s/%u", LEADERBOARD_URL, clientId, levelPack, levelNumber);
        Get(url, responseBuffer);
}
#endif