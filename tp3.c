#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "tp3.h"

#define MAX_LINE_LENGTH 256

void print_banner() {
    printf("Application de traitement d'images PPM réalisée par XXX.\n");
}

char* get_filename_without_ext(const char* filename) {
    static char result[100];
    strcpy(result, filename);
    char* dot = strrchr(result, '.');
    if (dot) *dot = '\0';
    return result;
}

int est_dans_limites(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void trier_tableau(int* tab, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (tab[j] > tab[j+1]) {
                int temp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = temp;
            }
        }
    }
}

int mediane(int* tab, int n) {
    if (n == 0) return 0;
    trier_tableau(tab, n);
    return tab[n/2];
}

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

int decouper_ppm(const char* filename, int l1, int l2, int c1, int c2, const char* filename_out) {
    ImagePPM image;
    if (!lire_ppm(filename, &image)) {
        return 0;
    }
    
    // Vérifier les limites (les coordonnées sont 1-based dans la commande)
    if (l1 < 1 || l2 > image.hauteur || c1 < 1 || c2 > image.largeur || l1 >= l2 || c1 >= c2) {
        printf("Erreur: Coordonnées de découpage invalides. Doit respecter: 1 <= l1 < l2 <= %d et 1 <= c1 < c2 <= %d\n", 
               image.hauteur, image.largeur);
        liberer_ppm(&image);
        return 0;
    }
    
    // Convertir en indices 0-based
    l1--; l2--; c1--; c2--;
    
    ImagePPM resultat;
    strcpy(resultat.version, image.version);
    resultat.largeur = c2 - c1 + 1;
    resultat.hauteur = l2 - l1 + 1;
    resultat.valeur_max = image.valeur_max;
    
    // Allouer la mémoire pour le résultat
    resultat.pixels = (Pixel**)malloc(resultat.hauteur * sizeof(Pixel*));
    for (int i = 0; i < resultat.hauteur; i++) {
        resultat.pixels[i] = (Pixel*)malloc(resultat.largeur * sizeof(Pixel));
        for (int j = 0; j < resultat.largeur; j++) {
            resultat.pixels[i][j] = image.pixels[l1 + i][c1 + j];
        }
    }
    
    int res = ecrire_ppm(filename_out, &resultat);
    
    // Libérer la mémoire
    for (int i = 0; i < resultat.hauteur; i++) {
        free(resultat.pixels[i]);
    }
    free(resultat.pixels);
    liberer_ppm(&image);
    
    return res;
}

int filtre_median_ppm(const char* filename_in, const char* filename_out) {
    ImagePPM image;
    if (!lire_ppm(filename_in, &image)) {
        return 0;
    }
    
    ImagePPM resultat;
    strcpy(resultat.version, image.version);
    resultat.largeur = image.largeur;
    resultat.hauteur = image.hauteur;
    resultat.valeur_max = image.valeur_max;
    
    // Allouer la mémoire pour le résultat
    resultat.pixels = (Pixel**)malloc(resultat.hauteur * sizeof(Pixel*));
    for (int i = 0; i < resultat.hauteur; i++) {
        resultat.pixels[i] = (Pixel*)malloc(resultat.largeur * sizeof(Pixel));
    }
    
    // Appliquer le filtre médian
    for (int i = 0; i < image.hauteur; i++) {
        for (int j = 0; j < image.largeur; j++) {
            int voisins_r[9], voisins_g[9], voisins_b[9];
            int count = 0;
            
            // Collecter les voisins (incluant le pixel central)
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    
                    if (ni >= 0 && ni < image.hauteur && nj >= 0 && nj < image.largeur) {
                        voisins_r[count] = image.pixels[ni][nj].r;
                        voisins_g[count] = image.pixels[ni][nj].g;
                        voisins_b[count] = image.pixels[ni][nj].b;
                        count++;
                    }
                }
            }
            
            // Calculer les médianes
            resultat.pixels[i][j].r = mediane(voisins_r, count);
            resultat.pixels[i][j].g = mediane(voisins_g, count);
            resultat.pixels[i][j].b = mediane(voisins_b, count);
        }
    }
    
    int res = ecrire_ppm(filename_out, &resultat);
    
    // Libérer la mémoire
    for (int i = 0; i < resultat.hauteur; i++) {
        free(resultat.pixels[i]);
    }
    free(resultat.pixels);
    liberer_ppm(&image);
    
    return res;
}

void process_command(const char* command) {
    char cmd[20], arg1[100], arg2[100], arg3[100], arg4[100], arg5[100], arg6[100];
    int l1, l2, c1, c2, val;
    char couleur;
    
    // Initialiser les arguments
    memset(arg1, 0, sizeof(arg1));
    memset(arg2, 0, sizeof(arg2));
    memset(arg3, 0, sizeof(arg3));
    memset(arg4, 0, sizeof(arg4));
    memset(arg5, 0, sizeof(arg5));
    memset(arg6, 0, sizeof(arg6));
    
    int args = sscanf(command, "%19s %99s %99s %99s %99s %99s %99s", 
                     cmd, arg1, arg2, arg3, arg4, arg5, arg6);
    
    if (args < 1) return;
    
    // Appliquer trim à tous les arguments
    trim_whitespace(cmd);
    trim_whitespace(arg1);
    trim_whitespace(arg2);
    trim_whitespace(arg3);
    trim_whitespace(arg4);
    trim_whitespace(arg5);
    trim_whitespace(arg6);
    
    if (strcmp(cmd, "size") == 0) {
        if (args >= 2) {
            if (size_ppm(arg1)) {
                // Le message est affiché dans size_ppm
            } else {
                printf("Erreur: Impossible de lire le fichier %s\n", arg1);
            }
        } else {
            printf("Erreur: Arguments manquants pour 'size'. Utilisation: size fichier.ppm\n");
        }
    }
    else if (strcmp(cmd, "dom") == 0) {
        if (args >= 4) {
            if (strlen(arg1) != 1 || (arg1[0] != 'R' && arg1[0] != 'G' && arg1[0] != 'B')) {
                printf("Erreur: La couleur doit être R, G ou B\n");
            } else {
                couleur = arg1[0];
                val = atoi(arg2);
                if (dominante_ppm(arg3, couleur, val)) {
                    printf("opération effectuée ; %s_dom.ppm créé\n", get_filename_without_ext(arg3));
                } else {
                    printf("Erreur lors du traitement de la dominante\n");
                }
            }
        } else {
            printf("Erreur: Arguments manquants pour 'dom'. Utilisation: dom c val fichier.ppm\n");
        }
    }
    else if (strcmp(cmd, "gris") == 0) {
        if (args >= 2) {
            if (gris_ppm(arg1)) {
                printf("opération effectuée ; %s_gris.ppm créé\n", get_filename_without_ext(arg1));
            } else {
                printf("Erreur lors de la conversion en gris\n");
            }
        } else {
            printf("Erreur: Arguments manquants pour 'gris'. Utilisation: gris fichier.ppm\n");
        }
    }
    else if (strcmp(cmd, "neg") == 0) {
        if (args >= 3) {
            if (negatif_ppm(arg1, arg2)) {
                printf("opération effectuée\n");
            } else {
                printf("Erreur lors de la création du négatif\n");
            }
        } else {
            printf("Erreur: Arguments manquants pour 'neg'. Utilisation: neg fichier.ppm fichier_resultat.ppm\n");
        }
    }
    else if (strcmp(cmd, "cut") == 0) {
        if (args >= 7) {
            l1 = atoi(arg2); l2 = atoi(arg3); c1 = atoi(arg4); c2 = atoi(arg5);
            if (l1 <= 0 || l2 <= 0 || c1 <= 0 || c2 <= 0) {
                printf("Erreur: Les coordonnées doivent être des nombres positifs\n");
            } else if (decouper_ppm(arg1, l1, l2, c1, c2, arg6)) {
                printf("opération effectuée\n");
            } else {
                printf("Erreur lors du découpage\n");
            }
        } else {
            printf("Erreur: Arguments manquants pour 'cut'. Utilisation: cut fichier.ppm l1 l2 c1 c2 fichier_resultat.ppm\n");
        }
    }
    else if (strcmp(cmd, "fil") == 0) {
        if (args >= 3) {
            if (filtre_median_ppm(arg1, arg2)) {
                printf("opération effectuée\n");
            } else {
                printf("Erreur lors de l'application du filtre médian\n");
            }
        } else {
            printf("Erreur: Arguments manquants pour 'fil'. Utilisation: fil fichier.ppm fichier_resultat.ppm\n");
        }
    }
    else if (strcmp(cmd, "quit") == 0) {
        printf("Au revoir!\n");
        exit(0);
    }
    else {
        printf("Commande non reconnue. Commandes disponibles: size, dom, gris, neg, cut, fil, quit\n");
    }
}
