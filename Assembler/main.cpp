#include "LA.h"
#include "SA.h"

int main() {
    ifstream test;
    ofstream test_output;
    string testFiles[] = {"test1.txt", "test2.txt", "test3.txt"};

    for (const auto& file : testFiles) {
        test.open(file);
        test_output.open("LA_output_" + file);
        testerFunction(test, test_output);
        test.close();
        test_output.close();

        // After parsing tokens then create SA instance and call lexer to assign first token, then call Rat21SU to start syntactical analysis
        SA syntaxAnalyzer;
        syntaxAnalyzer.outputFile.open( "Assembly_output_" + file);
        syntaxAnalyzer.current_token = lexer();
        syntaxAnalyzer.Rat21SU();
        //syntaxAnalyzer.testTimSection();
        if (syntaxAnalyzer.outputFile.is_open()) {
            syntaxAnalyzer.outputFile.close();
        }
    }
    return 0;
}