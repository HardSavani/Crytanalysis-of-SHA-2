#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

ifstream conditionFile("resultConditions.txt");
ofstream conditionOut("27-steps-register-condition.txt");

int main()
{
    int A[27][32], E[27][32], W[27][32];
    memset(A, -1, sizeof(A));
    memset(E, -1, sizeof(E));
    memset(W, -1, sizeof(W));

    string s;
    int x;

    while (conditionFile >> s)
    {
        conditionFile >> x;
        string s1 = "";
        string s2 = "";
        int x1 = 0;
        int x2 = 0;
        int flag = 0;
        for (int i = 1; i < s.length(); i++)
        {
            if (s[i] == ',')
            {
                flag = 1;
                continue;
            }
            if (flag)
            {
                x2 *= 10;
                x2 += (s[i] - '0');
            }
            else
            {
                x1 *= 10;
                x1 += (s[i] - '0');
            }
        }
        cout << s << " " << x1 << " " << x2 << endl;
        // int x1 = stoi(s1);
        // int x2 = stoi(s2);
        if (s[0] == 'W')
        {
            W[x1][x2] = x;
        }
        else if (s[0] == 'A')
        {
            A[x1][x2] = x;
        }
        else
        {
            E[x1][x2] = x;
        }
    }

    for (int i = 0; i < 27; i++)
    {
        conditionOut << i << " ";
        for (int j = 31; j >= 0; j--)
        {
            if (A[i][j] == -1)
            {
                conditionOut << '?';
            }
            else
            {
                conditionOut << A[i][j];
            }
        }

        conditionOut << " ";

        for (int j = 31; j >= 0; j--)
        {
            if (E[i][j] == -1)
            {
                conditionOut << '?';
            }
            else
            {
                conditionOut << E[i][j];
            }
        }

        conditionOut << " ";

        for (int j = 31; j >= 0; j--)
        {
            if (W[i][j] == -1)
            {
                conditionOut << '?';
            }
            else
            {
                conditionOut << W[i][j];
            }
        }
        conditionOut << endl;
    }

    return 0;
}