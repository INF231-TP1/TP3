#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp3.h"

#define MAX_LINE_LENGTH 256

void trim_whitespace(char* str) {
    char* end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0) return;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    *(end + 1) = '\0';
}

int lire_ppm(const char* filename, ImagePPM* image) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("fichier non trouvé\n");
        return 0;
    }
    
    char line[MAX_LINE_LENGTH];
    
    // Lire la version
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    sscanf(line, "%2s", image->version);
    
    // Vérifier que c'est bien P3
    if (strcmp(image->version, "P3") != 0) {
        printf("Erreur: Seul le format P3 est supporté\n");
        fclose(file);
        return 0;
    }
    
    // Lire les dimensions en sautant les commentaires
    while (fgets(line, sizeof(line), file)) {
        trim_whitespace(line);
        
        // Ignorer les lignes vides
        if (strlen(line) == 0) continue;
        
        // Ignorer les commentaires
        if (line[0] == '#') continue;
        
        if (sscanf(line, "%d %d", &image->largeur, &image->hauteur) == 2) break;
    }
    
    // Lire la valeur maximale
    while (fgets(line, sizeof(line), file)) {
        trim_whitespace(line);
        
        // Ignorer les lignes vides
        if (strlen(line) == 0) continue;
        
        // Ignorer les commentaires
        if (line[0] == '#') continue;
        
        if (sscanf(line, "%d", &image->valeur_max) == 1) break;
    }
    
    // Allouer la mémoire pour les pixels
    image->pixels = (Pixel**)malloc(image->hauteur * sizeof(Pixel*));
    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = (Pixel*)malloc(image->largeur * sizeof(Pixel));
    }
    
    // Lire les pixels
    for (int i = 0; i < image->hauteur; i++) {
        for (int j = 0; j < image->largeur; j++) {
            int values_read;
            do {
                values_read = fscanf(file, "%d %d %d", &image->pixels[i][j].r, 
                          &image->pixels[i][j].g, &image->pixels[i][j].b);
                if (values_read != 3) {
                    // Si on n'a pas lu 3 valeurs, on avance jusqu'à la prochaine ligne non vide/non commentaire
                    if (fgets(line, sizeof(line), file) == NULL) {
                        // Libérer la mémoire en cas d'erreur
                        for (int k = 0; k <= i; k++) {
                            free(image->pixels[k]);
                        }
                        free(image->pixels);
                        fclose(file);
                        return 0;
                    }
                    trim_whitespace(line);
                    // Ignorer les lignes vides et commentaires
                    if (strlen(line) == 0 || line[0] == '#') continue;
                    // Réessayer de lire depuis cette ligne
                    values_read = sscanf(line, "%d %d %d", &image->pixels[i][j].r, 
                              &image->pixels[i][j].g, &image->pixels[i][j].b);
                }
            } while (values_read != 3);
        }
    }
    
    fclose(file);
    return 1;
}

int ecrire_ppm(const char* filename, const ImagePPM* image) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur: Impossible de créer le fichier %s\n", filename);
        return 0;
    }
    
    fprintf(file, "%s\n", image->version);
    fprintf(file, "%d %d\n", image->largeur, image->hauteur);
    fprintf(file, "%d\n", image->valeur_max);
    
    for (int i = 0; i < image->hauteur; i++) {
        for (int j = 0; j < image->largeur; j++) {
            fprintf(file, "%d %d %d", image->pixels[i][j].r, 
                    image->pixels[i][j].g, image->pixels[i][j].b);
            if (j < image->largeur - 1) fprintf(file, " ");
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 1;
}

void liberer_ppm(ImagePPM* image) {
    if (image->pixels) {
        for (int i = 0; i < image->hauteur; i++) {
            free(image->pixels[i]);
        }
        free(image->pixels);
    }
}

int size_ppm(const char* filename) {
    ImagePPM image;
    if (!lire_ppm(filename, &image)) {
        return 0;
    }
    
    printf("%d x %d\n", image.largeur, image.hauteur);
    liberer_ppm(&image);
    return 1;
}

char* get_filename_without_ext(const char* filename) {
    static char result[100];
    strcpy(result, filename);
    char* dot = strrchr(result, '.');
    if (dot) *dot = '\0';
    return result;
}

int gris_ppm(const char* filename) {
    ImagePPM image;
    if (!lire_ppm(filename, &image)) {
        return 0;
    }
    
    char output_filename[150];
    snprintf(output_filename, sizeof(output_filename), "%s_gris.ppm", get_filename_without_ext(filename));
    
    for (int i = 0; i < image.hauteur; i++) {
        for (int j = 0; j < image.largeur; j++) {
            Pixel* p = &image.pixels[i][j];
            int moyenne = (p->r + p->g + p->b) / 3;
            p->r = p->g = p->b = moyenne;
        }
    }
    
    int result = ecrire_ppm(output_filename, &image);
    liberer_ppm(&image);
    return result;
}

int negatif_ppm(const char* filename_in, const char* filename_out) {
    ImagePPM image;
    if (!lire_ppm(filename_in, &image)) {
        return 0;
    }
    
    for (int i = 0; i < image.hauteur; i++) {
        for (int j = 0; j < image.largeur; j++) {
            Pixel* p = &image.pixels[i][j];
            p->r = image.valeur_max - p->r;
            p->g = image.valeur_max - p->g;
            p->b = image.valeur_max - p->b;
        }
    }
    
    int result = ecrire_ppm(filename_out, &image);
    liberer_ppm(&image);
    return result;
}

int est_dans_limites(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

int dominante_ppm(const char* filename, char couleur, int valeur) {
    ImagePPM image;
    if (!lire_ppm(filename, &image)) {
        return 0;
    }
    
    // Créer le nom du fichier de sortie
    char output_filename[150];
    snprintf(output_filename, sizeof(output_filename), "%s_dom.ppm", get_filename_without_ext(filename));
    
    // Appliquer l'effet de dominante
    for (int i = 0; i < image.hauteur; i++) {
        for (int j = 0; j < image.largeur; j++) {
            Pixel* p = &image.pixels[i][j];
            int max_val = p->r;
            char dom = 'R';
            
            if (p->g > max_val) {
                max_val = p->g;
                dom = 'G';
            }
            if (p->b > max_val) {
                max_val = p->b;
                dom = 'B';
            }
            
            if (dom == couleur) {
                p->r = est_dans_limites(p->r + valeur, 0, image.valeur_max);
                p->g = est_dans_limites(p->g + valeur, 0, image.valeur_max);
                p->b = est_dans_limites(p->b + valeur, 0, image.valeur_max);
            }
        }
    }
    
    int result = ecrire_ppm(output_filename, &image);
    liberer_ppm(&image);
    return result;
}
