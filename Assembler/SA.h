# include "LA.h"
struct Symbol_Column {
    string name;
    string type;
    int memory;
};

struct Instruct_Column {
    int int_num;
    string operation;
    int operand;
};
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
    bool lookup_symbol(string symbol);
    void insert_symbol_name(string symbol);
    void insert_symbol_type(string type);
    void print_symbols();
    void print_instructs();
    void generate_inst(string op, int operand);
    int  get_address(string id);
    void back_patch(int jump_addr);
    void outputToFile(const string &production);
    Symbol_Column symbol_table[100];
    int curr_table_loc = 0;
    int memory_address = 10001;
    vector<Instruct_Column> instruct_table;
    vector<int> jump_stack;
    void clearData();
public:
    token current_token;
    ofstream outputFile;
    void Rat21SU(); //R1 - will be the function to be initially called after lexer analyzes all tokens
    void testTimSection();
};
