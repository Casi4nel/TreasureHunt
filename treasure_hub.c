#include "thunt.h"
int active_monitor = 0;
pid_t monitor_pid;
char huntid[6], id[5];
int hunt_cnt;
int treasure_cnt[MAXSIZE];
mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

void calculate_score()
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        execlp("./calculate_score","calculate_score",(char* )NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
        } 
    }
}

void list_hunts() 
{
    DIR *d = opendir(".");
    if (!d) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    char path[300];
    int count;
    printf("Hunts and treasure counts:\n");
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, sizeof(path), "%s/treasures", entry->d_name);
            int fd = open(path, O_RDONLY);
            if (fd >= 0) 
            {

                count = 0;
                char buf[168]; 
                while (read(fd, buf, sizeof(buf)) > 0) count++;
                close(fd);
                printf("Hunt: %s, Count: %d\n", entry->d_name, count);
            }
        }
    }
    closedir(d);
}

void list_treasures()
{
    printf("\n");
    printf("Enter huntid: "); scanf("%s", huntid);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        execlp("./treasure_manager","treasure_manager","--list", huntid,(char* )NULL);
        perror("execlp");
        _exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
        } 
    }

}

void view_treasure()
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

void handle_commands(int sig)
{
    char command[2];
    int fd;

    if ((fd = open("./commands.txt", O_RDONLY)) < 0)
    {
        exit(-1);
    }

    if (read(fd, command, 2) < 0)
    {
        perror("error reading");
    }
    if (strcmp(command, "1") == 0)
    {
        list_hunts();
    }
    else if (strcmp(command, "2") == 0)
    {
        list_treasures();
    }
    else if (strcmp(command, "3") == 0)
    {
        view_treasure();
    }
    else if (strcmp(command, "4") == 0)
    {
        calculate_score();
    }
    else
    {
        printf("Invalid command\n");
    }

    if (close(fd) < 0)
    {
        perror("Error closing file");
    }
}

void monitor_procces()
{

    struct sigaction monitor_actions;
    memset(&monitor_actions, 0x00, sizeof(struct sigaction));

    monitor_actions.sa_handler = handle_commands;
    if (sigaction(SIGUSR1, &monitor_actions, NULL) < 0)
    {
        perror("Process a SIGUSR1 sigaction");
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
    int fd;
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
                if ((fd = open("./commands.txt", O_CREAT, mode)) < 0)
                {
                    perror("Error creating commands file");
                    exit(-1);
                }
                if (close(fd) == -1)
                {
                    perror("Error closing commands file");
                    exit(-1);
                }
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
                if ((fd = open("./commands.txt", O_WRONLY | O_TRUNC, mode)) == -1)
                {
                    exit(-1);
                }
                if (write(fd, "1", 2) < 0)
                {
                    perror("Error writing in file");
                }
                if (close(fd) < 0)
                {
                    perror("Error closing the file");
                }
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
                if ((fd = open("./commands.txt", O_WRONLY | O_TRUNC, mode)) == -1)
                {
                    exit(-1);
                }
                if (write(fd, "2", 2) < 0)
                {
                    perror("Error writing in file");
                }
                if (close(fd) < 0)
                {
                    perror("Error closing the file");
                }
                if (kill(monitor_pid, SIGUSR1) < 0)
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
                if ((fd = open("./commands.txt", O_WRONLY | O_TRUNC, mode)) == -1)
                {
                    perror("Error opening treasures file:add_treasure");
                    exit(-1);
                }
                if (write(fd, "3", 2) < 0)
                {
                    perror("Error writing in file");
                }
                if (close(fd) < 0)
                {
                    perror("Error closing the file");
                }
                if (kill(monitor_pid, SIGUSR1) < 0)
                {
                    printf("Error sending SIGILL to child\n");
                    exit(2);
                }
                //waitpid(monitor_pid, NULL, 0);
                //wait(NULL);
                sleep(7);
            }
        }
        else if (strcmp(action, "calculate_score") == 0)
        {
            if (active_monitor == 0)
            {
                printf("Start monitor before executing list_hunt action\n");
            }
            else
            {
                if ((fd = open("./commands.txt", O_WRONLY | O_TRUNC, mode)) == -1)
                {
                    exit(-1);
                }
                if (write(fd, "4", 2) < 0)
                {
                    perror("Error writing in file");
                }
                if (close(fd) < 0)
                {
                    perror("Error closing the file");
                }
                if (kill(monitor_pid, SIGUSR1) < 0)
                {
                    printf("Error sending SIGUSR to child\n");
                    exit(2);
                }
                sleep(2);
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
            printf("\n List of actions: <utilisation> \n\n Start monitor: start_monitor \n List hunts: list_hunts \n List treasures: list_treasures \n View treasure: view_treasure \n Calculate score: calculate_score \n Stop monitor: stop_monitor \n Exit: exit\n");
        }
        else
        {
            printf("Invalid action");
        }
        printf("\n");
    }
}