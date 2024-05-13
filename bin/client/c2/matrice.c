#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct matrice_s {
	int ligne;
	int ncol;
	float **tab ;
};
typedef struct matrice_s *matrice;
struct matrice_s ;
matrice init_mat(int nl, int col ){
        //initialisation de la matrice vide 
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
    return mat ;
}
matrice creer_matrice(int nl , int col, float **tableau)
{

     
    // remplissage de la matrice 
    int i,j ;
    matrice mat = init_mat(nl,col);
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
  
    matrice mat = init_mat(a->ligne,a->ncol);
     int i,j;
       // calcule de la somme
       for(i=0; i< a->ligne; i++)
       {
        for (j=0; j< a->ncol; j++){
            mat->tab[i][j]= (a->tab[i][j]) + (b->tab[i][j]);
        }
       }
    return mat ;

}
matrice transposee_mat(matrice mat)
{
   
    matrice trans = init_mat(mat->ncol,mat->ligne);
    int i,j;
    //transposition de la matrice
   for(i=0;i< trans->ncol;i++)
    {
       // matrice temp = mat;
        for(j=0;j<trans->ligne;j++){
            trans->tab[j][i]=(mat->tab[i][j]);
            
        }
    }
     
   
    return trans ;
}
matrice produit_mat(matrice a, matrice b)
{
  
    matrice mat = init_mat(a->ligne,a->ligne);
    int i,j;
    // produit des matrices 
    float som;
     for(i=0; i< a->ligne;i++)
     {
        
        for(j=0;j< a->ncol;j++){
            mat->tab[i][j]=0;
            for (int k=0; k<a->ligne;k++)
                mat->tab[i][j]+=a->tab[i][k]*b->tab[k][j];
           
        
        }
 
     }
     return mat ;
}
// generation d'une matrice aleatoire 
    matrice generer_mat(int nl, int col)
    {
        matrice mat=init_mat(nl,col);
        srand(time(NULL));
        for(int i=0;i<nl;i++){
        for(int j=0;j<col;j++){
            
            mat->tab[i][j] = rand() % 101; //generer un nombre aleatoire entre 0 et 100
        }
    }
    return mat ;
    }
    void print_fichier(matrice a,char nom_fichier[]){
        FILE *file1;
        file1=fopen(nom_fichier,"w");
        fprintf(file1,"%d %d\n",a->ligne,a->ncol);
        for(int i=0;i<a->ligne;i++)
        {
            for(int j=0;j<a->ncol;j++)
            fprintf(file1,"%f\t",a->tab[i][j]);
            fprintf(file1,"\n");
        }
        fclose(file1);
    }
    matrice recup_fichier(char nom_fichier[]){
        int i,j, ligne, ncol;
        FILE *file1= fopen(nom_fichier,"r");
        fscanf(file1, "%d %d", &ligne,&ncol);
        matrice mat=init_mat(ligne,ncol);
        for(i=0;i<ligne;i++)
        {
            for( j=0;j<ncol;j++)
            fscanf(file1, "%f",&mat->tab[i][j]);
        }
        fclose(file1);
        return mat;
    }
int main()
{
   
    int rows, col,i,j ;
    float **tableau1 **tableau2 ;
    printf("taille de la matrice , format ligne colonnes\n");
    scanf("%d %d", &rows,&col);
    printf("entrer les donnees de la premiere matrice\n");
       for(i=0;i<rows;i++){
        for(int j=0;j<col;j++){
            printf("valeur pour [%d][%d]? ",i+1,j+1);
            scanf("%f", &tableau1[i][j]);
        }
    }
     matrice mat1 = creer_matrice(rows,col,tableau);
    printf("entrer les donnees de la seconde matrice\n");
     matrice mat2 = creer_matrice(rows,col,tableau);
    
        // affichage de la matrice
     /*   
    matrice mat1=generer_mat(rows,col);
    sleep(1);
    matrice mat2=generer_mat(rows,col);

        printf("premiere matrice\n");
     affichage_mat(mat1);
        printf("seconde matrice\n");

     affichage_mat(mat2);
     matrice som = somme_mat(mat1,mat2);
     printf("la somme de ces deux matrices est:\n");
    
     affichage_mat(som);
     
      /*  printf("le produit de ces deux matrices est impossible because the colunm's number of the first matrix is different from the rows's number of thsecond \n");
        printf("pour cela , nous allons utiliser la transposée de la deuxieme matrice pour faire ce produit\n");
        matrice transp = transposee_mat(mat2);
        printf("matrice transposée\n");
        affichage_mat(transp);
        */
       /* char nom_fichier[]="resultat.txt";
        matrice result=recup_fichier(nom_fichier);
        affichage_mat(result);
        */
     /*
     printf("le produit des deux matrices est:\n");
      matrice produit = produit_mat(mat1,mat2);
        affichage_mat(produit);
        char result[]="resultat.txt";
        print_fichier(mat1,result);
     
    //liberation de la matrice
    liberation_mat(mat1);
    liberation_mat(mat2);
    liberation_mat(som);
   // liberation_mat(transp);
*/

    return 0;
}