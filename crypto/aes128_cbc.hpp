#include <openssl/evp.h>
#include <openssl/conf.h>

#ifndef CRYPTO_AES128CBC_HPP
#define CRYPTO_AES128CBC_HPP

void free_openssl
(
    EVP_CIPHER_CTX* context
);

int encrypt_aes128_cbc
(
    const unsigned char* input,
    const int input_length,
    const unsigned char* encryption_key,
    unsigned char* cipher,
    const unsigned char* iv
);

int decrypt_aes128_cbc
(
    const unsigned char* cipher,
    const int cipher_length,
    const unsigned char* encryption_key,
    unsigned char* output,
    const unsigned char* iv
);

#endif
