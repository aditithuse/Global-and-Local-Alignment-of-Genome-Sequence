#include "cell.h"
#include<iostream>
#include <fstream>
#include <string>	//getline
#include "GlobalAlignment.h"
#include <algorithm> 

#define INFINITY = 999

using namespace std;

void GlobalAlignment::globalmain(std::string s1, std::string s2, std::string* parameters)
{
	std::cout << "\nGlobal Alignment\n\n";
	Cell alignment;
	Cell** T = initializeMatrixGlobal(s1, s2, parameters);
	T = optimalScore(T, s1, s2, parameters);

	alignment.scoreMatrixPrint(T, s1, s2);
	alignmentGlobal(T, s1, s2, parameters);
}

Cell** GlobalAlignment::initializeMatrixGlobal(std::string s1, std::string s2, std::string* parameters)
{
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);

	//length of s1 and s2
	const int m = s1.length();
	const int n = s2.length();

	Cell** T = new Cell*[m + 1];
	for (int i = 0; i < m + 1; i++)
		T[i] = new Cell[n + 1];

	//initializations
	T[0][0].S = 0;
	T[0][0].D = -999;
	T[0][0].I = -999;

	for (int i = 1; i < m + 1; i++)
	{
		T[i][0].S = -999;
		T[i][0].I = -999;
		T[i][0].D = h + (i * g);
	}

	for (int j = 1; j < n + 1; j++)
	{
		T[0][j].S = -999;
		T[0][j].I = h + (j * g);
		T[0][j].D = -999;
	}
	return T;
}

Cell** GlobalAlignment::optimalScore(Cell** T, std::string s1, std::string s2, std::string* parameters)
{
	Cell alignment;
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);
	const int m = s1.length();
	const int n = s2.length();
	for (int i = 1; i < m + 1; i++)
	{
		for (int j = 1; j < n + 1; j++)
		{
			//Substitution
			T[i][j].S = max(max(T[i - 1][j - 1].S, T[i - 1][j - 1].I), T[i - 1][j - 1].D) + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);

			//Delete
			T[i][j].D = max((max(T[i - 1][j].S, T[i - 1][j].I) + h + g), (T[i - 1][j].D + g));

			//Insert
			T[i][j].I = max((max(T[i][j - 1].S, T[i][j - 1].D) + h + g), (T[i][j - 1].I + g));

		}
	}

	std::cout << "Optimal score before alignment: " << max(max(T[m][n].S, T[m][n].D), T[m][n].I) << endl;
	return T;
}

void GlobalAlignment::alignmentGlobal(Cell **T, std::string s1, std::string s2, std::string* parameters)
{
	Cell alignment;
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);

	int i = s1.length(), m = s1.length();
	int j = s2.length(), n = s2.length();
	int count = m + n;
	int countMatch = 0, countMismatch = 0, countOpeningGap = 0, countGap = 0;
	char **align = (char**)calloc(3, sizeof(char*));
	for (int i = 0; i < 3; i++)
		align[i] = (char*)calloc(m + n, sizeof(char));
	char direction;

	float current = max(max(T[i][j].S, T[i][j].D), T[i][j].I);
	if (current == T[i][j].S) direction = 'S';
	if (current == T[i][j].D) direction = 'D';
	if (current == T[i][j].I) direction = 'I';

	while (i > 0 && j > 0)
	{
		float sTemp;
		float dTemp;
		float iTemp;

		if (T[i][j].D == current && direction == 'D')
		{
			align[0][count] = s1[i - 1];
			align[1][count] = ' ';
			align[2][count] = '_';

			sTemp = T[i - 1][j].S + h + g;
			iTemp = T[i - 1][j].I + h + g;
			dTemp = T[i - 1][j].D + g;

			if (T[i][j].D == dTemp)
			{
				current = T[i - 1][j].D;
				direction = 'D';
			}
			else if (T[i][j].D == iTemp)
			{
				current = T[i - 1][j].I;
				direction = 'I';
			}
			else if (T[i][j].D == sTemp)
			{
				current = T[i - 1][j].S;
				direction = 'S';
			}
			i--;
			count--;
		}
		else if (T[i][j].S == current && direction == 'S')
		{
			align[0][count] = s1[i - 1];
			align[2][count] = s2[j - 1];
			if (s1[i - 1] == s2[j - 1]) {
				align[1][count] = '|';
			}

			else {
				align[1][count] = ' ';
			}

			sTemp = T[i - 1][j - 1].S + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			iTemp = T[i - 1][j - 1].I + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			dTemp = T[i - 1][j - 1].D + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);

			if (T[i][j].S == sTemp)
			{
				current = T[i - 1][j - 1].S;
				direction = 'S';
			}
			else if (T[i][j].S == iTemp)
			{
				current = T[i - 1][j - 1].I;
				direction = 'I';
			}
			else if (T[i][j].S == dTemp)
			{
				current = T[i - 1][j - 1].D;
				direction = 'D';
			}
			i--;
			j--;
			count--;
		}

		else if (T[i][j].I == current && direction == 'I')
		{
			align[0][count] = '_';
			align[1][count] = ' ';
			align[2][count] = s2[j - 1];

			sTemp = T[i][j - 1].S + h + g;
			iTemp = T[i][j - 1].I + g;
			dTemp = T[i][j - 1].D + h + g;

			if (T[i][j].I == sTemp)
			{
				current = T[i][j - 1].S;
				direction = 'S';
			}
			else if (T[i][j].I == iTemp)
			{
				current = T[i][j - 1].I;
				direction = 'I';
			}
			else if (T[i][j].I == dTemp)
			{
				current = T[i][j - 1].D;
				direction = 'D';
			}
			j--;
			count--;
		}
	}

	while (i > 0)
	{
		align[0][count] = s1[i - 1];
		align[1][count] = ' ';
		align[2][count] = '_';
		i--;
		count--;
	}
	while (j > 0)
	{
		align[0][count] = '_';
		align[1][count] = ' ';
		align[2][count] = s2[j - 1];
		j--;
		count--;
	}
	for (int loop = count; loop <= m + n; loop++)
	{
		if (align[0][loop] == align[2][loop] && align[0][loop] != '\0' && align[2][loop] != '\0')
			countMatch++;
		if (align[0][loop] != align[2][loop] && align[0][loop] != '_' &&
			align[2][loop] != '_' && align[0][loop] != '\0' && align[2][loop] != '\0')
			countMismatch++;
		if (align[0][loop] == '_')
		{
			countGap++;
			if (align[0][loop + 1] != '_')
				countOpeningGap++;
		}
		if (align[2][loop] == '_')
		{
			countGap++;
			if (align[2][loop + 1] != '_')
			{
				countOpeningGap++;
			}
		}
	}
	std::cout << "OUTPUT:\n\n";

	std::cout << "Parameters: " << "\nMatch: " << m_a << "\tMisMatch: " << m_i << "\th: " << h << "\tg: " << g << endl;
	std::cout << "Sequence 1: s1\t length: " << s1.length() << endl;
	std::cout << "Sequence 2: s2\t length: " << s2.length() << endl;

	alignment.printAlignment(align, count + 1, m, n);

	std::cout << "\n\n\nGlobal Optimal Score: " << (countMatch*m_a + countMismatch*m_i + countOpeningGap*h + countGap*g) << endl;
	std::cout << "\nNumber of Matches: " << countMatch << "\nMismatches: " << countMismatch << "\nOpening Gaps: " << countOpeningGap << "\nGaps: " << countGap << endl;;
	std::cout << "\nIdentities: " << ((countMatch * 100) / (countMatch + countMismatch + countOpeningGap + countGap)) << "%" << endl;
	std::cout << "Gaps: " << (countGap * 100) / (countMatch + countMismatch + countOpeningGap + countGap) << "%" << endl;
}