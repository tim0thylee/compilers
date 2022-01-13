#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct token {
    string type;
    string lexeme;
};

bool checkForSeparator(char c);
bool checkForSeparator(string c);
bool checkForKeyword(string s);
bool checkForOperator(string c);
bool checkForOperator(char c);
void testerFunction(ifstream & test, ofstream & test_output);
int integerFsm(string integerString);
token lexerChecker(string s);
token lexer();