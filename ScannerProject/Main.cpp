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
ifstream inputFile;

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
    FUNCALL = 22,

    //operatos and other misc
    LPAREN = 23,
    RPAREN = 24,
    ADD_OP = 25,
    SUB_OP = 26,
    MULT_OP = 27,
    DIV_OP = 28,
    COMMA = 29,
    GREATER = 30,
    LOWER = 31,
    NOTEQ = 32,
    COLON = 33,
    SEMICOLON = 34

};

int main() {
    
    //replace the string with the file path of whatever C file to test for comment blocks.
    inputFile.open("input1.txt");
    if (!inputFile.is_open()) {
		cerr << "Error opening file" << endl;
		return 1;
    }
    else {
        getChar();
        do {
            lex();
        } while (nextToken!= EOF);
    }
    
    return 0;

}

int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LPAREN;
        break;
    case ')':
        addChar();
        nextToken = RPAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case ',':
        addChar();
        nextToken = COMMA;
        break;
    case ':':
        addChar();
        nextToken = COLON;
        break;
    case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

void addChar() {
    //Check if lexeme is within bounds
    if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	} else {
		cout << "Error - lexeme is too long \n";
	}
}

void getChar() {
    //Get the next character from the input file
    if(inputFile.get(nextChar)) {
        if (isalpha(nextChar) || nextChar == '_')  {
			charClass = LETTER;
		    } else if (isdigit(nextChar)) {
			    charClass = DIGIT;
		        } else {
			        charClass = UNKNOWN;
		        }
    } else { 
		charClass = EOF;
	}
}

void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

int lex() {
    lexLen = 0;
    getNonBlank();

    //Starts cases for identifiers
    switch (charClass) {
    //parses Identifiers
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = ID;
        break;

    //parses digits
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = NUM;
        break;

    // for parentheses,operators, and other misc
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
    // end of file
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    cout << "Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
    return nextToken;
}