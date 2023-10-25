#include "Implicant.h"

//constructor to set the starting Boolean expression and Index of the Implicant
Implicant::Implicant(string& Bool_exp, int Starting_index) {
	// Initialize the class members with the provided values
	Bool_expression = Bool_exp;
	Indexes.push_back(Starting_index);
}