#include "system_program.h"

int main(){

    char *backup_dir = getenv("BACKUP_DIR");

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

    char filename[2048];
    snprintf(filename, sizeof(filename), "backup-%s.zip", date);

    char zip_cmd[PATH_MAX];
    snprintf(zip_cmd, sizeof(zip_cmd), "zip -r %s %s", filename, backup_dir);

    system(zip_cmd);

    char move_cmd[PATH_MAX];
    snprintf(move_cmd, sizeof(move_cmd), "mv %s ./archive", filename);

    system(move_cmd);

    return EXIT_SUCCESS;
}