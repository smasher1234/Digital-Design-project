#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;
vector<char> input_data; // Create a vector to store input data (global variable)
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

