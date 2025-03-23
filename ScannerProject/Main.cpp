#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;

//Variables
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;

//Function declarations
void addChar();
void getChar();
void getNonBlank();
int lex();

//Character Classes
enum CharacterClass {
	LETTER = 0,
    DIGIT = 1,
    UNKNOWN = 99
};

//Token Codes
enum TokenCode {
    PROGRAM = 4,
    DECL_SEC = 5,
    DECL = 6,
    ID_LIST = 7,
    ID = 8,
    STMT_SEC = 9,
    STMT = 10,
    ASSIGN = 11,
    IFSTMT = 12,
    WHILESTMT = 13,
    INPUT = 14,
    OUTPUT = 15,
    EXPR = 16,
    FACTOR = 17,
    OPERAND = 18,
    NUM = 19,
    COMP = 20,
    TYPE = 21,
    FUNCALL = 22
};

int main() {
    /*
    //replace the string with the file path of whatever C file to test for comment blocks.
    string cProjectFile = "C:\\Users\\Jacob\\Desktop\\Programming Concepts\\HW\\testingcode.c";
    ifstream file(cProjectFile);
    FileScan(cProjectFile);
    */
    return 0;

}

void addChar() {

}

void getChar() {

}

void getNonBlank() {

}