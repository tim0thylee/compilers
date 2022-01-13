 include "LA.h"

class SA {
private:
    // Functions for the productions (R2-25) from the syntax rules - private as it will only used by the SA
    void OptDeclarationList();
    void DeclarationList();
    void DeclarationListPrime();
    void Declaration();
    void Qualifier();
    void StatementList();
    void StatementListPrime();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void IfPrime();
    void Put();
    void Get();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void ExpressionPrime();
    void Term();
    void TermPrime();
    void Factor();
    void Primary();
    void Empty();
    void printToken();
    void outputToFile(const string &production);
public:
    token current_token;
    ofstream outputFile;
    void Rat21SU(); //R1 - will be the function to be initially called after lexer analyzes all tokens
    void testTimSection();
};