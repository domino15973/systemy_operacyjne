#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>


// Struktura danych w pamięci współdzielonej
struct shared_data {
    int size;
    char text[1000];
};

// Union potrzebny do operacji na semaforach
union semun {
    int val;
};

// Funkcja do ustawienia wartości semafora
void setSemaphoreValue(int semaphoreID, int value) {
    union semun argument;
    argument.val = value;
    if (semctl(semaphoreID, 0, SETVAL, argument) == -1){
        perror("Nie udało się ustawić wartości semafora");
        exit(EXIT_FAILURE);
    }
}

// Funkcja do odczytu wartości semafora
int getSemaphoreValue(int semaphoreID) {
    return semctl(semaphoreID, 0, GETVAL);
}

int main() {
    printf("PRODUCENT\n\n");

    // Tworzenie lub uzyskiwanie ID segmentu pamięci współdzielonej
    key_t key = ftok(".", 'A');
    int sharedMemID = shmget(key, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (sharedMemID == -1) {
        perror("Błąd podczas tworzenia segmentu pamięci współdzielonej");
        exit(EXIT_FAILURE);
    }

    // Dołączanie segmentu pamięci współdzielonej do przestrzeni adresowej procesu
    struct shared_data *sharedMemory = (struct shared_data *)shmat(sharedMemID, NULL, 0);
    if (sharedMemory == (struct shared_data *)(-1)) {
        perror("Błąd podczas dołączania segmentu pamięci współdzielonej");
        exit(EXIT_FAILURE);
    }

    // Tworzenie semafora
    int semaphore = semget(key, 1, IPC_CREAT | 0666);
    if (semaphore == -1) {
        perror("Nie można utworzyć semafora");
        exit(EXIT_FAILURE);
    }

    // Inicjalizacja semafora
    setSemaphoreValue(semaphore, 0);

    while (1) {
        // Producent oczekuje na zezwolenie do zapisu
        printf("Oczekiwanie na zwolnienie semafora\n");
        while (getSemaphoreValue(semaphore));
        setSemaphoreValue(semaphore, 1);

        // Wprowadzanie danych z wejścia standardowego
        printf("Producent: Wprowadź dane (max 1000 znaków): ");
        char inputBuffer[1000];
        fgets(inputBuffer, 1000, stdin);

        printf("Zapisywanie danych do pamięci współdzielonej\n");
        // Kopiowanie danych i zapis do pamięci współdzielonej
        sharedMemory->size = strlen(inputBuffer); // zapisanie długości łańcucha znaków
        strncpy(sharedMemory->text, inputBuffer, 1000); // kopiowanie danych do pamięci współdzielonej

        printf("Zwalnianie semafora\n");
        setSemaphoreValue(semaphore, 0);
    }

    return 0;
}
