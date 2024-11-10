
#ifndef _SFG_NETWORKING_H
#define _SFG_NETWORKING_H
#include <stdio.h>
#include <curl/curl.h>

CURL* curl;
CURLcode res;

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

size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    // Print received data to stdout
    fwrite(ptr, size, nmemb, stdout);
    return size * nmemb;
}

void NTW_GetPlayerScore(char* clientId, char* levelPack, uint8_t levelNumber)
{
    if (curl)
    {
        char url[MAX_URL_SIZE];
        sprintf(url, "url/%s/%s/%s", clientId, levelPack, levelNumber);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}






#endif