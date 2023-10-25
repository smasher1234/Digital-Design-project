#include <iostream>
#include "Implicant.h"
#include <algorithm>


vector<int> minterms;
//declare first implicant vector to be used, this vector will contains the resulted minterms, then it will be sorted and use in other functions
vector<Implicant> imp1;


//functions declaration
string convert_decimal_to_binary(int decimal, int length);
bool compareByOnesCount(Implicant& impli1, Implicant& impli2);
int get_1_count(string binary);

int main()
{
    //test with random minterms and its value
    minterms.push_back(12);
    minterms.push_back(4);
    minterms.push_back(7);
    minterms.push_back(3);
    minterms.push_back(5);
    minterms.push_back(0);
    minterms.push_back(14);
    minterms.push_back(15);

    //temp string to store decimal to binary result
    string temp = "";
    //loop over minterms
    for (int x = 0; x < minterms.size(); x++) 
    {
        //convert the current minterm (decimal) to its binary representation
        temp = convert_decimal_to_binary(minterms[x], 4);

        //check if the conversion was succesful by checking if the string has the word "Error" or not
        if (temp.find("Error") == string::npos) {

            //create an implicant object and set its binary representation to temp which has been assigned before
            Implicant implicant(temp, minterms[x]);

            //add the result ob to the first implicants vector
            imp1.push_back(implicant);
        }
    }
    //use sort algorithm to order the implicant vector imp1 by the number of ones
    sort(imp1.begin(), imp1.end(), compareByOnesCount);

    //output results
    for (int x = 0; x < imp1.size(); x++) {
        cout << imp1[x].Get_Indexes(0) << "," << imp1[x].Get_Boolexp() << endl;
    }
}

//a utility function that uses sort alogrithm
bool compareByOnesCount(Implicant& impli1, Implicant& impli2) {
    if (get_1_count(impli1.Get_Boolexp()) < get_1_count(impli2.Get_Boolexp())) 
    {
        return true;
    }
    return false;
}

//a utility function that count the number of 1s in a binary string
int get_1_count(string binary) {

    int counter=0;
    for (int x = 0; x < binary.size(); x++) 
    {     
        if (binary[x] == '1') 
        {
            counter++;
        }
    }
    return counter;
}


//a utility function that converts a decimal (minterm) to a binary
string convert_decimal_to_binary(int decimal, int length)
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



//Function to check if two minterms differ by only one bit
bool the_difference_is_one_bit(int mint_1, int mint_2)
{
    int xor_result = mint_1 ^ mint_2; // XOR the two minterms to find differing bits
    int count_of_ones = 0; // Initialize a count of differing bits
    while (xor_result)
    {
        count_of_ones += xor_result & 1; // Count the number of differing bits
        xor_result >>= 1; // Shift right to check the next bit
    }
    return count_of_ones == 1; // Return true if there is only one differing bit, indicating the minterms can be merged
}
// Function to merge two minterms into a new one
int merge_the_minterms(int mint_1, int mint_2)
{
    int done_merged = 0; // Initialize the merged minterm
    int xor_result = mint_1 ^ mint_2; // XOR the two minterms to find differing bits
    while (xor_result)
    {
        done_merged = (done_merged << 1) | 1; // Shift left and add a 1 for each differing bit
        xor_result >>= 1; // Shift right to check the next bit
    }
    return done_merged; // Return the merged minterm with differing bits combined
}