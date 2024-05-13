//
// Created by joana on 14-05-2024.
//

#include "Parser.h"
Parser::Parser(const std::string &fName) {
    file_.open(fName);
}

std::vector<std::vector<std::string>> Parser::getData() {
    std::string line;
    std::getline(file_, line);
    while (std::getline(file_, line)) {
        std::istringstream iss(line);
        std::string value;
        std::vector<std::string> v;
        while (std::getline(iss, value, ',')) {
            v.push_back(value);
        }
        data_.push_back(v);
    }
    return data_;
}
