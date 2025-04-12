#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>

using namespace std;

//Variables
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream inputFile;

//Function prototypes
void addChar();
void getChar();
void getNonBlank();
void ParseDecl();
void ParseProgram();
void ParseIDList();
void ParseDeclSec();
void ParseStmtSec();
void ParseSTMT();
void ParseAssign();
void ParseFactor();
void ParseExpr();
void ParseOperand();
void ParseInput();
void ParseOutput();
void ParseIfStmt();
void ParseComp();
void ParseWhileStmt();
void ParseFuncCall();
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

    //operatos
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
    SEMICOLON = 34,
    EQUALS = 35,

    //reserved word values set
    BEGIN = 36,
    IF = 37,
    THEN = 38,
    ELSE = 39,
    WHILE = 40,
    LOOP = 41,
    INT = 42,
    END = 43
};

//container of reserved words and the string that it is pointing to
map<string, int> reservedwords = {
    {"program",PROGRAM},
    {"begin",BEGIN},
    {"end",END},
    {"if",IF},
    {"then",THEN},
    {"else",ELSE},
    {"input",INPUT},
    {"output",OUTPUT},
    {"int",INT},
    {"while",WHILE},
    {"loop",LOOP},
    {"call", FUNCALL}

};

int main() {

    //replace the string with the file path of whatever text file to test for comment blocks.
    inputFile.open("input7.txt");
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    getChar();
    lex();
    ParseProgram();

    if (nextToken != EOF) {
        cerr << "Unexpected input after end of program.";
        return 1;
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
        getChar();
        if (nextChar == '=') {
            addChar();
            nextToken = ASSIGN;
        }
        else {
            nextToken = COLON;
        }
        break;
    case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
    case '=':
        addChar();
        nextToken = EQUALS;
        break;
    case '<':
        addChar();
        getChar();
        if (nextChar == '>') {
            addChar();
            nextToken = NOTEQ;
            getChar();
        }
        else {
            nextToken = GREATER;
        }
        break;
    case '>':
        addChar();
        nextToken = LOWER;
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
    }
    else {
        cout << "Error - lexeme is too long \n";
    }
}

void getChar() {
    //Get the next character from the input file
    if (inputFile.get(nextChar)) {
        if (isalpha(nextChar) || nextChar == '_') {
            charClass = LETTER;
        }
        else if (isdigit(nextChar)) {
            charClass = DIGIT;
        }
        else {
            charClass = UNKNOWN;
        }
    }
    else {
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
    case LETTER: {
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }

        //checks the string of the current lexeme to see if it matches a reservedword
        string lexStr = string(lexeme);
        auto it = reservedwords.find(lexStr);
        if (it != reservedwords.end()) {
            if (lexStr == "int" || lexStr == "float" || lexStr == "double") {
                nextToken = TYPE;
            }
            else {
                nextToken = it->second;
            }
        }
        else {
            nextToken = ID;
        }
        break;
    }
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
    // This was mostly for debugging
    // cout << "\n Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
    return nextToken;
}
void ParseProgram() {
    if (nextToken == PROGRAM) {
        cout << "PROGRAM" << endl;
        lex();
    }

    ParseDeclSec();

    if (nextToken == BEGIN) {
        lex();
    }
    else {
        cerr << "Expected 'begin' after declaration section" << endl;
        exit(1);
    }

    ParseStmtSec();


}
// start of parser. this is the declaration section
void ParseDeclSec() {
    while (nextToken == ID) {
        cout << "DECL_SEC" << endl;
        ParseDecl();
    }
}

void ParseDecl() {
    cout << "DECL" << endl;

    if (nextToken == ID) {
        ParseIDList();
    }

    if (nextToken == COLON) {
        lex();
    }
    else {
        cerr << "Expected ':' after identifier list";

    }

    if (nextToken == TYPE) {
        lex();
    }
    else {
        cerr << "Expected a type after ':'" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' after declaration";
        exit(1);
    }
}

void ParseIDList() {
    cout << "ID_LIST" << endl;
    if (nextToken == ID) {
        lex();
        if (nextToken == COMMA) {
            lex();
            ParseIDList();
        }
    }
    else {
        cerr << "Expected identifier in declaration";
    }
}

void ParseStmtSec() {
    while (nextToken == ID || nextToken == IF || nextToken == WHILE || nextToken == INPUT || nextToken == OUTPUT || nextToken == FUNCALL) {
        cout << "STMT_SEC" << endl;
        ParseSTMT();
    }
}

//Switch case of all the stmt sec requirements
void ParseSTMT() {
    cout << "STMT" << endl;

    switch (nextToken) {
    case ID:

        ParseAssign();
        break;

    case IF:

        ParseIfStmt();
        break;

    case WHILE:

        ParseWhileStmt();
        break;

    case INPUT:

        ParseInput();
        break;

    case OUTPUT:

        ParseOutput();
        break;

    case FUNCALL:
        ParseFuncCall();
        break;

    default:
        cerr << "Unexpected token in statement" << endl;
        exit(1);

    }

}

//Parsing section that checks for assignment, then expression, factor, and operands
void ParseAssign() {
    if (nextToken == ID) {
        lex();
    }
    else {
        cerr << "Expected identifier at beginning of assignment" << endl;
        exit(1);
    }

    if (nextToken == ASSIGN) {
        lex();
    }
    else {
        cerr << "Expected ':=' in assignment" << endl;
        exit(1);
    }
    cout << "ASSIGN" << endl;
    //cout << "  Debug: about to parse EXPR, nextToken = " << nextToken << ", lexeme = " << lexeme << endl;


    ParseExpr();

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' at end of assignment" << endl;
        exit(1);
    }
}

void  ParseExpr() {
    cout << "EXPR" << endl;
    ParseFactor();

    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        cout << "EXPR" << endl;
        ParseFactor();
    }
}

void ParseFactor() {
    cout << "FACTOR" << endl;
    ParseOperand();

    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        cout << "FACTOR" << endl;
        ParseOperand();
    }
}

//checks for operands
void ParseOperand() {
    cout << "OPERAND" << endl;

    if (nextToken == NUM || nextToken == ID) {
        lex();
    }
    else if (nextToken == LPAREN) {
        lex();
        ParseExpr();
        if (nextToken == RPAREN) {
            lex();
        }
        else {
            cerr << "Expected ')' after expression" << endl;
            exit(1);
        }
    }
    else if (nextToken == FUNCALL) {
        ParseFuncCall();
    }
    else {
        cerr << "Expected NUM, ID, (EXPR), or FUNCALL as operand" << endl;
        exit(1);
    }
}


//Check for input reserved word
void ParseInput() {
    cout << "INPUT" << endl;
    lex();

    if (nextToken == ID) {
        ParseIDList();
    }
    else {
        cerr << "Expected identifier list after 'input'" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' after input statement" << endl;
        exit(1);
    }
}

//Checks for output reserved word
void ParseOutput() {
    cout << "OUTPUT" << endl;
    lex();

    if (nextToken == ID) {
        ParseIDList();
    }
    else if (nextToken == NUM) {
        lex();
    }
    else {
        cerr << "Expected identifier list or number after 'output'" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' after output statement" << endl;
        exit(1);
    }
}

//parses the if statement
void ParseIfStmt() {
    cout << "IF_STMT" << endl;
    lex();

    if (nextToken == LPAREN) {
        lex();
    }
    else {
        cerr << "Expected '(' before condition in if-statement" << endl;
        exit(1);
    }

    ParseComp();

    if (nextToken == RPAREN) {
        lex();
    }
    else {
        cerr << "Expected ')' after condition in if-statement" << endl;
        exit(1);
    }

    if (nextToken == THEN) {
        lex();
    }
    else {
        cerr << "Expected 'then' after condition in if-statement" << endl;
        exit(1);
    }

    ParseStmtSec();

    if (nextToken == ELSE) {
        lex();
        ParseStmtSec();
    }

    if (nextToken == END) {
        lex();
    }
    else {
        cerr << "Expected 'end' in if-statement" << endl;
        exit(1);
    }

    if (nextToken == IF) {
        lex();
    }
    else {
        cerr << "Expected 'if' after 'end' in if-statement" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' after if-statement" << endl;
        exit(1);
    }
}



//while statement parsing
void ParseWhileStmt() {
    cout << "WHILE_STMT" << endl;
    lex();

    if (nextToken == LPAREN) {
        lex();
    }
    else {
        cerr << "Expected '(' before condition in while-statement" << endl;
        exit(1);
    }

    ParseComp();

    if (nextToken == RPAREN) {
        lex();
    }
    else {
        cerr << "Expected ')' before condition in while-statement" << endl;
        exit(1);
    }

    if (nextToken == LOOP) {
        lex();
    }
    else {
        cerr << "Expected 'loop' in while-statement" << endl;
        exit(1);
    }

    ParseStmtSec();

    if (nextToken == END) {
        lex();
    }
    else {
        cerr << "Expected 'end' in while _statement" << endl;
        exit(1);
    }

    if (nextToken == LOOP) {
        lex();
    }
    else {
        cerr << "Expected 'while' after 'end' in while-statement" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex(); // consume ';'
    }
    else {
        cerr << "Expected ';' after while-statement" << endl;
        exit(1);
    }
}


//parses the comp logic
void ParseComp() {
    cout << "COMP" << endl;

    ParseOperand();

    if (nextToken == GREATER || nextToken == LOWER || nextToken == NOTEQ || nextToken == EQUALS) {
        lex();
    }
    else {
        cerr << "Expected comparison operator in condition" << endl;
        exit(1);
    }

    ParseOperand();
}

void ParseFuncCall() {
    cout << "FUNCALL" << endl;

    lex();

    if (nextToken == ID) {
        lex();
    }
    else {
        cerr << "Expected identifier (function name) after 'call'" << endl;
        exit(1);
    }

    if (nextToken == LPAREN) {
        lex();
    }
    else {
        cerr << "Expected '(' after function name" << endl;
        exit(1);
    }

    ParseIDList();

    if (nextToken == RPAREN) {
        lex();
    }
    else {
        cerr << "Expected ')' after ID_LIST in function call" << endl;
        exit(1);
    }

    if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cerr << "Expected ';' after function call" << endl;
        exit(1);
    }
}