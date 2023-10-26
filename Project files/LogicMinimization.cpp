#include <iostream>
#include <bitset>
#include <cmath>
#include <string>
#include <algorithm>
#include "Implicant.h"
using namespace std;
vector<char> input_data; // Create a vector to store input data (global variable)


//declare first implicant vector to be used, this vector will contains the resulted minterms, then it will be sorted and use in other functions
vector<Implicant> imp1;
//a vector which contains the column if implicants generated by matching column 1
vector<Implicant> imp2;
//a vector which contains prime implicants
vector<Implicant> prime_imp;
//a vector which contains essentail prime implicants
vector<Implicant> ess_prime_imp;
//a vector which contains all minterm but can be modified to help with finding the minterms that didn't match with any implicant
vector<int> minterm_copy;
int get_1_count(string binary);
void add_minterm_to_prime_list(int minterm_toadd);

bool is_the_expression_valid(string expression)
{
    int the_open_parentheses = 0; // Initialize a count for open parentheses
    for (char character_c : expression)
    {
        if ((character_c >= 'a' && character_c <= 'z') || character_c == '\'' || character_c == '+' || character_c == '(' || character_c == ')' || character_c == ' ')
        {
            // Check if the character is a valid character in the expression
            // Valid characters: 'a' to 'z', single quote ('), plus (+), open parenthesis ('('), close parenthesis (')'), and space
            continue; // Continue to the next character
        }
        return false; // Invalid character found in the expression, return false
    }
    for (char character_c : expression)
    {
        if (character_c == '(')
        {
            the_open_parentheses++; // Increment the count for open parentheses
        }
        else if (character_c == ')')
        {
            if (the_open_parentheses > 0)
            {
                the_open_parentheses--; // Decrement the count for open parentheses when a close parenthesis is encountered
            }
            else {
                return false; // Unbalanced parentheses (no matching open parenthesis), return false
            }
        }
    }
    if (the_open_parentheses != 0)
    {
        return false; // Unbalanced parentheses (open parentheses left without matching close parentheses), return false
    }
    return true; // Expression is valid (no invalid characters or unbalanced parentheses), return true
}
vector<vector<int>> compute_the_truth_table(string expression)
{
    vector<vector<int>> truth_table; // Initialize a 2D vector to store the truth table
    vector<char> input_variables; // Initialize a vector to store unique input variables
    int variable_count = 0; // Initialize a count for unique input variables
    // Iterate through the characters in the expression to identify unique input variables
    for (char character_c : expression)
    {
        // Check if the character is a lowercase letter and not already in the input_variables vector
        if (character_c >= 'a' && character_c <= 'z' && find(input_variables.begin(), input_variables.end(), character_c) == input_variables.end())
        {
            variable_count++; // Increment the count of unique input variables
            input_variables.push_back(character_c); // Add the unique input variable to the vector
        }
    }
    sort(input_variables.begin(), input_variables.end()); // Sort the input variables in lexicographic order
    int number_of_rows = pow(2, variable_count); // Calculate the number of rows in the truth table
    for (int i = 0; i < number_of_rows; i++)
    {
        vector<int> row; // Initialize a vector to represent a row in the truth table
        bitset<8> bits(i); // Convert the row index to binary
        int j = 0;
        for (char ch : input_variables)
        {
            if (ch >= 'a' && ch <= 'z')
            {
                row.push_back(bits[variable_count - j - 1]); // Extract the binary values for input variables
                j++;
            }
            else if (ch == '\'')
            {
                row.back() = row.back() == 1 ? 0 : 1; // Handle negation (complement) for the last variable in the row
            }
        }
        bool the_result = true; // Initialize the result for the current row as true

        for (int a = 0; a < expression.length(); a++)
        {
            char character_c = expression[a];

            if (character_c >= 'a' && character_c <= 'z')
            {
                int index = find(input_variables.begin(), input_variables.end(), character_c) - input_variables.begin();
                if (a < expression.length() - 1 && expression[a + 1] == '\'')
                {
                    the_result = the_result && (row[index] == 0 ? true : false); // Handle the negated input variable
                }
                else
                {
                    the_result = the_result && (row[index] == 1 ? true : false); // Handle the non-negated input variable
                }
            }
            else if (character_c == '+')
            {
                if (the_result == true)
                {
                    break; // If the result is true, no need to continue evaluating the OR operation
                }
                the_result = the_result || true; // Implement the OR operation
            }
        }
        row.push_back(the_result ? 1 : 0); // Append the result (1 or 0) to the row
        truth_table.push_back(row); // Add the row to the truth table
    }
    cout << "Hello, The Truth Table for your expression is:" << endl;
    // Display the truth table
    for (int i = 0; i < truth_table.size(); i++)
    {
        for (int j = 0; j < truth_table[i].size(); j++)
        {
            if (truth_table[i][j] == -1)
            {
                cout << "-";
            }
            else {
                cout << truth_table[i][j];
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
    return truth_table; // Return the computed truth table
}
string generate_sum_of_product_SOP_expression(vector<vector<int>>& truth_table, vector<char>& input_variables)
{
    string SOP_exp; // Initialize a string to store the SOP expression
    for (int i = 0; i < truth_table.size(); i++) // Iterate through the rows of the truth table
    {
        if (truth_table[i].back() == 1) // Check if the output is true (1) for the current row
        {
            string t; // Initialize a string to store a product term (minterm)
            for (int j = 0; j < input_variables.size(); j++) // Iterate through the input variables
            {
                if (truth_table[i][j] == 1) // If the variable is true (1) for the current row
                {
                    t += input_variables[j]; // Append the variable to the term
                }
                else if (truth_table[i][j] == 0) // If the variable is false (0) for the current row
                {
                    t += input_variables[j]; // Append the variable to the term
                    t += "'"; // Add the negation (complement) to the term
                }
            }
            if (!SOP_exp.empty())
            {
                SOP_exp += " + "; // If not the first term, add a '+' before the term
            }

            SOP_exp += t; // Append the product term (minterm) to the SOP expression
        }
    }
    return SOP_exp; // Return the generated SOP expression
}
string generate_product_of_sum_POS_expression(vector<vector<int>>& truth_table, vector<char>& input_variables)
{
    string POS_exp; // Initialize a string to store the POS expression
    for (int i = 0; i < truth_table.size(); i++) // Iterate through the rows of the truth table
    {
        if (truth_table[i].back() == 0) // Check if the output is false (0) for the current row
        {
            string t; // Initialize a string to store a sum term (maxterm)
            for (int j = 0; j < input_variables.size(); j++) // Iterate through the input variables
            {
                if (truth_table[i][j] == 0) // If the variable is false (0) for the current row
                {
                    t += input_variables[j]; // Append the variable to the term
                }
                else if (truth_table[i][j] == 1)
                {
                    t += input_variables[j]; // Append the variable to the term
                    t += "'"; // Add the negation (complement) to the term
                }
            }
            if (!POS_exp.empty())
            {
                POS_exp += " + "; // If not the first term, add a '+' before the term
            }
            POS_exp += t; // Append the sum term (maxterm) to the POS expression
        }
    }
    return POS_exp; // Return the generated POS expression
}
// Function to convert a binary string to an integer
int convert_binary_string_to_integer(const string& b_s)
{
    int the_result = 0; // Initialize the result as 0
    // Iterate through each character in the binary string
    for (char bit : b_s)
    {
        // Convert the binary string to an integer by multiplying the result by 2 and adding the numeric value of the current character
        the_result = the_result * 2 + (bit - '0');
    }
    return the_result; // Return the converted integer
}
// Function to generate a list of minterms from the truth table
vector<int> generate_the_minterms(vector<vector<int>>& truth_table)
{
    vector<int> mints; // Initialize a vector to store minterms
    for (int i = 0; i < truth_table.size(); i++) // Iterate through the rows of the truth table
    {
        if (truth_table[i].back() == 1) // Check if the output is true (1) for the current row
        {
            // Convert the binary representation of the row index to an integer and add it to the list of minterms
            int mint = convert_binary_string_to_integer(bitset<8>(i).to_string());
            mints.push_back(mint);
        }
    }
    return mints; // Return the list of minterms
}
// Function to generate a list of maxterms from the truth table
vector<int> generate_the_maxterms(vector<vector<int>>& truth_table)
{
    vector<int> maxts; // Initialize a vector to store maxterms
    for (int i = 0; i < truth_table.size(); i++) // Iterate through the rows of the truth table
    {
        if (truth_table[i].back() == 0) // Check if the output is false (0) for the current row
        {
            // Convert the binary representation of the row index to an integer and add it to the list of maxterms
            int maxt = convert_binary_string_to_integer(bitset<8>(i).to_string());
            maxts.push_back(maxt);
        }
    }
    return maxts; // Return the list of maxterms
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








//Function to check if two minterms differ by only one bit
bool the_difference_is_one_bit(string mint_1, string mint_2)
{
    int index = 0; // XOR the two minterms to find differing bits
    int count_of_diffs = 0; // Initialize a count of differing bits
    while (index < mint_1.size() && count_of_diffs <= 1)
    {
        if (mint_1[index] == '0' || mint_1[index] == '1')
        {
            if (mint_2[index] != mint_1[index]) {
                count_of_diffs++;
            }
        }
        else if (mint_1[index] == '-')
        {
            if (mint_2[index] != '-') {
                count_of_diffs = 10;
            }
        }
        // Count the number of differing bits
        index++; // Shift right to check the next bit
    }
    return count_of_diffs <= 1; // Return true if there is only one differing bit, indicating the minterms can be merged
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
    for (int x = length - 1; x >= 0; x--) {
        //check if the decimal is bigger than the current size, if yes that means its binart must be 1
        if (decimal >= pow(2, x))
        {
            //reduce the decimal value by the added binary representation
            decimal = decimal - pow(2, x);
            //add 1 to the binary representation
            binary_rep = binary_rep + '1';
        }
        //if not, then it must be 0 and no reduction happens
        else {
            binary_rep = binary_rep + '0';
        }
    }
    //return desired binary representation string
    return binary_rep;
}

//a function to scan a column and produce the next column of implicants
void compare_loop_imp()
{
    //clear previous elements of the implicant vector as they aren't needed
    imp2.clear();
    //a variable to store difference in number of 1s between two implicants
    int temp = 0;
    //a string to store the new binary generated from 2 matching implicants
    string new_bin;
    //2 temporary variables to store the boolean expression of 2 implicants
    string temp2;
    string temp3;
    //loop over each implicant
    for (int x = 0; x < imp1.size() - 1; x++)
    {
        //loop over each implicant again to compare two implicants with each other
        for (int y = x + 1; y < imp1.size(); y++)
        {
            new_bin = "";
            //set temp2 to the boolean expression of the minterm we are at
            temp2 = imp1[x].Get_Boolexp();
            //set temp3 to the boolean expression of the minterm we will compare with
            temp3 = imp1[y].Get_Boolexp();
            //set temp to the difference in ones between the two minterms, this is very important to reduce complexity
            temp = (get_1_count(temp3) - (get_1_count(temp2)));
            //check if difference in ones is 1, in which case we continue
            if (temp == 1)
            {
                //check if difference in bits is just 1 number that isn't -
                if (the_difference_is_one_bit(temp2, temp3) == 1) {
                    //set the new_bin to the newly generated implicant by looping over both implicants and on the difference add a '-'
                    for (int i = 0; i < temp2.length(); i++) {
                        if (temp2[i] == temp3[i])
                        {
                            new_bin = new_bin + temp2[i];
                        }
                        else {
                            new_bin = new_bin + '-';
                        }
                    }
                    //create a new implicant with the new binary value
                    Implicant implicant(new_bin);
                    //add the minterms of both implicants to the newly generated implicant
                    for (int i = 0; i < imp1[x].Get_Indexes_Size(); i++)
                    {
                        implicant.Add_Indexes(imp1[x].Get_Indexes(i));
                    }
                    for (int i = 0; i < imp1[y].Get_Indexes_Size(); i++)
                    {
                        implicant.Add_Indexes(imp1[y].Get_Indexes(i));
                    }
                    //push the newly generated implicant to a new implicant vector
                    imp2.push_back(implicant);
                }
            }
            //since implicants are sorted, once it reaches difference two, that means it will never find matching implicants anymore, so break
            else if (temp >= 2)
            {
                break;
            }
        }
    }

}

//a function that gets essential prime implicants
void get_essential_prime_implicants()
{
    //index to determine the implicant which could be essential prime
    int ess_index = 0;
    //loop over all minterms
    for (int x = 0; x < minterm_copy.size(); x++)
    {
        //couter number of times a minterm is repeated
        int ctr = 0;
        //loop over prime implicants
        for (int y = 0; y < prime_imp.size(); y++)
        {
            //loop over the minterms the implicant have
            for (int q = 0; q < prime_imp[y].Get_Indexes_Size(); q++)
            {
                //if minterm exists increase counter
                if (prime_imp[y].Get_Indexes(q) == minterm_copy[x])
                {
                    ess_index = y;
                    ctr++;
                    //if counter is 2, this means that the minterm exists in two or more implicants so it can't be essentail
                    if (ctr >= 2)
                    {
                        //break the loop 
                        y = prime_imp.size();
                        break;
                    }
                }
            }
            //check if the loop is about to exist, this means that if counter is 1 then there exists a prime implicant which is essentail as it has that minterm
            if ((y + 1) == prime_imp.size())
            {
                if (ctr == 1)
                {
                    //loop over minterms the marked prime implicant have
                    for (int q = 0; q < prime_imp[ess_index].Get_Indexes_Size(); q++)
                    {
                        //check if the minterm already exists, if it does, remove the minterm using its value from minterm_copy vector
                        auto ele = find(minterm_copy.begin(), minterm_copy.end(), prime_imp[ess_index].Get_Indexes(q));
                        //check if minterm exists in minterm_copy, if yes then remove it and adjust the loop
                        if (ele != minterm_copy.end())
                        {
                            //remove minterm by value
                            minterm_copy.erase(remove(minterm_copy.begin(), minterm_copy.end(), prime_imp[ess_index].Get_Indexes(q)), minterm_copy.end());
                            x--;
                        }
                    }
                    //found essentail prime, add to to ess_prime_imp vector
                    ess_prime_imp.push_back(prime_imp[ess_index]);

                }
            }
        }
    }
}
//check if there are any prime implicants in the current iteration
void check_for_prime()
{
    //loop over minterms
    for (int x = 0; x < minterm_copy.size(); x++)
    {
        //if the next column wasn't generated (all implicants became prime)
        if (imp2.size() == 0)
        {
            //add the minterm to the list of primes
            add_minterm_to_prime_list(minterm_copy[x]);
            //remove the minterm from the vector
            minterm_copy.erase(minterm_copy.begin() + x);
            x--;
        }
        else
        {
            //if column was generated, check if the minterm was included in the next column
            for (int y = 0; y < imp2.size(); y++)
            {
                //loop over the implicant minterms
                for (int q = 0; q < imp2[q].Get_Indexes_Size(); q++)
                {
                    //check if minterm exists, meaning implicants that have it will not be prime
                    if (imp2[y].Get_Indexes(q) == minterm_copy[x])
                    {
                        //stop the 2 loops
                        y = imp2.size();
                        break;
                    }
                }
                //check if the loop ended, if loop ended natuerally that means the minterm doesn't exist hence implicants that have it must be marked as prime
                if ((y + 1) == imp2.size())
                {
                    //mark the implicant as prime
                    add_minterm_to_prime_list(minterm_copy[x]);
                    //reset the loop and remove minterm element
                    minterm_copy.erase(minterm_copy.begin() + x);
                    x--;
                }
            }
        }
    }
}
//add the implicant to the prime implicant vector if it has the passed minterm
void add_minterm_to_prime_list(int minterm_toadd)
{
    //loop over implicants in first column
    for (int y = 0; y < imp1.size(); y++)
    {
        //loop over minterms in implicant
        for (int q = 0; q < imp1[y].Get_Indexes_Size(); q++)
        {
            //bool to detect repeat prime implicants
            bool repeated = false;
            //check if the implicant has the minterm sent to the function
            if (imp1[y].Get_Indexes(q) == minterm_toadd)
            {
                //check if prime implicant vector has size more than 0
                if (prime_imp.size() > 0)
                {
                    //loop over current prime implicants
                    for (int z = 0; z < prime_imp.size(); z++)
                    {
                        //check if prime implicant already exists using their boolean expression
                        if (prime_imp[z].Get_Boolexp() == imp1[y].Get_Boolexp())
                        {
                            //mark repeated as true and stop loop
                            repeated = true;
                            q = imp1[y].Get_Indexes_Size();
                            break;
                        }
                    }
                    //if not repeated then add to prime implicant vector
                    if (repeated == false)
                    {
                        prime_imp.push_back(imp1[y]);
                    }
                }
                else
                {
                    //add to prime implicant vector
                    prime_imp.push_back(imp1[y]);
                    break;
                }
            }
        }
    }
}

//a utility function that uses sort alogrithm
bool compare_onescount(Implicant& impli1, Implicant& impli2) {
    if (get_1_count(impli1.Get_Boolexp()) < get_1_count(impli2.Get_Boolexp()))
    {
        return true;
    }
    return false;
}

//a utility function that count the number of 1s in a binary string
int get_1_count(string binary) {

    int counter = 0;
    for (int x = 0; x < binary.size(); x++)
    {
        if (binary[x] == '1')
        {
            counter++;
        }
    }
    return counter;
}
int main()
{
    string logic_expression; // Declare a string to store the user's input boolean expression
    cout << "Hello, please enter a boolean expression (SoP or PoS): "; // Display a prompt for user input
    getline(cin, logic_expression); // Read the user's input into the logic_expression variable
    if (is_the_expression_valid(logic_expression)) // Check if the entered expression is valid
    {
        cout << "congrats, your expression is a valid expression!" << endl; // Display a success message
        input_data.clear(); // Clear the vector that stores input variables

        // Iterate through the characters in the input expression to extract unique input variables
        for (char c : logic_expression)
        {
            // Check if the character is a letter and not already in the input_data vector
            if (isalpha(c) && find(input_data.begin(), input_data.end(), c) == input_data.end())
            {
                input_data.push_back(c); // Add the unique input variable to the vector
            }
        }
        vector<vector<int>> truthTable = compute_the_truth_table(logic_expression); // Compute the truth table for the entered expression
        cout << "the truth table of the entered boolean expression is:" << endl;
        // Display the truth table
        for (int i = 0; i < truthTable.size(); i++)
        {
            for (int j = 0; j < truthTable[i].size(); j++)
            {
                if (truthTable[i][j] == -1)
                {
                    cout << "-";
                }
                else
                {
                    cout << truthTable[i][j];
                }
                cout << " ";
            }
            cout << endl;
        }
        string sopExpression = generate_sum_of_product_SOP_expression(truthTable, input_data); // Generate the Sum of Products (SOP) expression
        cout << "the canonical SOP expression is: " << sopExpression << endl; // Display the SOP expression
        string posExpression = generate_product_of_sum_POS_expression(truthTable, input_data); // Generate the Product of Sum (POS) expression
        cout << "the canonical POS expression is: " << posExpression << endl; // Display the POS expression
        vector<int> minterms = generate_the_minterms(truthTable); // Generate a list of minterms
        vector<int> maxterms = generate_the_maxterms(truthTable); // Generate a list of maxterms
        cout << "Minterms are: ";
        // Display the list of minterms
        for (int minterm : minterms)
        {
            cout << minterm << " ";
        }
        cout << endl;
        cout << "Maxterms are: ";

        // Display the list of maxterms
        for (int maxterm : maxterms)
        {
            cout << maxterm << " ";
        }
        cout << endl;
    }
    else
    {
        cout << "Sorry, Invalid expression!!! Please enter a valid Boolean expression." << endl; // Display an error message for an invalid expression
    }
    return 0; // Return 0 to indicate successful program execution
}
