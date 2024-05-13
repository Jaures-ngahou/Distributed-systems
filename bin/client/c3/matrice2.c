#include <stdio.h>
#include <stdlib.h>
struct matrice_s {
	int ligne;
	int ncol;
	float **tab ;
};
typedef struct matrice_s *matrice;
struct matrice_s ;
matrice creer_matrice(int nl , int col, float **tableau)
{
    //creation de la matrice
matrice mat = malloc(sizeof(struct matrice_s));
    
    mat->ligne = nl;
    mat->ncol = col ;
    int i;
    mat->tab= malloc(nl * sizeof(int*));
    if(mat== NULL)
       exit(1);
    for(i=0;i<nl;i++){
        mat->tab[i]= malloc(col * sizeof(int));
       if(mat == NULL)
       exit(1); 
      
    }
    /*for(i=0;i<rows;i++){
        free(matrix[i]);
        matrix[i]= NULL ;
    }*/
    // remplissage de la matrice 
    for(i=0;i<nl;i++){
        for(int j=0;j<col;j++){
            printf("valeur pour [%d][%d]? ",i+1,j+1);
            scanf("%f", &mat->tab[i][j]);
        }
    }
     return mat ;
}
void affichage_mat(matrice x)
{
 for(int i=0;i< x->ligne ;i++){
        printf("[");
        for(int j=0;j < x->ncol ; j++){
          // printf("[%d][%d] = %d\n",i+1,j+1, matrix[i][j]); 
             printf("%f\t",x->tab[i][j]);
        }
        printf("]\n");
    }
}
void liberation_mat(matrice x)
{
 for (int i=0;i< x->ligne ;i++)
    {
        free(x->tab[i]);
        x->tab[i]= NULL;
    }
    free(x);
    x= NULL ;
}
matrice somme_mat(matrice a, matrice b)
{
    if (a->ligne != b->ligne || a->ncol != b->ncol)
       printf("les matrices doivent avoir la meme taille");
    // initialisation de la matrice
    matrice mat = malloc(sizeof(struct matrice_s));
    int i,j;
     mat->ligne = a->ligne;
    mat->ncol = b->ncol ;
    mat->tab= malloc(a->ligne* sizeof(int*));
    if(mat== NULL)
       exit(1);
    for(i=0;i< a->ligne;i++){
        mat->tab[i]= malloc(a->ncol * sizeof(int));
       if(mat == NULL)
       exit(1); 
    }
       // calcule de la somme
       for(i=0; i< a->ligne; i++)
       {
        for (j=0; j< a->ncol; j++){
            mat->tab[i][j]= (a->tab[i][j]) + (b->tab[i][j]);
        }
       }
    return mat ;

}
int main()
{
    int rows, col,i,j ;
    float **tableau ;
    printf("taille de la matrice , format ligne colonnes\n");
    scanf("%d %d", &rows,&col);
    printf("entrer les donnees de la premiere matrice\n");
     matrice mat1 = creer_matrice(rows,col,tableau);
    printf("entrer les donnees de la seconde matrice\n");
     matrice mat2 = creer_matrice(rows,col,tableau);
    
        // affichage de la matrice
        printf("premiere matrice\n");
     affichage_mat(mat1);
        printf("seconde matrice\n");

     affichage_mat(mat2);
     matrice som = somme_mat(mat1,mat2);
     printf("la somme de ces deux matrices est:\n");
    
     affichage_mat(som);
    //liberation de la matrice
    /*liberation_mat(mat1);
    liberation_mat(mat2);
    liberation_mat(som);*/

    return 0;
}