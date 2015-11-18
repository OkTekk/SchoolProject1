/****************************************************************************************
* Dossier 1 : Generation d'images .bmp 24 bits a partir d'une table de segments
*
* Implementation des fonctions systemes : lecture/ecriture sur disque, copie,
*                                         allocation dynamique, affichage
*
* Programmation procedurale 2015 - Laurent Fournier - v4
****************************************************************************************/
#include "global.h"

/****************************************************************************************
* Lecture du fichier BMP 24bits original
****************************************************************************************/
image * Lire_Image(char * nom_bas, char * nom_tag) {
    FILE *fp = NULL;
    image *img = NULL;

	int i=0, j=0, k=0;

    char nom_fic[FIC_NM_SIZE] = {0};
    char nom_rep[FIC_BA_SIZE] = {0};

    unsigned char buf_head[HEADER_SIZE] = {0};
    unsigned char *buf_data = NULL;

    img = (image *)malloc(sizeof(image));
	if (img==NULL) {
		printf("Lire_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    strncpy(nom_rep, PHOTO_DIR, FIC_BA_SIZE);
    strncat(nom_rep, nom_bas, FIC_BA_SIZE);
    #ifdef __WIN32__
        strncat(nom_rep, "\\", FIC_BA_SIZE);
    #else
        strncat(nom_rep, "/", FIC_BA_SIZE);
    #endif
	strncpy(nom_fic, nom_rep, FIC_NM_SIZE);
	strncat(nom_fic, nom_bas, FIC_NM_SIZE);
	strncat(nom_fic, nom_tag, FIC_NM_SIZE);
	strncat(nom_fic, ".bmp", FIC_NM_SIZE);

	/* Ouverture fichier binaire en lecture */
	if ((fp = fopen(nom_fic, "rb")) == NULL) {
		printf("\nError opening source file %s\n", nom_fic);
		img=NULL;
		return img;
	}

	/* lecture de l'entete(54 octets) */
	fread(buf_head, 1, HEADER_SIZE,fp);

	img->taille = *(unsigned long int *)&buf_head[2];
	img->offset = (unsigned char)buf_head[10];
	img->bitpix = (unsigned char)buf_head[28];
	img->largeur = *(int *)&buf_head[18];
	img->hauteur = *(int *)&buf_head[22];
	img->nb_pix = img->hauteur * img->largeur;
    strncpy(img->nom_base, nom_bas, FIC_BA_SIZE);

    img->header = (unsigned char *)malloc(img->offset);
	if (img->header == NULL) {
		printf("Lire_Image 2 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    fseek(fp, 0, SEEK_SET);

	fread(img->header, 1, img->offset, fp);

    img->pic = Malloc_Pic(img->hauteur, img->largeur);

    buf_data = (unsigned char *)malloc(3*img->nb_pix);
	if (buf_data == NULL) {
		printf("Lire_Image 3 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	if (fread(buf_data, 1, 3*img->nb_pix, fp) != 3*img->nb_pix) {
		printf("Lire_Image 4 : Buffer depasse\n" );
		exit(EXIT_FAILURE);
	}

	for(i=0; i<img->hauteur; i++) {
		for (j=0; j<img->largeur; j++) {
			for (k=0; k<3; k++) {
				img->pic[i][j][k] = buf_data[i*3*img->largeur + j*3 + k];
			}
		}
	}

	free(buf_data);

    fclose(fp);

    return img;
}

/****************************************************************************************
* Copier une image
****************************************************************************************/
image * Copier_Image(image * img) {
    image *img_cpy = NULL;

	int i=0, j=0, k=0;

    // Copie de la structure

    img_cpy = (image *)malloc(sizeof(image));
	if (img_cpy==NULL) {
		printf ("Copier_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	img_cpy->taille = img->taille;
	img_cpy->offset = img->offset;
	img_cpy->bitpix = img->bitpix;
	img_cpy->largeur = img->largeur;
	img_cpy->hauteur = img->hauteur;
	img_cpy->nb_pix = img->nb_pix;
    strncpy(img_cpy->nom_base, img->nom_base, FIC_BA_SIZE);

    img_cpy->header = (unsigned char *)malloc(img->offset);
	if (img_cpy->header == NULL) {
		printf("Copier_Image 2 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	memcpy(img_cpy->header, img->header, img->offset);

    // Copie de l'image

    img_cpy->pic = Malloc_Pic(img_cpy->hauteur, img_cpy->largeur);

	for(i=0; i<img->hauteur; i++) {
		for (j=0; j<img->largeur; j++) {
			for (k=0; k<3; k++) {
				img_cpy->pic[i][j][k] = img->pic[i][j][k];
			}
		}
	}

    return img_cpy;
}


/****************************************************************************************
* Copier pixel
****************************************************************************************/
void Copier_Pixel(image * img, image * fond) {
    int i=0, j=0, k=0;

    // Copie de l'image

    for(i=0; i<img->hauteur; i++) {
		for (j=0; j<img->largeur; j++) {
			for (k=0; k<3; k++) {
				img->pic[i][j][k] = fond->pic[i][j][k];
			}
		}
	}
}


/****************************************************************************************
* Creer une image
* Attention la largeur doit etre un multiple de 4
****************************************************************************************/
image * Creer_Image(char * nom_bas, int hauteur, int largeur, int col, int niv) {
    image *img = NULL;

    // Allocation de l'image

    img = (image *)malloc(sizeof(image));
	if (img == NULL) {
		printf ("Creer_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    // Remplissage des parametres

    strncpy(img->nom_base, nom_bas, FIC_BA_SIZE);

	img->largeur = largeur;
	img->hauteur = hauteur;
	img->nb_pix = largeur * hauteur;

	img->bitpix = BIT_PIXEL;
	img->offset = HEADER_SIZE;
	img->taille = img->offset + (3* (img->largeur + img->largeur%4) * img->hauteur);

    // Creation du header

    img->header = (unsigned char *)calloc(1, HEADER_SIZE);
	if (img->header == NULL) {
		printf ("Creer_Image 2 : Impossible d'initialiser avec malloc\n");
		exit(EXIT_FAILURE);
	}

    img->header[0] = 'B';
    img->header[1] = 'M';

	memcpy(&img->header[2], &img->taille, sizeof(unsigned long int));

	img->header[6] = 'E';     // Applic ID
	img->header[7] = 'R';
	img->header[8] = 'I';
	img->header[9] = 'C';

	memcpy(&img->header[10], &img->offset, sizeof(unsigned long int));

	img->header[14] = HEADER_SIZE - 14;

	memcpy(&img->header[18], &img->largeur, sizeof(int));

	memcpy(&img->header[22], &img->hauteur, sizeof(int));

	img->header[26] = 1;                    // Planes = 1 : ImageMagick

	img->header[28] = img->bitpix;          // 24 bits par pixel

    // Creation de l'image

    img->pic = Malloc_Pic(img->hauteur, img->largeur);

    Initialiser_Image(img, Get_Color(col, niv) );

    return img;
}

/****************************************************************************************
* Ecrire l'image sur disque
****************************************************************************************/
void Ecrire_Image(image * img, char * nom_tag) {
    FILE *fp = NULL;

    int i=0, j=0, k=0;

    char nom_fic[FIC_NM_SIZE] = {0};
    char nom_rep[FIC_BA_SIZE] = {0};

    unsigned char *buf_data = NULL;

    buf_data = (unsigned char *)malloc(3*img->nb_pix);
	if (buf_data == NULL) {
		printf("Ecrire_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    strncpy(nom_rep, PHOTO_DIR, FIC_BA_SIZE);
    strncat(nom_rep, img->nom_base, FIC_BA_SIZE);
    #ifdef __WIN32__
        strncat(nom_rep, "\\", FIC_BA_SIZE);
    #else
        strncat(nom_rep, "/", FIC_BA_SIZE);
    #endif
	strncpy(nom_fic, nom_rep, FIC_NM_SIZE);
	strncat(nom_fic, img->nom_base, FIC_NM_SIZE);
	strncat(nom_fic, nom_tag, FIC_NM_SIZE);
	strncat(nom_fic, ".bmp", FIC_NM_SIZE);

    printf("Fic : %s\n\n", nom_fic);

	/* Ouverture fichier binaire en ecriture */
	if ((fp = fopen(nom_fic, "wb")) ==NULL) {
		printf("\nError opening source file\n");
		exit(EXIT_FAILURE);
	}

	fwrite(img->header, 1, img->offset, fp);

	for(i=0; i<img->hauteur; i++) {
		for (j=0; j<img->largeur; j++) {
			for (k=0; k<3; k++) {
				buf_data[i*3*img->largeur + j*3 + k] = img->pic[i][j][k];
			}
		}
	}

	if (fwrite(buf_data, 1, 3*img->nb_pix, fp) != 3*img->nb_pix) {
		printf("Ecrire_Image 2 : Buffer depasse\n" );
		exit(EXIT_FAILURE);
	}

	free(buf_data);

    fclose(fp);

    return;
}

/* Allocation dynamique d'une matrice image
---------------------------------------------------------*/
unsigned char *** Malloc_Pic(int hauteur, int largeur) {
	int i;
	int j;
	unsigned char ***t;

	/* première dimension */
	t = (unsigned char***)malloc(sizeof(*t) * hauteur);
	if (t==NULL) {
		printf ("Malloc_pic 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	for (i=0; i<hauteur; i++) {
		t[i] = (unsigned char**)malloc(sizeof(**t) * largeur);
		/* deuxième dimension */
		if (t[i]==NULL) {
			printf ("Malloc_pic 2 : Impossible d'initialiser avec malloc\n" );
			exit(EXIT_FAILURE);
		}

		for (j=0; j<largeur; j++) {
			/* troisième dimension */
			t[i][j] = (unsigned char*)malloc(sizeof(***t) * 3);
			if (t[i][j]==NULL) {
				printf ("Malloc_pic 3 : Impossible d'initialiser avec malloc\n" );
				exit(EXIT_FAILURE);
			}
		}
	}

	return t;
}

/* Desallocation d'une image
---------------------------------------------------------*/
void Free_Image(image * img) {
    Free_Pic(img->pic,img->hauteur,img->largeur);
    free(img->header);
    free(img);

	return;
}

/* Desallocation d'une matrice image
---------------------------------------------------------*/
void Free_Pic(unsigned char *** t, int hauteur, int largeur) {
	int i;
	int j;

	for (i=0; i<hauteur; i++) {
		for (j=0; j<largeur; j++) {
			/* troisième dimension */
			free(t[i][j]);
		}

		free(t[i]);
		/* deuxième dimension */
	}

	free(t);
	/* première dimension */

	return;
}

/****************************************************************************************
* Afficher les donnees du fichier BMP 24bits original
****************************************************************************************/
void Afficher_Header(image *img) {
    printf("\n%-12s: %s\n\n", "Image", img->nom_base);
	printf("%-12s: %ld bytes   soit : %ld Kbytes \n","Taille", img->taille, img->taille/1024);
	printf("%-12s: %lu  \n","Offset", img->offset);
	printf("%-12s: %u  \n","Bitpix", img->bitpix);
	printf("%-12s: %d  \n","Largeur", img->largeur);
	printf("%-12s: %d  \n","Hauteur", img->hauteur);
	printf("%-12s: %i  \n","Pixels", img->nb_pix);

	printf("\n\n");

    return;
}

/****************************************************************************************
* Donne une couleur de base sur 8 niveaux possibles
****************************************************************************************/
int * Get_Color(int col, int niv) {
    int i;
    static int c[3];

    int col_base[8][3] = {
        {0,0,0}, {255,255,255},                  // Noir, Blanc
        {255,0,0}, {0,255,0},{0,0,255},          // Bleu, Vert, Rouge
        {0,255,255}, {255,255,0}, {255,0,255}    // Jaune, Cyan, Magenta
    };

    int niveau[8] = {224,192,160,128,96,64,32,0};

    for (i=0; i<3; i++)
        c[i] = col_base[col][i] == 0 ? niveau[niv] : col_base[col][i];

    return c;
}

