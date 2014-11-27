EncryptKSS: kss.o getkey.o filesize.o crypt.o md5.o aes.o
	gcc -o EncryptKSS kss.o getkey.o filesize.o crypt.o md5.o aes.o
kss.o: kss.h getkey.h crypt.h kss.c
	gcc -c -o kss.o kss.c
getkey.o: kss.h ./libcrypt/md5.h getkey.c
	gcc -c -o getkey.o getkey.c
filesize.o: kss.h filesize.c
	gcc -c -o filesize.o filesize.c
crypt.o: kss.h filesize.h ./libcrypt/aes.h ./libcrypt/md5.h crypt.c
	gcc -c -o crypt.o crypt.c
md5.o: ./libcrypt/md5.h ./libcrypt/md5.c
	gcc -c -o md5.o ./libcrypt/md5.c
aes.o: ./libcrypt/aes.h ./libcrypt/aes.c
	gcc -c -o aes.o ./libcrypt/aes.c

clean:
	rm *.o EncryptKSS
