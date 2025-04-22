#include "thunt.h"
int active_monitor = 0;
pid_t monitor_pid;
char huntid[6], id[5];
int hunt_cnt;
int treasure_cnt[MAXSIZE];

//still not done
void list_hunts(int sig)
{
    printf("\n");
    system("./treasure_manager --list_hunts");
}

void list_treasures(int sig)
{
    printf("\n");
    printf("Enter huntid: "); scanf("%s", huntid);
    char comm[256];
    sprintf(comm,"./treasure_manager --list %s", huntid);
    system(comm);

}

void view_treasure(int sig)
{
    printf("\n");
    printf("Enter huntid: "); scanf("%s", huntid);
    printf("Enter id: "); scanf("%s", id);
    char comm[256];
    sprintf(comm,"./treasure_manager --view %s %s", huntid, id);
    system(comm);
}

void stop_monitor()
{
    int status;

    if (kill(monitor_pid, SIGTERM) < 0)
    {
        printf("Error sending SIGTERM to child\n");
        exit(2);
    }

    wait(&status);
    printf("Monitor procces ended with code %d\n", WEXITSTATUS(status));
}

void _exit(int sig)
{
    exit(0);
}

void monitor_procces()
{

    struct sigaction monitor_actions;
    memset(&monitor_actions, 0x00, sizeof(struct sigaction));

    monitor_actions.sa_handler = list_hunts;
    if (sigaction(SIGUSR1, &monitor_actions, NULL) < 0)
    {
        perror("Process a SIGUSR1 sigaction");
        exit(-1);
    }

    monitor_actions.sa_handler = list_treasures;
    if (sigaction(SIGUSR2, &monitor_actions, NULL) < 0)
    {
        perror("Process a SIGUSR2 sigaction");
        exit(-1);
    }

    monitor_actions.sa_handler = view_treasure;
    if (sigaction(SIGILL, &monitor_actions, NULL) < 0)
    {
        perror("Process a SIGILL sigaction");
        exit(-1);
    }

    monitor_actions.sa_handler = _exit;
    if (sigaction(SIGTERM, &monitor_actions, NULL) < 0)
    {
        perror("Process a SIGTERM sigaction");
        exit(-1);
    }

    while (1)
    {
        pause();
    }
}

int main(void)
{

    char action[100] = "";

    while (1)
    {

        printf("Select an action (type help to view list of actions): ");
        scanf("%s", action);

        if (strcmp(action, "start_monitor") == 0)
        {
            if (active_monitor == 1)
            {
                printf("Monitor already running\n");
            }
            else
            {
                active_monitor = 1;
                printf("Monitor started\n");
                if ((monitor_pid = fork()) < 0)
                {
                    printf("Child process not created\n");
                    exit(1);
                }
                if (monitor_pid == 0)
                {
                    monitor_procces();
                    exit(0);
                }
                else
                {
                    sleep(1);
                    printf("\n");
                    continue;
                }
            }
        }
        else if (strcmp(action, "list_hunts") == 0)
        {
            if (active_monitor == 0)
            {
                printf("Start monitor before executing list_hunt action\n");
            }
            else
            {

                if (kill(monitor_pid, SIGUSR1) < 0)
                {
                    printf("Error sending SIGUSR to child\n");
                    exit(2);
                }
                sleep(2);
            }
        }
        else if (strcmp(action, "list_treasures") == 0)
        {

            if (active_monitor == 0)
            {
                printf("Start monitor before executing list_treasures action\n");
            }
            else
            {

                if (kill(monitor_pid, SIGUSR2) < 0)
                {
                    printf("Error sending SIGUSR to child\n");
                    exit(2);
                }
                //waitpid(monitor_pid, NULL, 0);
                //wait(NULL);
                sleep(7);
            }
        }
        else if (strcmp(action, "view_treasure") == 0)
        {
            if (active_monitor == 0)
            {
                printf("Start monitor before executing view_treasure action\n");
            }
            else
            {

                if (kill(monitor_pid, SIGILL) < 0)
                {
                    printf("Error sending SIGILL to child\n");
                    exit(2);
                }
                //waitpid(monitor_pid, NULL, 0);
                //wait(NULL);
                sleep(7);
            }
        }
        else if (strcmp(action, "stop_monitor") == 0)
        {
            if (active_monitor == 0)
            {
                printf("Monitor is not currently running");
            }
            else
            {
                stop_monitor();
                active_monitor = 0;
            }
        }
        else if (strcmp(action, "exit") == 0)
        {
            if (active_monitor == 1)
            {
                printf("Monitor running, please close monitor before exiting.\n ");
            }
            else
            {
                exit(0);
            }
        }
        else if (strcmp(action, "help") == 0)
        {
            sleep(1);
            printf("\n List of actions: <utilisation> \n\n Start monitor: start_monitor \n List hunts: list_hunts \n List treasures: list_treasures \n View treasure: view_treasure \n Stop monitor: stop_monitor \n Exit: exit\n");
        }
        else
        {
            printf("Invalid action");
        }
        printf("\n");
    }
}