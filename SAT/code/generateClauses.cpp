#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

ofstream MyFile("inputClauses.txt");
ifstream inputFile("inputConditions.txt");
ifstream resultInput("SAT.txt");
ofstream resultOutput("resultConditions.txt");

int main()
{

    map<string, int> wordToNum;
    map<int, string> numToWord;
    vector<vector<int>> v;

    int num = 0;

    // string u;
    // getline(cin, u);

    MyFile.clear();
    wordToNum.clear();
    numToWord.clear();

    int yo = 0;

    string s;
    while (getline(inputFile, s))
    {
        {
            yo++;
            // getline(cin, s);
            // cout << s << endl;
            int n = s.length();

            vector<string> temp;

            for (int i = 0; i < n; i++)
            {
                if (s[i] == '!')
                {
                    temp.push_back(s.substr(0, i));
                    temp.push_back("-");
                    string e = s.substr(i + 2, n - i - 2);
                    string q = "";
                    for (int j = 0; j < e.size(); j++)
                    {
                        if (s[j] == ' ')
                        {
                            cout << "YES" << endl;
                        }
                    }
                    temp.push_back(e);
                    break;
                }
                if (s[i] == '=')
                {
                    temp.push_back(s.substr(0, i));
                    temp.push_back("+");
                    string e = s.substr(i + 1, n - i - 1);
                    string q = "";
                    for (int j = 0; j < e.size(); j++)
                    {
                        if (s[j] == ' ')
                        {
                            cout << "YES" << endl;
                        }
                    }
                    temp.push_back(e);
                    break;
                }
            }

            if (temp[2] == "0" || temp[2] == "1")
            {
                if (wordToNum.find(temp[0]) == wordToNum.end())
                {
                    num++;
                    wordToNum[temp[0]] = num;
                    numToWord[num] = temp[0];
                }
                vector<int> x;
                if (temp[2] == "0")
                {
                    x.push_back(-wordToNum[temp[0]]);
                }
                else
                {
                    x.push_back(wordToNum[temp[0]]);
                }
                x.push_back(0);
                v.push_back(x);
            }

            else if (temp[1] == "+")
            {
                int x1, x2;
                if (wordToNum.find(temp[0]) == wordToNum.end())
                {
                    num++;
                    wordToNum[temp[0]] = num;
                    numToWord[num] = temp[0];
                    x1 = num;
                }
                else
                {
                    x1 = wordToNum[temp[0]];
                }
                if (wordToNum.find(temp[2]) == wordToNum.end())
                {
                    num++;
                    wordToNum[temp[2]] = num;
                    numToWord[num] = temp[2];
                    x2 = num;
                }
                else
                {
                    x2 = wordToNum[temp[2]];
                }
                vector<int> x;
                x.push_back(x1);
                x.push_back(-x2);
                x.push_back(0);
                v.push_back(x);

                x.clear();
                x.push_back(-x1);
                x.push_back(x2);
                x.push_back(0);
                v.push_back(x);
            }
            else
            {
                int x1, x2;
                if (wordToNum.find(temp[0]) == wordToNum.end())
                {
                    num++;
                    wordToNum[temp[0]] = num;
                    numToWord[num] = temp[0];
                    x1 = num;
                }
                else
                {
                    x1 = wordToNum[temp[0]];
                }
                if (wordToNum.find(temp[2]) == wordToNum.end())
                {
                    num++;
                    wordToNum[temp[2]] = num;
                    numToWord[num] = temp[2];
                    x2 = num;
                }
                else
                {
                    x2 = wordToNum[temp[2]];
                }
                vector<int> x;
                x.push_back(x1);
                x.push_back(x2);
                x.push_back(0);
                v.push_back(x);

                x.clear();
                x.push_back(-x1);
                x.push_back(-x2);
                x.push_back(0);
                v.push_back(x);
            }
        }
    }
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            MyFile << v[i][j] << " ";
        }
        MyFile << endl;
    }
    cout << wordToNum.size() << endl;
    cout << v.size() << endl;

    int r = 0;

    for (auto i : wordToNum)
    {
        cout << i.first << " " << i.second << endl;
    }

    int a;
    while (resultInput >> a)
    {
        int sign;
        if (a > 0)
        {
            sign = 1;
        }
        else
        {
            sign = 0;
        }
        string s = numToWord[abs(a)];
        resultOutput << s << "\t" << sign << endl;
    }

    return 0;
}