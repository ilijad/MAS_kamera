#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "aes.h"
#include "ff.h"

int len;


int xcrypt_ctr(const char* xcrypt);

int encrypt_ctr(void) {
    return xcrypt_ctr("encrypt");
}

int decrypt_ctr(void) {
    return xcrypt_ctr("decrypt");
}

u8 crypt[3*640*480] __attribute__ ((aligned(32))) = {0};

int xcrypt_ctr(const char* xcrypt) {

    // key
    uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

    // iv
    uint8_t iv[16] = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
    
    struct AES_ctx ctx;
    UINT read;

    char pic[32] = "photo.yuv";
    
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, crypt, len);
    
    FIL outFile;
    f_open(&outFile, pic, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

    f_write(&outFile,(void *)crypt, len, &read);
    f_close(&outFile);

    return 0;

}

int encrypt(void) {

	char pic[32] = "photo.yuv";
    FIL file;
    UINT bytes_read;

    f_open(&file, pic, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    len = file_size(&file);
    printf("%d \r\n", file_size(&file));
    f_read(&file, (void *) crypt, len, &bytes_read);
    f_close(&file);
    
    int exit;
    exit = encrypt_ctr();
    return exit;
}
