#pragma once
#include "cell.h"
class GlobalAlignment
{

public:

	void globalmain(std::string s1, std::string s2, std::string* parameters);
	Cell** initializeMatrixGlobal(std::string s1, std::string s2, std::string* parameters);
	Cell** optimalScore(Cell **T, std::string s1, std::string s2, std::string* parameters);
	void alignmentGlobal(Cell **T, std::string s1, std::string s2, std::string* parameters);

};

