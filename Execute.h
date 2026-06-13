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
