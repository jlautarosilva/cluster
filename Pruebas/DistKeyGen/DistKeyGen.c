/*
* Nombre      : DistKeyGen
* Autor       : Juan Lautaro Silva Ortíz, Rodrigo Alexis Venegas Muñoz
* fecha       : 03-02-2012
* Descripción : Prueba que consiste en generar 80.000 claves RSA con encriptación de 2048bits
*		y es distribuida en 40 procesos fork's simultáneamente.
*
* Require     : gcc compiler and OpenSSL library
* Ejecutar    : g++ DistKeyGen.C -lcrypto -o DistKeyGen
*
*
****************************************************************************
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
* USA
*
****************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <openssl/rsa.h>
//#include <iostream.h>
#include <string.h>
#include <time.h>

void keyGenetator(int fileIndex, int keyLength);

void keyGenerator(int fileIndex, int keyLength)
{
	int err=0;
	int size = 0;
	char * buffer = 0;
	char * buffer2 = 0;

	FILE * publicKeyFile = NULL;
	FILE * privateKeyFile = NULL;

	char fileName[256];
	char publicKeyFileName[512];
	char privateKeyFileName[512];

	for(int i = 0; i< 1000; i++)
   	{
		if( i < 10)
		{
			sprintf(publicKeyFileName, "%d00%d-PublicKey", fileIndex, i);
			sprintf(privateKeyFileName, "%d00%d-PrivateKey", fileIndex,i);
		} 
		else if(i >= 10 && i < 100)
		{
			sprintf(publicKeyFileName, "%d0%d-PublicKey", fileIndex, i);
			sprintf(privateKeyFileName, "%d0%d-PrivateKey", fileIndex,i);
		}
		else
		{
			sprintf(publicKeyFileName, "%d%d-PublicKey", fileIndex, i);
			sprintf(privateKeyFileName, "%d%d-PrivateKey", fileIndex,i);
		}

		RSA * rsa;
		rsa = RSA_generate_key(keyLength, RSA_F4, NULL, (char*)stdout);
		
		int lenE = 0; 
		unsigned char * tmp = new unsigned char[keyLength];
		unsigned char *p;
		
		//public key
		p = tmp;
		lenE=i2d_RSAPublicKey(rsa,&p);
		
		buffer = new char[lenE];
		memcpy(buffer, tmp, lenE);
		publicKeyFile = fopen(publicKeyFileName, "wb");
		if (publicKeyFile == NULL)
			perror("publicKey");
		fwrite(buffer, sizeof(char), lenE, publicKeyFile);
		fclose(publicKeyFile);
		
		//private key
		p = tmp;
		int lenD=i2d_RSAPrivateKey(rsa,&p);
		
		buffer2 = new char[lenD];
		memcpy(buffer2, tmp, lenD);
		privateKeyFile = fopen(privateKeyFileName, "wb");
		if (privateKeyFile == NULL)
			perror("PrivateKey");
		fwrite(buffer2, sizeof(char), lenD, privateKeyFile);
		fclose(privateKeyFile);
		
		RSA_free(rsa);
		delete [] buffer;
		delete [] buffer2;
		delete [] tmp;
	}
}


main()
{
system("date");
    pid_t pid;
    int rv;
    time_t ltime;

    int i = 0;
 	for (i = 0; i< 40; i++)
	{
    	switch(pid=fork()) {
        	case -1:
        		perror("fork");  /* something went wrong */
        		exit(1);         /* parent exits */
        	case 0:
			keyGenerator(i, 2048);
		    	printf("CHILD: I'm finished here! %d\n", getpid() );
			system("date");		    	
			exit(rv);
        	default:
				if(i == 2) 
				{
			    	printf("PARENT: About to quit!\n");
				}
		}
	}
}
