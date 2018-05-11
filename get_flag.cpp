#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "services.h"
#include "util.hpp"

int main(int argc, char **argv)
{
    Json::Value jsonData;
    Json::Reader jsonReader;

    uid_t uid = getuid();

    int service_score = 0;

    for (int i = 0; i < (sizeof(service)/sizeof(service[0])); i++)
    {
        if (uid == service[i].uid)
            service_score = service[i].score;
    }

    std::string data = util::getServiceFlag(uid, service_score);
    std::cout << data << std::endl;

    exit(EXIT_SUCCESS);
}
