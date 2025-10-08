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
int dominante_ppm(const char* filename, char couleur, int valeur);
int gris_ppm(const char* filename);
int negatif_ppm(const char* filename_in, const char* filename_out);
int decouper_ppm(const char* filename, int l1, int l2, int c1, int c2, const char* filename_out);
int filtre_median_ppm(const char* filename_in, const char* filename_out);

// Fonctions utilitaires
void trim_whitespace(char* str);
char* get_filename_without_ext(const char* filename);
int est_dans_limites(int x, int min, int max);
void trier_tableau(int* tab, int n);
int mediane(int* tab, int n);
void print_banner();
void process_command(const char* command);

#endif
