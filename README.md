# Minishell - A Simple Unix Shell Implementation

## 📌 Description

**Minishell** is a lightweight shell written in **C**, designed to replicate a subset of the behavior of Unix shells like Bash or Zsh. It supports command execution, pipelines, redirections, environment variable handling, and built-in commands.

Users can interact with the shell through standard input and execute commands with various functionalities, including redirections (`<`, `>`, `>>`), pipes (`|`), and heredocs (`<<`).

---

## 🎮 Features

- 🏗 **Built-in commands**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- 🔗 **Pipes (`|`)**: Execute multiple commands in sequence
- 📂 **Redirections (`<`, `>`, `>>`)**: Handle input/output files
- 🔁 **Heredoc (`<<`)**: Read input until a defined delimiter
- 🏗 **Abstract Syntax Tree (AST)**: Efficient parsing and execution
- ⚙️ **Process handling**: Execution of external commands
- 📜 **History management**: Save and clear command history
- 🚀 **Signal handling**: Manage `SIGINT`, `SIGQUIT` for a smooth experience
- 🛠 **Error handling and input validation**

---

## 🛠 Compilation and Execution

### 🔧 **Compilation**

Compile the project using:

```sh
make
```

### ▶️ **Execution**

Run the shell with:

```sh
./minishell
```

or

```sh
make run
```

---

## ⌨️ Controls

| Key Combination | Action                      |
| -------------- | -------------------------- |
| `CTRL + C`     | Interrupt current command  |
| `CTRL + D`     | Exit the shell             |
| `CTRL + \`     | Ignore (SIGQUIT)           |

---

## 📂 Project Structure

```
42_minishell/
├── include/                  # Header files
│   ├── app.h
│   ├── ast_builder.h
│   ├── ast_exec.h
│   ├── built_in.h
│   ├── input.h
│   ├── minishell.h            # Main header file
│   ├── struct.h
│   ├── tokenizer.h
│   ├── utils.h
├── src/                      # Source files
│   ├── app.c
│   ├── built_in.c
│   ├── built_in_bis.c
│   ├── exec_ast.c
│   ├── exec_ast1.c
│   ├── exec_ast2.c
│   ├── input.c
│   ├── main.c                 # Entry point
│   ├── minishell.c
│   ├── prepare_ast.c
│   ├── tokenizer.c
│   ├── tokenizer1.c
│   ├── utils.c
├── Makefile                   # Compilation script
├── minishell.subject.pdf       # Project subject file
├── README.md                   # Project documentation
├── .gitignore                  # Git ignore rules
```

---

## 📜 Author

- **vbonnard** (<vbonnard@student.42perpignan.fr>)
- **ezeppa** (<ezeppa@student.42perpignan.fr>)

---

## 📌 Note

This project was developed as part of **42 School**. If you encounter a bug or have suggestions, feel free to contribute! 🚀
