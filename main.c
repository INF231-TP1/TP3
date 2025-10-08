#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp3.h"

#define MAX_COMMAND_LENGTH 256

int main() {
    char command[MAX_COMMAND_LENGTH];
    
    printf("Application de traitement d'images PPM réalisée par XXX.\n");
    
    while (1) {
        printf("ppmviewer> ");
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        // Supprimer le saut de ligne
        command[strcspn(command, "\n")] = '\0';
        
        // Ignorer les lignes vides
        if (strlen(command) == 0) continue;
        
        // Traitement basique des commandes
        if (strcmp(command, "quit") == 0) {
            printf("Au revoir!\n");
            break;
        }
    }
    
    return 0;
}
