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
        } else if (strncmp(command, "gris ", 5) == 0) {
            char filename[100];
            if (sscanf(command + 5, "%99s", filename) == 1) {
                if (gris_ppm(filename)) {
                    printf("opération effectuée ; %s_gris.ppm créé\n", get_filename_without_ext(filename));
                } else {
                    printf("Erreur lors de la conversion en gris\n");
                }
            } else {
                printf("Erreur: Arguments manquants pour 'gris'. Utilisation: gris fichier.ppm\n");
            }
        } else if (strncmp(command, "neg ", 4) == 0) {
            char filename_in[100], filename_out[100];
            if (sscanf(command + 4, "%99s %99s", filename_in, filename_out) == 2) {
                if (negatif_ppm(filename_in, filename_out)) {
                    printf("opération effectuée\n");
                } else {
                    printf("Erreur lors de la création du négatif\n");
                }
            } else {
                printf("Erreur: Arguments manquants pour 'neg'. Utilisation: neg fichier.ppm fichier_resultat.ppm\n");
            }
        }
    }
    
    return 0;
}
