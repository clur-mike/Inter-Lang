#pragma once
#include "parser.hpp"

std::unordered_map<std::string, std::variant<std::string, int>> variables;

std::variant<std::string, int> interpret_expression(Expression orig_expr);

std::variant<std::string, int> interpret_literal(Literal_Term lit_term){
    switch(lit_term.var.index()){
        case 0:
            return std::stoi(std::get<Integer_Term>(lit_term.var).token.value);
            break;
        case 1:
            return std::get<String_Term>(lit_term.var).token.value;
            break;
        case 2:
            return variables[std::get<Identifier_Term>(lit_term.var).token.value];
            break;
        case 3:
            return interpret_expression(*std::get<Parenthesis_Term>(lit_term.var).expr);
            break;
    }
}

std::variant<std::string, int> interpret_equation_literal(Arithmetical_Expression ari_expr){
    switch(ari_expr.var.index()){
        case 0:{
            Addition_Expression add_expr = std::get<Addition_Expression>(ari_expr.var);
            auto interpreted_lhs = interpret_expression(*add_expr.lhs);
            auto interpreted_rhs = interpret_expression(*add_expr.rhs);
            switch(interpreted_lhs.index()){
                case 0:{
                    std::string string_lhs = std::get<std::string>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            return string_lhs + std::get<std::string>(interpreted_rhs);
                        case 1:
                            return string_lhs + std::to_string(std::get<int>(interpreted_rhs));
                    }
                    break;
                }
                case 1:{
                    int int_lhs = std::get<int>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            return std::to_string(int_lhs) + std::get<std::string>(interpreted_rhs);
                        case 1:
                            return int_lhs + std::get<int>(interpreted_rhs);
                    }
                    break;
                }
            }
            break;
        }
        case 1:{
            Division_Expression divi_expr = std::get<Division_Expression>(ari_expr.var);
            auto interpreted_lhs = interpret_expression(*divi_expr.lhs);
            auto interpreted_rhs = interpret_expression(*divi_expr.rhs);
            switch(interpreted_lhs.index()){
                case 0:{
                    std::string string_lhs = std::get<std::string>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            exit(1);
                        case 1:
                            exit(1);
                    }
                    break;
                }
                case 1:{
                    int int_lhs = std::get<int>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            exit(1);
                        case 1:
                            return int_lhs / std::get<int>(interpreted_rhs);
                    }
                    break;
                }
            }
            break;
        }
        case 2:{
            Multiplication_Expression multi_expr = std::get<Multiplication_Expression>(ari_expr.var);
            auto interpreted_lhs = interpret_expression(*multi_expr.lhs);
            auto interpreted_rhs = interpret_expression(*multi_expr.rhs);
            switch(interpreted_lhs.index()){
                case 0:{
                    std::string string_lhs = std::get<std::string>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            exit(1);
                        case 1:{
                            std::string new_string = "";
                            for(int i = 0;i < std::get<int>(interpreted_rhs);i++){
                                new_string += string_lhs;
                            }
                            return new_string;
                        }
                    }
                    break;
                }
                case 1:{
                    int int_lhs = std::get<int>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:{
                            std::string new_string = "";
                            for(int i = 0;i < int_lhs;i++){
                                new_string += std::get<std::string>(interpreted_rhs);
                            }
                            return new_string;
                        }
                        case 1:
                            return int_lhs * std::get<int>(interpreted_rhs);
                    }
                    break;
                }
            }
            break;
        }
        case 3:{
            Subtraction_Expression subtract_expr = std::get<Subtraction_Expression>(ari_expr.var);
            auto interpreted_lhs = interpret_expression(*subtract_expr.lhs);
            auto interpreted_rhs = interpret_expression(*subtract_expr.rhs);
            switch(interpreted_lhs.index()){
                case 0:{
                    std::string string_lhs = std::get<std::string>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            exit(1);
                        case 1:
                            exit(1);
                    }
                    break;
                }
                case 1:{
                    int int_lhs = std::get<int>(interpreted_lhs);
                    switch(interpreted_rhs.index()){
                        case 0:
                            exit(1);
                        case 1:
                            return int_lhs - std::get<int>(interpreted_rhs);
                    }
                    break;
                }
            }
            break;
        }
    }
}

std::variant<std::string, int> interpret_expression(Expression orig_expr){
    switch(orig_expr.var.index()){
        case 0:
            return interpret_equation_literal(std::get<Arithmetical_Expression>(orig_expr.var));
            break;
        case 1:
            return interpret_literal(std::get<Literal_Term>(orig_expr.var));
            break;
    }
}

void interpret_statement(Print_Statement print_stmt){
    auto interpreted_expr = interpret_expression(*print_stmt.expr);
    switch(interpreted_expr.index()){
        case 0:
            std::cout << std::get<std::string>(interpreted_expr) << '\n';
            break;
        case 1:
            std::cout << std::get<int>(interpreted_expr) << '\n';
            break;
    }
}

void interpret_statement(Variable_Statement var_stmt){
    auto interpreted_expr = interpret_expression(*var_stmt.expr);
    variables[var_stmt.ident.value] = interpreted_expr;
}   

void interpret_statement(Assign_Statement assign_stmt){
    auto interpreted_expr = interpret_expression(*assign_stmt.expr);
    variables[assign_stmt.ident.value] = interpreted_expr;
}

void interpret(Program program){
    for(Statement stmt: program.statements){
        switch(stmt.var.index()){
            case 0:
                interpret_statement(std::get<Print_Statement>(stmt.var));
                break;
            case 1:
                interpret_statement(std::get<Variable_Statement>(stmt.var));
                break;
            case 2:
                interpret_statement(std::get<Assign_Statement>(stmt.var));
                break;
        }
    }
}