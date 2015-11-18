/****************************************************************************************
* Dossier 1 : Generation d'images .bmp 24 bits a partir d'une table de segments
*
* Programme principal de test
*
* Programmation procedurale 2015 - Laurent Fournier - v4
****************************************************************************************/
#include "global.h"

/****************************************************************************************
* Prototypes locaux
****************************************************************************************/
void Tst_01(void);
void Tst_02(void);
void Tst_03(void);
void Tst_04(void);
void Tst_05(void);

seg_i poly[10000];
seg_f form[1000];

/****************************************************************************************
* Test des differentes fonctions
****************************************************************************************/
int main(void) {

//    Tst_01();
//    Tst_02();
//    Tst_03();
//    Tst_04();
    Tst_05();

    return 0;
}

/****************************************************************************************
* Test d'ecriture dans l'espace image
****************************************************************************************/
void Tst_01(void) {
    int x, y, i, k = 0;
    int px1, py1, px2, py2;
    image *img=NULL;

    img=Creer_Image("Test_01", 1200, 1800, BLANC, NIVEAU_1);
    Afficher_Header(img);

    Seg_Poly(1, 1,  100, 100, 100, 200, ROUGE, NIVEAU_8);
    Seg_Poly(2, 1,  100, 200, 200, 200, ROUGE, NIVEAU_8);
    Seg_Poly(3, 1,  200, 200, 200, 100, ROUGE, NIVEAU_8);
    Seg_Poly(4, 1,  200, 100, 100, 100, ROUGE, NIVEAU_8);
    Seg_Poly(5, 1,  100, 100, 200, 200, VERT, NIVEAU_8);
    Seg_Poly(6, 1,  100, 200, 200, 100, BLEU, NIVEAU_8);

    Seg_Poly(7, 2, 700, 700, 800, 900, VERT, NIVEAU_8);
    Seg_Poly(8, 2, 800, 700, 700, 900, VERT, NIVEAU_8);
    Seg_Poly(9, 2, 100, 100, 200, 300, VERT, NIVEAU_8);
    Seg_Poly(10, 3, 200, 100, 100, 300, VERT, NIVEAU_8);
    Seg_Poly(11, 4, 400, 300, 300, 500, BLEU, NIVEAU_8);
    Seg_Poly(12, 4, 300, 300, 400, 500, BLEU, NIVEAU_8);

    for(i=13, x=500, y=900; i<=32; x+=10, y-=20, i++) {
        if(k == 10) k = 0;
        Seg_Poly(i, 5, x, y, 100, 700, k, NIVEAU_8);
        k++;
    }

    Tracer_Cercle(6, 33, 70, 900, 500, 150, 150, NOIR, NIVEAU_8);

    /** Paraboles */
    for(x=1, i=104; x<=100; x++, i++) {
        px1 = 1100 + x;
        py1 = f(500, x, 50, 15);
        px2 = 1100 + x + 1;
        py2 = f(500, x, 51, 15);

        Seg_Poly(i, 4, px1, py1, px2, py2, JAUNE_2, NIVEAU_8);
    }

    for(x=1, i=205; x<=100; x++, i++) {
        px1 = 1100 + x;
        py1 = fNeg(500, x, 50, 15);
        px2 = 1100 + x + 1;
        py2 = fNeg(500, x, 51, 15);

        Seg_Poly(i, 4, px1, py1, px2, py2, JAUNE_2, NIVEAU_8);
    }
    /**************************/

    Tracer_Polygone(img, 306);

    Ecrire_Image(img, "_05");
    Free_Image(img);

    printf("\n\nTest 01 termine\n\n");

    return;
}

/****************************************************************************************
* Test de la fonction de calque et du remplissage dans l'espace image
****************************************************************************************/
void Tst_02(void) {
    int i, c=VERT;
    char nr_tag[10];

    image *img=NULL;
    image *nuage_av=NULL;
    image *nuage_ar=NULL;
    image *oiseau=NULL;

    nuage_ar=Lire_Image("nuage","_2");
    Afficher_Header(nuage_ar);
    nuage_av=Lire_Image("nuage","_1");
    Afficher_Header(nuage_av);
    oiseau=Lire_Image("oiseau","_1");
    Afficher_Header(oiseau);

    #ifdef __WIN32__
        system("pause");
    #else
        system("read -n1 -r -p");
    #endif

    for(i = 1; i <= 295; i++) {
        img=Creer_Image("Test_02", 400, 640, BLEU, NIVEAU_2);

        Ajouter_Calque(img, nuage_ar, 150, i-100);

        Tracer_Cercle(1, 1, 30, 20+i*1, 20+i*2, 20, 20, NOIR, NIVEAU_8);

        Tracer_Polygone(img, 30);

        if(i % 10 == 0) {
            if(c == VERT) c = ROUGE;
            else c = VERT;
        }

        Remplir_Polygone(img, i, i+40, i*2, i*2+40, c, NIVEAU_5);

        Ajouter_Calque(img, nuage_av, 100+i/3, 3*i-100);

        Ajouter_Calque(img, oiseau, 250-i/2, 700-3*i);

        sprintf(nr_tag, "_%04d", i);

        Ecrire_Image(img, nr_tag);
        Free_Image(img);
    }

    Free_Image(nuage_av);
    Free_Image(nuage_ar);
    Free_Image(oiseau);

    printf("\n\nTest 02 termine\n\n");

    return;
}

/****************************************************************************************
* Test de creation de formes dans l'espace cartesien
****************************************************************************************/
void Tst_03(void) {
    image *img=NULL;
    float fac=200.0;

    printf("\n\nTest 03\n\n");

    img=Creer_Image("Test_03", 800, 1200, NOIR, NIVEAU_1);

    Seg_Forme(1, 1,  0.0, 0.0, 1.0, 0.0, ROUGE, NIVEAU_8); // Mur
    Seg_Forme(2, 1,  1.0, 0.0, 1.0, 1.0, ROUGE, NIVEAU_8);
    Seg_Forme(3, 1,  1.0, 1.0, 0.0, 1.0, ROUGE, NIVEAU_8);
    Seg_Forme(4, 1,  0.0, 1.0, 0.0, 0.0, ROUGE, NIVEAU_8);

    Seg_Forme(5, 1,  0.0, 1.0, 0.5, 1.3, ROUGE, NIVEAU_8); // Toit
    Seg_Forme(6, 1,  1.0, 1.0, 0.5, 1.3, ROUGE, NIVEAU_8);

    Seg_Forme(7, 1,  0.2, 0.0, 0.2, 0.4, ROUGE, NIVEAU_8); // Porte
    Seg_Forme(8, 1,  0.4, 0.0, 0.4, 0.4, ROUGE, NIVEAU_8);
    Seg_Forme(9, 1,  0.2, 0.4, 0.4, 0.4, ROUGE, NIVEAU_8);

    Afficher_Seg_Forme(9);

    Translation_Forme(9, 1, 0.1, 0.1, 1.0);
    Copier_Forme(0, 9, fac);
    Translation_Forme(9, 1, -0.1, -0.1, 1.0);

    Afficher_Seg_Forme(9);

    Translation_Forme(9, 1, 1.7, 1.5, 0.5);
    Copier_Forme(9, 9, fac);
    Translation_Forme(9, 1, -0.7, -1.5, 1.0);
    Translation_Forme(9, 1, 0.0, 0.0, 2.0);

    Afficher_Seg_Forme(9);

    Rotation_Forme(9, 1, 3.141592/6.0);
    Translation_Forme(9, 1, 3.5, 2.8, 0.25);
    Copier_Forme(18, 9, fac);

    Afficher_Seg_Forme(9);
    Afficher_Seg_Image(27);

    Tracer_Polygone(img, 27);

    Ecrire_Image(img, "_03");
    Free_Image(img);

    printf("\n\nTest 03 termine\n\n");

    return;
}

/****************************************************************************************
* Test des coordonnées : Attention au passage de l'espace cartesien à l'espace image
****************************************************************************************/
void Tst_04(void) {
    image *img=NULL;

    img=Creer_Image("Test_04", 40, 60, JAUNE, NIVEAU_5);
    Afficher_Header(img);

    Seg_Poly(1, 1,  0,  0,  4,  4, NOIR,  NIVEAU_8);
    Seg_Poly(2, 1, 0,  39, 4,  35, BLEU,  NIVEAU_8);
    Seg_Poly(3, 1,  59, 0,  55, 4, VERT,  NIVEAU_8);
    Seg_Poly(4, 1, 56, 36, 60, 40, ROUGE, NIVEAU_8);

    Tracer_Polygone(img, 4);

    Ecrire_Image(img, "_04");
    Free_Image(img);

    printf("\n\nTest 04 termine\n\n");

    return;
}

/****************************************************************************************
* Test de la fonction de calque et du remplissage dans l'espace image
****************************************************************************************/
void Tst_05(void) {
    int i, j, k;
    int m, n, o, p;
    int px1, py1, px2, py2;
    char nr_tag[10];

    image *img=NULL;
    image *nuage_ar=NULL;
    image *nuage_mid=NULL;
    image *nuage_av=NULL;
    image *background=NULL;
    image *oiseau[3];
    image *mario[8];

    j = 0; k = 0;
    m = 1; n = 1; o = 1; p = 1;

    nuage_ar   = Lire_Image("nuage","_1");     Afficher_Header(nuage_ar);
    nuage_mid  = Lire_Image("nuage","_2");     Afficher_Header(nuage_mid);
    nuage_av   = Lire_Image("nuage","_6");     Afficher_Header(nuage_av);
    background = Lire_Image("mario", "_fond"); Afficher_Header(background);
    oiseau[0]  = Lire_Image("oiseau", "_1");   Afficher_Header(oiseau[0]);
    oiseau[1]  = Lire_Image("oiseau", "_2");   Afficher_Header(oiseau[1]);
    oiseau[2]  = Lire_Image("oiseau", "_3");   Afficher_Header(oiseau[2]);
    mario[0]   = Lire_Image("mario", "_1");    Afficher_Header(mario[0]);
    mario[1]   = Lire_Image("mario", "_2");    Afficher_Header(mario[1]);
    mario[2]   = Lire_Image("mario", "_3");    Afficher_Header(mario[2]);
    mario[3]   = Lire_Image("mario", "_4");    Afficher_Header(mario[3]);
    mario[4]   = Lire_Image("mario", "_5");    Afficher_Header(mario[4]);
    mario[5]   = Lire_Image("mario", "_6");    Afficher_Header(mario[5]);
    mario[6]   = Lire_Image("mario", "_7");    Afficher_Header(mario[6]);
    mario[7]   = Lire_Image("mario", "_8");    Afficher_Header(mario[7]);

    #ifdef __WIN32__
        system("pause");
    #else
        system("read -r -n 1 -p \0");
    #endif

    img = Creer_Image("Test_05", 300, 400, BLANC, NIVEAU_1);

    for(i = 1; i <= 295; i++) {
        Copier_Pixel(img, background);

        if(i%3 == 0) j = j++;
        if(j == 3) j = 0;

        if(i%6 == 0) k = k++;
        if(k == 6) k = 0;

        Ajouter_Calque(img, nuage_ar, 150, i-100);
        //Ajouter_Calque(img, nuage_av, 170, 2*i-115);

/****************** Animation de l'oiseau ****************/
        if(i <= 32)
            Ajouter_Calque(img, oiseau[j], 250-i/2, 450-2*i);

        if(i >= 33 && i <= 125)
            Ajouter_Calque(img, oiseau[j], 250-i, 450-2*i);

        if(i >= 126) {
            Ajouter_Calque(img, oiseau[j], 125+m, 450-2*i);
            m++;
        }
/*********************************************************/

        Ajouter_Calque(img, nuage_mid, 200/3, 400-3*i);

/****************** Animation de Mario *******************/
        if(i <= 32)
            Ajouter_Calque(img, mario[k], 93, 2*i);

        if(i >= 33 && i <= 69) {
            Ajouter_Calque(img, mario[k], 95-o, 2*i);
            o++;
        }

        if(i >= 70 && i <= 125)
            Ajouter_Calque(img, mario[k], 59, 2*i);

        if(i >= 126 && i <= 200) {
            Ajouter_Calque(img, mario[k], 59+p/2, 2*i);
            p++;
        }
/*********************************************************/

        sprintf(nr_tag, "_%04d", i);

        Ecrire_Image(img, nr_tag);
    }

    Free_Image(nuage_ar);
    Free_Image(nuage_mid);
    Free_Image(nuage_av);
    for(i=0; i<3; i++) Free_Image(oiseau[i]);
    for(i=0; i<8; i++) Free_Image(mario[i]);
    Free_Image(background);
    Free_Image(img);

    printf("\n\nTest 02 termine\n\n");

    return;
}
