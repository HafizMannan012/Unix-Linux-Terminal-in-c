#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

int main() {
  char input[MAX_COMMAND_LENGTH];
  char current_directory[MAX_COMMAND_LENGTH];

  while (1) {
    // Display shell prompt and read user input
    printf("%s> ", getcwd(current_directory, MAX_COMMAND_LENGTH));
    fgets(input, MAX_COMMAND_LENGTH, stdin);

    // Remove newline character from the input
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n') {
      input[input_length - 1] = '\0';
    }

    // Check if the user wants to exit
    if (strcmp(input, "exit") == 0) {
      break;
    }

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

    // Check for specific commands and execute them
    if (strcmp(args[0], "cp") == 0) {
      // Handle the 'cp' command
      // You can execute 'cp' by copying args[1] to args[2]
      if (arg_count == 3) {
        pid_t child_pid = fork();

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
    } else if (strcmp(args[0], "mkdir") == 0) {
      // Handle the 'mkdir' command
      if (arg_count == 2) {
        pid_t child_pid = fork();

        if (child_pid < 0) {
          perror("Fork failed");
          exit(1);
        } else if (child_pid == 0) {
          execlp("mkdir", "mkdir", args[1], NULL);
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
        printf("Usage: mkdir <directory_name>\n");
      }
    } else if (strcmp(args[0], "ls") == 0) {
      // Handle the 'ls' command
      pid_t child_pid = fork();

      if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
      } else if (child_pid == 0) {
        execlp("ls", "ls", NULL);
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
    } else if (strcmp(args[0], "cd") == 0) {
      // Handle the 'cd' command
      if (arg_count == 2) {
        if (chdir(args[1]) != 0) {
          perror("chdir failed");
        }
      } else {
        printf("Usage: cd <directory_name>\n");
      }
    } else if (strcmp(args[0], "cd..") == 0) {
      // Handle the 'cd..' command
      if (chdir("..") != 0) {
        perror("chdir failed");
      }
    } else if (strcmp(args[0], "cd~") == 0) {
      // Handle the 'cd~' command
      if (chdir(getenv("HOME")) != 0) {
        perror("chdir failed");
      }
    } else if (strcmp(args[0], "touch") == 0) {
      // Handle the 'touch' command
      if (arg_count == 2) {
        pid_t child_pid = fork();

        if (child_pid < 0) {
          perror("Fork failed");
          exit(1);
        } else if (child_pid == 0) {
          execlp("touch", "touch", args[1], NULL);
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
        printf("Usage: touch <filename>\n");
      }
    } else if (strcmp(args[0], "rmkdir") == 0) {
      // Handle the 'rmkdir' command
      if (arg_count == 2) {
        pid_t child_pid = fork();

        if (child_pid < 0) {
          perror("Fork failed");
          exit(1);
        } else if (child_pid == 0) {
          execlp("rm", "rm", "-rf", args[1], NULL);
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
        printf("Usage: rmkdir <directory_name>\n");
      }
    } else {
      // If it's not a recognized command, execute as is
      pid_t child_pid = fork();

      if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
      } else if (child_pid == 0) {
        execvp(args[0], args);
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
    }
  }

  return 0;
}
