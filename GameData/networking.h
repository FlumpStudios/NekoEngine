
#ifndef _SFG_NETWORKING_H
#define _SFG_NETWORKING_H
#include <stdio.h>
#include <curl/curl.h>

CURL* curl;
CURLcode res;

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

void NTW_GetPlayerScore(char* clientId, char* levelPack, uint8_t levelNumber)
{
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, LEADERBOARD_URL);

        const char* post_data = "name=example&age=30";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    
    }
}


size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    // Print received data to stdout
    fwrite(ptr, size, nmemb, stdout);
    return size * nmemb;
}



#endif