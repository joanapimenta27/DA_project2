//
// Created by joana on 14-05-2024.
//

#include "Parser.h"
Parser::Parser(const std::string &fName) {
    file_.open(fName);
    if (!file_) {
        throw std::runtime_error("Could not open file: " + fName);
    }
}

std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) {
        start++;
    }

    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    return str.substr(start, end - start);
}

std::vector<std::vector<std::string>> Parser::getData() {
    std::string line;
    while (std::getline(file_, line)) {
        if(std::isalpha(line[0])) {
            std::getline(file_, line);
        }
        std::istringstream iss(line);
        std::string value;
        std::vector<std::string> v;
        while (std::getline(iss, value, ',')) {
            value = trim(value);
            v.push_back(value);


        }

        data_.push_back(v);
    }
    return data_;
}
