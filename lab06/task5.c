#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    // Inicjalizacja indeksow i rozmiaru tablicy zmiennych srodowiskowych
    int env_index = 0;
    int argv_index = 1;
    int env_size = 0;

    // Obliczanie rozmiaru tablicy zmiennych srodowiskowych
    while (envp[env_size] != NULL) {
        env_size++;
    }

    // Sprawdzenie czy program zostal wywolany bez argumentow
    if(argc < 2){
        // Wyswietlenie wszystkich zmiennych srodowiskowych
        while(envp[env_index] != NULL) {
            printf("%s\n", envp[env_index]);
            env_index++;
        }
    } else {
        // Przetwarzanie podanych argumentow
    	while(envp[env_index] != NULL){
            // Sprawdzenie czy przetworzono wszystkie argumenty lub zmienne srodowiskowe
            if (argv_index == argc || env_index == env_size) {
                break;
            }
            
            // Pobranie nazwy zmiennej srodowiskowej
            char *variable_name = argv[argv_index];
            // Pobranie wartosci zmiennej srodowiskowej
            char *variable_value = getenv(variable_name);

            // Sprawdzenie czy zmienna istnieje
            if(variable_value != NULL) {
                // Sformatowanie i wyswietlenie nazwy i wartosci zmiennej
                char result[512];
                sprintf(result, "%s=%s", variable_name, variable_value);
                printf("%s\n", result);
                
                // Reset indeksu zmiennych srodowiskowych i przejscie do kolejnego argumentu
                env_index = 0;
                argv_index++;
                continue;
            } else if(env_index == env_size - 1) {
                // Komunikat o braku zmiennej srodowiskowej o podanej nazwie
                printf("Nie ma zmiennej środowiskowej o nazwie:  %s", argv[argv_index]);
                printf("\n");
	        
		// Sprawdzenie czy to ostatni argument i ewentulne zakonczenie petli
                if (argv_index == argc - 1){
                    break;
                } else {
	            // Przejscie do kolejnego argumentu i reset indeksu zmiennych srodowiskowych
                    argv_index++;
                    env_index = 0;
                }
            } else {
		// Przejscie do kolejnej zmiennej srodowiskowej 
                env_index++;
            }
        }
    }
    return 0;
}
