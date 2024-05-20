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
#include "DeliveryManager.h"


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
    std::shared_ptr<DeliveryManager> dev_man;

    int location = 0;
    int page = 0;
    int elements_per_page = 10;
    std::string data_choice;
    std::string edge_choice;
    std::string vertex_file;
    std::string edge_file;
    std::pair<double,double> backtrack_res;
    std::pair<double,double> approximation_res;
    std::pair<double,double> christofile_res;
    std::pair<double,double> real_res;
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
                L"Quit"
            },
            {
                L"Toy-Graphs",
                L"Extra_Fully_Connected_Graphs",
                L"Real-world Graphs",
                L"Back"

            },
             {
                L"shipping.csv",
                L"stadiums.csv",
                L"tourism.csv",
                 L"Back"

            },
             {
                L"edges_25.csv",
                L"edges_50.csv",
                L"edges_75.csv",
                L"edges_100.csv",
                L"edges_200.csv",
                L"edges_300.csv",
                L"edges_400.csv",
                L"edges_500.csv",
                L"edges_600.csv",
                L"edges_700.csv",
                L"edges_800.csv",
                L"edges_900.csv",
                 L"Back",
            },
            {
                L"graph1",
                L"graph2",
                L"graph3",
                L"Back"
            },
            {
                L"Change DataSet",
                L"Cost using Backtracking",
                L"Cost using Trinagular Approximation",
                L"Cost using Christofides Approximation",
                L"Cost using Christofides Approximation in Real Graphs",
                L"Back",
                L"Quit"

            },
            {
                    L"Back",
                    L"Quit"

                },
            {
                    L"Back",
                    L"Quit"

                },
            {
                    L"Back",
                    L"Quit"

                },
            {
                L"Back",
                L"Quit"

            }

    };

    std::vector<std::wstring> helpers{
            L"  You can use 'up arrow', 'down arrow', and 'ENTER' to select the options",
            L"  You can use 'tab' to change to the table, and 'ENTER' to select one",
            L"  You can use 'n' and 'p' to go to the next and previous page of the table respectively",
            L"  Choose your DataSet",

    };

    std::vector<std::wstring> deliveryManagement = {
            L" ---------------------",
            L"| Delivery Management |",
            L" ---------------------",
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
