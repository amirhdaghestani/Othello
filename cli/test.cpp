#include <iostream>
#include <fstream>

using namespace std;
int main()
{
    ofstream a;
    string b;
    a.open("test.in");
    char arch[8] = {'g','d','b','a','c','f','h','e'};
    for(int i = 0; i < 8; i++)
    {
        for(int j = 1; j < 9; j++)
        {
            a << arch[i] << j << endl;
        }
    }
    a.close();
    ifstream q;
    q.open("test.in");
    getline(q, b);
    cout << b;

    return 0;
}