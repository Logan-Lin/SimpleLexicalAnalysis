//
//  LexAn.cpp
//  LexicalAnalyze
//
//  Created by Logan Lin on 2018/4/19.
//  Copyright © 2018年 Logan Lin. All rights reserved.
//

#include "LexAn.hpp"

LexAn::LexAn() {
    LexAn("in.txt");
}

LexAn::LexAn(const char file_name[]) {
    fin = fopen(file_name, "r");
    if (fin == NULL) {
        cout << "Unable to open file!" << std::endl;
        exit(1);
    }
}

LexAn::~LexAn() {
    fclose(fin);
}

void LexAn::scan_file() {
    char line_buffer[1024];
    int line_count = 0;
    while (std::fgets(line_buffer, 1024, fin) != NULL) {
        line_count++;
        cout << "Line " << line_count << ": ";
        scan_row(line_buffer);
        cout << std::endl;
    }
}

LexAn::conditions LexAn::scan_row(char * row) {
    scan_index = 0;
    while(true) {
        buffer = "";
        char c = row[scan_index];
        if (c == '\0' || c == '\n')
            return VALID;
        if (comment_stat == IN_COMMENT && c != '*') {
            scan_index++;
            continue;
        }
        switch (get_char_kind(c)) {
            case NUMBER: {
                bool valid = unsigned_number(&row[scan_index]);
                if (valid) {
                    cout << "(" << UNSIGNED_INTEGER << ", " << dec2bin(buffer) << ")";
                } else {
                    cout << "(" << INVALID_ID << ", " << buffer << ")";
                }
                break;
            }
            case ALPHABET: {
                bool valid = identifier(&row[scan_index]);
                if (valid) {
                    int keyword_coding = get_keyword_coding(buffer);
                    if (keyword_coding == -1) {
                        cout << "(" << IDENTIFIFER << ", " << buffer << ")";
                    } else {
                        cout << "(" << keyword_coding << ", -)";
                    }
                } else {
                    cout << "(" << buffer << ", invalid)";
                }
                break;
            }
            case SPACE: {
                scan_index++;
                break;
            }
            case OTHER: {
                switch (c) {
                    case '+': {
                        cout << "(" << plus(&row[scan_index]) << ", -)";
                        break;
                    }
                    case '-': {
                        cout << "(" << minus(&row[scan_index]) << ", -)";
                        break;
                    }
                    case '=': {
                        cout << "(" << equal(&row[scan_index]) << ", -)";
                        break;
                    }
                    case '<': {
                        cout << "(" << less(&row[scan_index]) << ", -)";
                        break;
                    }
                    case '>': {
                        cout << "(" << greater(&row[scan_index]) << ", -)";
                        break;
                    }
                    case '/': {
                        int value = slash(&row[scan_index]);
                        cout << "(" << slash(&row[scan_index]) << ", -)";
                        if (value == DOUBLE_SLASH)
                            return ONE_LINE_COMMENT;
                        else if (value == ANNO_START)
                            comment_stat = IN_COMMENT;
                        break;
                    }
                    case '*': {
                        int value = asterisk(&row[scan_index]);
                        if (value == ANNO_END)
                            comment_stat = COMMENT_END;
                        if (comment_stat != IN_COMMENT) {
                            cout << "(" << value << ", -)";
                        }
                        break;
                    }
                    default:
                        int value = get_single_byte_coding(c);
                        if (value != -1) {
                            cout << "(" << value << ", -)";
                        } else {
                            cout << "(" << UNIDENTIFY << ", " << c <<")";
                        }
                        scan_index++;
                }
                break;
            }
            default:
                break;
        }
    }
    return VALID;
}

int LexAn::get_char_kind(char c) {
    if (is_number(c))
        return NUMBER;
    if (is_character(c))
        return ALPHABET;
    if (is_space(c))
        return SPACE;
    return OTHER;
}

bool LexAn::is_character(char c) {
    if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
        return true;
    return false;
}

bool LexAn::is_number(char c) {
    if (c <= '9' && c >= '0')
        return true;
    return false;
}

bool LexAn::is_space(char c) {
    if (c == '\t' or c == ' ' or c == '\n')
        return true;
    return false;
}

int LexAn::get_keyword_coding(string s) {
    for (int i = 0; i < keywords.size(); i++) {
        if (s.compare(keywords[i]) == 0) {
            // As you noticed, this kind of trick requires
            // the keywords array to have the exact sequence
            // compared to codings enum array.
            return i + 1;
        }
    }
    return -1;
}

int LexAn::get_single_byte_coding(char c) {
    int result = -1;
    switch (c) {
        case '(': {
            result = LEFT_PARA;
            break;
        }
        case ')': {
            result = RIGHT_PARA;
            break;
        }
        case '{': {
            result = LEFT_BIG_PARA;
            break;
        }
        case '}': {
            result = RIGHT_BIG_PARA;
            break;
        }
        case '[': {
            result = LEFT_BRACKET;
            break;
        }
        case ']': {
            result = RIGHT_BRACKET;
            break;
        }
        case ',': {
            result = COMMA;
            break;
        }
        case ';': {
            result = SEMICOLON;
            break;
        }
        case ':': {
            result = COLON;
            break;
        }
        default:
            break;
    }
    return result;
}

bool LexAn::unsigned_number(char * s) {
    if (is_number(s[0])) {
        buffer += s[0];
        scan_index++;
        return unsigned_number(&s[1]);
    } else if (is_character(s[0])) {
        buffer += s[0];
        scan_index++;
        invalid(&s[1]);
        return false;
    }
    return true;
}

bool LexAn::invalid(char * s) {
    if (is_number(s[0]) || is_character(s[0])) {
        buffer += s[0];
        scan_index++;
        invalid(&s[1]);
    }
    return true;
}

bool LexAn::identifier(char * s) {
    if (is_character(s[0]) || is_number(s[0])) {
        buffer += s[0];
        scan_index++;
        return identifier(&s[1]);
    }
    return true;
}

int LexAn::plus(char * s) {
    if (s[1] == '+') {
        scan_index += 2;
        return DOUBLE_PLUS;
    }
    scan_index++;
    return PLUS;
}

int LexAn::minus(char * s) {
    if (s[1] == '-') {
        scan_index += 2;
        return DOUBLE_PLUS;
    }
    scan_index++;
    return MINUS;
}

int LexAn::equal(char * s) {
    if (s[1] == '=') {
        scan_index += 2;
        return DOUBLE_EQUAL;
    }
    scan_index++;
    return EQUAL;
}

int LexAn::less(char * s) {
    if (s[1] == '=') {
        scan_index += 2;
        return NO_GREATER;
    } else if (s[1] == '>') {
        scan_index += 2;
        return NOT_EQUAL;
    }
    scan_index++;
    return LESS;
}

int LexAn::greater(char * s) {
    if (s[1] == '=') {
        scan_index++;
        return NO_LESS;
    }
    scan_index++;
    return GREATER;
}

int LexAn::slash(char * s) {
    if (s[1] == '/') {
        scan_index += 2;
        return DOUBLE_SLASH;
    } else if (s[1] == '*') {
        scan_index += 2;
        return ANNO_START;
    }
    scan_index++;
    return SLASH;
}

int LexAn::asterisk(char * s) {
    if (s[1] == '*') {
        scan_index += 2;
        return DOUBLE_ASTERISK;
    } else if (s[1] == '/') {
        scan_index += 2;
        return ANNO_END;
    }
    scan_index++;
    return ASTERISK;
}

string LexAn::dec2bin(string dec_str) {
    return std::bitset<8>(std::stoi(dec_str)).to_string();
}
