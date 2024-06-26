#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#define PATH_MAX 4096 

void execute_command(char *command) {
    struct rusage usage;
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        // In child process
        execlp(command, command, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // In parent process
        wait4(pid, &status, 0, &usage);

        if (WIFEXITED(status)) {
            printf("Command executed successfully with exit status %d\n", WEXITSTATUS(status));
        } else {
            printf("Command execution failed\n");
        }

        printf("CPU time used: User = %ld.%06lds, System = %ld.%06lds\n",
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
               usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        printf("Maximum resident set size: %ld KB\n", usage.ru_maxrss);
        printf("Integral shared memory size: %ld KB\n", usage.ru_ixrss);
        printf("Integral unshared data size: %ld KB\n", usage.ru_idrss);
        printf("Integral unshared stack size: %ld KB\n", usage.ru_isrss);
        printf("Page reclaims (soft page faults): %ld\n", usage.ru_minflt);
        printf("Page faults (hard page faults): %ld\n", usage.ru_majflt);
        printf("Swaps: %ld\n", usage.ru_nswap);
        printf("Block input operations: %ld\n", usage.ru_inblock);
        printf("Block output operations: %ld\n", usage.ru_oublock);
        printf("IPC messages sent: %ld\n", usage.ru_msgsnd);
        printf("IPC messages received: %ld\n", usage.ru_msgrcv);
        printf("Signals received: %ld\n", usage.ru_nsignals);
        printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
        printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
    } else {
        // fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

int main() {
    for (int i = 0; i<10000; i++){
    char command[PATH_MAX];
    snprintf(command, sizeof(command), "ps -p %d -o %%cpu,%%mem,cmd", getpid());

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
        printf("%s", output);
    }

    // Close the pipe
    pclose(fp);

    
    }
    // char command[256];
    // while (1) {
    //     printf("shell> ");
    //     if (fgets(command, sizeof(command), stdin) == NULL) {
    //         break;
    //     }

    //     // Remove newline character from the command
    //     command[strcspn(command, "\n")] = '\0';

    //     if (strcmp(command, "exit") == 0) {
    //         break;
    //     }

    //     execute_command(command);
    // }
    return 0;
}