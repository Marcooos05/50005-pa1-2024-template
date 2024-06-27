#include "system_program.h"
#define PATH_MAX 4096 

char output_file_path[PATH_MAX];

static int daemon_work()
{
    // put your full PROJECT_DIR path here  
    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {

        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int main(){

    // Setup path
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log"); 

    //STEP 1
    pid_t pid = fork();

    // if pid < 0 failed to fork
    if (pid < 0){
       perror("Failed to fork. \n");
       exit(1);
    }

    //child process
    if (pid == 0){

        //STEP 3
        if (setsid() < 0) {
            perror("setsid intermediate failed");
            exit(1);
        }

        //printf("Child process has setsid with pid %d, pgid %d, session id: %d\n", getpid(), getpgid(getpid()), getsid(getpid()));

        //STEP 4
        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);

        //STEP 5
        pid_t pid_2 = fork();

        if (pid_2 < 0){
            perror("second fork failed");
            exit(1);
        }

        if (pid_2 == 0) {
            //printf("Daemon process, PID: %d\n", getpid());
            
            //STEP 6
            umask(0);

            //STEP 7
            if (chdir("/") <0){
                perror("chdir failed");
                exit(1);
            }

            //STEP 8
            /* Close all open file descriptors */
            int fd0, fd1, fd2;
            int x;
            for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
            {
                close (x);
            }

            /* Attach file descriptors 0, 1, and 2 to /dev/null. */
            fd0 = open("/dev/null", O_RDWR);
            fd1 = dup(0);
            fd2 = dup(0);

            if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
                perror("Failed to reassign file descriptors");
                exit(1);
            }

            // Step 9: Execute daemon_work()
            daemon_work();
            
            return 0;
        }
        else{
            //printf("Intermediate process exiting, PID: %d\n", getpid());
            exit(1);
        }
    }

    //parent process
    else{
        //printf("Parent process with pid %d, pgid %d, session id :%d\n", getpid(), getpgid(getpid()), getsid(getpid()));

        //STEP 2
        return EXIT_SUCCESS;
    }
}