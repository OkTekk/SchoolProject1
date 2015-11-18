/****************************************************************************************
* Dossier 1 : Generation d'images .bmp 24 bits a partir d'une table de segments
*
* Implementation des fonctions de l'espace image
*
* Programmation procedurale 2015 - Laurent Fournier - v4
****************************************************************************************/
#include "global.h"

extern seg_i poly[10000];
extern seg_f form[1000];

/****************************************************************************************
*
****************************************************************************************/
void Initialiser_Image(image * img, int * col) {
    int i, j, k;

    for(i = 0; i < img->hauteur; i++) {
        for(j = 0; j < img->largeur; j++) {
            for(k = 0; k < 3; k++) {
                img->pic[i][j][k] = (unsigned char)col[k];
            }
        }
    }

    return;
}


void Seg_Poly(int i, int obj, int x1, int y1, int x2, int y2, int col, int niv) {
    poly[i].obj = obj;
    poly[i].x1 = x1;
    poly[i].y1 = y1;
    poly[i].x2 = x2;
    poly[i].y2 = y2;
    poly[i].col = col;
    poly[i].niv = niv;
}


void Ajouter_Calque(image * img, image * cal, int x, int y) {
    int i, j, k;
    int xi, yj;

    for(i = 0; i < cal->hauteur; i++) {
        for(j = 0; j < cal->largeur; j++) {
            xi = x+i;
            yj = y+j;

            if(cal->pic[i][j][0] + cal->pic[i][j][1] + cal->pic[i][j][2] != 0 &&
               xi < img->hauteur && yj < img->largeur && yj >= 0 && xi >= 0) {

                for(k = 0; k < 3; k++) img->pic[xi][yj][k] = cal->pic[i][j][k];
            }
        }
    }
}


void Tracer_Polygone(image * img, int nmax) {
    int i;

    for(i = 1; i <= nmax; i++)
        Tracer_Segment(img, i, Get_Color(poly[i].col, poly[i].niv));
}


void Tracer_Segment(image * img, int pI, int * col) {
    int i, k;
    int x1 = poly[pI].x1, y1 = poly[pI].y1;
    int x2 = poly[pI].x2, y2 = poly[pI].y2;

    float m;
    int x, y;
    int dx = x2 - x1, dy = y2 - y1;
    int dxabs = abs(dx), dyabs = abs(dy);
    int sdx = sgn(dx), sdy = sgn(dy);

    if(dxabs >= dyabs) {
        m = (float)dy / (float)dx;
        for(i = 0; i != dx; i += sdx) {
            x = i + x1;
            y = (int)((m * i + y1));

            if(x < img->largeur && y < img->hauteur && x >= 0 && y >= 0) {
                for(k = 0; k < 3; k++) {
                    img->pic[y][x][k] = (unsigned char)col[k];
                }
            }
        }
    }
    else {
        m = (float)dx / (float)dy;
        for(i = 0; i != dy; i += sdy) {
            x = (int)((m * i + x1));
            y = i + y1;

            if(x < img->largeur && y < img->hauteur && x >= 0 && y >= 0) {
                for(k = 0; k < 3; k++) {
                    img->pic[y][x][k] = (unsigned char)col[k];
                }
            }
        }
    }
}


void Tracer_Cercle(int obj, int ii, int ns, int x, int y, int a, int b, int col, int niv) {
    int c, i;
    float pas, pol, x1, x2, y1, y2;

    pol = 0.0;
    pas = 2.0*3.141592 / (float)ns;
    i = ii;

    for(c = ii; c < ii+ns; c++) {
        x1 = x + a * cos(pol);
        y1 = y + b * sin(pol);
        x2 = x + a * cos(pol+pas);
        y2 = y + b * sin(pol+pas);
        poly[i].obj = obj;
        poly[i].x1 = (int)x1;
        poly[i].y1 = (int)y1;
        poly[i].x2 = (int)x2;
        poly[i].y2 = (int)y2;
        poly[i].col = col;
        poly[i].niv = niv;
        pol = pol+pas;
        i++;
    }
}


int f(int extremum, int x, int center, int scale) {
    return extremum - pow(x - center, 2) / scale;
}


int fNeg(int extremum, int x, int center, int scale) {
    return extremum + pow(x - center, 2) / scale;
}


void Remplir_Polygone(image * img, int i1, int i2, int j1, int j2, int col, int niv) {

}


void Afficher_Seg_Image(int imax) {
    int i;

    for(i = 1; i <= imax; i++) {
        printf("segment : %d\n", i);
        printf("objet : %d\n", poly[i].obj);
        printf("x1 : %d\n", poly[i].x1);
        printf("y1 : %d\n", poly[i].y1);
        printf("x2 : %d\n", poly[i].x2);
        printf("y2 : %d\n", poly[i].y2);
        printf("col : %d\n", poly[i].col);
        printf("niv : %d\n", poly[i].niv);
        printf("-------------\n");
    }
}
