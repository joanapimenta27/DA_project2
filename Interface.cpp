//
// Created by joana on 13-05-2024.
//

#include <cstring>
#include <utility>
#include "Interface.h"
#include "Printer.h"


Interface::Interface()= default;

void Interface::refreshDirectories() {
    directories.clear();
    directories.emplace_back(L"");
    directories.emplace_back(L"Choose DataSet");
    directories.emplace_back(L"Choose DataSet");
    directories.emplace_back(L"Choose DataSet");
    directories.emplace_back(L"Choose DataSet");
    directories.emplace_back(converter.from_bytes(data_choice)+L" "+converter.from_bytes(edge_choice));

}

void Interface::stackClear(std::stack<int> &s){
    while (!s.empty()){
        s.pop();
    }
}

std::wstring Interface::smooth_string(const std::wstring& w){
    std::wstring sw;
    for (wchar_t c : w) {
        if (!iswspace(c)) {
            c = std::tolower(c);
            switch (c) {
                case L'á':
                case L'à':
                case L'â':
                case L'ä':
                case L'ã':
                case L'Á':
                case L'À':
                case L'Ã':
                case L'Â':
                case L'Ä':
                    sw += L'a';
                    break;
                case L'é':
                case L'è':
                case L'ê':
                case L'ë':
                case L'ẽ':
                case L'Ë':
                case L'É':
                case L'È':
                case L'Ê':
                case L'Ẽ':
                    sw += L'e';
                    break;
                case L'í':
                case L'ì':
                case L'î':
                case L'ï':
                case L'ĩ':
                case L'Í':
                case L'Ì':
                case L'Î':
                case L'Ĩ':
                case L'Ï':
                    sw += L'i';
                    break;
                case L'ó':
                case L'ò':
                case L'ô':
                case L'ö':
                case L'õ':
                case L'Ó':
                case L'Ò':
                case L'Õ':
                case L'Ô':
                case L'Ö':
                    sw += L'o';
                    break;
                case L'ú':
                case L'ù':
                case L'û':
                case L'ü':
                case L'ũ':
                case L'Ú':
                case L'Ù':
                case L'Ũ':
                case L'Û':
                case L'Ü':
                    sw += L'u';
                    break;
                case L'ç':
                    sw += L'c';
                default:
                    if (!ispunct(c)) {
                        sw += c;
                    }
            }
        }
    }
    return sw;
}

void Interface::initializeTable(){

}


void Interface::enterInputHandler(int loc, unsigned long sel, bool back, bool main_menu, bool main_menu2){
    refreshDirectories();
    if (back){
        if(loc == 0){
            location = earlier_locations.top();
            earlier_locations.pop();
        }
        else{
            location = loc;
        }

    }
    else if (main_menu){
        location = 0;
        stackClear(earlier_locations);
    }
    else if (main_menu2){
        location = 5;
        stackClear(earlier_locations);
    }
    else{
        earlier_locations.push(location);
        location = loc;
    }
    directory = directories[location];
    selected = sel;

}

void Interface::inputResponseInWriteMode(wchar_t user_in){
    if ((isalpha(user_in) || isalnum(user_in) || (user_in >= 128 && user_in <= 255) || (ispunct(user_in)))) {
        if (write == write_default) {
            write = L"";
        }
        write += user_in;
    }
    if (user_in == 32 && write != write_default) {
        write += L" ";
    }
    if ((user_in == 8 || user_in == 127) && write != write_default) {
        if (!write.empty()) {
            write.pop_back();
        }
    }
    if (write.empty()) {
        write = write_default;
    }
    if (write.size() > capOfWrite[location] && write != write_default) {
        write.pop_back();
    }
}

void Interface::tableModeCleaner(std::unordered_map<std::string, std::string> um){
    write = write_default;
    selected_in_page = 0;
    selected = 0;
    page = 0;
    table_mode = !table_mode;
    filteredWstringPairsVector = std::move(um);
}

void Interface::tableModeCleaner2(std::vector<std::wstring> ws){
    write = write_default;
    selected_in_page = 0;
    selected = 0;
    page = 0;
    table_mode = !table_mode;
    filteredStringVector = std::move(ws);
}

void Interface::inputer(){
    user_input = getwchar();
    if (!write_mode) {
        if (user_input == 27) {
            user_input = getwchar();
            user_input = getwchar();
        }
        basicInputResponse(user_input);
    }
    else{
        if (user_input == 27){
            user_input = getwchar();
            user_input = getwchar();
            write_mode = false;
            basicInputResponse(user_input);
        }
        else if ((user_input == '\n' || user_input == '\t') && locationHasTable[location]){
            write_mode = false;
            table_mode = true;
        }
        else{
            inputResponseInWriteMode(user_input);
        }
    }
}

void Interface::basicInputResponse(unsigned int user_in) {
    if (locationHasTable[location]){
        if (user_in == '\t'){
            table_mode = !table_mode;
            selected = 0;
            selected_in_page = 0;
        }
        if (user_in == 'n'){
            page ++;
            selected_in_page = 0;
            if (page > filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0) {
                page--;
            }
            else if (page > filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                page--;
            }
        }
        if (user_in == 'p'){
            page --;
            selected_in_page = 0;
            if (page < 0){
                page ++;
            }
        }
    }
    if (!table_mode){
        if (user_in == 'A') {
            if (selected > 0) { selected -= 1; }
            else { selected = options[location].size() - 1; }
        }
        if (user_in == 'B') {
            selected ++;
            if (selected >= options[location].size()) {selected = 0;}
        }
    }
    else{
        if (user_in == 'A') {
            if (selected_in_page > 0) { selected_in_page -= 1; }
            else if (page == filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0){
                selected_in_page = filteredWstringPairsVector.size()%elements_per_page - 1;
            }
            else if (page == filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                selected_in_page = filteredStringVector.size()%elements_per_page - 1;
            }
            else {
                selected_in_page = elements_per_page - 1;
            }
        }
        if (user_in == 'B') {
            selected_in_page ++;
            if (page == filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0){
                if (selected_in_page > filteredWstringPairsVector.size()%elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
            else if (page == filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                if (selected_in_page > filteredStringVector.size()%elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
            else{
                if (selected_in_page > elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
        }
    }
    if (user_in == 'q') {
        location = -1;
    }
    //========================= ENTER INPUTS ===========================//
    if (user_in == '\n') {
        switch (location) {

            case 0:       //============================= MAIN MENU ===============================//
                switch (selected) {
                    case 0:
                        enterInputHandler(1, 0, false, false, false);
                    break;
                    case 1:
                        location = -1;
                    break;
                }
            break;
            case 1:
                switch (selected) {
                    case 0:
                        data_choice=converter.to_bytes(options[location][selected]);
                        enterInputHandler(2, 0, false, false, false);
                    break;
                    case 1:
                        data_choice=converter.to_bytes(options[location][selected]);
                        enterInputHandler(3, 0, false, false, false);
                    break;
                    case 2:
                        data_choice=converter.to_bytes(options[location][selected]);
                        enterInputHandler(4, 0, false, false, false);
                    break;
                }
            break;
            case 3:
                switch (selected) {
                    default:
                        edge_choice=converter.to_bytes(options[location][selected]);
                        edge_file="../dataset/"+data_choice+"/"+edge_choice;
                        vertex_file="../dataset/"+data_choice+"/nodes.csv";
                        enterInputHandler(5, 0, false, false, false);
                        break;
                }
            break;
            case 4:
                switch (selected) {
                    default:
                        edge_choice=converter.to_bytes(options[location][selected]);
                        edge_file="../dataset/"+data_choice+"/"+edge_choice+"/edges.csv";
                        vertex_file="../dataset/"+data_choice+"/"+edge_choice+"/nodes.csv";
                        enterInputHandler(5, 0, false, false, false);
                        break;
                }
            break;

        }
    }
}

void Interface::run(){
    setlocale(LC_CTYPE, "en_US.UTF-8"); // enconding to UTF-8 for extended characters such as "ç"

    struct termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt); // Get the current terminal settings

    newt = oldt; // Copy the current settings to the new settings

    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode (line buffering) and echoing

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Set the new settings


    while(location != -1){

        system("clear");

        switch (location) {
            case 0:
                printBoldTitle(deliveryManagement);
                printOptions(options[location],selected,false);
                printHelper(helpers, {0});
                inputer();
                break;
            case 1:
                printDirectory(directory);
                printOptions(options[location],selected,false);
                printHelper(helpers, {0});
                inputer();
                break;
            case 2:
                printOptions(options[location],selected,false);
                printHelper(helpers, {0});
                inputer();
                break;

            case 3:
                printOptions(options[location],selected,false);
                printHelper(helpers, {0});
                inputer();
                break;
            case 4:
                printOptions(options[location],selected,false);
                printHelper(helpers, {0});
                inputer();
                break;
            case 5:
                printDirectory(directory);
                dev_man=std::make_shared<DeliveryManager>(vertex_file,edge_file);

                inputer();

        }

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore old terminal

    system("clear");

};