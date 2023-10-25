#pragma once

#include <vector>
#include <string>
using namespace std;

class Implicant
{

	//this is a string which contains the boolean expression of the Implicant eg:01000,  -100-10, or 00-11
	string Bool_expression;
	//this is a vector which contains a list of the minterms the implicant consists of, eg: 18,32,50,4
	vector<int> Indexes;

	//constructor to set the starting Boolean expression and Index of the Implicant
	Implicant(string& Bool_exp, int Starting_index);
};

