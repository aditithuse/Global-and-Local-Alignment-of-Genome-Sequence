#pragma once
#include "cell.h"
class LocalAlignment
{
	int maxI;
	int maxJ;
	int maxScore;

public:

	void localMain(std::string s1, std::string s2, std::string* parameters);
	Cell** createMatrixLocal(std::string s1, std::string s2, std::string* parameters);
	Cell** optimalScore(Cell **T, std::string s1, std::string s2, std::string* parameters, LocalAlignment &local);
	//void scoreMatrixPrint(Cell** T, std::string s1, std::string s2);
	//void alignmentLocal(Cell **T, std::string s1, std::string s2);
	//int substitution(char a, char b, int m_a, int m_i);
	void alignmentLocal(Cell **T, std::string s1, std::string s2, std::string* parameters, LocalAlignment &local);

	LocalAlignment()
	{
		maxI = 0;
		maxJ = 0;
		maxScore = 0;
	}

};
