#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#define max_process 64
#define Running 0
#define Waiting 1
#define Zombie 2
#define Terminated 3

struct PCB {
    int pid;
    int ppid;
    int exit_status;
    int state;
};

struct PCB process_table[max_process];
int process_count = 0;
int next_pid = 1;
sem_t table_sem; 
sem_t monitor_sem; 
sem_t done_sem;
FILE *snapshot_file;
char msg[256]; 
char **worker_files;

void pm_ps();
void pm_fork();
void pm_exit();
void pm_wait();
void pm_kill();
void *monitor_thread(void *arg);
void *worker_thread(void *arg);

int main(int argc, char *argv[]) {
    worker_files = argv + 1;
    sem_init(&table_sem, 0, 1);
    sem_init(&monitor_sem, 0, 0);
    sem_init(&done_sem, 0, 0);
    snapshot_file = fopen("snapshots.txt", "w");

    pthread_t monitor_tid;
    pthread_create(&monitor_tid, NULL, monitor_thread, NULL);

    sem_wait(&table_sem);
    process_table[process_count].pid = next_pid++;
    process_table[process_count].ppid = 0;
    process_table[process_count].state = Running;
    process_count++;
    sprintf(msg, "Initial Process Table\n");
    sem_post(&monitor_sem);
    sem_wait(&done_sem);
    sem_post(&table_sem);

    int num_workers = argc - 1;
    pthread_t workers[num_workers];
    int thread_ids[num_workers]; 
    for (int i = 0; i < num_workers; i++) {
        thread_ids[i] = i; 
        pthread_create(&workers[i], NULL, worker_thread, &thread_ids[i]);
    }

    for (int i = 0; i < num_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    fclose(snapshot_file);
    return 0;
}

void pm_ps() {
    fprintf(snapshot_file, "%s", msg);
    fprintf(snapshot_file, "%-12s%-12s%-12s%-12s\n", "PID", "PPID", "STATE", "EXIT_STATUS");
    fprintf(snapshot_file, "-----------------------------------------------------\n");
    
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].state != Terminated) {
            char *st;
            if (process_table[i].state == Running) st = "RUNNING";
            else if (process_table[i].state == Waiting) st = "WAITING";
            else st = "ZOMBIE";
            
            if (process_table[i].state == Zombie) {
                char status[15];
                sprintf(status, "%d", process_table[i].exit_status);
                fprintf(snapshot_file, "%-12d%-12d%-12s%-12s\n", process_table[i].pid, process_table[i].ppid, st, status);
            } else {
                fprintf(snapshot_file, "%-12d%-12d%-12s%-12s\n", process_table[i].pid, process_table[i].ppid, st, "-");
            }
        }
    }
    fprintf(snapshot_file, "\n");
    fflush(snapshot_file);
}

void pm_fork(int threadID, int ppid) {
    sem_wait(&table_sem);
    if (process_count < max_process) {
        process_table[process_count].pid = next_pid++;
        process_table[process_count].ppid = ppid;
        process_table[process_count].state = Running;
        process_count++;
        sprintf(msg, "Thread %d calls pm_fork %d\n", threadID, ppid);
        sem_post(&monitor_sem);
        sem_wait(&done_sem);
    }
    sem_post(&table_sem);
}

void pm_exit(int threadID, int pid, int status) {
    sem_wait(&table_sem);
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid && process_table[i].state != Terminated) {
            process_table[i].state = Zombie;
            process_table[i].exit_status = status;
            sprintf(msg, "Thread %d calls pm_exit %d %d\n", threadID, pid, status);
            sem_post(&monitor_sem);
            sem_wait(&done_sem);
            break;
        }
    }
    sem_post(&table_sem);
}

void pm_wait(int threadID, int ppid, int child_pid) {
    sem_wait(&table_sem);
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].ppid == ppid) {
            if (child_pid == -1 || process_table[i].pid == child_pid) {
                if (process_table[i].state == Zombie) {
                    process_table[i].state = Terminated;
                    sprintf(msg, "Thread %d calls pm_wait %d %d\n", threadID, ppid, child_pid);
                    sem_post(&monitor_sem);
                    sem_wait(&done_sem);
                    break;
                }
            }
        }
    }
    sem_post(&table_sem);
}

void pm_kill(int threadID, int pid) {
    sem_wait(&table_sem);
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid && process_table[i].state != Terminated) {
            process_table[i].state = Zombie;
            process_table[i].exit_status = -1;
            sprintf(msg, "Thread %d calls pm_kill %d\n", threadID, pid);
            sem_post(&monitor_sem);
            sem_wait(&done_sem);
            break;
        }
    }
    sem_post(&table_sem);
}

void *monitor_thread(void *arg) {
    (void)arg;
    while (1) {
        sem_wait(&monitor_sem);
        pm_ps();
        sem_post(&done_sem);
    }
}

void *worker_thread(void *arg) {
    int threadID = *(int*)arg;
    char *filename = worker_files[threadID];
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    char cmd[20];
    int arg1, arg2;
    while (fscanf(file, "%s", cmd) != EOF) {
        if (strcmp(cmd, "fork") == 0) {
            fscanf(file, "%d", &arg1);
            pm_fork(threadID, arg1);
        } else if (strcmp(cmd, "exit") == 0) {
            fscanf(file, "%d %d", &arg1, &arg2);
            pm_exit(threadID, arg1, arg2);
        } else if (strcmp(cmd, "wait") == 0) {
            fscanf(file, "%d %d", &arg1, &arg2);
            pm_wait(threadID, arg1, arg2);
        } else if (strcmp(cmd, "kill") == 0) {
            fscanf(file, "%d", &arg1);
            pm_kill(threadID, arg1);
        } else if (strcmp(cmd, "sleep") == 0) {
            fscanf(file, "%d", &arg1);
            usleep(arg1 * 1000);
        }
    }
    fclose(file);
    return NULL;
}