#include "inter-interpreter.hpp"

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cout << "Please use this tool as the following: main <filename>" << std::endl;
    }
    std::ifstream src_file(argv[1]);
    std::stringstream src_buf;
    src_buf << src_file.rdbuf();
    std::vector<Token> tokens = lex(src_buf.str());
    interpret(parse(tokens));
    return 0;
}