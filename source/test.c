#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
/* limits.h defines "PATH_MAX". */
#include <limits.h>
#include <ctype.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <time.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/utsname.h>

#include <sys/sysinfo.h>
#include <stdio.h>

int main(){

    char *backup_dir = "./files";

    if (!backup_dir) {
        fprintf(stderr, "BACKUP_DIR environment variable is not set.\n");
        return 1;
    }

    FILE *fp;
    char date[1024];

    fp = popen("date '+%Y%m%d%H%M%S'", "r");
    if (fp == NULL) {
        printf("Failed to run command\n");
        exit(1);
    }
    fgets(date, sizeof(date), fp);
    date[strcspn(date, " \n")] = 0;

    pclose(fp);

    char zip_cmd[PATH_MAX];

    snprintf(zip_cmd, sizeof(zip_cmd), "zip -r backup-%s.zip %s", date, backup_dir);

    printf(zip_cmd);

    // char zipname[1024] = "backup-";
    // 
    // strcat(zipname, date);
    // strcat(zipname, ".zip ");

    // char command[1024] = "zip -r ";

    // strcat(command, zipname);
    // strcat(command, backup_dir);
    // system(command);
}