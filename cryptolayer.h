#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string.h>
#include <stdint.h>

void encrypt(char* in, char* out, unsigned char* key);
void decrypt(char* in, char* out, unsigned char* key);
void base64Encode(const unsigned char* buffer, size_t length, char** b64text);
void base64Decode(char* b64message, unsigned char** buffer, size_t* length);
size_t calcDecodeLength(const char* b64message);
int isB64Char(char c);
void sanitizeB64String(char* buffer);
