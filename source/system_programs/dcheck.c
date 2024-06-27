#include "system_program.h"

int main(){
    char command[] = "ps -efj | grep dspawn | grep -Ev 'tty|pts' | wc -l";

    // Execute the command and read the output
    FILE *fp;
    char output[1024];

    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }

    // Read the output from the command
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("Number of live daemon processes: %s", output);
    }

    // Close the pipe
    pclose(fp);

    return EXIT_SUCCESS;
}