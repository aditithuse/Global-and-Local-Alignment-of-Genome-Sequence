
#include "cell.h"
#include<cstring>
#include <fstream>
#include "GlobalAlignment.h"
#include "LocalAlignment.h"
#include <algorithm> //transform
#include <string>  //getline
#include<stdlib.h> //exit

using namespace std;

/*
<summary>Below function is to read the input fasta file and extract the strings.</summary>
<param name="inputPath">this is the file path of the input file</param>
*/
string* input(char* inputPath)
{
	string line, str = "";
	ifstream input;
	int count = 0;
	string *gene = new string[2];

	input.open(inputPath);

	if (input.is_open())
	{
		while (getline(input, line))
		{
			if (line[0] == '>')
			{
				count++;
				str = "";
			}
			else
			{
				str = str + line;
				gene[count - 1] = str;
			}
		}
		input.close();
		transform(gene[0].begin(), gene[0].end(), gene[0].begin(), toupper);
		transform(gene[1].begin(), gene[1].end(), gene[1].begin(), toupper);
		//remove_if(gene[0].begin(), gene[0].end(), isspace);
		//remove_if(gene[1].begin(), gene[1].end(), isspace);
		return gene;
	}
	else {
		std::cout << "\nUnable to open input file\n";
		exit(EXIT_FAILURE);
	}
}

/*
<summary>Below function is to read the parameter config file and extract the parameters.</summary>
<param name="parameterPath">This is the file path of the input parameter file</param>
*/
string* readParamters(char* parameterPath)
{
	string line;
	ifstream input;
	int loop = 0, count = 0;
	string value = "", param = "";
	string *parameters = new string[4];

	input.open(parameterPath);

	if (input.is_open())
	{
		while (getline(input, line))
		{
			int pos = line.find_first_of('\t');
			value = line.substr(pos + 1);
			param = line.substr(0, pos);

			if (param == "match")
				parameters[0] = value;
			if (param == "mismatch")
				parameters[1] = value;
			if (param == "h")
				parameters[2] = value;
			if (param == "g")
				parameters[3] = value;
			count++;
		}
		input.close();
		return parameters;
	}
	else {
		std::cout << "\nUnable to open parameter file\n";
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]) // executable name   input file containing both s1 and s2		0: global, 1: local
{
	GlobalAlignment *ga = new GlobalAlignment();
	LocalAlignment *la = new LocalAlignment();

	try
	{
		if (argc != 4)
		{
			std::cout << "\nNumber of command line argumets are wrong.\nCommands should be\n1. Input file containing both s1 and s2\n2. 0: global, 1: local\n3. Path to parameters config file\n\n";
			exit(EXIT_FAILURE);
		}
		//console parameters
		char* executableName = argv[0];
		char* inputPath = argv[1];
		char alignmentType = stoi(argv[2]);
		char* parameterPath = argv[3];

		string* gene = input(inputPath);
		string s1 = gene[0];
		string s2 = gene[1];
		string* parameters = readParamters(parameterPath);

		//if the alignment type=0 ie global alignment
		if (alignmentType == 0)
			ga->globalmain(s1, s2, parameters);

		//if the alignment type=1 ie local alignment
		else if (alignmentType == 1)
			la->localMain(s1, s2, parameters);
		else
		{
			std::cout << "Wrong alignment type\nPlease give alignment as 0: global, 1: local\n\n";
			exit(EXIT_FAILURE);
		}
		getchar();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}