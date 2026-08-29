#pragma once

#include <bits/stdc++.h>

enum Token_Type{
    Open_Parenthesis,
    Closed_Parenthesis,
    Identifier,
    String_Literal,
    Integer_Literal,
    Print,
    Addition,
    Multiplication,
    Subtraction,
    Division,
    Semi_Colon,
    Variable,
    Equal
};

struct Token{
    std::string value;
    const Token_Type type;
};

std::optional<int> arithmetic_priority(Token symbol){
    switch(symbol.type){
        case Token_Type::Multiplication:
        case Token_Type::Division:
            return 1;
        case Token_Type::Addition:
        case Token_Type::Subtraction:
            return 0;
        default:
            return {};
    }
}

bool Is_Digits(std::string str){
    if(str.length() == 0){
        return false;
    }
    for(char i: str){
        if(!isdigit(i)){
            return false;
        }
    }
    return true;
}

void add_word(std::string word_to_add, std::vector<Token> *token_list){
    int wrd_length = word_to_add.length();
    if(word_to_add == "print"){
        Token print_token = {.value = word_to_add.c_str(), .type = Token_Type::Print};
        token_list->push_back(print_token);
    }
    else if(word_to_add == "var"){
        Token var_token = {.value = "var", .type = Token_Type::Variable};
        token_list->push_back(var_token);
    }
    else if (Is_Digits(word_to_add)){
        Token int_lit_token = {.value = word_to_add.c_str(), .type = Token_Type::Integer_Literal};
        token_list->push_back(int_lit_token);
    }
    else if (wrd_length >= 2 && (word_to_add[0] == '\"' && word_to_add[wrd_length-1] == '\"')){
        std::string quotation_less_string = "";
        for(int i = 1; i < wrd_length-1 ;i++){
            quotation_less_string.push_back(word_to_add[i]);
        }
        Token string_lit_token = {.value = quotation_less_string.c_str(), .type = Token_Type::String_Literal};
        token_list->push_back(string_lit_token);
    }
    else if (wrd_length != 0){
        Token ident_token = {.value = word_to_add.c_str(), .type = Token_Type::Identifier};
        token_list->push_back(ident_token);
    }
}

std::vector<Token> lex(std::string source_code){
    std::vector<Token> generated_tokens;
    int source_code_size = source_code.length();
    std::string current_word = "";
    bool quotations = false;

    for(int index = 0; index < source_code_size; index++){
        char cur_let = source_code[index];
        if(cur_let == '\"' && quotations == false){
            quotations = true;
            current_word.push_back(cur_let);
            continue;
        }
        else if(cur_let == '\"' && quotations == true){
            quotations = false;
            current_word.push_back(cur_let);
            continue;
        }
        else if(quotations == true){
            current_word.push_back(cur_let);
            continue;
        }
        switch (cur_let){
            case ' ':
            case '\n':
                add_word(current_word, &generated_tokens);
                current_word = "";
                break;
            case '(':{
                add_word(current_word, &generated_tokens);
                Token open_paren_token = {.value = "(", .type = Token_Type::Open_Parenthesis};
                generated_tokens.push_back(open_paren_token);
                current_word = "";
                break;
            }
            case ')':{
                add_word(current_word, &generated_tokens);
                Token closed_paren_token = {.value = ")", .type = Token_Type::Closed_Parenthesis};
                generated_tokens.push_back(closed_paren_token);
                current_word = "";
                break;
            }
            case '=':{
                add_word(current_word, &generated_tokens);
                Token equals_token = {.value = "=", .type = Token_Type::Equal};
                generated_tokens.push_back(equals_token);
                current_word = "";
                break;
            }
            case ';':{
                add_word(current_word, &generated_tokens);
                Token semi_colon_token = {.value = ";", .type = Token_Type::Semi_Colon};
                generated_tokens.push_back(semi_colon_token);
                current_word = "";
                break;
            }
            case '+':{
                add_word(current_word, &generated_tokens);
                Token plus_token = {.value = "+", .type = Token_Type::Addition};
                generated_tokens.push_back(plus_token);
                current_word = "";
                break;
            }
            case '-':{
                add_word(current_word, &generated_tokens);
                Token minus_token = {.value = "-", .type = Token_Type::Subtraction};
                generated_tokens.push_back(minus_token);
                current_word = "";
                break;
            }
            case '*':{
                add_word(current_word, &generated_tokens);
                Token multi_token = {.value = "*", .type = Token_Type::Multiplication};
                generated_tokens.push_back(multi_token);
                current_word = "";
                break;
            }
            case '/':{
                add_word(current_word, &generated_tokens);
                Token divi_token = {.value = "/", .type = Token_Type::Division};
                generated_tokens.push_back(divi_token);
                current_word = "";
                break;
            }
            default:
                current_word.push_back(cur_let);
                break;
        }
    }

    return generated_tokens;
}