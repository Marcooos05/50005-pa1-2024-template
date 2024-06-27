#include "system_program.h"

void getname(struct utsname uts){
    if (uname(&uts) < 0)
    {perror("uname error");}
    else{
        printf("OS name:  %s\n", uts.sysname);
        printf("Host name:  %s\n", uts.nodename);
        printf("Kernel:  %s\n", uts.release);
        // printf("Version:  %s\n", uts.version); 
        // printf("Machine:  %s\n", uts.machine);
    }
    return;
}

void getsysinfo(struct sysinfo sys){
    if (sysinfo(&sys) < 0)
    {perror("sysinfo error");}
    else{
        long total_seconds = sys.uptime;
        long seconds = total_seconds%60;
        long hours = total_seconds/3600;
        long minutes = total_seconds/60 - hours*60;

        printf("Uptime:  %ld Hours %ld Min %ld Sec\n", hours, minutes, seconds);
        printf("Total Ram Space:  %ld KB\n", sys.totalram/1000);
        printf("Total Swap Space:  %ld KB\n", sys.totalswap/1000);
    }
}

int main()
{
    
    struct utsname uts;
    getname(uts);

    char *user=getenv("USER");
    if(user==NULL) {
        perror("User error");
    }else{
        printf("User:  %s\n",user);
    }

    char *shell=getenv("SHELL");
    if(shell==NULL) {
        perror("Shell error");
    }else{
        printf("Shell:  %s\n",shell);
    }

    if (printf("CPU:  ") < 0){
        perror("CPU info error");
    }else{
        fflush(stdout);
        system("lscpu | grep 'Model name' | cut -f 2 -d ':' | awk '{$1=$1}1'");
    }
    
    struct sysinfo sys;
    getsysinfo(sys);

    return EXIT_SUCCESS;
}