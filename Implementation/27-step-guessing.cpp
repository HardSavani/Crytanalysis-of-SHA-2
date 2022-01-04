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

// #define int long long int

using namespace std;

ofstream MyFile("result.txt");
ifstream dW_File("dW-27-steps.txt");
ifstream A_E_W_File("A-E-W-27-steps.txt");

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

// const uint32_t k[64] = {
//     0,
//     0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0,
//     0, 0, 0, 0, 0, 0, 0, 0};

uint32_t reg1[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

uint32_t reg2[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

uint32_t W[27];
uint32_t W_dash[27];
uint32_t dW[27] = {0};
string A_s[27];
string E_s[27];
string W_s[27];
int guess[27][32];
int flag;

uint32_t w_to_set_register_A(uint32_t desired_a, int step)
{
    return desired_a - (SIG0(reg1[0]) + Maj(reg1[0], reg1[1], reg1[2]) + SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step]);
}

uint32_t w_to_set_register_E(uint32_t desired_e, int step)
{
    return desired_e - (reg1[3] + SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step]);
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

string toBits(uint32_t x)
{
    string res = "";
    uint32_t temp = x;
    while (temp > 0)
    {
        if (temp % 2 == 1)
        {
            res = "1" + res;
        }
        else
        {
            res = "0" + res;
        }
        temp = temp / 2;
    }
    while (res.length() < 32)
    {
        res = "0" + res;
    }

    return res;
}

long long toInt(string s)
{
    long long res = 0;
    string s1 = s;
    reverse(s1.begin(), s1.end());
    for (int i = 0; i < s1.length(); i++)
    {
        res += (long long)(((long long)(s1[i] - '0')) << (long long)i);
    }
    return res;
}

void stateUpdate(int step)
{
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
    // MyFile << step << "\t\t" << -(int)(reg1[0] - reg2[0]) << "\t\t" << -(int)(reg1[1] - reg2[1]) << "\t\t" << -(int)(reg1[2] - reg2[2]) << "\t\t" << -(int)(reg1[3] - reg2[3]) << "\t\t" << -(int)(reg1[4] - reg2[4]) << "\t\t" << -(int)(reg1[5] - reg2[5]) << "\t\t" << -(int)(reg1[6] - reg2[6]) << "\t\t" << -(int)(reg1[7] - reg2[7]) << endl;

    MyFile << step << "\t\t" << toBits(reg2[0] - reg1[0]) << "\t\t" << toBits(reg2[4] - reg1[4]) << "\t\t" << toBits(W_dash[step] - W[step]) << endl;
    // MyFile << step << "\t\t" << (long long)(-(reg1[0] - reg2[0])) << "\t\t" << (long long)(-(reg1[4] - reg2[4])) << "\t\t" << (long long)(dW[step]) << endl;

    // printf("%d\t %08x\t %08x\t %08x\n", step, W[step], dW[step], W_dash[step]);
}

uint32_t a_to_set_W(uint32_t desired_w, int step)
{
    uint32_t temp1_1 = SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step] + desired_w;
    uint32_t temp2_1 = SIG0(reg1[0]) + Maj(reg1[0], reg1[1], reg1[2]);

    return (temp1_1 + temp2_1);
}

uint32_t e_to_set_W(uint32_t desired_w, int step)
{
    uint32_t temp1_1 = SIG1(reg1[4]) + IF(reg1[4], reg1[5], reg1[6]) + reg1[7] + k[step] + W[step];
    uint32_t temp2_1 = SIG0(reg1[0]) + Maj(reg1[0], reg1[1], reg1[2]);

    return (reg1[3] + temp1_1);
}

void print_diff_char(int step)
{
    printf("%d\t %08x\t %08x\t %08x\n", step, (reg2[0] - reg1[0]), (reg2[4] - reg1[4]), W_dash[step] - W[step]);
}

void check(string desired_string, string target_string, int &x)
{
    for (int j = 0; j < 32; j++)
    {
        if (target_string[j] == '-')
        {
            ;
        }
        else if (target_string[j] == 'u' || target_string[j] == '1')
        {
            if (desired_string[j] != '1')
            {
                return;
            }
        }
        else
        {
            if (desired_string[j] != '0')
            {
                return;
            }
        }
    }
    x = 1;
}

void dfs(int i, string &s, uint32_t &temp, int &flag, int step, int isE)
{
    if (i == 32)
    {
        if (isE)
        {
            W[step] = w_to_set_register_E(temp, step);
        }
        else
        {
            W[step] = temp;
        }

        uint32_t desired_quantity;
        string corresponding_string, target_string;

        if (count(W_s[step].begin(), W_s[step].end(), '-') <= count(A_s[step].begin(), A_s[step].end(), '-'))
        {
            desired_quantity = W[step];
            target_string = W_s[step];
        }
        else
        {
            desired_quantity = a_to_set_W(W[step], step);
            target_string = A_s[step];
        }

        if (!isE)
        {
            desired_quantity = e_to_set_W(W[step], step);
            target_string = E_s[step];
        }

        corresponding_string = toBits(desired_quantity);

        cout << toBits(temp) << " " << corresponding_string << " " << target_string << endl;

        check(corresponding_string, target_string, flag);

        return;
    }
    if (flag)
    {
        return;
    }
    if (s[i] == 'u' || s[i] == '1')
    {
        temp += (1 << (31 - i));
        dfs(i + 1, s, temp, flag, step, isE);
        temp -= (1 << (31 - i));
    }
    else if (s[i] == '-')
    {
        int x = rand() % 2;
        if (!x)
        {
            dfs(i + 1, s, temp, flag, step, isE);
            if (flag)
            {
                return;
            }
            temp += (1 << (31 - i));
            dfs(i + 1, s, temp, flag, step, isE);
            temp -= (1 << (31 - i));
        }
        else
        {

            temp += (1 << (31 - i));
            dfs(i + 1, s, temp, flag, step, isE);
            temp -= (1 << (31 - i));

            if (flag)
            {
                return;
            }
            dfs(i + 1, s, temp, flag, step, isE);
        }
    }
    else
    {
        dfs(i + 1, s, temp, flag, step, isE);
    }
}

uint32_t generateE(string s, int step)
{
    uint32_t e_temp = 0;
    for (int j = 0; j < 32; j++)
    {
        if (s[31 - j] == '1' || s[31 - j] == 'u')
        {
            e_temp += (1 << (31 - j));
        }
        else if (s[31 - j] == '-')
        {
            int x = rand() % 2;
            if (x)
            {
                e_temp += (1 << (31 - j));
            }
        }
    }
    return e_temp;
}

void read_A_E_W()
{
    string s_a, s_e, s_w;
    for (int i = 0; i < 27; i++)
    {
        A_E_W_File >> s_a >> s_e >> s_w;
        cout << s_a << " " << s_e << " " << s_w << endl;
        A_s[i] = s_a;
        E_s[i] = s_e;
        W_s[i] = s_w;
    }
    cout << endl;
    A_E_W_File.close();

    // uint32_t temp[16] = {0x725a0370, 0x0daa9f1b, 0x071d92df, 0xec8282c1, 0x7913134a, 0xbc2eb291, 0x02d33a84, 0x278dfd29, 0x0c40f8ea, 0xd8bd68a0, 0x0ce670c5, 0x5ec7155d, 0x9f6407a8, 0x729fbfe8, 0xaa7c7c08, 0x607ae76d};

    // uint32_t temp_dash[16] = {0x725a0370, 0x0daa9f1b, 0x071d92df, 0xec8282c1, 0x7913134a, 0xbc2eb291, 0x02d33a84, 0x27460e6d, 0x08c8fbea, 0xd8bd68a0, 0x0ce670c5, 0x5ec7155d, 0x9f4425fb, 0x729fbfe8, 0xaa7c7c08, 0x2d32d129};

    // for (int i = 0; i < 16; i++)
    // {
    //     W[i] = temp[i];
    // }

    // for (int i = 0; i < 16; i++)
    // {
    //     W_dash[i] = temp_dash[i];
    // }
}

void read_dW()
{
    char c;
    for (int i = 0; i < 27; i++)
    {
        string s = W_s[i];
        for (int j = 31; j >= 0; j--)
        {
            c = s[31 - j];
            if (c == 'u')
            {
                dW[i] -= (1 << j);
            }
            else if (c == 'n')
            {
                dW[i] += (1 << j);
            }
        }
    }
}

signed main()
{
    memset(guess, 0, sizeof(guess));

    MyFile << "step\t\t a\t\t b\t\t c\t\t d\t\t e\t\t f\t\t g\t\t h\n";

    //     // printf("step\t W\t\t dW\t\t W_dash\n");

    srand(time(NULL));
    srandom(time(nullptr));

    flag = 0;

    read_A_E_W();
    read_dW();

    // while (true)
    // {
    // A_E_W_File.seekg(0);
    string s;
    for (int i = 0; i < 16; i++)
    {
        uint32_t e_temp = 0;

        int isCompatible = 0;

        // while (!isCompatible)
        // {

        // if (i == 15)
        // {
        //     W[i] = generateE(W_s[i], i);
        // }
        // else
        // {
        //     e_temp = generateE(s, i);
        //     W[i] = w_to_set_register_E(e_temp, i);
        // }

        // uint32_t desired_quantity;
        // string corresponding_string, target_string;

        // if (count(W_s[i].begin(), W_s[i].end(), '-') <= count(A_s[i].begin(), A_s[i].end(), '-'))
        // {
        //     desired_quantity = W[i];
        //     target_string = W_s[i];
        // }
        // else
        // {
        //     desired_quantity = a_to_set_W(W[i], i);
        //     target_string = A_s[i];
        // }

        // corresponding_string = toBits(desired_quantity);

        // check(corresponding_string, target_string, isCompatible);

        if (i == 15)
        {
            dfs(0, W_s[i], e_temp, isCompatible, i, 0);
        }
        else
        {
            dfs(0, E_s[i], e_temp, isCompatible, i, 1);
        }

        cout << "  " << isCompatible << endl;

        W_dash[i] = W[i] + dW[i];

        stateUpdate(i);
        print_diff_char(i);
    }

    for (int i = 16; i < 27; i++)
    {
        W[i] = sig1(W[i - 2]) + W[i - 7] + sig0(W[i - 15]) + W[i - 16];
        W_dash[i] = sig1(W_dash[i - 2]) + W_dash[i - 7] + sig0(W_dash[i - 15]) + W_dash[i - 16];
        stateUpdate(i);
        print_diff_char(i);
    }

    for (int i = 16; i < 27; i++)
    {
        printf("%08x\t %08x\n", W_dash[i] - W[i], dW[i]);
    }

    cout << endl;

    printf("\n");
    printf("Register values at the end of 27-step attack: \n");

    printRegisters();
    cout << endl;

    printf("%08x\t %08x\n", (sig1(W_dash[17]) - sig1(W[17])), W_dash[12] - W[12]);

    // MyFile.close();
    // }
}