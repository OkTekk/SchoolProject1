/****************************************************************************************
* Dossier 1 : Generation d'images .bmp 24 bits a partir d'une table de segments
*
* Definitions globales du projet
*
* Programmation procedurale 2015 - Laurent Fournier - v4
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __WIN32__
    #include <windows.h>
    #define PHOTO_DIR ".\\"
#else
    #include <unistd.h>
    #define PHOTO_DIR "./"
#endif

/****************************************************************************************
* Constantes symboliques
****************************************************************************************/
#define FFh 0

#define HEADER_SIZE 54
#define BIT_PIXEL 24

#define FIC_NM_SIZE 100
#define FIC_BA_SIZE 40
#define COM_SIZE 80

#define sgn(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))

enum couleur {
	NOIR,
	BLANC,
	BLEU,
	VERT,
	ROUGE,
	JAUNE,		// Vert + Rouge
	CYAN,		// Bleu + Vert
	MAGENTA,	// Bleu + Rouge
	JAUNE_1,
	JAUNE_2,
	JAUNE_3
};

enum intensite {
	NIVEAU_1,
	NIVEAU_2,
	NIVEAU_3,
	NIVEAU_4,
	NIVEAU_5,
	NIVEAU_6,
	NIVEAU_7,
	NIVEAU_8
};

/****************************************************************************************
* Definition de la structure de donnees image
****************************************************************************************/
typedef struct Image {
    unsigned char *header;      // contenu du header
    unsigned char ***pic;       // matrice image
	unsigned long int taille;   // taille du fichier
	unsigned long int offset;   // offset dans le fichier BMP du 1er pixel
	unsigned char bitpix;       // nb bits par pixels
	int largeur;                // largeur de l'image
	int hauteur;                // hauteur de l'image
	int nb_pix;                 // nombre de pixels sur l'image
    char nom_base[40];          // nom de base de l'image sans extension
} image;

/****************************************************************************************
* Definition d'un segment dans l'espace image :
*   Coordonnees entieres de (0, 0) à (Hauteur, Largeur)
*   Exemple : de (0, 0) à (799, 1199) pour une image de 800x1200 pixels
****************************************************************************************/
typedef struct Segment_Image {
    int obj;
    int x1;
    int y1;
    int x2;
    int y2;
    int col;
    int niv;
} seg_i;

/****************************************************************************************
* Definition d'un segment dans l'espace cartesien
*   Coordonnees réelles de (xmin, ymin) à (xmax, ymax)
*   Exemple : de (-2.5, -2.5) à ( 2.5, 5.0)
****************************************************************************************/
typedef struct Segment_Form {
    int obj;
    float x1;
    float y1;
    float x2;
    float y2;
    int col;
    int niv;
} seg_f;

/****************************************************************************************
* Prototypes Systeme : Gestion des image sur disque et allocation dynamique memoire
****************************************************************************************/
image * Lire_Image(char * nom_bas, char * nom_tag);
image * Copier_Image(image * img);
void Copier_Pixel(image * img, image * fond);
image * Creer_Image(char * nom_bas, int hauteur, int largeur, int col, int niv);

void Ecrire_Image(image * img, char * nom_tag);
unsigned char *** Malloc_Pic(int hauteur, int largeur);
void Free_Pic(unsigned char *** t, int hauteur, int largeur);
void Free_Image(image * img);
void Afficher_Header(image * img);
int * Get_Color(int col, int niv);

/****************************************************************************************
* Prototypes Espace Image
****************************************************************************************/
void Initialiser_Image(image * img, int * col);
void Seg_Poly(int i, int obj, int x1, int y1, int x2, int y2, int col, int niv);
void Ajouter_Calque(image * img, image * cal, int x, int y);
void Tracer_Polygone(image * img, int nmax);
void Tracer_Segment(image * img, int pI, int * col);
void Tracer_Cercle(int obj, int ii, int ns, int x, int y, int a, int b, int co, int ni);
int f(int extremum, int x, int center, int scale);
int fNeg(int extremum, int x, int center, int scale);
void Remplir_Polygone(image * img, int i1, int i2, int j1, int j2, int col, int niv);
void Afficher_Seg_Image(int imax);

/****************************************************************************************
* Prototypes Formes dans Espace Cartesien
****************************************************************************************/
void Copier_Forme(int iori, int imax, float fac); // Passage dans espace image
void Seg_Forme(int i, int obj, float x1, float y1, float x2, float y2, int col, int niv);
void Rotation_Forme(int imax, int obj, float rot);
void Translation_Forme(int imax, int obj, float x, float y, float fac);
void Afficher_Seg_Forme(int imax);
