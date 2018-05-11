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

    std::string data = util::getServiceFlag(uid);
    std::cout << data << std::endl;

    exit(EXIT_SUCCESS);
}
