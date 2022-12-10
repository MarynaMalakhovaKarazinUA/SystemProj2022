#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#define DELAY 3 // in sec
#define REPEAT_TIMES 60

int become_daemon(); // returns -1 on error, 0 on success

int main() {
    int d = become_daemon();

    if(d == -1) {
        printf("failed to start daemon :(\n");
        exit(EXIT_FAILURE);
    }
    
    syslog (LOG_NOTICE, "Daemon started.");
    
    for(int i = 1; i <= REPEAT_TIMES; i++) {
        sleep(DELAY);
        syslog(LOG_NOTICE, "Daemon still alive (%d/%d)\n", i, REPEAT_TIMES);
    }

    syslog (LOG_NOTICE, "Daemon terminated.");
    closelog();

    return 0;
}

int become_daemon() {
    switch(fork()) { // fork main procces
        case -1: return -1; // on error
        case 0: break;                  
        default: exit(EXIT_SUCCESS); // terminate parent proccess
    }

    if(setsid() ==  -1) {
        return -1;
    }

    switch(fork()) {
        case -1: return -1; // on error
        case 0: break;                 
        default: exit(EXIT_SUCCESS); // terminate parent proccess
    }

    umask(0);
    
    if((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    for(int fd = 0; fd < sysconf(_SC_OPEN_MAX); fd++) {
      close(fd);
    }

    openlog("daemon", LOG_PID, LOG_DAEMON);

    return 0;
}