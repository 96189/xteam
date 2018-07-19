#ifndef SHA1_H
#define SHA1_H
/* ================ sha1.h ================ */
/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain
*/

typedef struct {
    // 32 * 5 = 160bit SHA-1算法得出的消息摘要
    uint32_t state[5];
    // 32 * 2 = 64bit 存储消息的长度
    uint32_t count[2];
    // 8 * 64 = 512bit 由原始消息得出的消息块
    unsigned char buffer[64];
} SHA1_CTX;

void SHA1Transform(uint32_t state[5], const unsigned char buffer[64]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, const unsigned char* data, uint32_t len);
// 
void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

#ifdef REDIS_TEST
int sha1Test(int argc, char **argv);
#endif
#endif
