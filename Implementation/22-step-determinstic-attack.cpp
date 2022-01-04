#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <fstream>

#define Ch(X, Y, Z) ((X | Y) & (~X | Z))
#define Maj(X, Y, Z) ((X & Y) ^ (X & Z) ^ (Y & Z))
#define IF(X, Y, Z) ((X & Y) ^ (~X & Z))
#define ROTLEFT(x, y) (((x) << (y)) | ((x) >> (32 - y)))
#define ROTRIGHT(x, y) (((x) >> (y)) | ((x) << (32 - y)))
#define sig0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define sig1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
#define SIG0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define SIG1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))

using namespace std;

const uint32_t k[64] = {
    0x428a2f98,
    0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// uint32_t reg1[21][8];
// reg1[0][0] = 0x6a09e667;
// reg1[0][1] = 0xbb67ae85;
// reg1[0][2] = 0x3c6ef372;
// reg1[0][3] = 0xa54ff53a;
// reg1[0][4] = 0x510e527f;
// reg1[0][5] = 0x9b05688c;
// reg1[0][6] = 0x1f83d9ab;
// reg1[0][7] = 0x5be0cd19;

// uint32_t reg2[21][8];
// reg2[0][0] = 0x6a09e667;
// reg2[0][1] = 0xbb67ae85;
// reg2[0][2] = 0x3c6ef372;
// reg2[0][3] = 0xa54ff53a;
// reg2[0][4] = 0x510e527f;
// reg2[0][5] = 0x9b05688c;
// reg2[0][6] = 0x1f83d9ab;
// reg2[0][7] = 0x5be0cd19;

uint32_t reg1[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

uint32_t reg2[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

uint32_t W[22];
uint32_t W_dash[22];
uint32_t dW[22] = {0};

uint32_t w_to_set_register_A(uint32_t desired_a, int step)
{
    return desired_a - (SIG0(reg1[0]) + Maj(reg1[0], reg1[1], reg1[2]) + SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step]);
}

uint32_t w_to_set_register_E(uint32_t desired_e, int step)
{
    return desired_e - (reg1[3] + SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step]);
}

ofstream MyFile("result.txt");

void stateUpdate(int step)
{

    if (step == 8)
    {
        dW[8] = -1 - (IF((reg1[4] + 1), reg1[5], reg1[6]) - IF(reg1[4], reg1[5], reg1[6])) - (SIG1((reg1[4] + 1)) - SIG1(reg1[4]));
    }
    else if (step == 10)
    {
        // dW[10] should come equal to DELTA
        dW[10] = -(IF((reg1[4] - 1), (reg1[5] - 1), (reg1[6] + 1)) - IF(reg1[4], reg1[5], reg1[6])) - (SIG1((reg1[4] - 1)) - SIG1(reg1[4]));
    }

    uint32_t temp1_1 = SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step] + W[step];
    uint32_t temp2_1 = SIG0(reg1[0]) + Maj(reg1[0], reg1[1], reg1[2]);

    reg1[7] = reg1[6];
    reg1[6] = reg1[5];
    reg1[5] = reg1[4];
    reg1[4] = reg1[3] + temp1_1;
    reg1[3] = reg1[2];
    reg1[2] = reg1[1];
    reg1[1] = reg1[0];
    reg1[0] = temp1_1 + temp2_1;

    W_dash[step] = W[step] + dW[step];
    if (step >= 16)
    {
        W_dash[step] = sig1(W_dash[step - 2]) + W_dash[step - 7] + sig0(W_dash[step - 15]) + W_dash[step - 16];
    }

    uint32_t temp1_2 = SIG1(reg2[4]) + IF(reg2[4], reg2[5], reg2[6]) + reg2[7] + k[step] + W_dash[step];
    uint32_t temp2_2 = SIG0(reg2[0]) + Maj(reg2[0], reg2[1], reg2[2]);

    reg2[7] = reg2[6];
    reg2[6] = reg2[5];
    reg2[5] = reg2[4];
    reg2[4] = reg2[3] + temp1_2;
    reg2[3] = reg2[2];
    reg2[2] = reg2[1];
    reg2[1] = reg2[0];
    reg2[0] = temp1_2 + temp2_2;

    // MyFile << step << "\t" << W[step] << "\t" << dW[step] << "\t" << W_dash[step] << "\t" << -(int)(reg1[0] - reg2[0]) << "\t" << -(int)(reg1[1] - reg2[1]) << "\t" << -(int)(reg1[2] - reg2[2]) << "\t" << -(int)(reg1[3] - reg2[3]) << "\t" << -(int)(reg1[4] - reg2[4]) << "\t" << -(int)(reg1[5] - reg2[5]) << "\t" << -(int)(reg1[6] - reg2[6]) << "\t" << -(int)(reg1[7] - reg2[7]) << endl;
    MyFile << step << "\t" << -(int)(reg1[0] - reg2[0]) << "\t" << -(int)(reg1[1] - reg2[1]) << "\t" << -(int)(reg1[2] - reg2[2]) << "\t" << -(int)(reg1[3] - reg2[3]) << "\t" << -(int)(reg1[4] - reg2[4]) << "\t" << -(int)(reg1[5] - reg2[5]) << "\t" << -(int)(reg1[6] - reg2[6]) << "\t" << -(int)(reg1[7] - reg2[7]) << endl;

    // cout << step << "\t\t" << W[step] << "\t\t" << dW[step] << "\t\t" << W_dash[step] << endl;
    printf("%d\t %08x\t %08x\t %08x\n", step, W[step], dW[step], W_dash[step]);
}

void printRegisters()
{
    printf("\n");
    printf("%08x\t %08x\n", reg1[0], reg2[0]);
    printf("%08x\t %08x\n", reg1[1], reg2[1]);
    printf("%08x\t %08x\n", reg1[2], reg2[2]);
    printf("%08x\t %08x\n", reg1[3], reg2[3]);
    printf("%08x\t %08x\n", reg1[4], reg2[4]);
    printf("%08x\t %08x\n", reg1[5], reg2[5]);
    printf("%08x\t %08x\n", reg1[6], reg2[6]);
    printf("%08x\t %08x\n", reg1[7], reg2[7]);
}

void localCollision()
{

    srand(time(NULL));

    dW[7] = 1;
    dW[15] = -1;

    W[15] = (uint32_t)rand();

    // DELTA = dW10

    uint32_t DELTA = sig1(W[15]) - sig1(W[15] - 1);

    printf("DELTA : %d\n", DELTA);
    printf("step\t W\t\t dW\t\t W_dash\n");
    printf("\n");

    // MyFile << "step\t W\t dW\t W_dash\t a\t b\t c\t d\t e\t f\t g\t h\n";
    MyFile << "step\t a\t b\t c\t d\t e\t f\t g\t h\n";

    W[0] = (uint32_t)rand();
    stateUpdate(0);

    W[1] = (uint32_t)rand();
    stateUpdate(1);

    W[2] = w_to_set_register_A(DELTA - 1 + Maj(-1, -2, DELTA - 3), 2);
    stateUpdate(2);

    // a3 = dW10 - 3

    W[3] = w_to_set_register_A(DELTA - 3, 3);
    stateUpdate(3);

    W[4] = w_to_set_register_A(-2, 4);
    stateUpdate(4);

    W[5] = w_to_set_register_A(-1, 5);
    stateUpdate(5);

    W[6] = w_to_set_register_A(-1, 6);
    stateUpdate(6);

    W[7] = w_to_set_register_A(-1, 7);
    stateUpdate(7);

    // printRegisters();

    W[8] = w_to_set_register_E(0, 8);
    stateUpdate(8);

    W[9] = w_to_set_register_E(0, 9);
    stateUpdate(9);

    W[10] = w_to_set_register_E(-1, 10);
    stateUpdate(10);

    W[11] = w_to_set_register_E(-1, 11);
    stateUpdate(11);

    W[12] = w_to_set_register_E(-1, 12);
    stateUpdate(12);

    W[13] = w_to_set_register_E(-1, 13);
    stateUpdate(13);

    srand(time(NULL));

    W[14] = (uint32_t)rand();
    stateUpdate(14);

    // W[15] already generated
    stateUpdate(15);

    W[16] = sig1(W[14]) + W[9] + sig0(W[1]) + W[0];
    stateUpdate(16);

    W[17] = sig1(W[15]) + W[10] + sig0(W[2]) + W[1];
    stateUpdate(17);

    W[18] = sig1(W[16]) + W[11] + sig0(W[3]) + W[2];
    stateUpdate(18);

    W[19] = sig1(W[17]) + W[12] + sig0(W[4]) + W[3];
    stateUpdate(19);

    W[20] = sig1(W[18]) + W[13] + sig0(W[5]) + W[4];
    stateUpdate(20);

    W[21] = sig1(W[19]) + W[14] + sig0(W[6]) + W[5];
    stateUpdate(21);

    printf("\n");
    printf("Register values at the end of 22-step attack: \n");

    printRegisters();

    MyFile.close();
}

int main()
{

    localCollision();
    printf("\n");
    return 0;
}