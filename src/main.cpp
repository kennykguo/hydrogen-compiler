#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

using namespace std;


enum class TokenType{
    _return,
    int_lit,
    semi
};

struct Token{
    TokenType type;
    optional<string> value;
};

vector<Token> tokenize(const string& str){
    vector <Token> tokens;
    string buf;
    for (int i = 0; i < str.length(); i++){
        char c = str.at(i);
        if(isalpha(c)){
            buf.push_back(c);
            i++;
            while (isalpha(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            if(buf == "return"){
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                // cout << "return" << endl;
                continue;
            }
            else{
                cerr << "Error" << endl;
                exit(EXIT_FAILURE);
            }

        }

        else if(isspace(c)){
            continue;
        }
        
        else if (isdigit(c)){
            buf.push_back(c);
            i++;

            while (isdigit(str.at(i))){
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear();
            // cout << "int" << endl;

        }

        else if(c == ';'){
            tokens.push_back({.type = TokenType::semi});;
            // cout << "semi" << endl;
        }

        else{
            cerr << "Syntax error" << endl;
            exit(EXIT_FAILURE);
        }

    }
    return tokens;

}



string tokens_to_asm(vector <Token>& tokens){
    stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++){
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_return){
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit){
                if(i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi){
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i + 1).value.value()<< "\n";
                    output<< "    syscall";
                }
            }
        }
    }
    return output.str();
}



int main(int argc, char* argv[]){

    if (argc != 2){
        cerr << "Incorrect usage of compiler" << endl;
        cerr << "hydro <input.hy> " << endl;
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << std::endl;
    string contents;
    std::stringstream contents_stream;
    fstream input(argv[1], ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
    input.close();

    // cout << contents << endl;
    vector <Token> tokens = tokenize(contents);
    // cout << tokens_to_asm(tokens) << endl;

    fstream file("../out.asm", std::ios::out);
    file << tokens_to_asm(tokens);
    file.close();
    return 0;

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    system("./out");

}
