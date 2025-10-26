#include "aes128_cbc.hpp"

#include <string.h>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/conf.h>

// Free OpenSSL instance.
void free_openssl
(
    EVP_CIPHER_CTX* context
)
{
    if (context != nullptr) EVP_CIPHER_CTX_free(context); // Free the EVP Cipher context if provided.
    CONF_modules_free(); // Free the configuration modules.
    EVP_cleanup();       // Free EVP.
}

// Encrypt data using AES-128 in CBC mode.
int encrypt_aes128_cbc
(
    const unsigned char* input,
    const int input_length,
    const unsigned char* encryption_key,
    unsigned char* cipher,
    const unsigned char* iv
)
{
    if (!input)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The input provided for AES-128 CBC encryption is not valid!");
    }

    if (input_length < 1)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The input length provided for AES-128 CBC encryption is not valid!");
    }

    if (!encryption_key)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The encryption key provided for AES-128 CBC encryption is not valid!");
    }

    if (!cipher)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The cipher provided for AES-128 CBC encryption is not valid!");
    }

    if (!iv)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The IV provided for AES-128 CBC encryption is not valid!");
    }

    int cipher_length = 0;
    int length = 0;

    // Create the encryption context.
    EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();

    if (!context)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The creation of the EVP Cipher context failed for AES-128 CBC encryption!");
    }

    // Initialize CBC encryption.
    const bool initialization = EVP_EncryptInit_ex(context, EVP_aes_128_cbc(), nullptr, encryption_key, iv);

    if (!initialization)
    {
        free_openssl(context);
        throw std::runtime_error("The initialization of the AES-128 CBC encryption failed!");
    }

    // Do the encryption.
    const bool update = EVP_EncryptUpdate(context, cipher, &length, input, input_length);

    if (!update)
    {
        free_openssl(context);
        throw std::runtime_error("Failed to encrypt some data using AES-128 CBC!");
    }

    cipher_length += length;
    const bool finalization = EVP_EncryptFinal_ex(context, cipher + length, &length); // Add paddings.

    if (!finalization)
    {
        free_openssl(context);
        throw std::runtime_error("AES-128 CBC encryption finalization failed!");
    }

    cipher_length += length;
    EVP_CIPHER_CTX_free(context); // Free the context once the encryption has finished.

    return cipher_length;
}

// Decrypt data using AES-128 in CBC mode.
int decrypt_aes128_cbc
(
    const unsigned char* cipher,
    const int cipher_length,
    const unsigned char* encryption_key,
    unsigned char* output,
    const unsigned char* iv
)
{
    if (!cipher)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The cipher provided for the AES-128 CBC decryption is not valid!");
    }

    if (!cipher_length)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The cipher length provided for the AES-128 CBC decryption is not valid!");
    }

    if (!encryption_key)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The encryption key provided for the AES-128 CBC decryption is not valid!");
    }

    if (!iv)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The IV provided for the AES-128 CBC decryption is not valid!");
    }

    int input_length = 0;
    int length = 0;

    // Create a new decryption context.
    EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();

    if (!context)
    {
        free_openssl(nullptr);
        throw std::runtime_error("The creation of the EVP Cipher context failed!");
    }

    // Initialize CBC decryption.
    const bool initialization = EVP_DecryptInit_ex(context, EVP_aes_128_cbc(), nullptr, encryption_key, iv);

    if (!initialization)
    {
        free_openssl(context);
        throw std::runtime_error("The initialization of the AES-128 decryption failed!");
    }

    // Do the decryption.
    const bool update = EVP_DecryptUpdate(context, output, &length, cipher, cipher_length);

    if (!update)
    {
        free_openssl(context);
        throw std::runtime_error("Failed to decrypt some data using AES-128!");
    }

    input_length += length;
    const bool finalization = EVP_DecryptFinal_ex(context, output + length, &length); // Remove the paddings.

    if (!finalization)
    {
        free_openssl(context);
        throw std::runtime_error("AES-128 decryption finalization failed!");
    }

    input_length += length;
    EVP_CIPHER_CTX_free(context); // Free the context once the decryption has finished.

    return input_length;
}
