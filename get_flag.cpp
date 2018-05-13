#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#include "crypto.hpp"
#include "util.hpp"

int main(int argc, char **argv)
{
    byte iv[BLOCK_SIZE];
    byte *key = (byte *) "c3bb356d542b36ca5a352cd3a6276014";
    EVP_add_cipher(EVP_aes_256_cbc());

    crypto::gen_params(iv);

    secure_string ptext = util::getLocalIP();
    secure_string ctext;

    crypto::aes_encrypt(key, iv, ptext, ctext);
    ctext.append("::");
    ctext.append((char *)iv, BLOCK_SIZE);

    OPENSSL_cleanse(iv, BLOCK_SIZE);

    secure_string b64text = crypto::base64_encode(ctext);

    std::cout << b64text;

    exit(EXIT_SUCCESS);
}
