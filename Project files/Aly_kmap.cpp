#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

void generateKMap(int numVariables, const vector<int>& minterms)
{

    int numRows = 0;
    int numCols = 0;

    if (numVariables == 1)
    {
        numRows = 1;
        numCols = 1;
    }

    else if (numVariables == 2)
    {
        numRows = 2;
        numCols = 2;
    }

    else if (numVariables == 3)
    {
        numRows = 2;
        numCols = 4;
    }

    else if (numVariables == 4)
    {
        numRows = 4;
        numCols = 4;
    }

    else
    {
        cout << "number of variables is more than four.";
        return;
    }

    vector<vector<int>> kMap(numRows, vector<int>(numCols, -1));

    if (numVariables == 1 || numVariables ==  2)
    {
        for (int minterm : minterms)
        {
            int row = minterm / numCols;
            int col = minterm % numCols;
            kMap[row][col] = minterm;
        }

    }

    else
    {
        for (int minterm : minterms)
        {
            if (minterm == 0)
            {
                int row = 0;
                int col = 0;
                kMap[row][col] = minterm;
            }

            if (minterm == 1)
            {
                int row = 0;
                int col = 1;
                kMap[row][col] = minterm;
            }

            if (minterm == 2)
            {
                int row = 0;
                int col = 3;
                kMap[row][col] = minterm;
            }

            if (minterm == 3)
            {
                int row = 0;
                int col = 2;
                kMap[row][col] = minterm;
            }

            if (minterm == 4)
            {
                int row = 1;
                int col = 0;
                kMap[row][col] = minterm;
            }

            if (minterm == 5)
            {
                int row = 1;
                int col = 1;
                kMap[row][col] = minterm;
            }

            if (minterm == 6)
            {
                int row = 1;
                int col = 3;
                kMap[row][col] = minterm;
            }

            if (minterm == 7)
            {
                int row = 1;
                int col = 2;
                kMap[row][col] = minterm;
            }

            if (minterm == 8)
            {
                int row = 3;
                int col = 0;
                kMap[row][col] = minterm;
            }

            if (minterm == 9)
            {
                int row = 3;
                int col = 1;
                kMap[row][col] = minterm;
            }

            if (minterm == 10)
            {
                int row = 3;
                int col = 3;
                kMap[row][col] = minterm;
            }

            if (minterm == 11)
            {
                int row = 3;
                int col = 2;
                kMap[row][col] = minterm;
            }

            if (minterm == 12)
            {
                int row = 2;
                int col = 0;
                kMap[row][col] = minterm;
            }

            if (minterm == 13)
            {
                int row = 2;
                int col = 1;
                kMap[row][col] = minterm;
            }

            if (minterm == 14)
            {
                int row = 2;
                int col = 3;
                kMap[row][col] = minterm;
            }

            if (minterm == 15)
            {
                int row = 2;
                int col = 2;
                kMap[row][col] = minterm;
            }
        }
    }
 
    cout << "K Map:\n";

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            if (kMap[i][j] != -1)
            {
                cout << "1" << " ";
            }
            else
            {
                cout << "0 ";
            }
        }
        cout << "\n";
    }
}

int main()
{
    int numVariables;
    vector<int> minterms;

    cout << "Enter the number of variables: ";
    cin >> numVariables;

    cout << "Enter the minterms (enter -1 to end input):\n";
    int minterm;
    while (true) 
    {
        cin >> minterm;
        if (minterm == -1) 
        {
            break;
        }
        minterms.push_back(minterm);
    }

    generateKMap(numVariables, minterms);

    return 0;
}