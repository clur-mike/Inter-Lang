#pragma once
#include "lexer.hpp"

struct Integer_Term{
    Token token;
};
struct String_Term{
    Token token;
};

struct Expression;

struct Identifier_Term{
    Token token;
};

struct Parenthesis_Term{
    Expression* expr;
};

struct Literal_Term{
    std::variant<Integer_Term, String_Term, Identifier_Term, Parenthesis_Term> var;
};

struct Addition_Expression{
    Expression* lhs;
    Expression* rhs;
};

struct Subtraction_Expression{
    Expression* lhs;
    Expression* rhs;
};

struct Division_Expression{
    Expression* lhs;
    Expression* rhs;
};

struct Multiplication_Expression{
    Expression* lhs;
    Expression* rhs;
};

struct Arithmetical_Expression{
    std::variant<Addition_Expression, Division_Expression, Multiplication_Expression, Subtraction_Expression> var;
};

struct Expression{
    std::variant <Arithmetical_Expression, Literal_Term> var;
};

struct Print_Statement{
    Expression* expr;
};

struct Variable_Statement{
    Expression* expr;
    Token ident;
};

struct Assign_Statement{
    Expression* expr;
    Token ident;
};

struct Statement{
    std::variant<Print_Statement, Variable_Statement, Assign_Statement> var;
};

struct Program{
    std::vector<Statement> statements;
};

bool is_next(Token_Type looking_for, int ahead, int *index, std::vector<Token>& tokens){
    if(tokens.size()-1 < *index+ahead){
        return false;
    }
    else if(tokens[*index + ahead].type == looking_for){
        return true;
    }
    return false;
}

std::optional<Token> get_next(int *index, std::vector<Token> &tokens, int offset = 1){
    if(tokens.size()-1 > *index+offset){
        return tokens[*index + offset];
    }
    return {};
}

Token increment(int *index, std::vector<Token> &tokens){
    *index += 1;
    return tokens[*index-1]; 
}

std::optional<Expression> parse_expression(int *index, std::vector<Token>& tokens, int arithmetical_importance = 0);

std::optional<Literal_Term>parse_term(int *index, std::vector<Token> &tokens){
    if(is_next(Token_Type::Integer_Literal, 0, index, tokens)){
        Token integer_lit = increment(index, tokens);
        Integer_Term int_term = {.token = integer_lit}; 
        Literal_Term literal_term = {.var = int_term};
        return literal_term;
    }
    else if(is_next(Token_Type::String_Literal, 0, index, tokens)){
        Token string_lit = increment(index, tokens);
        String_Term string_term = {.token = string_lit};
        Literal_Term literal_term = {.var = string_term};
        return literal_term;
    }
    else if(is_next(Token_Type::Identifier, 0, index, tokens)){
        Token ident = increment(index, tokens);
        Identifier_Term ident_term = {.token = ident};
        Literal_Term literal_term = {.var = ident_term};
        return literal_term; 
    }
    else if(is_next(Token_Type::Open_Parenthesis, 0, index, tokens)){
        auto expr = parse_expression(index, tokens);
        if (!expr.has_value()){
            exit(1);
            if(!is_next(Token_Type::Closed_Parenthesis, 1, index, tokens)){
                exit(1);
            }
            else{
                increment(index,tokens);
            }
        }
        Parenthesis_Term paren_term = {.expr = &expr.value()};
        Literal_Term literal_term = {.var = paren_term};
        return literal_term;

    }
    return {};
}

std::optional<Expression> parse_expression(int *index, std::vector<Token>& tokens, int arithmetical_importance){
    auto left_side = parse_term(index, tokens);
    if (!left_side.has_value()){
        return {};
    }
    Expression left_side_expr = {.var = left_side.value()};
    while(true){
        auto c_token = get_next(index, tokens, 0);
        if(!c_token.has_value()){
            break;
        }

        auto arith_impor = arithmetic_priority(c_token.value());

        if(!arith_impor.has_value() || arith_impor.value() < arithmetical_importance){
            return left_side_expr;
        }

        Token symbol = increment(index, tokens);
        auto right_side_expr = parse_expression(index, tokens, arith_impor.value() + 1);
        if (!right_side_expr.has_value()){
            break;
        }

        Arithmetical_Expression ari_expr;
        Expression* left_side_heap = new Expression{left_side_expr};
        Expression* right_side_heap = new Expression{right_side_expr.value()};

        if(symbol.type == Token_Type::Addition){
            ari_expr.var = Addition_Expression{.lhs = left_side_heap, .rhs = right_side_heap};
        }
        else if(symbol.type == Token_Type::Division){
            ari_expr.var = Division_Expression{.lhs = left_side_heap, .rhs = right_side_heap};
        }
        else if(symbol.type == Token_Type::Subtraction){
            ari_expr.var = Subtraction_Expression{.lhs = left_side_heap, .rhs = right_side_heap};
        }
        else if(symbol.type == Token_Type::Multiplication){
            ari_expr.var = Multiplication_Expression{.lhs = left_side_heap, .rhs = right_side_heap};
        }
        else{
            exit(1);
        }

        left_side_expr.var = ari_expr;
        std::cout << "stuck in expression" << '\n';
        return left_side_expr;
    }
    return {};
}

std::optional<Statement> parse_statement(int *index, std::vector<Token>& tokens){
    if(is_next(Token_Type::Print, 0, index, tokens) && is_next(Token_Type::Open_Parenthesis, 1, index, tokens)){
        increment(index, tokens);
        increment(index, tokens);

        auto expr = parse_expression(index, tokens);

        if(!expr.has_value()){
            exit(1);
        }

        if(is_next(Token_Type::Closed_Parenthesis, 0, index, tokens) && is_next(Token_Type::Semi_Colon, 1, index, tokens)){
            increment(index, tokens);
            increment(index, tokens);
            Expression* heap_expr = new Expression(expr.value());
            Print_Statement print_stmt = {.expr = heap_expr};
            Statement stmt = {.var = print_stmt};
            return stmt;
        }
        return {};
    }
    else if(is_next(Token_Type::Variable, 0, index, tokens) && is_next(Token_Type::Identifier, 1, index, tokens) && is_next(Token_Type::Equal, 2, index, tokens)){
        increment(index, tokens);
        Token identifier = increment(index, tokens);
        increment(index, tokens);

        auto expr = parse_expression(index, tokens);

        if(!expr.has_value()){
            exit(1);
        }

        if(is_next(Token_Type::Semi_Colon, 0, index, tokens)){
            increment(index, tokens);
            Expression* heap_expr = new Expression(expr.value());
            Variable_Statement var_stmt = {.expr = heap_expr, .ident = identifier};
            Statement stmt = {.var = var_stmt};
            return stmt;
        }
        return {};
    }
    else if(is_next(Token_Type::Identifier, 0, index, tokens) && is_next(Token_Type::Equal, 1, index, tokens)){
        Token identifier = increment(index, tokens);
        increment(index, tokens);

        auto expr = parse_expression(index, tokens);

        if(!expr.has_value()){
            exit(1);
        }

        if(is_next(Token_Type::Semi_Colon, 0, index, tokens)){
            increment(index, tokens);
            Expression* heap_expr = new Expression(expr.value());
            Assign_Statement assign_stmt = {.expr = heap_expr, .ident = identifier};
            Statement stmt = {.var = assign_stmt};
            return stmt;
        }
        return {};
    }
    return {};
}

Program parse(std::vector<Token> tokens){
    Program program;
    int index = 0;
    while(get_next(&index, tokens).has_value()){
        if(auto stmt = parse_statement(&index, tokens)){
            program.statements.push_back(stmt.value());
        }
        else{
            break;
        }
    }
    return program;
}