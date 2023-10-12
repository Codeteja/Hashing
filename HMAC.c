#include "SHA1_hmac.c"
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>



void hmac_main(unsigned char* msg , uint32_t *hmac)
{
    unsigned char key[64] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
	                     0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
			     0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
			     0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};

    
    printf("\nKey: \n");
    
    for(int i = 0;i < 64;i++)
    {
	
    printf("%02x%c",key[i],(i % 16 == 15) ? '\n' : ' ');
    }

    unsigned char ikey[64];

    unsigned char IPAD[64];
    
    for (int i = 0; i < 64; i++) 
    {
        IPAD[i] = 0x36; // Fill the array with the value 0x36
    }

     printf("\n IPAD: \n");
     
     for(int i = 0;i < 64;i++)
     {
     printf("%02x%c",IPAD[i],(i % 16 == 15) ? '\n' : ' ');

     }

    unsigned char S_I[64];

     for(int i=0 ; i < 64; i++)
    {
        S_I[i] = key[i] ^ IPAD[i];

    }

    printf("\n Inner key\n");

    for(int i=0 ; i < 64 ; i++)
    {
        printf("%02x%c",S_I[i] , (i % 16 == 15) ? '\n' : ' ');
    }

    uint64_t msg_len = strlen(msg);

   // printf("%d",msg_len);

    //printf("\n");

    unsigned char msg1[msg_len + 64];

    memcpy(msg1 , S_I , 64);
    memcpy(msg1 + 64 , msg , msg_len);

    /*for(int i = 0; i < (msg_len +64); i++){
			if(i < 64)
				msg1[i] = S_I[i];
			else
				msg1[i] = msg[i - 64];
		}*/

    printf("\n First data to hash\n");

    for(int i=0 ; i< 64 + msg_len ; i++)
    {
        printf("%02x%c",msg1[i] , (i % 16 == 15) ? '\n' : ' ');
    }

     //sha1((key ^ ipad) || text)
    uint32_t hash1[5];
    
    sha1_main(msg1, hash1, msg_len+64);
    
    printf("\nsha1((key ^ ipad) || text) : \n");
    
    for(int i = 0; i<5;i++)
    {
	
    printf("%08x ",hash1[i]);

    }
    printf("\n");


    unsigned char OPAD[64];
    for (int i = 0; i < 64; i++) 
    {
        OPAD[i] = 0x5C; 
    }

    for(int i = 0;i < 64;i++)
    {
         printf("%02x%c",OPAD[i],(i % 16 == 15) ? '\n' : ' ');
    }


    unsigned char S_O[64];

    for(int i=0 ; i < 64 ; i++)
    {
        S_O[i] = key[i] ^ OPAD[i];  
    }


    printf("\n Outer key\n");

    for(int i=0 ; i < 64 ; i++)
    {
        printf("%02x%c",S_O[i] , (i % 16 == 15) ? '\n' : ' ');
    }

    unsigned char msg2[84];

    memcpy(msg2 , S_O , 64);
    
     int i = 64;
    
    for (int j = 0; j < 5; j++) 
    {
        for (int k = 3; k >= 0; k--) 
        {
            msg2[i++] = (unsigned char)((hash1[j] >> (k * 8)) & 0xff);
        }
    }
    
    //sha1((key ^ opad) || sha1((key ^ ipad) || text)
    
    sha1_main(msg2, hmac, 84);


}

int main()
{
    uint32_t hmac[5];
    
    unsigned char msg[9] = {0x53,0x61,0x6d,0x70,0x6c,0x65,0x20,0x23,0x31};
    
    hmac_main(msg, hmac);
    
    printf("\nHMAC : ");
    
    for(int j=0;j<5;j++)
    {
        printf("%08x ",hmac[j]);

    }
    printf("\n\n");
}



