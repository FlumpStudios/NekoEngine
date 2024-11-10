
#ifndef _SFG_NETWORKING_H
#define _SFG_NETWORKING_H
#include <stdio.h>
#include <curl/curl.h>


CURL* curl;
CURLcode res;
#define HTTP_RESPONSE_BUFFER_SIZE 5000
#define MAX_URL_SIZE 250
#define LEADERBOARD_URL "https://localhost:7229/api/v1/leaderboard"

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

static void Get(char* url, char* responseBuffer)
{   
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    if(responseBuffer)
    {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseBuffer);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }    

    curl_easy_cleanup(curl);
}


void NTW_GetLeaderboards(char* responseBuffer, char* clientId, char* levelPack, uint8_t levelNumber, int skip, int take) {
    char url[MAX_URL_SIZE];
    snprintf(url, sizeof(url), "%s/asstring?levelPack=%s&levelNumber=%i&clientId=%s&skip=%i&take=%i", LEADERBOARD_URL, levelPack , levelNumber, clientId, skip, take);
    Get(url, responseBuffer);
}

void NTW_GetPlayerScore(char* responseBuffer, char* clientId, char* levelPack, uint8_t levelNumber) {        
        char url[MAX_URL_SIZE];
        snprintf(url, sizeof(url), "%s/%s/%s/%u", LEADERBOARD_URL, clientId, levelPack, levelNumber);
        Get(url, responseBuffer);
}



#endif