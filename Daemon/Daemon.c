#include <stdio.h>
#include <stdlib.h>

#include <syslog.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>

static void process_demon();

int main()
{
    process_demon();
    while(1)
    {
        syslog(LOG_NOTICE, "Daemon started.");
        sleep(20);
        break;
    }
    syslog(LOG_NOTICE, "Daemon terminated.");
    closelog();
    return EXIT_SUCCESS;
}

static void process_demon()
{
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    if((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    int tmp;
    
    for (tmp = sysconf(_SC_OPEN_MAX); tmp >= 0; tmp--)
    {
        close(tmp);
    }

    openlog("daemon", LOG_PID, LOG_DAEMON);
}
