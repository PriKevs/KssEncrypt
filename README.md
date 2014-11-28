EncryptKSS
==========

A file encryption implemented in C

        ————借以此工具向KSS(恺叔叔)致以崇高敬意


USAGE:
-------------------------

	usage:encryptkss [FILENAME] |-d FILENAME [-s FILENAME]

	optional arguments:
		FILENAME		Encrypt file
		-d FILENAME		Decrypt file
		-s FILENAME		New file name (Default ORIGINAL.kss or RM .kss)
 
	Online help: <https://github.com/PrKevince/EncryptKSS>


Encryption Medthod:
-----------------------
* random number
* MD5
* AES-128-ECB
