#ifndef _TP3_
#define _TP3_

// Structure pour un pixel RGB
typedef struct {
    int r, g, b;
} Pixel;

// Structure pour une image PPM
typedef struct {
    char version[3];
    int largeur;
    int hauteur;
    int valeur_max;
    Pixel** pixels;
} ImagePPM;

// Fonctions de traitement d'images PPM
int lire_ppm(const char* filename, ImagePPM* image);
int ecrire_ppm(const char* filename, const ImagePPM* image);
void liberer_ppm(ImagePPM* image);

int size_ppm(const char* filename);

// Fonctions utilitaires
void trim_whitespace(char* str);
int gris_ppm(const char* filename);
char* get_filename_without_ext(const char* filename);
int negatif_ppm(const char* filename_in, const char* filename_out);

#endif
