#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

pid_t backDaemon() {
    pid_t pid;

    pid = fork();

    if(pid < 0) {
        printf("Помилка FORK!\n");
        exit(1);
    }

    chdir("/");
    openlog("daemon", LOG_PID, LOG_DAEMON);

    return pid;
}

int main() {
    pid_t pid = backDaemon();

    while(1) {
        syslog(LOG_NOTICE, "Початок роботи Демону");
        sleep(35);
        break;
    }

    syslog(LOG_NOTICE, "Завершення роботи Демону");
    closelog();

    return 0;
}