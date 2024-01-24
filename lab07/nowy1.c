#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Obsluga sygnalow INT i TERM
void signal_handler(int signum) {
    // Funkcja wywolywana w momencie otrzymania sygnalu
    // Wyswietla informacje o numerze otrzymanego sygnalu
    printf("Nowy1 Signal: %d\n", signum);

    // Zakonczenie programu z kodem zakonczenia rownym numerowi otrzymanego sygnalu
    exit(signum);
}
 
int main(){
    // Ustawienie obslugi sygnalow dla programu
    // Sygnal SIGINT spowoduje wywolane funkcji signal_handler z argumentem SIGINT
    // Sygnal SIGTERM spowoduje wywolanie funkcji signal_handler z argumentem SIGTERM
    signal(SIGINT,signal_handler);
    signal(SIGTERM,signal_handler);

    printf("Process nowy1: Begin \n");
    sleep(20);
    printf("Process nowy1: Finish \n");

    return 0;
}
