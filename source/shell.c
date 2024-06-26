// Include the shell header file for necessary constants and function declarations
#include "shell.h"

// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
  static int first_time = 1;
  if (first_time)
  {
    // Clear the screen on the first call
#ifdef _WIN32
    system("cls"); // Windows command to clear screen
#else
    //system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }
  struct utsname uts;
  uname(&uts);
  char *user=getenv("USER");
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  fflush(stdout); // Flush the output buffer

  char prompt_text[PATH_MAX*2];
  snprintf(prompt_text, sizeof(prompt_text), "\e[1;34m%s@%s:\e[1;31m %s", user, uts.nodename, cwd);
  printf("%s %s $$ ", prompt_text, text_color);  // Print the shell prompt
}

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
  else if (strcmp(args[1], "color") == 0){
      printf("Type: color COLOR to change the color of text input\n");
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

void shell_no_bold(char *args){
  if (strcmp(args, "black") == 0){
      text_color = COLOR_BLACK;
    }
  else if (strcmp(args, "red") == 0){
      text_color = COLOR_RED_BOLD;
    }
  else if (strcmp(args, "green") == 0){
      text_color = COLOR_GREEN;
    }
  else if (strcmp(args, "yellow") == 0){
      text_color = COLOR_YELLOW;
    }
  else if (strcmp(args, "blue") == 0){
      text_color = COLOR_BLUE;
    }
  else if (strcmp(args, "magenta") == 0){
      text_color = COLOR_MAGENTA;
    }
  else if (strcmp(args, "cyan") == 0){
      text_color =  COLOR_CYAN;
    }
  else if (strcmp(args, "white") == 0){
      text_color =  COLOR_WHITE;
    }
  else {
    printf("The command you gave: %s, is not part of the CSEShell's builtin command\n", args);
    return;
    }
  printf("Set color of input to %s\n", args);
}

void shell_bold(char *args){
  if (strcmp(args, "black") == 0){
      text_color = COLOR_BLACK_BOLD;
    }
  else if (strcmp(args, "red") == 0){
      text_color = COLOR_RED_BOLD;
    }
  else if (strcmp(args, "green") == 0){
      text_color = COLOR_GREEN_BOLD;
    }
  else if (strcmp(args, "yellow") == 0){
      text_color = COLOR_YELLOW_BOLD;
    }
  else if (strcmp(args, "blue") == 0){
      text_color = COLOR_BLUE_BOLD;
    }
  else if (strcmp(args, "magenta") == 0){
      text_color = COLOR_MAGENTA_BOLD;
    }
  else if (strcmp(args, "cyan") == 0){
      text_color =  COLOR_CYAN_BOLD;
    }
  else if (strcmp(args, "white") == 0){
      text_color =  COLOR_WHITE_BOLD;
    }
  else {
    printf("The command you gave: %s, is not part of the CSEShell's builtin command\n", args);
    return;
    }
  printf("Set color of input to %s\n", args);
}

int shell_color(char **args) {
  if (args[1] == NULL) {
      printf("Command not given. Type color <color option>.\n");
    } 
  else if (strcmp(args[1], "reset") == 0)
  {
    text_color = COLOR_RESET;
  }
  
  else if (args[2] == NULL || strcmp(args[2], "bold") != 0)
  {
    shell_no_bold(args[1]);
  }
  else
  {
    shell_bold(args[1]);
  }
    return 1; // Return 1 to continue running the shell
}

int shell_resource(){
    char cmd[PATH_MAX];
    snprintf(cmd, sizeof(cmd), "ps -p %d -o %%cpu,%%mem,cmd", getpid());

    // Execute the command and read the output
    FILE *fp;
    char output[1024];

    fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    // Read the output from the command
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
    }

    // Close the pipe
    pclose(fp);
    return 0;
}

void process_rc_file(){
  
  const char *filePath = ".cseshellrc";
  FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

  //SUCCESSFULLY OPEN FILE
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file))
    {
      //skip program if line is empty
      if (strcmp(line, "\n") == 0){
       continue;
      }

      //clear all trailing whitespaces
      line[strcspn(line, " \n")] = 0;
      
      if (strncmp(line, "PATH=", 5) == 0) {
          if (setenv("PATH", line + 5, 1) != 0) {
            perror("Failed to set PATH");
          } else {
              // Debug: Confirm PATH is set
              printf("PATH set to: %s\n", getenv("PATH"));
          }
      }

      else{

        pid_t pid = fork();

        if (pid < 0){
          perror("Failed to fork. \n");
          exit(1);
        }
        if (pid == 0){
          //child process
          char *cmd[MAX_ARGS];
          char *token= strtok(line, " ");
          //printf(line);
          int i = 0;
          while (token != NULL && i < MAX_ARGS - 1) {
              cmd[i++] = token;
              token = strtok(NULL, " ");
          }
          cmd[i] = NULL;

          printf("Executing command: %s\n", cmd[0]);

          execvp(cmd[0], cmd);

          perror("execvp failed");
          _exit(1);
        }
        else {
          //parent process
          int status;
          waitpid(pid, &status, 0);

          //printf("parent process running\n");
        }
      }
    }
  fclose(file);
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;
  process_rc_file();

  while (1){
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    //continue while loop if no arguments
    if(cmd[0] == NULL){
      continue;
    }

    //in-built exit command
    // if (strcmp(cmd[0], "exit") == 0){
    //   // break;
    //   return 0;
    // }

    if (execute_builtin_command(cmd) == 1) {
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
        cmd[i] = NULL;
      }
      continue; // Go to the next iteration of the loop
    }

    pid = fork();

    // if pid < 0 failed to fork
    if (pid < 0){
       perror("Failed to fork. \n");
       exit(1);
    }

    // if pid == 0, child process
    if (pid == 0){
      // Formulate the full path of the command to be executed
      char full_path[PATH_MAX];
      char cwd[1024];
  
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
      }
      else
      {
        printf("Failed to get current working directory.");
        exit(1);
      }

      execv(full_path, cmd);

      // If execv returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);

      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
        cmd[i] = NULL;
      }
      memset(cwd, '\0', sizeof(cwd)); // clear the cwd array
      exit(1);
    }

    // if pid != 0, parent process
    else{
      
      int status, child_exit_status;
      // waitpid(pid, &status, WUNTRACED); //original waitpid code

      struct rusage usage;
      wait4(pid, &status, 0, &usage);
      
      //printf(("parent process\n")); //debugging line of code
      // if child terminates properly,
      if (WIFEXITED(status))
      {
        child_exit_status = WEXITSTATUS(status);
        for (int i = 0; cmd[i] != NULL; i++)
        {
          free(cmd[i]);
          cmd[i] = NULL;
        }
      } // checks child_exit_status and do something about it

      printf("CPU time used: User = %ld.%06lds, System = %ld.%06lds\n",
               (long)usage.ru_utime.tv_sec, (long)usage.ru_utime.tv_usec,
               (long)usage.ru_stime.tv_sec, (long)usage.ru_stime.tv_usec);
      
      shell_resource();
    }
  }
  return 0;
}