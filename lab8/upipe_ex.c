#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Deklaracja tablicy na dwa deskryptory plikow do potoku
    int pipefd[2];
    // Utworzenie potoku
    pipe(pipefd);

    // Utworzenie procesu potomnego dla polecenia "cat etc/passwd"
    pid_t cat_pid = fork();

    if (cat_pid == 0) { // Proces potomny dla "cat etc/passwd"
        close(pipefd[0]); // Zamkniecie niepotrzebnego konca potoku

        dup2(pipefd[1], 1); // Podlaczenie konca zapisu potoku do stdout

        execlp("cat", "cat", "/etc/passwd", NULL); // Uruchomienie polecenia "cat etc/passwd"

    } else {  // Proces rodzicielski
        // Utworzenie procesu potomnego dla polecenia "sort"
        pid_t sort_pid = fork();
        
        if (sort_pid == 0) { // Proces potomny dla "sort"
            close(pipefd[1]); // Zamkniecie niepotrzebnego konca potoku 
            
            dup2(pipefd[0], 0); // Podlaczenie konca odczytu potoku do stdin
            
            execlp("sort", "sort", NULL); // Uruchomienie polecenia "sort"

        } else { // Proces rodzicielski
            // Zamkniecie obu koncow potoku
            close(pipefd[0]);
            close(pipefd[1]);
	    
            // Oczekiwanie na zakonczenie obu procesow potomnych
            waitpid(cat_pid, NULL, 0);
            waitpid(sort_pid, NULL, 0);
        }
    }
    return 0;
}
