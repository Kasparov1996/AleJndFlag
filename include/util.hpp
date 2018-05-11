#ifndef UTIL_HPP
#define UTIL_HPP

namespace util
{
    std::size_t callback(const char *in, std::size_t size, std::size_t num, std::string *out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
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
}

#endif
