#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "aes.h"

char *in;
unsigned long fileLen;


int xcrypt_ctr(const char* xcrypt);

int encrypt_ctr(void) {
    return xcrypt_ctr("encrypt");
}

int decrypt_ctr(void) {
    return xcrypt_ctr("decrypt");
}

int xcrypt_ctr(const char* xcrypt) {
    
    // key
    uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	
    // iv
    uint8_t iv[16] = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };	
    
    struct AES_ctx ctx;
    
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, in, fileLen);
    
    FILE *outFile;
    outFile = fopen("dekriptirano.jpg", "wb");
    fwrite((char *)in, fileLen, 1, outFile);
    fclose(outFile);

}

int main(void) {
        
    FILE *file = fopen("enk", "rb");
    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    fseek(file, 0, SEEK_SET);
    // printf("fileLen: %d\n", fileLen);
    in = (char *)malloc((fileLen+1)*sizeof(char));
    
    while(!feof(file)) {
        fread(in, fileLen, 1, file);
    }
    
    fclose(file);
    
    int exit;
    exit = decrypt_ctr();
    return exit;
}