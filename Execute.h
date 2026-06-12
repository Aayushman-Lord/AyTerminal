#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring> 
    
using std::cout , std::vector, std::cin, std::string;

auto argToChar(const std::vector<std::string> &arg)
{
    std::vector<char *> args;

    for (const auto &str : arg)
    {
        args.push_back(const_cast<char*>(str.c_str())); 
    }

    args.push_back(nullptr);

    return args;
}

int execute(const vector<string>& args)
{
    auto char_args = argToChar(args);

    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(char_args[0], char_args.data());
        if (errno == ENOENT)cout << char_args[0] << ": command not found\n";
        else
        {
            cout << "Error executing command: " << char_args[0];
            cout << strerror(errno) << "\n";
        } 
        return 1;
    }
    
    else if(pid > 0)
    {
        waitpid(pid, nullptr, 0);
    }
    
    else 
    {
        cout << "Failed to fork process.\n";
    }
    
    return 0;
}

int execute_outputReDirection(const vector<string> & args)
{
    cout << "Sorry: This feature is not implemented yet.\n";
    return 0;
}
