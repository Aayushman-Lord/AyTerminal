#include <iostream>
#include <vector>
#include <filesystem>
#include "Parser.h"
#include "Execute.h"

using std::cout, std::vector, std::cin, std::string;

int main()
{
    string instructions;
    string CurrentPath = std::filesystem::current_path();
    bool stop = false;

    while (!stop)
    {
        cout << "\033[1;34m[" << CurrentPath << "]\033[32mSHELL$ \n >\033[0m ";
        std::getline(cin, instructions);

        vector<string> instructions_parsed = Parse(instructions);

        if (instructions_parsed.empty())
            continue;

        else if (instructions_parsed[0] == "exit")
        {
            stop = true;
            continue;
        }
        
        else if (instructions_parsed[0] == "cd")
        {
            if (instructions_parsed.size() < 2)
            {
                cout << "cd: missing argument\n";
                continue;
            }
           
            try
            {
            std::filesystem::path newPath = instructions_parsed[1];
            current_path(newPath);
            CurrentPath = std::filesystem::current_path();
            }
            
            catch(const std::filesystem::filesystem_error& e)
            {
                cout << "cd: " << e.what() << "\n";
            }
        }
        
        else
        {
            execute(instructions_parsed);
        }
    }

}
