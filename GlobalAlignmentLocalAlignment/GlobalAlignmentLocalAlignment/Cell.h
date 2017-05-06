#pragma once

#include<iostream>
#include <string>	//getline
#include <ctype.h>

#ifndef CELL_H
#define CELL_H

using namespace std;
struct Cell
{
	float S;
	float D;
	float I;

public:
	void scoreMatrixPrint(Cell** T, std::string s1, std::string s2)
	{
		for (int i = 0; i < (s1.length() + 1); i++)
		{
			for (int j = 0; j < (s2.length() + 1); j++)
			{
				std::cout << "S: " << T[i][j].S << " I: " << T[i][j].I << " D: " << T[i][j].D << " " << "\t";
			}
			std::cout << endl;
		}
	}

	int substitution(char a, char b, int m_a, int m_i)
	{
		if (a == b)
			return m_a;
		else return m_i;
	}

	void printAlignment(char **align, int count, int m, int n)
	{
		int counterS1 = 0, counterS2 = 0, counterAlign = 0, k;
		int countS1 = count, countAlign = count, countS2 = count;

		while (count < m + n + 1 && countS1 < m + n + 1 && countS2 < m + n + 1) {
			k = 0;
			std::cout << "\n\ns1\t" << counterS1 + 1 << "\t";
			for (int loop = countS1; loop < m + n + 1 && k < 70; loop++, k++)
			{
				std::cout << align[0][loop];
				countS1++;
				if (isalpha(align[0][loop]))
					counterS1++;
			}
			std::cout << "\t" << counterS1;

			k = 0;
			std::cout << "\n\t" << "\t";
			for (int loop = countAlign; loop < m + n + 1 && k < 70; loop++, k++)
			{
				std::cout << align[1][loop];
				countAlign++;
			}

			k = 0;
			std::cout << "\ns2\t" << counterS2 + 1 << "\t";
			for (int loop = countS2; loop < m + n + 1 && k < 70; loop++, k++)
			{
				std::cout << align[2][loop];
				countS2++;
				if (isalpha(align[2][loop]))
				{
					counterS2++;
				}
			}
			std::cout << "\t" << counterS2;
			count++;
		}
		return;
	}
};

#endif