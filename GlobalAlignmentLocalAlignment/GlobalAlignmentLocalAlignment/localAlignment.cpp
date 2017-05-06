#include "cell.h"
#include<iostream>
#include <fstream>
#include <string>	//getline
#include "LocalAlignment.h"
#include <algorithm> 

using namespace std;

void LocalAlignment::localMain(std::string s1, std::string s2, std::string* parameters)
{
	cout << "\n\nLocal Alignment\n";
	Cell** T = LocalAlignment::createMatrixLocal(s1, s2, parameters);
	Cell alignment;
	LocalAlignment local;

	T = LocalAlignment::optimalScore(T, s1, s2, parameters, local);
	cout << "\n\nLocal Alignment\n";
	LocalAlignment::alignmentLocal(T, s1, s2, parameters, local);
	return;
}

void LocalAlignment::alignmentLocal(Cell **T, std::string s1, std::string s2, std::string* parameters, LocalAlignment &local)
{
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);

	Cell alignment;

	int i = local.maxI, m = local.maxI;
	int j = local.maxJ, n = local.maxJ;
	int count = m + n;

	char **align = (char**)calloc(3, sizeof(char*));
	for (int i = 0; i < 3; i++)
		align[i] = (char*)calloc(m + n, sizeof(char));
	char direction;

	float current = max(max(T[i][j].S, T[i][j].I), T[i][j].D);
	if (current == T[i][j].S) direction = 'S';
	if (current == T[i][j].D) direction = 'D';
	if (current == T[i][j].I) direction = 'I';

	while (i > 0 && j > 0 && current != 0)
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
			if (s1[i - 1] == s2[j - 1])
				align[1][count] = '|';
			else
				align[1][count] = ' ';

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

	//calculating counts of match,mismatch, gaps and opening gaps
	int countMatch = 0, countMismatch = 0, countOpeningGap = 0, countGap = 0;
	for (int loop = count; loop <= m + n; loop++)
	{
		if (align[0][loop] == align[2][loop] && align[0][loop] != '\0' && align[2][loop] != '\0')
			countMatch++;
		if (align[0][loop] != align[2][loop] && align[0][loop] != '_'
			&& align[2][loop] != '_' && align[0][loop] != '\0' && align[2][loop] != '\0')
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
				countOpeningGap++;
		}
	}

	std::cout << "\nOUTPUT:\n\n";

	std::cout << "Parameters: " << "\nMatch: " << m_a << "\tMisMatch: " << m_i << "\th: " << h << "\tg: " << g << endl;
	std::cout << "Sequence 1: s1\t length: " << s1.length() << endl;
	std::cout << "Sequence 2: s2\t length: " << s2.length() << endl;

	alignment.printAlignment(align, count, m, n);

	std::cout << "\n\nLocal Optimal Score: " << (countMatch*m_a + countMismatch*m_i + countOpeningGap*h + countGap*g) << endl;
	std::cout << "\nNumber of Matches: " << countMatch << "\nMismatches: " << countMismatch << "\nOpening Gaps: " << countOpeningGap << "\nGaps: " << countGap << endl;;
	std::cout << "\nIdentities: " << ((countMatch * 100) / (countMatch + countMismatch + countOpeningGap + countGap)) << "%" << endl;
	std::cout << "Gaps: " << (countGap * 100) / (countMatch + countMismatch + countOpeningGap + countGap) << "%" << endl;

}
Cell** LocalAlignment::createMatrixLocal(std::string s1, std::string s2, std::string* parameters)
{
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);

	const int m = s1.length();
	const int n = s2.length();

	Cell** T = new Cell*[m + 1];
	for (int i = 0; i < m + 1; i++)
		T[i] = new Cell[n + 1];

	//initializations

	T[0][0].S = 0;
	T[0][0].D = -INFINITY;
	T[0][0].I = -INFINITY;

	for (int i = 1; i < m + 1; i++)
	{
		T[i][0].S = -INFINITY;
		T[i][0].I = -INFINITY;
		T[i][0].D = 0;
	}

	for (int j = 1; j < n + 1; j++)
	{
		T[0][j].S = -INFINITY;
		T[0][j].I = 0;
		T[0][j].D = -INFINITY;
	}
	return T;
}
Cell** LocalAlignment::optimalScore(Cell **T, std::string s1, std::string s2, std::string* parameters, LocalAlignment &local)
{
	Cell alignment;
	float m_a = stof(parameters[0]);
	float m_i = stof(parameters[1]);
	float h = stof(parameters[2]);
	float g = stof(parameters[3]);
	const int m = s1.length();
	const int n = s2.length();
	float temp, zero = 0.0;

	//calculating matrix and finding max scoring cell
	for (int i = 1; i < m + 1; i++)
	{
		for (int j = 1; j < n + 1; j++)
		{
			//Substitution
			temp = max(max(T[i - 1][j - 1].S, T[i - 1][j - 1].I), T[i - 1][j - 1].D) + alignment.substitution(s1[i - 1], s2[j - 1], m_a, m_i);
			T[i][j].S = max(temp, zero);
			//Delete
			temp = max((max(T[i - 1][j].S, T[i - 1][j].I) + h + g), (T[i - 1][j].D + g));
			T[i][j].D = max(temp, zero);
			//Insert
			temp = max((max(T[i][j - 1].S, T[i][j - 1].D) + h + g), (T[i][j - 1].I + g));
			T[i][j].I = max(temp, zero);

			float score = max(max(T[i][j].S, T[i][j].D), T[i][j].I);
			if (score>local.maxScore)
			{
				local.maxScore = score;
				local.maxI = i;
				local.maxJ = j;
			}
		}
	}
	cout << "MaxScore: " << local.maxScore;
	return T;
}

