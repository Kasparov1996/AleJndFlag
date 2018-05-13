#ifndef UTIL_HPP
#define UTIL_HPP

namespace util
{
#ifdef MULTISERVICE
    std::size_t callback(const char *in, std::size_t size, std::size_t num, std::string *out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }

    std::string getServiceFlag(uid_t u, std::string local_ip, int s)
    {
        std::string uid = std::to_string(u);
        const std::string flagsubmit_url(FLAGSUBMIT_URL + uid);

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
#else

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
            if (res->ifa_name == NULL)
                continue;

            if (strncmp(res->ifa_name, INTERFACE, strlen(INTERFACE)) !=0)
                continue;

            if (res->ifa_addr->sa_family == AF_INET)
            {
                freeifaddrs(ifaces);
                return inet_ntoa(((struct sockaddr_in *)res->ifa_addr)->sin_addr);
            }
        }
    }
#endif
}

#endif
