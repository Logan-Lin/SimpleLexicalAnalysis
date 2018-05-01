//
//  LexAn.hpp
//  LexicalAnalyze
//
//  Created by Logan Lin on 2018/4/19.
//  Copyright © 2018年 Logan Lin. All rights reserved.
//

#ifndef LexAn_hpp
#define LexAn_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <map>

using std::string;
using std::cout;
using std::vector;
using std::map;

class LexAn {
private:
    // Type codings
    enum codings {
        CHAR = 1, INT, DOUBLE, FLOAT,
        SHORT, LONG, VOID, BOOL,
        FOR, WHILE, SWITCH, DO, IF, ELSE, CASE, CONTINUE,
        BREAK, DEFAULT, RETURN, TRUE_, FALSE_,
        PLUS, MINUS, ASTERISK, SLASH, SEMICOLON, COMMA,
        LEFT_PARA, RIGHT_PARA, COLON, GREATER, LESS, EQUAL,
        LEFT_BIG_PARA, RIGHT_BIG_PARA, LEFT_BRACKET, RIGHT_BRACKET,
        DOUBLE_PLUS, DOUBLE_MINUS, DOUBLE_EQUAL,
        NOT_EQUAL, NO_LESS, NO_GREATER, ANNO_START, ANNO_END,
        DOUBLE_SLASH, DOUBLE_ASTERISK,
        IDENTIFIFER, UNSIGNED_INTEGER, UNIDENTIFY, INVALID_ID
    };
    
    enum types {
        NUMBER = 0,
        ALPHABET, SPACE, OTHER
    };
    
    enum conditions {
        VALID = 0, INVALID, ONE_LINE_COMMENT, COMMENT_START, COMMENT_END,
        IN_COMMENT
    };
    
    FILE *fin;
    
    vector<string> keywords = {"char" , "int", "double", "float",
        "short", "long", "void", "bool",
        "for", "while", "switch", "do", "if", "else",
        "case", "continue", "break", "default", "return", "true", "false"};
    /*
     * A public buffer string for storing number and identifier,
     * since their length can vary.
     */
    string buffer;
    
    // To indicate which character in the string the scanner is scanning.
    int scan_index;
    
    // To indicate whether the scanner is scanning annotation contents.
    int comment_stat;
    
    int get_char_kind(char c);
    bool is_character(char c);
    bool is_number(char c);
    bool is_space(char c);
    
    // Return keyword's coding number. Return -1 if string is not a keyword.
    int get_keyword_coding(string s);
    
    int get_single_byte_coding(char c);
    
    // State machine for number
    bool unsigned_number(char * s);
    
    // State machine for identifier
    bool identifier(char * s);
    bool invalid(char * s);
    
    // State machine for double bytes delimiter
    // Because the length can only be 1 or 2, these functions will
    // directly return the coding number.
    int plus(char * s); // Start with +
    int minus(char * s); // Start with -
    int equal(char * s); // Start with =
    int less(char * s); // Start with <
    int greater(char * s); // Start with >
    int slash(char * s); // Start with /
    int asterisk(char * s); // Start with *
    
    // Convert decimal (in string form) to binary (also in string form)
    string dec2bin(string dec_str);
    
public:
    LexAn();
    LexAn(const char file_name[]);
    ~LexAn();
    
    // Scan and analyze the content of one row
    conditions scan_row(char * row);
    
    void scan_file();
};

#endif /* LexAn_hpp */
