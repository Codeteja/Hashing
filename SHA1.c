#include<stdio.h>
#include<stdint.h>
#include<string.h>



#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476
#define E 0xC3D2E1F0 

#define F(x, y, z) ((x & y) ^ (~x & z))
#define G(x, y, z) (x ^ y ^ z)
#define H(x, y, z) ((x & y) ^ (x & z) ^ (y & z)) 

 void sha1_update(uint32_t *X);



uint8_t j;

void Padding(unsigned char *message,  uint64_t msg_len, uint64_t padding_len, unsigned char *paddedMsg){

int i;
	for(i = 0; i < msg_len + padding_len; i++){
		if(i < msg_len){
			paddedMsg[i] = message[i];
		}else if((msg_len == i) && padding_len > 0){
			paddedMsg[i] = 0x80;
		}else{
			paddedMsg[i] = 0x00;
		}
	}

}


uint32_t a, b, c, d,e;

uint32_t K[] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};

void sha1_main(unsigned char *message, uint32_t *digest, uint64_t msg_len){

uint64_t offset = msg_len / 64;
uint64_t offset2 = msg_len % 64;
uint64_t padding_len = offset2 < 56 ? 56 - offset2 : (64 + 56) - offset2;
uint64_t total_len = msg_len + padding_len;
uint8_t paddedMsg[total_len];

	// Padding
	Padding(message, msg_len, padding_len, paddedMsg);

uint32_t msg_words[total_len/4 + 2];
uint32_t X[16];
int i, j = 0;
	for(i = 0; i < total_len; i += 4){
		msg_words[j++] = (uint32_t) (paddedMsg[i] << 24 | paddedMsg[i+1] << 16 | paddedMsg[i+2] << 8 | paddedMsg[i+3]);

	}
	msg_words[j++] = (uint32_t) ((msg_len * 8) >> 32);
	msg_words[j++] = (uint32_t) ((msg_len * 8) & 0xFFFFFFFF);

	int msg_Block_count;
	msg_Block_count = (msg_len + padding_len + 8) / 64;

	a =A; b=B; c=C; d=D; e=E;
	for(i = 0; i < msg_Block_count; i++){
		for(int k = 0; k < 16; k++){
			X[k] = msg_words[i * 16 + k];
		}

		sha1_update(X);
	}

	digest[0] = a;
	digest[1] = b;
	digest[2] = c;
	digest[3] = d;
	digest[4] = e;

}


uint32_t rotateLeft(uint32_t x, uint32_t n) {
    return ((x << n) | (x >> (32 - n)));
}

uint32_t rotateRight(uint32_t x, uint32_t n) {
    return ((x >> n) | (x << (32 - n)));
}

void sha1_update(uint32_t *X) {
    uint32_t W[80];

    for (int i = 0; i < 16; i++) {
        W[i] = X[i];
    }

    for (int i = 16; i < 80; i++) {
        W[i] = rotateLeft((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1);
    }

    uint32_t AA = a, BB = b, CC = c, DD = d, EE = e;
    int r;

    for (r = 0; r < 20; r++) {
        uint32_t TEMP = rotateLeft(AA, 5) + F(BB, CC, DD) + EE + K[0] + W[r];
        EE = DD;
        DD = CC;
        CC = rotateLeft(BB, 30);
        BB = AA;
        AA = TEMP;
    }

    for (; r < 40; r++) {
        uint32_t TEMP = rotateLeft(AA, 5) + G(BB, CC, DD) + EE + K[1] + W[r];
        EE = DD;
        DD = CC;
        CC = rotateLeft(BB, 30);
        BB = AA;
        AA = TEMP;
    }

    for (; r < 60; r++) {
        uint32_t TEMP = rotateLeft(AA, 5) + H(BB, CC, DD) + EE + K[2] + W[r];
        EE = DD;
        DD = CC;
        CC = rotateLeft(BB, 30);
        BB = AA;
        AA = TEMP;
    }


    for (; r < 80; r++) {
        uint32_t TEMP = rotateLeft(AA, 5) + G(BB, CC, DD) + EE + K[3] + W[r];
        EE = DD;
        DD = CC;
        CC = rotateLeft(BB, 30);
        BB = AA;
        AA = TEMP;
    }

    a += AA;
    b += BB;
    c += CC;
    d += DD;
    e += EE;
}


int main(int argc, unsigned char *argv[]){

	unsigned char *message = argv[1];
	uint32_t digest[5];
        printf("The given plaintext: %s\n", message);
        if(argc > 2){
                printf("Please give the input \n");
        }else{
             sha1_main(message, digest, strlen(message));
	}
	printf("The Hash:\n");
	for(int i = 0; i < 5; i++){
		printf("%08x ", digest[i]);
	}
	printf("\n");

}



