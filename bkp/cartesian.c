/****************************************************************************************
* Dossier 1 : Generation d'images .bmp 24 bits a partir d'une table de segments
*
* Implementation des fonctions de formes dans l'espace Cartésien
*
* Programmation procedurale 2015 - Laurent Fournier - v4
****************************************************************************************/
#include "global.h"

extern seg_i poly[10000];
extern seg_f form[1000];

/****************************************************************************************
*
****************************************************************************************/
void Copier_Forme(int iori, int imax, float fac) {
    int i;

    for(i = 1; i <= imax; i++) {
        poly[i+iori].obj = form[i].obj;
        poly[i+iori].x1 = (int)(form[i].x1 * fac);
        poly[i+iori].y1 = (int)(form[i].y1 * fac);
        poly[i+iori].x2 = (int)(form[i].x2 * fac);
        poly[i+iori].y2 = (int)(form[i].y2 * fac);
        poly[i+iori].col = form[i].col;
        poly[i+iori].niv = form[i].niv;
    }
}


void Seg_Forme(int i, int obj, float x1, float y1, float x2, float y2, int col, int niv) {
    form[i].obj = obj;
    form[i].x1 = x1;
    form[i].y1 = y1;
    form[i].x2 = x2;
    form[i].y2 = y2;
    form[i].col = col;
    form[i].niv = niv;
}


void Rotation_Forme(int imax, int obj, float rot) {
    int i;
    float cosA = cos(rot), sinA = sin(rot);
    float x1, y1, x2, y2;

    for(i = 1; i <= imax; i++) {
        x1 = form[i].x1; y1 = form[i].y1;
        x2 = form[i].x2; y2 = form[i].y2;

        form[i].x1 = cosA * x1 - sinA * y1;
        form[i].y1 = sinA * x1 + cosA * y1;

        form[i].x2 = cosA * x2 - sinA * y2;
        form[i].y2 = sinA * x2 + cosA * y2;
    }
}


void Translation_Forme(int imax, int obj, float x, float y, float fac) {
    int i;

    for(i = 1; i <= imax; i++) {
        form[i].x1 *= fac;
        form[i].x1 += x;

        form[i].x2 *= fac;
        form[i].x2 += x;

        form[i].y1 *= fac;
        form[i].y1 += y;

        form[i].y2 *= fac;
        form[i].y2 += y;
    }
}


void Afficher_Seg_Forme(int imax) {
    int i;

    for(i = 1; i <= imax; i++) {
        printf("segment : %d\n", i);
        printf("obj : %d\n", form[i].obj);
        printf("x1 : %f\n", form[i].x1);
        printf("y1 : %f\n", form[i].y1);
        printf("x2 : %f\n", form[i].x2);
        printf("y2 : %f\n", form[i].y2);
        printf("col : %d\n", form[i].col);
        printf("niv : %d\n", form[i].niv);
        printf("-------------\n");
    }
}

