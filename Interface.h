//
// Created by joana on 13-05-2024.
//

#ifndef INTERFACE_H
#define INTERFACE_H



#include <iostream>
#include <codecvt>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <locale>
#include <thread>
#include <stack>
#include <unordered_map>


/**
 * @brief Class representing the user interface.
 */
class Interface {
public:

    /**
     * @brief Constructor for the Interface class.
     */
    explicit Interface();

    /**
     * @brief Function to start running the interface.
     */
    void run();


private:

    //----------------- COLOR SCHEMES --------------------//
    std::wstring bg_light_red = L"\x1b[101m";
    std::wstring end_bg = L"\x1b[0m";
    std::wstring italic = L"\x1b[3m";
    std::wstring end_italic = L"\x1b[0m";
    std::wstring red = L"\033[31m";
    std::wstring blue = L"\033[34m";
    std::wstring bold = L"\033[1m";
    std::wstring underline = L"\033[4m";;
    std::wstring end_effect = L"\033[0m";
    //----------------------------------------------------//

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::thread manThread;

    int location = 0;
    int page = 0;
    int elements_per_page = 10;

    unsigned long selected = 0;
    unsigned long selected_in_page = 0;

    unsigned int user_input = 0;

    bool write_mode = false;
    bool table_mode = false;
    bool is_done = false;

    std::wstring directory;
    std::vector<std::wstring> directories;
    std::wstring write;
    std::wstring write_default =  italic + L"  You can write here  " + end_italic;
    std::wstring selected_dataSet = L"none";

    std::stack<int> earlier_locations;

    std::vector<std::wstring> filteredStringVector;
    std::unordered_map<std::string, std::string> filteredWstringPairsVector;


    std::unordered_map<int, int> locationHasTable = {
            {7, 1}, {12, 1}, {14, 1}, {16, 1}
    };
    std::unordered_map<int, int> capOfWrite = {
            {7, 30}, {12, 4}, {14, 5}, {16, 10}
    };

    std::unordered_map<int, int> locationWithTable = {
            {7, 0}, {12, 1}, {14, 2}, {16, 3}
    };

    std::vector<std::vector<std::wstring>> options{
            {
                L"Choose DataSet",
                L"Credits",
                L"Quit"
            },
            {
                L"Change Name",
                L"Main Menu"
            },
            {
                L"Portugal DataSet",
                L"Madeira DataSet",
                L"Main Menu"
            },
            {
                L"Portugal DataSet " + bold + blue + L" <- ACTIVE" + end_effect,
                L"Madeira DataSet",
                L"Proceed"
            },
            {
                L"Portugal DataSet",
                L"Madeira DataSet " + bold + blue + L" <- ACTIVE" + end_effect,
                L"Proceed"
            },
            {
                L"Basic Service Metrics",
                L"Reliability and Sensitivity to Failures",
                L"Choose DataSet",
                L"Quit"
            },
            {
                L"Max Amount of Water",
                L"Cities in Deficit",
                L"Balance Difference",
                L"Main Menu"
            },
            {
                L"Search for a City",
                L"Total",
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Reservoir Out of Commission",
                L"Pumping Stations in Maintenance",
                L"Pipeline Failures",
                L"Main Menu"
            },
            {
                L"Search for a Reservoir",
                L"Back",
                L"Main Menu"
            },
            {
                L"Add One More Reservoir",
                L"Back",
                L"Main Menu"
            },
            {
                L"Search for a Pumping Station",
                L"Back",
                L"Main Menu"
            },
            {
                L"Add One More Pumping Station",
                L"Back",
                L"Main Menu"
            },
            {
                L"Search for a Pipe",
                L"Back",
                L"Main Menu"
            },
            {
                L"Add One More Pipe",
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            }
    };

    std::vector<std::wstring> helpers{
            L"  You can use 'up arrow', 'down arrow', and 'ENTER' to select the options",
            L"  You can use 'tab' to change to the table, and 'ENTER' to select one",
            L"  You can use 'n' and 'p' to go to the next and previous page of the table respectively",
            L"  Choose your DataSet",
            L"  You selected Portugal DataSet",
            L"  You selected Madeira DataSet"
    };

    std::vector<std::wstring> waterSupplyManagement = {
            L" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- ",
            L"| ██     ██  █████  ████████ ███████ ██████      ███████ ██    ██ ██████  ██████  ██     ██    ██     ███    ███  █████  ███    ██  █████   ██████  ███████ ███    ███ ███████ ███    ██ ████████   |",
            L"| ██     ██ ██   ██    ██    ██      ██   ██     ██      ██    ██ ██   ██ ██   ██ ██      ██  ██      ████  ████ ██   ██ ████   ██ ██   ██ ██       ██      ████  ████ ██      ████   ██    ██      |",
            L"| ██  █  ██ ███████    ██    █████   ██████      ███████ ██    ██ ██████  ██████  ██       ████       ██ ████ ██ ███████ ██ ██  ██ ███████ ██   ███ █████   ██ ████ ██ █████   ██ ██  ██    ██      |",
            L"| ██ ███ ██ ██   ██    ██    ██      ██   ██          ██ ██    ██ ██      ██      ██        ██        ██  ██  ██ ██   ██ ██  ██ ██ ██   ██ ██    ██ ██      ██  ██  ██ ██      ██  ██ ██    ██      |",
            L"|  ███ ███  ██   ██    ██    ███████ ██   ██     ███████  ██████  ██      ██      ██████    ██        ██      ██ ██   ██ ██   ████ ██   ██  ██████  ███████ ██      ██ ███████ ██   ████    ██      |",
            L" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- ",
    };

    void inputer();

    void basicInputResponse(unsigned int user_in);

    void inputResponseInWriteMode(wchar_t user_in);

    void refreshDirectories();

    static void stackClear(std::stack<int> &s);

    void enterInputHandler(int loc, unsigned long sel, bool back, bool main_menu, bool main_menu2);

    static std::wstring smooth_string(const std::wstring &w);

    void initializeTable();

    void tableModeCleaner(std::unordered_map<std::string, std::string> um);

    void tableModeCleaner2(std::vector<std::wstring> um);

};




#endif //INTERFACE_H
