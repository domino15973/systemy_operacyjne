#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    // Utworzenie pierwszego procesu potomnego 
    pid_t pid1 = fork();

    if(pid1 == 0){
        // Kod procesu potomnego 1 - uruchomienie programu nowy1
        execl("nowy1","nowy1");
    }

    // Utworzenie drugiego procesu potomnego
    pid_t pid2 = fork(); 

    if(pid2 == 0){
        // Kod procesu potomnego 2 - uruchomienie programu nowy2
        execl("nowy2","nowy2");
    }

    // Oczekiwanie na zakonczenie obu procesow potomnych
    int status1,status2;

    // Wyswietlenie informacji o zakonczeniu procesow
    waitpid(pid1,&status1,0);
    printf("Process PID: %d Finished, Code Finished: %d, Numer sygnału: %d \n",pid1,WEXITSTATUS(status1),WTERMSIG(status1));
    
    waitpid(pid2,&status2,0);
    printf("Process PID: %d Finished, Code Finished: %d, Numer sygnału: %d \n",pid2,WEXITSTATUS(status2),WTERMSIG(status2));

    return 0;
}
