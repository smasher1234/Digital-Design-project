#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to convert decimal to binary
string decimalToBinary(int n, int num_vars) {
    string binary = "";
    for (int i = 0; i < num_vars; i++) {
        binary = char('0' + (n % 2)) + binary;
        n = n / 2;
    }
    return binary;
}

// Function to find the number of ones in a binary string
int countOnes(string binary) {
    int count = 0;
    for (char bit : binary) {
        if (bit == '1') count++;
    }
    return count;
}

// Function to find the XOR of two binary strings 1001 & 0011 = 1010
string XOR(string a, string b) {
    string result = "";
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b[i]) result += '0';
        else result += '1';
    }
    return result;
}


// Function to perform Step 2 of the heuristic
vector<string> step2(vector<string> implicants, int num_vars) {
    vector<string> new_implicants;

    for (int i = 0; i < implicants.size(); i++) 
    {
        for (int j = i + 1; j < implicants.size(); j++) 
        {
            if (countOnes(XOR(implicants[i], implicants[j])) == 1) 
            {
                string new_implicant = implicants[i];
                int diff_position = -1;

                for (int k = 0; k < num_vars; k++) 
                {
                    if (implicants[i][k] != implicants[j][k])
                    {
                        diff_position = k;
                        break;
                    }
                }

                new_implicant[diff_position] = '-';
                new_implicants.push_back(new_implicant);
            }
        }
    }
    
    return new_implicants;
}

// Function to perform Step 3 of the heuristic
string step3(vector<string> implicants, int num_vars) {
    string minimized_expr = "";

    for (string implicant : implicants) {
        for (int i = 0; i < num_vars; i++) {
            if (implicant[i] == '0') {
                minimized_expr += ('A' + i);
                minimized_expr += '\'';
            }
            else if (implicant[i] == '1') {
                minimized_expr += ('A' + i);

            }
        }
        minimized_expr += " + ";
    }

    // Remove the trailing " + "
    minimized_expr = minimized_expr.substr(0, minimized_expr.size() - 3);

    return minimized_expr;
}
vector<int> minterms;
vector<string> implicants;

int main() {
    int num_vars;
    //setup example
    num_vars = 4;

    minterms.push_back(0);
    minterms.push_back(2);
    minterms.push_back(5);
    minterms.push_back(6);
    minterms.push_back(7);
    minterms.push_back(8);
    minterms.push_back(10);
    minterms.push_back(12);
    minterms.push_back(13);
    minterms.push_back(14);
    minterms.push_back(15);

    implicants.push_back("0000");
    implicants.push_back("0010");
    implicants.push_back("0101");
    implicants.push_back("0110");
    implicants.push_back("0111");
    implicants.push_back("1000");
    implicants.push_back("1010");
    implicants.push_back("1100");
    implicants.push_back("1101");
    implicants.push_back("1110");
    implicants.push_back("1111");

    //end example
    
    
    step2(implicants, num_vars);

    int v = 4;
    while (v != 0) 
    {
        v--;
        vector<string> new_implicants = step2(implicants, num_vars);
        if (new_implicants.empty()) break;
        //implicants.insert(implicants.end(), new_implicants.begin(), new_implicants.end());
        implicants = new_implicants;
    }

     string minimized_expr = step3(implicants, num_vars);

    cout << "Minimized Boolean Expression: " << minimized_expr << endl;





    return 0;
}