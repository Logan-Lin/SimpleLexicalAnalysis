//
//  main.cpp
//  LexicalAnalyze
//
//  Created by Logan Lin on 2018/4/20.
//  Copyright © 2018年 Logan Lin. All rights reserved.
//

#include "main.hpp"
#include <stdlib.h>

int main(int argh, char * argv[]) {
    const char * file_name = "in.txt";
    if (argh > 1) {
        file_name = argv[1];
    }
    LexAn lex = LexAn(file_name);
    lex.scan_file();

	system("pause");
}
