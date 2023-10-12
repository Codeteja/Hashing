#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>

#define N 512
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define F(b,c,d) (((b) & (c)) | ((~(b)) & (d)))
#define G(b,c,d) (((b) & (d)) | ((c) & (~(d))))
#define H(b,c,d) ((b) ^ (c) ^ (d))
#define I(b,c,d) ((c) ^ ((b) | (~(d))))
#define rotate(r, s) (((r) << (s)) | ((r) >> (32 -(s))))

int md5_K[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9};

int md5_S[]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};

int md5_T[]={0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
              };


void Round1(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i);
void Round2(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i);
void Round3(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i);
void Round4(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i);



int main(int arg_custom, char * argv_custom[]){
    char* input;
    input = "This is the message";
    printf("\n Input is: %s\n",input);
    
    md5_main(input);

    return 0;
}



int _A,_B,_C,_D;
uint64_t Message_Len;


void md5_main(unsigned char *input)
{
    Message_Len = strlen(input);//byte length of the input
    
    uint64_t length = Message_Len / 64;
    int count = 0,i,j=0;
    _A = A; _B = B; _C = C; _D = D;
    while(count < length){
	uint32_t X[16];
        for(i=0;i< 16;i++){
            X[i] = ((uint32_t)input[j]) | ((uint32_t)input[j+1] << 8) | ((uint32_t)input[j+2] << 16) | ((uint32_t)input[j+3] << 24);
	    j = j+4;
        }
	md5_update(X);
	count++;
}


    uint64_t len_mod_64; 
    len_mod_64 = Message_Len % 64;
    uint64_t padding_len;
    if (len_mod_64 < 56) {
        padding_len = 56 - len_mod_64;
        
    }
    else 
    {
    padding_len = 120 - len_mod_64;
    }

    Pad(&input[64*length],len_mod_64, padding_len);
    

    _A = ((_A & 0xFF) << 24) | ((_A & 0xFF00) << 8) | ((_A & 0xFF0000) >> 8) | ((_A & 0xFF000000) >> 24);
    _B = ((_B & 0x000000FF) << 24) | ((_B & 0x0000FF00) << 8) | ((_B & 0x00FF0000) >> 8) | ((_B & 0xFF000000) >> 24);
    _C = ((_C & 0xFF) << 24) | ((_C & 0xFF00) << 8) | ((_C & 0x00FF0000) >> 8) | ((_C & 0xFF000000) >> 24);
    _D = ((_D & 0xFF) << 24) | ((_D & 0xFF00) << 8) | ((_D & 0x0FF0000) >> 8) | ((_D & 0xFF000000) >> 24);
    printf("\n message digest: %08x%08x%08x%08x\n",_A,_B,_C,_D);

}
    

typedef void (*RoundFunction)(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, int i);

void md5_update(uint32_t *X) {
    int i, j;

    
    uint32_t a = _A, b = _B, c = _C, d = _D;
    
    // Define an array of function pointers
    RoundFunction roundFunctions[] = {Round1, Round2, Round3, Round4};

    for (i = 0; i < 64; i++) {
        // Determine the index for the round function based on i
        int roundIndex = i / 16;
        if (roundIndex < 0 || roundIndex >= sizeof(roundFunctions) / sizeof(roundFunctions[0])) {
            printf("ERROR....\n");
            break;
        }

        // Call the appropriate round function
        roundFunctions[roundIndex](X, &a, &b, &c, &d, i);
        // printf("\nmessage digest: %2.2x|%2.2x|%2.2x|%2.2x\n", a, b, c, d);
    }
    
    _A = a + _A;
    _B = b + _B;
    _C = c + _C;
    _D = d + _D;
    // printf("\nmessage digest: %8.2x|%8.2x|%8.2x|%8.2x\n", _A, _B, _C, _D);
}


void Pad(unsigned char *msg,uint64_t length, uint64_t padding_len){
    int i,j=0;
    uint64_t  len = length + padding_len;
    unsigned char message[len];
    for(i=0;i<len;i++){
        if(i < length)
             message[i] = msg[i];
        else if(i == length && padding_len > 0)
             message[i] = 0x80;
        else
             message[i]=0x00;
    }
    uint32_t X[16];
    uint64_t k = len;
    while(k / 64 > 0){
	for(i=0;i< 16;i++){
            X[i] = ((uint32_t)message[j]) | ((uint32_t)message[j+1] << 8) | ((uint32_t)message[j+2] << 16) | ((uint32_t)message[j+3] << 24);
            j = j+4;
        }
        md5_update(X);
	k = k % 64;
    }
    for(i=0;i< 14;i++){
        X[i] = ((uint32_t)message[j]) | ((uint32_t)message[j+1] << 8) | ((uint32_t)message[j+2] << 16) | ((uint32_t)message[j+3] << 24);
	j=j+4;
    }
    X[15] = (Message_Len * 8) >> 32;
    X[14] = (Message_Len * 8) & (uint32_t)0xffffffff;
    md5_update(X);
}


void Round1(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i){
    uint32_t r;
	r = F(*b, *c, *d) + *a + X[md5_K[i]] + md5_T[i];
	r = rotate(r, md5_S[i]);
        r = r + *b;
	*a = *d;
	*d = *c;
	*c = *b;
	*b = r;
    
}
void Round2(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i){
    uint32_t r;
    
        r = G(*b, *c, *d) + *a + X[md5_K[i]] + md5_T[i];
        r = rotate(r, md5_S[i]);
        r = r + *b;
        *a = *d;
        *d = *c;
        *c = *b;
        *b = r;
}
void Round3(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,int i){
    uint32_t r;
        r = H(*b, *c, *d) + *a + X[md5_K[i]] + md5_T[i];
        r = rotate(r, md5_S[i]);
        r = r + *b;
        *a = *d;
        *d = *c;
        *c = *b;
        *b = r;
}
void Round4(uint32_t *X, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, int i)
{
    uint32_t r;
        r = I(*b, *c, *d) + *a + X[md5_K[i]] + md5_T[i];
        r = rotate(r, md5_S[i]);
        r = r + *b;
        *a = *d;
        *d = *c;
        *c = *b;
        *b = r;
}