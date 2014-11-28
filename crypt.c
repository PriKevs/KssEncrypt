#include "kss.h"
#include "filesize.h"
#include "libcrypt/aes.h"
#include "libcrypt/md5.h"

int Encrypt(char *fileopen, char *filesave, unsigned char *key, int keylen)
{
    FILE *fo, *fs;
    unsigned char *data;
    unsigned char digest[16];
    size_t size, msize, fillsize, crypt;
    int i, add;
    md5_context mdf;
    aes_context aes;

    /*Use time as seed*/
    srand(time(NULL));

    fo = fopen(fileopen, "rb");
    if (fo == NULL){
        perror(fileopen);
        return -1;
    }    

    /*Caculate the chars of file and add rand() numbers*/
    add = 0 + rand() % 16;
    size = Getfilesize(fo);
    if (size == 0){
        printf("Empty file.\n");
        return -1;
    }
    msize = (((size + add)/16) + 2)*16;
    fillsize = msize - size;
    data = (unsigned char *)malloc(msize);
    fread(data, 1, size, fo);

    fclose(fo);
    
    /*Get the MD5 hash*/
    memmove(data + fillsize, data, size);
    data[16] = (unsigned char)(fillsize - 16);
    for(i = 17; i < fillsize; i++)
        *(data + i) = rand() % 255;
    md5_starts(&mdf);
    md5_update(&mdf, data + 16, msize - 16);
    md5_finish(&mdf, digest);
    memcpy(data, digest, 16);

    /*AES encryption begin*/
    aes_set_key(&aes, key, keylen * 8);
    crypt = 0;
    while(crypt < msize){
        aes_encrypt(&aes, data + crypt, data + crypt);
        crypt += 16; 
    }

    fs = fopen(filesave, "wb");
    if (fs == NULL){
        perror(filesave);
        free(data);
        return -1;
    }
    fwrite(data, 1, msize, fs);
    /*Add some Invalid chars at the end*/
    fwrite(data, 1, 0 + rand() % 16, fs);
    fclose(fs);
    free(data);
    return 0;
}

int Decrypt(char *fileopen, char *filesave, unsigned char *key, int keylen)
{
    FILE *fo, *fs;
    unsigned char *data;
    unsigned char fdigest[16], digest[16];
    size_t size, msize, fsize, fillsize, crypt;
    int i;
    md5_context mdf;
    aes_context aes;

    fo = fopen(fileopen, "rb");
    if (fo == NULL){
        perror(fileopen);
        return -1;
    }    

    /*Get size of file and judge and minimum length of file is 32*/
    size = Getfilesize(fo);
    msize = (size/16)*16;
    if (msize <= 32){
        printf("Invalid file or Data damaged.\n");
        return -1;
    }
    data = (unsigned char *)malloc(msize);
    fread(data, 1, msize, fo);
    fclose(fo); 

    /*AES decryption*/
    aes_set_key(&aes, key, keylen * 8);
    crypt = 0; 
    while(crypt < msize){
        aes_decrypt(&aes, data + crypt, data + crypt);
        crypt += 16;
    }

    memcpy(fdigest, data, 16);
    
    /*Get MD5 hash*/
    md5_starts(&mdf);
    md5_update(&mdf, data + 16, msize - 16);
    md5_finish(&mdf, digest);
    
    /*Compare MD5 now and before*/
    if (memcmp(fdigest, digest, 16) != 0){
        free(data);
        printf("Invalid PASSWD or Data damaged.\n");
        return -1;
    }

    /*Reading data...*/
    fillsize = data[16] + 16;

    fs = fopen(filesave, "wb");
    if (fs == NULL){
        perror(filesave);
        free(data);
        return -1;
    }
    fwrite(data + fillsize, 1, msize - fillsize, fs);
    fclose(fs);
    free(data);
    return 0; 
}
