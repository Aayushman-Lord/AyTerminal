#include <iostream>
#include <vector>
#include <filesystem>
#include <readline/readline.h>
#include <readline/history.h>
#include "Parser.h"
#include "Execute.h"

using std::cout, std::string, std::vector;

int main()
{
    bool stop = false;
    cout << "\033[1;32m AyTerminal v1.2.0 \033[0m\n";

    while (!stop)
    {
        string CurrentPath = std::filesystem::current_path();

        string prompt = "\033[1;34m[" + CurrentPath + "]\033[32m SHELL$ \n > \033[0m";

        char* input = readline(prompt.c_str());

        // CTRL + D
        if (!input)
            break;

        string instructions = input;

        // Add non-empty commands to history
        if (!instructions.empty())
            add_history(input);

        free(input);

        vector<string> instructions_parsed = Parse(instructions);

        if (instructions_parsed.empty())
            continue;

        // EXIT
        if (instructions_parsed[0] == "exit")
        {
            return 0;
        }

        // CD
        else if (instructions_parsed[0] == "cd")
        {
            if (instructions_parsed.size() < 2)
            {
                cout << "cd: missing argument\n";
                continue;
            }

            try
            {
                std::filesystem::current_path(instructions_parsed[1]);
            }

            catch (const std::filesystem::filesystem_error& e)
            {
                cout << "cd: " << e.what() << '\n';
            }
        }

        // OTHER COMMANDS
        else
        {
            bool pipe = false;
            bool outputReDirection = false;
            for (const auto& arg : instructions_parsed)
            {
                if (arg == "|")
                {
                    cout << "Piping is not supported yet.\n";
                    pipe = true;
                }
                else if (arg == ">")
                {
                    outputReDirection = true;
                    execute_outputReDirection(instructions_parsed);
                }
            }
            if (pipe || outputReDirection)continue;

            execute(instructions_parsed);
        }
    }

    return 0;
}
