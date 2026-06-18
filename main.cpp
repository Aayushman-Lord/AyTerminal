#include <iostream>
#include <vector>
#include <filesystem>
#include <readline/readline.h>
#include <readline/history.h>
#include "Parser.h"
#include "Execute.h"

using std::cout, std::string, std::vector;

// Functions to update the status of all jobs
void update_job_status(job &j)
{
    if (!j.status)
        return; // already marked finished, skip

    int wstatus;
    pid_t result = waitpid(j.pid, &wstatus, WNOHANG);

    if (result == j.pid)
    {
        // child finished, reaped it just now
        j.status = false;
    }
}
void update_all_jobs(vector<job> &jobs)
{
    for (auto &j : jobs)
        update_job_status(j);
}

int main()
{
    bool stop = false;
    cout << "\033[1;32m AyTerminal v3 \033[0m\n";

    while (!stop)
    {
        update_all_jobs(jobs); // Update the status of all jobs before displaying the prompt

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

        if (instructions_parsed.empty())continue;

        // EXIT
        if (instructions_parsed[0] == "exit" || instructions_parsed[0] == "Exit" || instructions_parsed[0] == "EXIT")
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
		continue;
            }

            catch (const std::filesystem::filesystem_error& e)
            {
                cout << "cd: " << e.what() << '\n';
            }
        }

        //job
	else if (instructions_parsed[0] == "job")
        {
            if (jobs.empty())
            {
                cout << "No background jobs.\n";
                continue;
            }
            for (const auto& job : jobs)
            {
                cout << "PID: " << job.pid << " | Command: " << job.command << " | Status: " << (job.status ? "Running" : "Stopped") << "\n";
            }
        }

        // OTHER COMMANDS
        else
        {
            bool pipe = false;
            bool outputReDirection = false;
            bool inputReDirection = false;
            bool background = false;
            for (const auto& arg : instructions_parsed)
            {
                if (arg == "|")
                {
                    execute_pipe(instructions_parsed);
                    pipe = true; 
                }
                else if (arg == ">")
                {
                    outputReDirection = true;
                    execute_outputReDirection(instructions_parsed);
                }
                else if (arg == "<")
                {
                    inputReDirection = true;
                    execute_inputReDirection(instructions_parsed);
                }
                else if (arg == "&")
                {
                    background = true;
                    execute_background(instructions_parsed);
                }
            }
            if (pipe || outputReDirection || inputReDirection || background)continue;

            execute(instructions_parsed);
        }
    }

    return 0;
}
