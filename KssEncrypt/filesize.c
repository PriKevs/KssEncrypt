#include "kss.h"
size_t Getfilesize(FILE *fp)
{
    size_t size;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}
