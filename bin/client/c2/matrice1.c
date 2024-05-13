#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int **matrix ;
    int rows, col,i,j ;
    printf("taille de la matrice , format ligne colonnes\n");
    scanf("%d %d", &rows,&col);
 
    matrix = malloc(rows * sizeof(int*));
    if(matrix == NULL)
       exit(1);
    for(i=0;i<rows;i++){
        matrix[i]= malloc(col * sizeof(int));
       if(matrix == NULL)
       exit(1); 
    }
    /*for(i=0;i<rows;i++){
        free(matrix[i]);
        matrix[i]= NULL ;
    }*/
    // remplissage de la matrice 
    for(i=0;i<rows;i++){
        for(j=0;j<col;j++){
            printf("valeur pour [%d][%d]? ",i,j);
            scanf("%2d", &matrix[i][j]);
        }
    }
        // affichage de la matrice
    for(i=0;i<rows;i++){
        printf("[");
        for(j=0;j<col;j++){
          // printf("[%d][%d] = %d\n",i,j, matrix[i][j]); 
             printf("%d\t",matrix[i][j]);
        }
        printf("]\n");
    }
    //liberation de la matrice
    for (i=0;i<rows;i++)
    {
        free(matrix[i]);
        matrix[i]= NULL;
    }
    free(matrix);
    matrix= NULL ;
    return 0;
}
