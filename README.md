# Linux/Unix Terminal
This code is a simple shell program written in C. It allows you to execute various commands, similar to a Unix-like shell (e.g., Bash), by reading user input and executing the corresponding system commands. Below, I'll explain each part of the code:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
```

These are the included header files, which provide declarations and definitions for various functions and data types used in the code.

```c
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
```

These are constants used to set the maximum length of user input and the maximum number of command-line arguments.

```c
int main() {
```

This is the main function where the execution of the program starts.

```c
  char input[MAX_COMMAND_LENGTH];
  char current_directory[MAX_COMMAND_LENGTH];
```

These are character arrays used to store user input and the current working directory.

```c
  while (1) {
```

This is an infinite loop that repeatedly reads user input and executes commands until the user types "exit."

```c
    // Display shell prompt and read user input
    printf("%s> ", getcwd(current_directory, MAX_COMMAND_LENGTH));
    fgets(input, MAX_COMMAND_LENGTH, stdin);
```

This section of code displays a shell prompt, which includes the current working directory, and reads user input using `fgets`.

```c
    // Remove newline character from the input
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
      input[input_length - 1] = '\0';
    }
```

It removes the newline character from the end of the user input if it exists, making it easier to process.

```c
    // Check if the user wants to exit
    if (strcmp(input, "exit") == 0) {
      break;
    }
```

If the user types "exit," the program exits the infinite loop and terminates.

```c
    // Tokenize the input command
    char *token;
    char *args[MAX_ARGUMENTS];
    int arg_count = 0;

    token = strtok(input, " ");
    while (token != NULL && arg_count < MAX_ARGUMENTS) {
      args[arg_count++] = token;
      token = strtok(NULL, " ");
    }

    args[arg_count] = NULL; // Set the last element of the args array to NULL
```

This code tokenizes the user input, splitting it into individual words (tokens). It uses `strtok` to do this and stores the tokens in the `args` array. The last element of the `args` array is set to `NULL` to terminate the list of arguments properly.

```c
    // Check for specific commands and execute them
    if (strcmp(args[0], "cp") == 0) {
      // Handle the 'cp' command
      // You can execute 'cp' by copying args[1] to args[2]
      if (arg_count == 3) {
        pid_t child_pid = fork();
```

Here, the code checks if the first token (the command) is "cp." If it is, it proceeds to execute the "cp" command, which involves creating a child process using `fork()`.

```c
        if (child_pid < 0) {
          perror("Fork failed");
          exit(1);
        } else if (child_pid == 0) {
          execlp("cp", "cp", args[1], args[2], NULL);
          perror("Execvp failed");
          exit(1);
        } else {
          int status;
          waitpid(child_pid, &status, 0);
          if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
          } else {
            perror("Child process did not exit normally");
          }
        }
      } else {
        printf("Usage: cp <source> <destination>\n");
      }
    }
```

If the "cp" command is recognized and the number of arguments is correct (3), it creates a child process using `fork()`. The child process then uses `execlp()` to execute the "cp" command with the specified source and destination arguments. The parent process waits for the child process to complete and prints the exit status.

The code includes similar sections for other commands like "mkdir," "ls," "cd," "touch," and "rm." Each of these sections checks for the corresponding command, handles errors, and executes the command with appropriate arguments if it's recognized.

```c
  return 0;
}
```

Finally, the program returns 0 to indicate successful execution and terminates.
