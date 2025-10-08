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
- Sous linux:
```bash
./ppmviewer

- Sous windows:
```bash
c:\ > ppmviewer.exe

## Exemples d'utilisation
Dans l'interface interactive :
```text
Application de traitement d'images PPM réalisée par XXX.
ppmviewer> size image.ppm
81 x 81
ppmviewer> size image2.ppm
2048 x 2048
ppmviewer> q
Commande non reconnue. Commandes disponibles: size, dom, gris, neg, cut, fil, quit
ppmviewer> dom R 20 image2.ppm
opération effectuée ; image2_dom.ppm créé
ppmviewer> gris image3.ppm
opération effectuée ; image3_gris.ppm créé
ppmviewer> neg image2.ppm
Erreur: Arguments manquants pour 'neg'. Utilisation: neg fichier.ppm fichier_resultat.ppm
ppmviewer> neg image2.pp image2_neg.ppm
fichier non trouvé
Erreur lors de la création du négatif
ppmviewer> neg image2.ppm image2_neg.ppm
opération effectuée
ppmviewer> fil image2.ppm image3_fil.ppm
opération effectuée
ppmviewer> quit
Au revoir!


## Équipe de développement
- Kengni Diekong Bernard Loïc (23V2146) : Fonction pour le tri et filtre médian, coordination technique, architecture du projet, tests approfondis de toutes les fonctionnalités et correction des bugs
- Mbezele Zoa Danielle Naomi (24G2837) : Affichage des dimensions d'image, gestion de la mémoire dynamique, parsing des en-têtes PPM avec gestion des commentaires
- Abdel Ady Tchalla Ngando (23V2538) : Conversion en niveaux de gris, validation des entrées utilisateur, gestion de la mémoire
- Azambou Marthe Nefertyti (23V2357) : Création du négatif, documentation, interface en ligne de commande interactive
- Petang Daniel (23V2121) : Fonction pour foncer ou éclaircir les pixels, messages d'utilisateur clairs et informatifs, gestion des noms de fichiers automatiques
- Tekeng Kamwele Junior Cambell (23U2686) : Découpage d'images avec validation des coordonnées, vérification de la robustesse face aux entrées erronées, fonctions de base de lecture/écriture des fichiers PPM

## Format PPM supporté
Le programme supporte le format PPM version P3 (ASCII). Les images doivent avoir :
- En-tête avec version, largeur, hauteur et valeur maximale
- Données pixels au format RGB (3 valeurs par pixel)
- Les commentaires (lignes commençant par #) sont ignorés
