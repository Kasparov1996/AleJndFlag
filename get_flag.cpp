#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <csignal>
#include <unistd.h>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "services.h"
#include "util.hpp"
#include "sighandler.hpp"

int main(int argc, char **argv)
{
    // signal(SIGSEGV, segfault_handler);
    // signal(SIGABRT, segfault_handler);
    // signal(SIGINT, sigint_handler);

    Json::Value jsonData;
    Json::Reader jsonReader;

    uid_t uid = getuid();
    std::string local_ip = util::getLocalIP();
    std::string scoreboard_data = util::getScoreboardData();

    int service_score = 0;

    for (int i = 0; i < (sizeof(service)/sizeof(service[0])); i++)
    {
        if (uid == service[i].uid)
            service_score = service[i].score;
    }

    std::string data = util::getServiceFlag(uid, local_ip, service_score);
    std::cout << data << std::endl;

    exit(EXIT_SUCCESS);
}
