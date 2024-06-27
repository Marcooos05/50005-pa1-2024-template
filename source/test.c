#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void) {
    // printf("\e[1;42mHello\033[0m");
    printf("\e[1;47;32mREMEMBER TO BE \e[3;32m~ECO FRIENDLY`~\e[0;0m\n");
}

// int main(){
//     char* command = "wget";
//     char* argument_list[] = {"wget", "https://www.youtube.com/watch?v=SQ2ufFGm9xE", NULL};

//     printf("Before calling execvp()\n");

//     printf("Creating another process using fork()...\n");

//     if (fork() == 0) {
//         // Newly spawned child Process. This will be taken over by "ls -l"
//         system("xdg-open http://www.y8.com");

//         printf("wget has taken control of this child process. This won't execute unless it terminates abnormally!\n");
//         exit(1);
//     }
//     else {
//         // Old Parent process. The C program will come here
//         printf("This line will be printed\n");
//     }

//     return 0;
// }