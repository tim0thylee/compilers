#include "SA.h"
// TODO: change all cout to outputFile afterwards
// To keep track of the line count in the SA output for errors - increment on every successful print output to fil
bool printOutput = true; // set to false for assignment 3

// Function to print out token type and lexeme to output file
void SA::printToken() {
    cout << "Token: " << current_token.type << "  Lexeme: " << current_token.lexeme << endl;
    outputFile << "Token: " << current_token.type << "  Lexeme: " << current_token.lexeme << endl;
}

// Function to print and output the production rules for assignment 2. ERRORS NOT PASSED IN THROUGH IT
void SA::outputToFile(const string& production) {
    if (printOutput) {
        cout << production << endl;
        outputFile << production << endl;
    }
}

void SA::Rat21SU() {
    // checked that it is %% so call OptDeclarationList() and StatementList() to continue analysis
    if (current_token.lexeme == "%%") {
        printToken();
        cout << "<Rat21SU> -> %% <Opt Declaration List> <Statement List> %% " << endl;
        outputFile << "<Rat21SU> -> %% <Opt Declaration List> <Statement List> %% " << endl;
        // grab next token to assign and call NT functions for this production
        current_token = lexer();
        printToken();
        OptDeclarationList();
        StatementList();
        if (current_token.lexeme != "%%") {
            //TODO: Make this so it outputs to a file - CHECK FOR CORRECT LINECOUNT
            outputFile << "Syntax error - missing '%%' at end of file" << endl;
            exit(0);
        }
        else {
            outputFile << "<Rat21SU> end" << endl;
        }
        // close output file
        outputFile.close();
    }
    else {
        outputFile << "Error: '%%' missing at beginning of Rat21SU analysis" << endl;
        outputFile.close();
    }
}

void SA::OptDeclarationList() {
    // OptDeclarationList -> <DeclarationList> | <Empty>
    if (current_token.lexeme == "integer" || current_token.lexeme == "boolean") {
        // if current token to analyze is a qualifier <Declaration> -> <Qualifier> then left recurse down
        outputToFile("<Opt Declaration List> -> <Declaration List>");
        DeclarationList();
    }
    // <Empty> case where it is not Declaration symbol so we output epsilon and will move on to StatementList execution
    outputToFile("<Opt Declaration List> -> <Empty>");
    Empty();
}

void SA::DeclarationList() {
    outputToFile("<Declaration List> -> <Declaration>; <Declaration List Prime>");
    Declaration();
    current_token = lexer();
    // next token should be followed by ; after call to Declaration
    if (current_token.lexeme == ";") {
        printToken();
        // Call DeclarationListPrime
        DeclarationListPrime();
    }
    else {
        outputFile << "Error: Expected ';' after <identifier>" << endl;
        exit(0);
    }
}

void SA::DeclarationListPrime() {
    current_token = lexer();
    printToken();
    if (current_token.lexeme == "integer" || current_token.lexeme == "boolean") {
        // If token is start of another declaration then go back to declaration list
        outputToFile("<Declaration List Prime> -> <Declaration List>");
        DeclarationList();
    }
    else {
        // else if current token is not then output epsilon and print current token before moving to statement checking
        outputToFile("<Declaration List Prime> -> ε");
        //        printToken();
    }
}

void SA::Declaration() {
    outputToFile("<Declaration> -> <Qualifier> <identifier>");
    Qualifier();
    // Qualifier rule passes so get next token from lexer and check to be identifier type
    current_token = lexer();
    printToken();
    if (current_token.type == "IDENTIFIER") {
        outputToFile("<identifier> -> " + current_token.lexeme);
    }
    else {
        outputFile << "Error: Expected identifier to come after Qualifier" << endl;
        exit(0);
    }
}

void SA::Qualifier() {
    if (current_token.lexeme == "integer") {
        outputToFile("<Qualifier> -> integer");
    }
    else if (current_token.lexeme == "boolean") {
        outputToFile("<Qualifier> -> boolean");
    }
    else {
        // output error and exit program when for syntax error to prevent returning to calling function
        outputFile << "Error: Expected qualifier of integer or boolean." << endl;
        exit(0);
    }
}

void SA::StatementList() {
    outputToFile("<Statement List> -> <Statement> <Statement List Prime>");
    Statement();
    StatementListPrime();
}

void SA::StatementListPrime() {
    if (current_token.type == "IDENTIFIER" || current_token.lexeme == "while" ||
        current_token.lexeme == "if" || current_token.lexeme == "{" ||
        current_token.lexeme == "put" || current_token.lexeme == "get") {
        outputToFile("<Statement List Prime> -> <Statement List>");
        StatementList();
    }
    else if (current_token.lexeme == "integer" || current_token.lexeme == "boolean") {
        // Exit out with syntax error as declarations come first
        outputFile << "Error: Qualifier received but should be declared before any statements." << endl;
        exit(0);
    }
    else {
        outputToFile("<Statement List Prime> -> ε");
    }
}

void SA::testTimSection() {
    printToken();
    Statement();
}

void SA::Statement() {
    string lexeme = current_token.lexeme;
    string type = current_token.type;
    if (lexeme == "begin") {
        outputToFile("<Statement> -> <Compound>");
        Compound();
    } 
    else if (type == "IDENTIFIER") {
        outputToFile("<Statement> -> <Assign>");
        Assign();
    }
    else if (lexeme == "if") {
        outputToFile("<Statement> -> <If>");
        If();
    }
    else if (lexeme == "get") {
        outputToFile("<Statement> -> <Get>");
        Get();
    }
    else if (lexeme == "put") {
        outputToFile("<Statement> -> <Put>");
        Put();
    }
    else if (lexeme == "while") {
        outputToFile("<Statement> -> <While>");
        While();
    }
    else {
        outputFile << "Error: Expected <Statement> type: begin, if, get, put, while, or <Identifier> type." << endl;
        exit(0);
    }
}

void SA::Compound() {
    outputToFile("<Compound> -> begin <Statement List> end");
    current_token = lexer();
    printToken();
    StatementList();
    if (current_token.lexeme == "end") {
        current_token = lexer();
        printToken();
    }
    else {
        outputFile << "Error: Expected \"end\" for complete <Compound>" << endl;
        exit(0);
    }
}

void SA::Assign() {
    outputToFile("<Assign> -> <Identifier> = <Expression> ;");
    current_token = lexer();
    printToken();
    if (current_token.lexeme == "=") {
        current_token = lexer();
        printToken();
        Expression();
        if (current_token.lexeme == ";") {
            current_token = lexer();
            printToken();
        }
        else {
            outputFile << "Error: Expected separator \";\" for complete <Assign>" << endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Expected \"=\" for proper <Assign> " << endl;
        exit(0);
    }
}

void SA::If() {
    outputToFile("<If> -> if(<Condition>)<Statement><IfPrime>endif");
    current_token = lexer();
    printToken();
    if (current_token.lexeme == "(") {
        current_token = lexer();
        printToken();
        Condition();
        if (current_token.lexeme == ")") {
            current_token = lexer();
            printToken();
            Statement();
            IfPrime();
            if (current_token.lexeme == "endif") {
                current_token = lexer();
                printToken();
            }
            else {
                outputFile << "Error: Expected separator \")\"" << endl;
                exit(0);
            }
        }
        else {
            outputFile << "Error: Expected separator \")\" " << endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Expected separator \"(\" " << endl;
        exit(0);
    }
}

void SA::IfPrime() {
    if (current_token.lexeme == "else") {
        outputToFile("<IfPrime> -> else <Statement>");
        current_token = lexer();
        printToken();
        Statement();
    }
    else {
        outputToFile("<IfPrime> -> ε");
    }
}

void SA::Put() {
    outputToFile("<Put> -> put(<identifier>);");
    current_token = lexer();
    printToken();
    if (current_token.lexeme == "(") {
        current_token = lexer();
        printToken();
        if (current_token.type == "IDENTIFIER") {
            outputToFile("<identifier> -> " + current_token.lexeme);
            current_token = lexer();
            printToken();
            if (current_token.lexeme == ")") {
                current_token = lexer();
                printToken();
                if (current_token.lexeme == ";") {
                    current_token = lexer();
                    printToken();
                }
                else {
                    cout << "Error: Expected separator \";\" " << endl;
                    outputFile << "Error: Expected separator \";\" for <Put>" <<  endl;
                    exit(0);
                }
            }
            else {
                outputFile << "Error: Expected separator \")\" for <Put>" <<  endl;
                exit(0);
            }
        }
        else {
            outputFile << "Error: Must have Identifier type." <<  endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Expected separator \"(\" for <Put>"  << endl;
        exit(0);
    }
}

void SA::Get() {
    outputToFile("<Get> -> get(<identifier>);");
    current_token = lexer();
    printToken();
    if (current_token.lexeme == "(") {
        current_token = lexer();
        printToken();
        if (current_token.type == "IDENTIFIER") {
            outputToFile("<identifier> -> " + current_token.lexeme);
            current_token = lexer();
            printToken();
            if (current_token.lexeme == ")") {
                current_token = lexer();
                printToken();
                if (current_token.lexeme == ";") {
                    current_token = lexer();
                    printToken();
                }
                else {
                    outputFile << "Error: Expected separator \";\" for <Get>" <<  endl;
                    exit(0);
                }
            }
            else {
                outputFile << "Error: Expected separator \")\" for <Get>" <<  endl;
                exit(0);
            }
        }
        else {
            outputFile << "Error: Error: Must have Identifier type." <<  endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Expected separator \"(\" for <Get>" <<  endl;
        exit(0);
    }
}

void SA::While() {
    outputToFile("<While>->while(<Condition>)<Statement>");
    current_token = lexer();
    if (current_token.lexeme == "(") {
        current_token = lexer();
        printToken();
        Condition();
        if (current_token.lexeme == ")") {
            current_token = lexer();
            printToken();
            Statement();
        }
        else {
            outputFile << "Error: Expected separator \")\"" << endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Expected separator \"(\"" << endl;
        exit(0);
    }
}

void SA::Condition() {
    outputToFile("<Condition> -> <Expression> <Relop> <Expression>");
    Expression();
    Relop();
    Expression();
}

void SA::Relop() {
    outputToFile("<Relop> -> <Relop> ");
    if (current_token.lexeme == "==") {
        outputToFile("<Relop> -> ==");
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == ">") {
        outputToFile("<Relop> -> >");
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == "<") {
        outputToFile("<Relop> -> <");
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == "/=") {
        outputToFile("<Relop> -> /=");
        current_token = lexer();
        printToken();
    }
    else {
        // output error and exit program when for syntax error to prevent returning to calling function
        outputFile << "Error: Expected Relop of == or > or < or /= at line " << endl;
        exit(0);
    }
}

void SA::Expression() {
    outputToFile("<Expression> -> <Term> <Expression Prime>");
    Term();
    ExpressionPrime();
}

void SA::ExpressionPrime() {
    outputToFile("<Expression> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε");
    if (current_token.lexeme == "+") {
        current_token = lexer();
        printToken();
        Term();
        ExpressionPrime();
    }
    else if (current_token.lexeme == "-") {
        current_token = lexer();
        printToken();
        Term();
        ExpressionPrime();
    }
    else {
        outputToFile("<Expression Prime> -> ε");
    }
}

void SA::Term() {
    outputToFile("<Term> -> <Factor> <Term Prime>");
    Factor();
    TermPrime();
}

void SA::TermPrime() {
    outputToFile("<Term Prime> -> <Factor> <Term Prime>");
    if (current_token.lexeme == "*") {
        current_token = lexer();
        printToken();
        Factor();
        ExpressionPrime();
    }
    else if (current_token.lexeme == "/") {
        current_token = lexer();
        printToken();
        Factor();
        ExpressionPrime();
    }
    else {
        outputToFile("<Term Prime> -> ε");
    }
}

void SA::Factor() {
    outputToFile("<Factor> -> - <Primary> | <Primary>");
    if (current_token.lexeme == "-") {
        current_token = lexer();
        printToken();
        Primary();
    }
    else {
        Primary();
    }
}

void SA::Primary() {
    outputToFile("<Primary> -> <Identifier> | <Integer> | ( <Expression> ) | true | false");
    if (current_token.type == "IDENTIFIER") {
        outputToFile("<identifier> -> " + current_token.lexeme);
        current_token = lexer();
        printToken();
    }
    else if (current_token.type == "INTEGER") {
        outputToFile("<integer> -> " + current_token.lexeme);
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == "true") {
        outputToFile("<Primary> -> true");
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == "false") {
        outputToFile("<Primary> -> false");
        current_token = lexer();
        printToken();
    }
    else if (current_token.lexeme == "(") {
        outputToFile("<Primary> -> ( <Expression> )");
        current_token = lexer();
        printToken();
        Expression();
        if (current_token.lexeme == ")") {
            current_token = lexer();
            printToken();
        }
        else {
            outputFile << "Error: Expected the value \")\" " <<  endl;
            exit(0);
        }
    }
    else {
        outputFile << "Error: Not valid <Primary> value. " << endl;
        exit(0);
    }
}

void SA::Empty() {
    outputToFile("<Empty> -> ε");
}
