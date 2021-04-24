//
// Created by devilarx on 24.04.2021.
//

#include "md5.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>


// Cyclic shift left / Rotation left
// asm: rol x, bits
static uint32_t rol(uint32_t x, uint32_t bits)
{
    return x<<bits | x>>(32-bits);
}

// Noob realisation MD5 hash algorithm
char* md5(const char *data, size_t sz){
    // Size of data in bits
    uint64_t bsz = sz * 8;
    // Count of zeros in bits
    uint32_t zeros_count = (bsz % 512) > 447 ? 512 -(bsz%512) + 447 : 447 - (bsz%512);
    // Total buffer size in bytes
    uint32_t buff_sz = (bsz + zeros_count + 1 + 64)/8;
    // Pointer to low bytes of size
    uint32_t* f_sz_p = (uint32_t*) &bsz;
    // pointer to high bytes of size
    uint32_t* s_sz_p = ((uint32_t*) &bsz) + 1;
    // Data buffer with alignment and size
    uint8_t* buffer = (uint8_t*) malloc(buff_sz);
    // Integer pointer to out buffer
    uint32_t* iout;

    // Buffer in integers
    uint32_t A,B,C,D;
    // Constants array
    uint32_t T[64];
    // Iterators
    uint32_t i, b;
    // Shifts
    uint32_t s[] = {
            7,12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,12, 17, 22,
            5, 9 , 14, 20, 5, 9 , 14, 20, 5, 9, 14, 20, 5, 9 , 14, 20,
            4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4 , 11, 16, 23,
            6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10 , 15, 21
    };
    // Output buffer
    char* out;

    // Step 1 - Data alignment to length 447 modulo 512
    if (buffer == NULL) {
        return NULL;
    }
    memcpy(buffer, data, sz);
    buffer[sz] = 0x80;
    memset(buffer+sz+1, 0, (zeros_count/8));

    // Step 2 - Adding size to end of buffer
    memcpy(buffer+buff_sz-8, f_sz_p, 4);
    memcpy(buffer+buff_sz-4, s_sz_p, 4);
    // Buffer size is zero modulo 512

    // Step 3 - initializing buffer
    A = 0x67452301;
    B = 0xEFCDAB89;
    C = 0x98BADCFE;
    D = 0x10325476;

    // Step 4 - Loop calculations
    // Generate array with 64 constants. T[n] = int(2^32 * |sin(n)|)
    for (i = 0; i<64; ++i){
        T[i] = (uint32_t)floor(pow(2, 32)*fabs(sin(i+1)));
    }

    for (b = 0; b<buff_sz / 64; ++b){
        uint32_t* X = (uint32_t*) (buffer+b*64);
        uint32_t AA = A, BB = B, CC = C, DD = D;
        for( i = 0; i< 64; ++i){
            uint32_t F, k;

            // Round 1
            if (i<16){
                F = ( B & C ) | ( (~B) & D );
                k = i;
            // Round 2
            }else if (i<32){
                F = ( D & B ) | ( (~D) & C );
                k = (5*i+1)%16;
            // Round 3
            }else if (i<48){
                F = B^C^D;
                k = (3*i+5)%16;
            // Round 4
            }else{
                F = C ^ (B | (~D));
                k = (7*i) %16;
            }
            F = F + A + T[i] + X[k];
            A = D;
            D = C;
            C = B;
            B = B+rol(F, s[i]);
        }
        A+=AA;
        B+=BB;
        C+=CC;
        D+=DD;
    }

    free(buffer);

    // Step 5 - Result buffer
    out = (char*) malloc(16);
    if (out == NULL){
        return NULL;
    }
    iout = (uint32_t*)out;
    iout[0] = A;
    iout[1] = B;
    iout[2] = C;
    iout[3] = D;

    return out;
}