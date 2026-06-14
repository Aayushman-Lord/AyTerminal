#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

using std::cout, std::vector, std::cin, std::string;

auto argToChar(const std::vector<std::string> &arg)
{
    std::vector<char *> args;

    for (const auto &str : arg)
    {
        args.push_back(const_cast<char *>(str.c_str()));
    }

    args.push_back(nullptr);

    return args;
}

// Execute a command without redirection or pipe (e.g., ls -l)
int execute(const vector<string> &args)
{
    auto char_args = argToChar(args);

    pid_t pid = fork();

    if (pid == 0)
    {
        execvp(char_args[0], char_args.data());
        if (errno == ENOENT)
            cout << char_args[0] << ": command not found\n";
        else
        {
            cout << "Error executing command: " << char_args[0];
            cout << strerror(errno) << "\n";
        }
        return 1;
    }

    else if (pid > 0)
    {
        waitpid(pid, nullptr, 0);
    }

    else
    {
        cout << "Failed to fork process.\n";
    }

    return 0;
}

// Handle output redirection (e.g., ls > output.txt)
int execute_outputReDirection(const vector<string> &args)
{
    vector<string> command, file;
    bool found = false;

    for (const auto &arg : args)
    {
        if (arg == ">")
        {
            found = true;
            continue;
        }
        if (!found)
            command.push_back(arg);
        else
            file.push_back(arg);
    }

    if (file.empty())
    {
        cout << "Error: No output file specified for redirection.\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        int fd = open(file[0].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_WRONLY → write only O_CREAT → create file if it doesn't exist O_TRUNC → erase old contents 0644 → file permissions
        
        if (fd == -1) // Check if the file was opened successfully
        {
            perror("open");
        }

        dup2(fd, STDOUT_FILENO); // Redirect standard output to the file
        close(fd); // Close the file descriptor as it's no longer needed

        execvp(command[0].c_str(), argToChar(command).data());
        
        cout << "Error executing command: " << command[0];
        cout << strerror(errno) << "\n";
        return 1;
    }
    else if (pid > 0)
    {
        waitpid(pid, nullptr, 0);
    }
    else
    {
        cout << "Failed to fork process.\n";
    }

    return 0;
}

// Handle input redirection (e.g., sort < input.txt)
int execute_inputReDirection(const vector<string> &args)
{
    vector<string> command, file;
    bool found = false;
    for (const auto &arg : args)
    {
        if (arg == "<")
        {
            found = true;
            continue;
        }
        
        else if (!found)
            command.push_back(arg);
        
        else
            file.push_back(arg);
    }

    if (file.empty())
    {
        cout << "Error: No input file specified for redirection.\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        int fd = open(file[0].c_str(), O_RDONLY); // O_RDONLY → read only
        
        if (fd == -1) // Check if the file was opened successfully
        {
            perror("open");
        }

        dup2(fd, STDIN_FILENO); // Redirect standard input to the file
        close(fd); // Close the file descriptor as it's no longer needed

        execvp(command[0].c_str(), argToChar(command).data());
        
        cout << "Error executing command: " << command[0];
        cout << strerror(errno) << "\n";
        return 1;
    }
    else if (pid > 0)
    {
        waitpid(pid, nullptr, 0);
    }
    else
    {
        cout << "Failed to fork process.\n";
        return 1;
    }
    return 0;
}

// Handle pipe (e.g., ls -l | grep "txt")
int execute_pipe(const vector<string> &args)
{
    vector<string> command1, command2;
    bool found = false;
    for (auto & word: args)
    {
        if (word == "|")
        {
            found = true;
        }
        else if(!found)
        {
            command1.push_back(word);
        }
        else
        {
            command2.push_back(word);
        }
    }

    auto command1_arg = argToChar(command1);
    auto command2_arg = argToChar(command2);

    if (command1.empty() || command2.empty())
    {
        cout << "Error: Invalid pipe command. Both sides of the pipe must have a command.\n";
        return 1;
    }
    
    int fileD[2];// file descriptors for the pipe

    if (pipe(fileD) == -1)
    {
        cout << "Failed to create pipe.\n";
        return 1;
    }

    pid_t pid1 = fork();

    if (pid1 == 0)
    {
        dup2(fileD[1], STDOUT_FILENO); // Redirect standard output to the write end of the pipe
        close(fileD[0]); // Close the read end of the pipe in the first child
        close(fileD[1]); // Close the write end of the pipe in the first child

        execvp(command1_arg[0], command1_arg.data());
        cout << "Error executing command: " << command1[0];
        cout << strerror(errno) << "\n";
        return 1;
    }
    else if (pid1 > 0)
    {
        waitpid(pid1, nullptr, 0);

        pid_t pid2 = fork();

        if (pid2 == 0)
        {
            dup2(fileD[0], STDIN_FILENO); // Redirect standard input to the read end of the pipe
            close(fileD[1]); // Close the write end of the pipe in the second child
            close(fileD[0]); // Close the read end of the pipe in the second child

            execvp(command2_arg[0], command2_arg.data());
            cout << "Error executing command: " << command2[0];
            cout << strerror(errno) << "\n";
            return 1;
        }
        else if (pid2 > 0)
        {
            close(fileD[0]); // Close both ends of the pipe in the parent
            close(fileD[1]);
            waitpid(pid2, nullptr, 0);
        }
        else
        {
            cout << "Failed to fork second process.\n";
            return 1;
        }
    }
    else
    {
        cout << "Failed to fork first process.\n";
        return 1;
    }

    return 0;
}
