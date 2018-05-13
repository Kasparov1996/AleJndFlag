#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#define INTERFACE "ens33"

#ifdef MULTISERVICE
#define FLAGSUBMIT_URL "http://192.168.1.5:8888/flag/submit/"
#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include "services.h"
#else
#include "crypto.hpp"
#endif

#include "util.hpp"

int main(int argc, char **argv)
{
    byte iv[BLOCK_SIZE];
    byte *key = (byte *) "c3bb356d542b36ca5a352cd3a6276014";
    EVP_add_cipher(EVP_aes_256_cbc());

#ifdef MULTISERVICE
    Json::Value jsonData;
    Json::Reader jsonReader;

    uid_t uid = getuid();
    std::string flag = util::getServiceFlag(uid);
    std::cout << flag;
#else

    crypto::gen_params(iv);

    secure_string ptext = util::getLocalIP();
    secure_string ctext;

    crypto::aes_encrypt(key, iv, ptext, ctext);
    ctext.append("::");
    ctext.append((char *)iv, BLOCK_SIZE);

    OPENSSL_cleanse(iv, BLOCK_SIZE);

    secure_string b64text = crypto::base64_encode(ctext);

    std::cout << b64text;
#endif

    exit(EXIT_SUCCESS);
}
