#ifndef UTIL_HPP
#define UTIL_HPP

namespace util
{
    /*
     * NOTE: This should be the place where all the utilities subroutine is implemented
     *
     * TODO: How can we make use of this callback?
     */

    std::size_t callback(const char *in, std::size_t size, std::size_t num, std::string *out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }

    const std::string scoreboard_url("http://192.168.1.5:8888/scoreboard.json");

    extern "C"
    {
        #include <sys/socket.h>
        #include <arpa/inet.h>
        #include <ifaddrs.h>

        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        char *getLocalIP();
    };

    char *getLocalIP()
    {
        struct ifaddrs *ifaces,  *res;
        int ret;

        if ((ret = getifaddrs(&ifaces)) != 0)
        {
            fprintf(stderr, "Failed to get interface.\n");
            exit(EXIT_FAILURE);
        }

        for (res = ifaces; res != NULL; res = res->ifa_next)
        {
            if (strncmp(res->ifa_name, "ens33", 5) !=0)
                continue;

            if (res->ifa_addr->sa_family == AF_INET)
            {
                freeifaddrs(ifaces);
                return inet_ntoa(((struct sockaddr_in *)res->ifa_addr)->sin_addr);
            }
        }
    }

    std::string getServiceFlag(uid_t u, std::string local_ip, int s)
    {
        std::string uid = std::to_string(u);
        std::string score = std::to_string(s);
        const std::string flagsubmit_url("http://192.168.1.5:8888/flag/submit/" + uid + "/" + local_ip + "/" + score);

        CURL* curl = curl_easy_init();
        CURLcode res;

        curl_easy_setopt(curl, CURLOPT_URL, flagsubmit_url.c_str());
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        long httpCode(0);
        std::unique_ptr<std::string> httpData(new std::string());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, util::callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

        if ((res = curl_easy_perform(curl)) != CURLE_OK)
        {
            exit(EXIT_FAILURE);
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if (httpCode == 200)
            return *httpData.get();

        return NULL;
    }

    std::string getScoreboardData()
    {
        CURL* curl = curl_easy_init();
        CURLcode res;

        curl_easy_setopt(curl, CURLOPT_URL, scoreboard_url.c_str());
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

        if ((res = curl_easy_perform(curl)) != CURLE_OK)
        {
            exit(EXIT_FAILURE);
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);

        if (httpCode == 200)
            return *httpData.get();

        return NULL;
    }
}

#endif
