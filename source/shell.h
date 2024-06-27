#include <limits.h> // For PATH_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/utsname.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define BIN_PATH "./bin/"
#define PATH_MAX 4096 
#define WORK_DIR "/home/marcu/50005-pa1-2024-template"

#define COLOR_BLACK "\e[0;30m"
#define COLOR_RED "\e[0;31m"
#define COLOR_GREEN "\e[0;32m"
#define COLOR_YELLOW "\e[0;33m"
#define COLOR_BLUE "\e[0;34m"
#define COLOR_MAGENTA "\e[0;35m"
#define COLOR_CYAN "\e[0;36m"
#define COLOR_WHITE "\e[0;37m"

#define COLOR_BLACK_BOLD "\e[1;30m"
#define COLOR_RED_BOLD "\e[1;31m"
#define COLOR_GREEN_BOLD "\e[1;32m"
#define COLOR_YELLOW_BOLD "\e[1;33m"
#define COLOR_BLUE_BOLD "\e[1;34m"
#define COLOR_MAGENTA_BOLD "\e[1;35m"
#define COLOR_CYAN_BOLD "\e[1;36m"
#define COLOR_WHITE_BOLD "\e[1;37m"

#define COLOR_RESET "\e[0m"

#define USER_LIGHT "\e[1;47;36m"
#define PATH_LIGHT "\e[1;47;35m"
#define COLOR_BLACK_WHITEBG "\e[0;47;30m"
#define USER_DARK "\e[1;40;32m"
#define PATH_DARK "\e[1;40;33m"
#define COLOR_WHITE_BLACKBG "\e[0;40;37m"

char *text_color = COLOR_RESET;
char *user_color = COLOR_BLUE_BOLD;
char *path_color = COLOR_RED;

const char *builtin_commands[] = {
    "cd",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",  //  List all builtin commands in the shell
    "exit",  // Exits the shell
    "usage", // Provides a brief usage guide for the shell and its built-in command
    "env", // Lists all the environment variables currently set in the shell
    "setenv", // Sets or modifies an environment variable for this shell session
    "unsetenv", // Removes an environment variable from the shell
    "color",
    "theme"
    };

/*
Handler of each shell builtin function
*/
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_usage(char **args);
int list_env(char **args);
int set_env_var(char **args);
int unset_env_var(char **args);
int shell_color(char **args);
void shell_no_bold(char *args);
void shell_bold(char *args);
int shell_theme(char **args);

/*** This is array of functions, with argument char ***/
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd 
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var, // builtin_command_func[6]: unsetenv
    &shell_color,
    &shell_theme
};