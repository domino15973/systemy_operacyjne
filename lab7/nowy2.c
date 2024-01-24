#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("Nowy2 Signal: %d\n", signum);
    exit(signum);
}
 
int main(){
    signal(SIGINT,signal_handler);
    signal(SIGTERM,signal_handler);

    printf("Process nowy2: Begin \n");
    sleep(30);
    printf("Process nowy2: Finish \n");
    
    return 0;
}
