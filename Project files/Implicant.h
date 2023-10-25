#pragma once

#include <vector>
#include <string>
using namespace std;

class Implicant
{
	//this is a vector which contains the boolean expression of the Implicant eg:01000,  -100-10, or 00-11
	vector<string> Bool_exp;
	//this is a vector which contains a list of the minterms the implicant consists of, eg: 18,32,50,4
	vector<int> Indexes;
};

