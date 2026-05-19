# AyTerminal

AyTerminal is a small custom Linux shell written in C++ as a learning project.

The goal of this project is to explore how real shells work internally while learning:

* command parsing
* terminal behavior
* filesystem interaction
* process execution
* shell architecture

Currently developed and tested on WSL using Ubuntu.

---

# Features

## Current Features

* Custom shell prompt
* Command parsing
* Built-in `cd` command
* Built-in `exit` command
* Command execution
* GNU Readline integration
* Arrow key navigation
* Command history support
* Ctrl + D exit support
* Colored terminal prompt

---

# Preview

```bash
[/home/aayushman] SHELL$
>
```

Supports:

* Up / Down arrow history
* Left / Right cursor movement
* Editable command lines

---

# Technologies Used

* C++
* GNU Readline
* STL
* `<filesystem>`

---

# Build Instructions

## Requirements

Install GNU Readline:

```bash
sudo apt install libreadline-dev
```

---

## Compile

```bash
g++ main.cpp Execute.cpp Parser.cpp -o AyT -lreadline
```

---

## Run

```bash
./AyT
```

---

# Project Structure

```text
AyTerminal/
│
├── main.cpp
├── Execute.cpp
├── Execute.h
├── Parser.cpp
├── Parser.h
└── README.md
```

---

# Goals

This project is mainly for:

* learning Linux internals
* understanding shell behavior
* improving C++ skills
* experimenting with terminal systems

Future plans may include:

* piping (`|`)
* redirection (`>`)
* tab completion
* aliases
* environment variables
* shell scripting
* process management

---

# Why This Project Exists

Most people use shells every day without thinking about how they work internally.

AyTerminal exists as a way to learn:

* how terminals process input
* how shells execute commands
* how UNIX-like systems behave

instead of treating the terminal like magic.

---

# Notes

This is currently a learning project and not intended to replace real shells like:

* bash
* zsh
* fish

But the goal is to gradually evolve it into a more capable shell over time.

---

# License

MIT License
