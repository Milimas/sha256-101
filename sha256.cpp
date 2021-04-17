#include <iostream>
#include <bitset>
#include <cstring>
#include <cstdint>
#include <sstream>

using namespace std;

#define SIZE 64
typedef long long unsigned int LLUI;

int main()
{
    // input
    char str[SIZE] = "abc";

    bitset<8> lengthInBits = strlen(str) * 8;
    str[strlen(str)] = char(128);
    str[SIZE - 1] = char(lengthInBits.to_ulong());

    // constents
    bitset<32> h[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19,
    };

    const bitset<32> k[64] = {
        0x428a2f98,
        0x71374491,
        0xb5c0fbcf,
        0xe9b5dba5,
        0x3956c25b,
        0x59f111f1,
        0x923f82a4,
        0xab1c5ed5,
        0xd807aa98,
        0x12835b01,
        0x243185be,
        0x550c7dc3,
        0x72be5d74,
        0x80deb1fe,
        0x9bdc06a7,
        0xc19bf174,
        0xe49b69c1,
        0xefbe4786,
        0x0fc19dc6,
        0x240ca1cc,
        0x2de92c6f,
        0x4a7484aa,
        0x5cb0a9dc,
        0x76f988da,
        0x983e5152,
        0xa831c66d,
        0xb00327c8,
        0xbf597fc7,
        0xc6e00bf3,
        0xd5a79147,
        0x06ca6351,
        0x14292967,
        0x27b70a85,
        0x2e1b2138,
        0x4d2c6dfc,
        0x53380d13,
        0x650a7354,
        0x766a0abb,
        0x81c2c92e,
        0x92722c85,
        0xa2bfe8a1,
        0xa81a664b,
        0xc24b8b70,
        0xc76c51a3,
        0xd192e819,
        0xd6990624,
        0xf40e3585,
        0x106aa070,
        0x19a4c116,
        0x1e376c08,
        0x2748774c,
        0x34b0bcb5,
        0x391c0cb3,
        0x4ed8aa4a,
        0x5b9cca4f,
        0x682e6ff3,
        0x748f82ee,
        0x78a5636f,
        0x84c87814,
        0x8cc70208,
        0x90befffa,
        0xa4506ceb,
        0xbef9a3f7,
        0xc67178f2,
    };

    bitset<32> word[64];

    // assign 32 bits words from str to word array
    for (LLUI i = 0; i < sizeof(str); i += 4)
    {
        string temp = "";
        for (int j = 0; j < 4; j++)
        {
            temp += bitset<8>(str[i + j]).to_string();
        }
        word[i / 4] = bitset<32>(temp);
    }

    for (int i = 16; i < 64; i++)
    {
        // s0 = (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3)
        bitset<32> s0_0 = bitset<32>(_rotr(word[i - 15].to_ulong(), 7));
        bitset<32> s0_1 = bitset<32>(_rotr(word[i - 15].to_ulong(), 18));
        bitset<32> s0_2 = word[i - 15] >> 3;
        bitset<32> s0 = s0_0 ^ s0_1 ^ s0_2;

        // s1 = (w[i- 2] rightrotate 17) xor (w[i- 2] rightrotate 19) xor (w[i- 2] rightshift 10)
        bitset<32> s1_0 = bitset<32>(_rotr(word[i - 2].to_ulong(), 17));
        bitset<32> s1_1 = bitset<32>(_rotr(word[i - 2].to_ulong(), 19));
        bitset<32> s1_2 = word[i - 2] >> 10;
        bitset<32> s1 = s1_0 ^ s1_1 ^ s1_2;

        // w[i] = w[i-16] + s0 + w[i-7] + s1
        word[i] = word[i - 16].to_ulong() + s0.to_ulong() + word[i - 7].to_ulong() + s1.to_ulong();
    }

    bitset<32> _a = h[0],
               _b = h[1],
               _c = h[2],
               _d = h[3],
               _e = h[4],
               _f = h[5],
               _g = h[6],
               _h = h[7];

    for (int i = 0; i < 64; i++)
    {
        bitset<32> s1 = bitset<32>(_rotr(_e.to_ulong(), 6)) ^ bitset<32>(_rotr(_e.to_ulong(), 11)) ^ bitset<32>(_rotr(_e.to_ulong(), 25));
        bitset<32> ch = (_e & _f) ^ (~_e & _g);
        bitset<32> temp1 = _h.to_ulong() + s1.to_ulong() + ch.to_ulong() + k[i].to_ulong() + word[i].to_ulong();
        bitset<32> s0 = bitset<32>(_rotr(_a.to_ulong(), 2)) ^ bitset<32>(_rotr(_a.to_ulong(), 13)) ^ bitset<32>(_rotr(_a.to_ulong(), 22));
        bitset<32> maj = (_a & _b) ^ (_a & _c) ^ (_b & _c);
        bitset<32> temp2 = s0.to_ulong() + maj.to_ulong();
        _h = _g;
        _g = _f;
        _f = _e;
        _e = _d.to_ulong() + temp1.to_ulong();
        _d = _c;
        _c = _b;
        _b = _a;
        _a = temp1.to_ulong() + temp2.to_ulong();
        if (i == 63)
        {
            h[0] = h[0].to_ulong() + _a.to_ulong();
            h[1] = h[1].to_ulong() + _b.to_ulong();
            h[2] = h[2].to_ulong() + _c.to_ulong();
            h[3] = h[3].to_ulong() + _d.to_ulong();
            h[4] = h[4].to_ulong() + _e.to_ulong();
            h[5] = h[5].to_ulong() + _f.to_ulong();
            h[6] = h[6].to_ulong() + _g.to_ulong();
            h[7] = h[7].to_ulong() + _h.to_ulong();
        }
    }

    stringstream digest;
    for (int i = 0; i < 8; i++)
    {
        // digest = (digest << 32) | h[i];
        digest << hex << h[i].to_ulong();
    }
    cout << digest.str() << endl;

    return 0;
}
