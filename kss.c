#include "kss.h"
#include "getkey.h"
#include "crypt.h"

void menu();

int main()
{
    int choice;
    char fileopen[NAMELEN];
    char filesave[NAMELEN];
    unsigned char passwd[PASSWDLEN];
    unsigned char key[KEYLEN];
again:
    menu();
    scanf("%d", &choice);
    switch(choice){
    case 1:
        printf("Path of file you want to Encrypt:");
        scanf("%s", fileopen);

        printf("Your PASSWD:");
        scanf("%s", passwd);

        printf("Path you want to save file:");
        scanf("%s", filesave);
        
        Getkey(passwd, strlen(passwd), key);
        if (Encrypt(fileopen, filesave, key, KEYLEN) < 0){
            printf("Errors occur while encrypting file.");
            return 0;
        }
        break;
    case 2:
        printf("Path of file you want to Decrypt:");
        scanf("%s", fileopen);

        printf("Your PASSWD:");
        scanf("%s", passwd);

        printf("Path you want to save file:");
        scanf("%s", filesave);

        Getkey(passwd, strlen(passwd), key);
        if (Encrypt(fileopen, filesave, key, KEYLEN) < 0){
            printf("Errors occur while decrypting file.");
            return 0;
        }
        break;
    }
    default:
        printf("Invalid choice.\n");        
        goto again;
    return 0;
}

void menu()
{
    printf("1.Encrypt\n");
    printf("2.Decrypt\n");
}
