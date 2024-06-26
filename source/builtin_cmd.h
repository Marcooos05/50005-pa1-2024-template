#include "shell.h"

/*
Helper Function
*/

int num_builtin_functions()
  {
    return sizeof(builtin_commands) / sizeof(char *);
  };

int execute_builtin_command(char **args) {
  // Loop through our command list and check if the commands exist in the builtin command list, function to return -1 if builtin command does not exist
  for (int command_index = 0; command_index < num_builtin_functions(); command_index++) {
    if (strcmp(args[0], builtin_commands[command_index]) == 0) {
      // Execute the builtin command
      return (*builtin_command_func[command_index])(args); //return 1 when builtin command successfully run
    }
  }
  return 0; // Return -1 if the command is not a builtin command
}

/*
EXTRA Built-In Commands
The shell must be expanded support the following 7 builtin commands: cd, help, exit, usage, env, setenv, unsetenv
Handler of each shell builtin function
*/
int shell_cd(char **args) {
    if (args[1] == NULL) {
      chdir(getenv("HOME"));
      //fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1; // Return 1 to continue running the shell
}

int shell_help(char **args) {
    printf("CSEShell Interface\nUsage: command arguments\nThe following commands are implemented within the shell:\n");
    for (int i = 0; i < num_builtin_functions(); i++) {
        printf("%s\n", builtin_commands[i]);
    }
    return 1; // Return 1 to continue running the shell
}

int shell_exit(char **args) {
    exit(0); // Exit the shell
    return 1; // Add return statement for consistency
}

int shell_usage(char **args) {
  if (args[1] == NULL) {
      printf("Command not given. Type usage <command>.\n");
    } 
  else if (strcmp(args[1], "cd") == 0){
      printf("Type: cd directory_name to change the current working directory of the shell\n");
    }
  else if (strcmp(args[1], "help") == 0){
      printf("Type: help for supported commands\n");
    }
  else if (strcmp(args[1], "exit") == 0){
      printf("Type: exit to terminate the shell gracefully\n");
    }
  else if (strcmp(args[1], "usage") == 0){
      printf("Type: usage cd/help/exit\n");
    }
  else if (strcmp(args[1], "env") == 0){
      printf("Type: env to list all registered env variables\n");
    }
  else if (strcmp(args[1], "setenv") == 0){
      printf("Type: setenv ENV=VALUE to set a new env variable\n");
    }
  else if (strcmp(args[1], "unsetenv") == 0){
      printf("Type: unsetenv ENV to remove this env from the list of env variables\n");
    }
  else {
    printf("The command you gave: %s, is not part of the CSEShell's builtin command\n", args[1]);
    }
    return 1; // Return 1 to continue running the shell
}

int list_env(char **args){
  extern char **environ;
  char **env = environ;
  while (*env) { // Loop until NULL pointer is encountered
        printf("%s\n", *env); // Print the current environment variable
        env++; // Move to the next environment variable
    }
  return 1;
}

int set_env_var(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"setenv\"\n");
        return 1;
    }

    char *key = strtok(args[1], "=");
    char *value = strtok(NULL, "=");

    if (key && value) {
        if (setenv(key, value, 1) != 0) {
            perror("shell");
        }
    } else {
        fprintf(stderr, "shell: invalid format for \"setenv\"\n");
    }

    return 1;
}

int unset_env_var(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"unsetenv\"\n");
    } else {
        if (unsetenv(args[1]) != 0) {
            perror("unsetenv");
        }
    }
    return 1; // Return 1 to continue running the shell
}