# Programme de traitement d'images PPM en C

## Description
Ce programme C implémente diverses opérations de traitement d'images au format PPM (Portable PixMap). Il a été développé dans le cadre du cours STRUCTURE DE DONNÉES II (INF 231_EC2) à l'Université de Yaoundé I.

## Fonctionnalités implémentées
1. **Affichage de la taille d'une image** (`size fichier.ppm`)
2. **Modification des pixels à dominante** (`dom c val fichier.ppm`)
3. **Conversion en niveaux de gris** (`gris fichier.ppm`)
4. **Création du négatif** (`neg fichier.ppm fichier_resultat.ppm`)
5. **Découpage d'image** (`cut fichier.ppm l1 l2 c1 c2 fichier_resultat.ppm`)
6. **Filtre médian** (`fil fichier.ppm fichier_resultat.ppm`)

## Structure du projet
- `main.c` : Programme principal avec interface en ligne de commande
- `tp3.h` : Fichier d'en-tête avec déclarations des structures et fonctions
- `tp3.c` : Implémentation de toutes les fonctions de traitement d'images
- `makefile` : Fichier de compilation
- `image.ppm` : Exemple d'image PPM pour tests
- `README.md` : Documentation du projet

## Compilation et exécution

### Compilation
```bash
make

### Exécution
```bash
./ppmviewer

## Exemples d'utilisation

## Équipe de développement
- Kengni Diekong Bernard Loïc (23V2146) : 
- Mbezele Zoa Danielle Naomi (24G2837) : 
- Abdel Ady Tchalla Ngando (23V2538) : 
- Azambou Marthe Nefertyti (23V2357) : 
- Petang Daniel (23V2121) : 
- Tekeng Kamwele Junior Cambell (23U2686) : 

## Format PPM supporté
Le programme supporte le format PPM version P3 (ASCII). Les images doivent avoir :
- En-tête avec version, largeur, hauteur et valeur maximale
- Données pixels au format RGB (3 valeurs par pixel)
- Les commentaires (lignes commençant par #) sont ignorés
