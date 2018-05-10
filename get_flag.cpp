#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <csignal>
#include <unistd.h>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "./services.h"
#include "./util.hpp"
#include "./sighandler.hpp"


const std::string url("http://192.168.1.5:8888/scoreboard.json");

int main(int argc, char **argv)
{
    signal(SIGSEGV, segfault_handler);
    signal(SIGINT, sigint_handler);

    uid_t user_id = getuid();
    int service_score = 0;

    for (int i = 0; i < (sizeof(service)/sizeof(service[0])); i++)
    {
        if (user_id == service[i].uid)
            service_score = service[i].score;
    }

    /*
     * TODO: Push to the scoreboard
     */


    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* NOTE: if not in long datatype, segfault */
    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, util::callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        std::cout << "\nGot successful response from " << url << std::endl;

        Json::Value jsonData;
        Json::Reader jsonReader;

        if (jsonReader.parse(*httpData.get(), jsonData))
        {
            std::cout << "Successfully parsed JSON data" << std::endl;
            std::cout << "\nJSON data received:" << std::endl;
            std::cout << jsonData.toStyledString() << std::endl;

            const std::string dateString(jsonData[0]["last_submit"].asString());
            const std::size_t unixTimeMs(jsonData[0]["last_submit"].asUInt64());
            const std::string timeString(jsonData[0]["last_submit"].asString());


            // const std::string dateString(jsonData["date"].asString());
            // const std::size_t unixTimeMs(
            //         jsonData["milliseconds_since_epoch"].asUInt64());
            // const std::string timeString(jsonData["time"].asString());
            //
            std::cout << "Natively parsed:" << std::endl;
            std::cout << "\tDate string: " << dateString << std::endl;
            std::cout << "\tUnix timeMs: " << unixTimeMs << std::endl;
            std::cout << "\tTime string: " << timeString << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Could not parse HTTP data as JSON" << std::endl;
            std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;
            std::cout << "Error: " << jsonReader.getFormattedErrorMessages() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
        std::cout << "HTTP Code: " << httpCode << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
