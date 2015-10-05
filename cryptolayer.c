#include "cryptolayer.h"

void encrypt(char* in, char* out, unsigned char* key)
{
	EVP_CIPHER_CTX ctx;
	unsigned char iv[16] = {0};
	unsigned char *tmp;
	char *tmp2;
	tmp = malloc(strlen(in) * sizeof(char) + 64);
	int outlen1 = 0;
	int outlen2 = 0;
	int len = 0;

	printf("Encrypting %d bytes\n", strlen(in));

	EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
	EVP_EncryptUpdate(&ctx, tmp, &outlen1, in, strlen(in));
	EVP_EncryptFinal(&ctx, tmp + outlen1, &outlen2);

	len = outlen1 + outlen2;
	printf("Encrypted %d bytes\n", len);

	tmp[outlen1 + outlen2] = '\0';

	base64Encode(tmp, len, &tmp2);
	
	strncpy(out, tmp2, (len / 3 + 1)*4);

	//WORKAROUND FOR UNRESOVLED ISSUE: SOME ENTRIES AREN'T PROPER BASE64 AND CAUSE SQL BAD REQUESTS
	//sanitizeB64String(out);

	free(tmp);
}

void decrypt(char* in, char* out, unsigned char* key)
{
	EVP_CIPHER_CTX ctx;
	unsigned char iv[16] = {0};
	unsigned char *tmp;
	size_t *length;
	length = malloc(sizeof(size_t));
	int outlen1 = 0;
	int outlen2 = 0;
	
	base64Decode(in, &tmp, length);

	EVP_DecryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
	EVP_DecryptUpdate(&ctx, out, &outlen1, tmp, *length);
	EVP_DecryptFinal(&ctx, out + outlen1, &outlen2);

	out[outlen1 + outlen2] = '\0';

}

void base64Encode(const unsigned char* buffer, size_t length, char** b64text) 
{ 
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;
}

size_t calcDecodeLength(const char* b64input)
{
	size_t len = strlen(b64input),
	       padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=')
		padding = 2;
	else if (b64input[len-1] == '=')
		padding = 1;

	return (len*3)/4 - padding;
}

void base64Decode(char* b64message, unsigned char** buffer, size_t* length)
{
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); 
	*length = BIO_read(bio, *buffer, strlen(b64message));
	BIO_free_all(bio);
}

int isB64Char(char c)
{
	return (c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57) || c == 43 || c == 47 || c == 61 ;
}

void sanitizeB64String(char* buffer)
{
	int cpt = 0;

	//printf("Sanitizing: %s\n", buffer);

	while(isB64Char(buffer[cpt]))
	{
		cpt ++;
	}

	buffer[cpt] = '\0';
}

