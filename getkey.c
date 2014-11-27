#include "kss.h"
#include "libcrypt/md5.h"
int Getkey(unsigned char *passwd, int passwd_len, unsigned char *key)
{
    md5_context context ctx;
    md5_starts(&ctx);
    md5_update(&ctx, passwd, passwd_len);
    md5_finish(&ctx, key);
    return 0;
}
