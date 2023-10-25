// Project files.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
//#include <Implicant.h>

string convert_decimal_to_binary(int decimal, int &length);
int main()
{
    //test random decimal and its value
    int deci;
    int size;
    cin >> deci;
    cin >> size;
    cout << convert_decimal_to_binary(deci, size);
}




//a utility function that converts a decimal (minterm) to a binary
string convert_decimal_to_binary(int decimal, int& length)
{
    //check if the decimal is larger than the size
    if (decimal >= pow(2, length)) {

        return "Error: Decimal too large or length is incorrect";
    }
    //create a binary_rep string to store the output
    string binary_rep = "";
    //loop inversely on the decimal
    for (int x = length-1; x >= 0; x--) {
        //check if the decimal is bigger than the current size, if yes that means its binart must be 1
        if (decimal >= pow(2, x))
        {
            //reduce the decimal value by the added binary representation
            decimal = decimal - pow(2, x);
            //add 1 to the binary representation
            binary_rep = binary_rep+ '1';
        }
        //if not, then it must be 0 and no reduction happens
        else {
            binary_rep = binary_rep+ '0';
        }
    }
    //return desired binary representation string
    return binary_rep;
}
