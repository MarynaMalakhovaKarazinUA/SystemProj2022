#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

pid_t my_daemon() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("fork failed!\n");
        exit(1);
    }

    chdir("/");

    openlog("firstDaemon", LOG_PID, LOG_DAEMON);
    return pid;
}

int main() {
    pid_t pid = my_daemon();
    if (pid == 0) {
        while(1) {
            syslog(LOG_NOTICE, "First daemon run");
            sleep(15);
        }
    }
    else {
        int choice;
        while(choice != 0) {
            printf("\nEnter \"0\" to end the program");
            printf("\n>> ");
            scanf(" %d", &choice);
        }
        kill(pid, SIGKILL);
        syslog(LOG_NOTICE, "First daemon terminated.");
        closelog();
    }
    return 0;
}
