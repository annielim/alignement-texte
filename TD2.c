#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <string.h>

struct alignement
{
  char * x;
  char * y;
};


/* =============================================================== */
char * readtextfile(char * filename)
/* Retourne le contenu du fichier texte filename */
/* =============================================================== */
{
  struct stat monstat;
  int N;
  char * text = NULL;
  FILE *fd = NULL;

  N = stat(filename, &monstat);
  if (N == -1)
  {
    fprintf(stderr, "error : bad file %s\n", filename);
    exit(0);
  }
  N = monstat.st_size;
  text = (char *)malloc(N+1);
  if (text == NULL)
  {   fprintf(stderr,"readtextfile() : malloc failed for text\n");
      exit(0);
  }
  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "readtextfile: can't open file %s\n", filename);
    exit(0);
  }
  
  fread(text, sizeof(char), N, fd);
  if((N>0) && (text[N-1] == '\n') ) text[N-1] = '\0';
  else text[N-1] = '\0';
  fclose(fd);
  return text;
}

/* =============================================================== */
int Imax(int a, int b)
/* Retourne  le maximum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return b;
  else return a;	       
}

/* =============================================================== */
int Imin2(int a, int b)
/* Retourne  le minimum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return a;
  else return b;	       
}

/* =============================================================== */
int Imin3(int a, int b, int c)
/* Retourne  le minimum de a, b et c                               */
/* =============================================================== */
{
  return Imin2(Imin2(a,b),c);
}

/* =============================================================== */
void retourne(char *c)
/* Retourner la chaine de caractere c                              */
/* =============================================================== */
{
  char tmp;
  int m, j, i;
  m = strlen(c);
  j = m/2;
  for(i = 0; i < j; i++ ){
    tmp = c[i];
    c[i] = c[m-i-1];
    c[m-i-1] = tmp;
  }
}
/* =============================================================== */
void afficheSeparateurHorizontal(int nbcar)
/* =============================================================== */
{
  int i;
  printf("|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|\n");
}


/* =============================================================== */
void affiche(char* texte1, char* texte2, int nbcar)
  /* Affiche simultanement texte1 et texte 2 en positionnnant nbcar  
     caracteres sur chaque ligne. */
/* =============================================================== */
{
  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);
  afficheSeparateurHorizontal(nbcar);
  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "| %c-%ds | %c-%ds |\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);
  }
  afficheSeparateurHorizontal(nbcar);
  free(t1);
  free(t2);
}



/* =============================================================== */
/* idem affiche, mais avec un formattage different*/
/* =============================================================== */
void affiche2(char* texte1, char* texte2, int nbcar)
{

  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);

  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "x: %c-%ds \ny: %c-%ds\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);

  }
  free(t1);
  free(t2);
}

int sub(char a, char b){
	if(a == b)
		return 0;
	return 1;
}

int** compute_distance(char* texte1, char* texte2){
	int n = strlen(texte1);
	int m = strlen(texte2);

	int** T= (int**) malloc((m+1)*sizeof(int*));
    for(int i=0; i<=m; i++)
  		T[i]= (int*) malloc((n+1)*sizeof(int));
	
	//T[m+1][n+1]
	T[0][0] = 0;
	for(int i=1; i<n+1;++i)
		T[0][i] = T[0][i-1] + 1; //Cout del
	for(int j=1; j<m+1;++j)
		T[j][0] = T[j-1][0] + 1; //Cout ins	
	T[1][1] = Imin3(T[0][1]+1,T[1][0]+1, T[0][0]);
	for(int i=1; i<n+1;++i)
		for(int j=1; j<m+1;++j){
			T[j][i] = Imin3(T[j-1][i]+1,\
						   	T[j][i-1]+1	,\
							T[j-1][i-1]+sub(texte1[i-1],texte2[j-1]));
		}
	
	return T;
}

char** get_alignement(int** T, char* texte1, char* texte2, int verbose){

	char blank = ' ';
	int n = strlen(texte1);
	int m = strlen(texte2);
	int i = m;
	int j = n;

	int len = m+n;
	//int len = Imax(m,n);
	char *texte1_aligned = (char *)malloc((len)*sizeof(char));
	char *texte2_aligned = (char *)malloc((len)*sizeof(char));

	int k = 0;
	int k_1 = 0;
	int k_2 = 0;
	while(i !=0 || j != 0){
		//Si on a choisi Ins
		if( i>0 && (T[i][j] == T[i-1][j] + 1)){
			if(verbose) printf("Ins %c dans texte 1\n",texte2[m-k_2-1]);
			texte1_aligned[k] = blank;
			texte2_aligned[k] = texte2[m-k_2-1];
			i--;
			k_2++;
		}
		//Si on a choisi Del
		else if( j>0 && (T[i][j] == T[i][j-1] + 1)){
			if(verbose) printf("Del %c dans texte 1\n",texte1[n-k_1-1]);
			texte1_aligned[k] = texte1[n-k_1-1];
			texte2_aligned[k] = blank;
			j--;
			k_1++;
		}
		//Si on a choisi Sub
		else if((T[i][j] == T[i-1][j-1] + sub(texte1[j-1],texte2[i-1]))){
			if(verbose) printf("Remplacer %c par %c dans le texte 1\n",texte1[n-k_1-1],texte2[m-k_2-1]);
			texte1_aligned[k] = texte1[n-k_1-1];
			texte2_aligned[k] = texte2[m-k_2-1];
			j--;
			i--;
			k_1++;
			k_2++;
		}
		else{
			printf("Erreur dans le calcul de la table\n");
			break;
		}
		k++;
	}

	char **textes = (char**)malloc(2*sizeof(char*));
	retourne(texte1_aligned);
	retourne(texte2_aligned);
	textes[0] = texte1_aligned;
	textes[1] = texte2_aligned;
	return textes; 
}


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  char *texte1, *texte2; 
  
  if(argc != 3){
    printf("usage: %s text1 text2\n", argv[0]);
    exit(0);
  }  

  texte1 = readtextfile(argv[1]);
  texte2 = readtextfile(argv[2]);;
  
  affiche(texte1, texte2, 50);
 

  int** T = compute_distance(texte1,texte2);
  printf("Cout: %d\n",T[strlen(texte2)][strlen(texte1)]);

  char** alignes = get_alignement(T, texte1, texte2,0);
  affiche(alignes[0],alignes[1],50);

  free(alignes[0]);
  free(alignes[1]);
  free(alignes);
  free(texte1);
  free(texte2);
}
