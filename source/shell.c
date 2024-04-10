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
  char *array[MAX_ARGS], *commandToken;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int currentChar = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)currentChar;
    // If a newline character is encountered, break out of the loop
    if (currentChar == '\n')
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
  commandToken = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (commandToken != NULL)
  {
    array[i++] = strdup(commandToken);  // Duplicate the token and store it
    commandToken = strtok(NULL, " \n"); // Get the next token
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
    system("clear"); // UNIX/Linux command to clear screen
#endif
    first_time = 0;
  }
  fflush(stdout); // Flush the output buffer
  printf("$$ ");  // Print the shell prompt
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int child_status;
  pid_t pid;

  type_prompt();     // Display the prompt
  read_command(cmd); // Read a command from the user

  // If the command is "exit", break out of the loop to terminate the shell
  if (strcmp(cmd[0], "exit") == 0)
    // break;
    return 0;

  // Formulate the full path of the command to be executed
  char fullPath[PATH_MAX];
  snprintf(fullPath, sizeof(fullPath), "./bin/%s", cmd[0]);

  execv(fullPath, cmd);

  // If execv returns, command execution has failed
  printf("Command %s not found\n", cmd[0]);
  exit(0);

  // Free the allocated memory for the command arguments before exiting
  for (int i = 0; cmd[i] != NULL; i++)
  {
    free(cmd[i]);
  }

  return 0;
}