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

    srand(time(NULL));

    fo = fopen(fileopen, "rb");
    if (fo == NULL){
        perror(fileopen);
        return -1;
    }    

    add = 0 + rand() % 16;
    size = Getfilesize(fo);
    msize = (((size + add)/16) + 2)*16;
    fillsize = msize - size;
    data = (unsigned char *)malloc(msize);
    fread(data, 1, size, fo);

    fclose(fo);
    
    memmove(data + fillsize, data, size);
    data[16] = (unsigned char)(fillsize - 16);
    for(i = 17; i < fillsize; i++)
        *(data + i) = rand() % 255;
    md5_starts(&mdf);
    md5_update(&mdf, data + 16, msize - 16);
    md5_finish(&mdf, digest);
    memcpy(data, digest, 16);

    /*
    printf("encrypt:\n");
    for(i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");
    */

    aes_set_key(&aes, key, keylen * 8);
    crypt = 0;
    while(crypt < msize){
        aes_encrypt(&aes, data + crypt, data + crypt);
        crypt += 16; 
    }

    fs = fopen(filesave, "wb");
    if (fs == NULL){
        perror(filesave);
        return -1;
    }
    fwrite(data, 1, msize, fs);
    fwrite(data, 1, 1 + rand() % 15, fs);
    fclose(fs);
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

    size = Getfilesize(fo);
    if (size < 32){
        printf("Data damaged.\n");
        return -1;
    }
    msize = (size/16)*16;
    data = (unsigned char *)malloc(msize);
    fread(data, 1, msize, fo);
    fclose(fo); 

    aes_set_key(&aes, key, keylen * 8);
    crypt = 0; 
    while(crypt < msize){
        aes_decrypt(&aes, data + crypt, data + crypt);
        crypt += 16;
    }

    memcpy(fdigest, data, 16);
    
    /*
    printf("ffdecrypt:\n");
    for(i = 0; i < 16; i++)
        printf("%02x", fdigest[i]);
      printf("\n");
      printf("\n");
      printf("fillsize=%d\nmsize=%d\n", (int)fillsize, (int)msize);
      printf("data16=%d\n", (int)data[16]);
      for(i = fillsize; i < msize; i++)
         printf("%c", data[i]);
    printf("\n\n");
    */
   
    md5_starts(&mdf);
    md5_update(&mdf, data + 16, msize - 16);
    md5_finish(&mdf, digest);
    
    /*
    printf("decrypt:\n");
    for(i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");
    */
    
    if (memcmp(fdigest, digest, 16) != 0){
        printf("Invalid PASSWD or Data damaged.\n");
        return -1;
    }
    fillsize = data[16] + 16;

    fs = fopen(filesave, "wb");
    if (fs == NULL){
        perror(filesave);
        return -1;
    }
    fwrite(data + fillsize, 1, msize - fillsize, fs);
    fclose(fs);

    return 0; 
}
