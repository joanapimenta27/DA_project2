//
// Created by joana on 14-05-2024.
//

#ifndef PARSER_H
#define PARSER_H


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

class Parser{
private:
    std::ifstream file_;
    std::vector<std::vector<std::string>> data_;

public:
    explicit Parser(const std::string &fName);
    std::vector<std::vector<std::string>> getData();
};



#endif //PARSER_H
