#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATHNAME "."
#define PROJ_ID 'A'

struct sembuf op_wait_down[2] = {0, 0, 0, 0, 1, 0}; // Czekaj, aż semafor będzie podniesiony, potem opuść
struct sembuf op_up[1] = {0, -1, 0};                // Podnieś semafor

int main() {
    int semid;
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget error");
        exit(EXIT_FAILURE);
    }

    int i, j;
    for (i = 1; i < 20; i += 2) {
        if (semop(semid, op_wait_down, 2) == -1) {
            perror("semop down error");
            exit(EXIT_FAILURE);
        }

	/*poczatek sekcji krytycznej */
        for(j=0;j<i;j++){
                printf("%d ",i);
                fflush(stdout);
        }
        sleep(1);
        printf("\n");
	/* koniec sekcji krytycznej */

        if (semop(semid, op_up, 1) == -1) {
            perror("semop up error");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

