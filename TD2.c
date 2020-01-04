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


/*================================================================ */
/* 					    Exercice 3                                 */
/*================================================================ */


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
	char *texte1_aligned = (char *)malloc((len+1)*sizeof(char));
	char *texte2_aligned = (char *)malloc((len+1)*sizeof(char));

	int k = 0;
	while(i !=0 || j != 0){
		//Si on a choisi Ins
		if( i>0 && (T[i][j] == T[i-1][j] + 1)){
			if(verbose) printf("Ins %c dans texte 1\n",texte2[i-1]);
			texte1_aligned[k] = blank;
			texte2_aligned[k] = texte2[i-1];
			i--;
		}
		//Si on a choisi Del
		else if( j>0 && (T[i][j] == T[i][j-1] + 1)){
			if(verbose) printf("Del %c dans texte 1\n",texte1[j-1]);
			texte1_aligned[k] = texte1[j-1];
			texte2_aligned[k] = blank;
			j--;
		}
		//Si on a choisi Sub
		else if((T[i][j] == T[i-1][j-1] + sub(texte1[j-1],texte2[i-1]))){
			if(verbose) printf("Remplacer %c par %c dans le texte 1\n",texte1[j-1],texte2[j-1]);
			texte1_aligned[k] = texte1[j-1];
			texte2_aligned[k] = texte2[i-1];
			j--;
			i--;
		}
		else{
			printf("Erreur dans le calcul de la table\n");
			break;
		}
		k++;
	}
	texte1_aligned[k] ='\0';
	texte2_aligned[k] ='\0';

	char **textes = (char**)malloc(2*sizeof(char*));
	retourne(texte1_aligned);
	retourne(texte2_aligned);
	textes[0] = texte1_aligned;
	textes[1] = texte2_aligned;
	return textes; 
}

void align_sentence(char* texte1, char*texte2){

	int** T = compute_distance(texte1, texte2);
	printf("Distance entre les textes: %d, longueur du texte1: %ld, longueur du texte2: %ld\n",\
			T[strlen(texte2)][strlen(texte1)], strlen(texte1),strlen(texte2));

	char** result = get_alignement(T, texte1, texte2, 0);
	affiche(result[0],result[1],80);

	//Free results
	free(result[0]);
	free(result[1]);
	free(result);

	//Free T
	for(int i=0;i<=strlen(texte2);++i)
		free(T[i]);
	free(T);
}


/*================================================================ */
/* 					    Exercice 4                                 */
/*================================================================ */

int count_occurences(char* texte, const char sep){
	int count = 0;
	for(int i = 0; i < strlen(texte);++i){
		if(texte[i] == sep)
			count++;
	}
	return count;	
}

char** split(char* texte,int count, const char* sep){
	if(count == 0)
		return NULL;

	char* texte_cop = strdup(texte);
	char** liste = (char**)malloc((count+1)*sizeof(char*));
	char* reste = NULL;
	char* token;
	int i = 0;
	for(token = strtok_r(texte_cop,sep,&reste); token !=NULL; token=strtok_r(NULL,sep,&reste))
	{
		liste[i] = strdup(token);
		i++;
	}
	return liste;
}

int sub_strings(char* texte1, char* texte2){
	int** T = compute_distance(texte1,texte2);
	int val = T[strlen(texte2)][strlen(texte1)];

	for(int i = 0; i <= strlen(texte2);++i)
		free(T[i]);
	free(T);
	return val;
}

int ins_strings(char* texte2){
	return strlen(texte2);
}
int del_strings(char* texte1){
	return strlen(texte1);
}

char* blank_string(char blank, int n){
	char* string = (char*)malloc((n+1)*sizeof(char));
	memset(string,blank,n);
	string[n] = '\0';
	return string;
}


int** compute_distance_strings(char** texte1,int n1, char** texte2,int n2){
	int n = n1+1;
	int m = n2+1;

	int** T= (int**) malloc((m+1)*sizeof(int*));
	for(int i=0; i<=m; i++)
		T[i]= (int*) malloc((n+1)*sizeof(int));

	//T[m+1][n+1]
	T[0][0] = 0;
	for(int i=1; i<n+1;++i)
		T[0][i] = T[0][i-1] + del_strings(texte1[i-1]); //Cout del
	for(int j=1; j<m+1;++j)
		T[j][0] = T[j-1][0] + ins_strings(texte2[j-1]); //Cout ins	
	T[1][1] = Imin3(T[0][1]+1,T[1][0]+1, T[0][0]);
	for(int i=1; i<n+1;++i)
		for(int j=1; j<m+1;++j){
			T[j][i] = Imin3(T[j-1][i]+ins_strings(texte2[j-1]),\
					T[j][i-1]+del_strings(texte1[i-1])	,\
					T[j-1][i-1]+sub_strings(texte1[i-1],texte2[j-1]));
		}


	return T;
}

char*** get_alignement_texts(int** T, char** texte1,int n1, char** texte2,int n2, int verbose, int* count){
	char blank = ' ';
	int n = n1+1; 
	int m = n2+1;
	int i = m;
	int j = n;

	int len = m+n;
	//int len = Imax(m,n);
	char **texte1_aligned = (char **)malloc((len)*sizeof(char*));
	char **texte2_aligned = (char **)malloc((len)*sizeof(char*));

	int k = 0;
	while(i !=0 || j != 0){
		//Si on a choisi Ins
		if( i>0 && (T[i][j] == T[i-1][j] + ins_strings(texte2[i-1]))){
			if(verbose) printf("Ins\n");
			texte1_aligned[k] = blank_string(blank,strlen(texte2[i-1]));
			texte2_aligned[k] = strdup(texte2[i-1]);
			i--;
		}
		//Si on a choisi Del
		else if( j>0 && (T[i][j] == T[i][j-1] + del_strings(texte1[j-1]))){
			if(verbose) printf("Del\n");
			texte1_aligned[k] = strdup(texte1[j-1]);
			texte2_aligned[k] = blank_string(blank, strlen(texte1[j-1]));
			j--;
		}
		//Si on a choisi Sub
		else if((T[i][j] == T[i-1][j-1] + sub_strings(texte1[j-1],texte2[i-1]))){
			if(verbose) printf("Sub\n");

			int** T_temp = compute_distance(texte1[j-1],texte2[i-1]);

			char** alignes = get_alignement(T_temp, texte1[j-1], texte2[i-1],0);

			texte1_aligned[k] = alignes[0];
			texte2_aligned[k] = alignes[1];

			//Free T
			int size =strlen(texte2[i-1]);
			for(int i=0; i<=size;++i){
				free(T_temp[i]);
			}
			free(T_temp);
			//Free alignes
			free(alignes);
			j--;
			i--;
		}
		else{
			printf("Erreur dans le calcul de la table\n");
			break;
		}
		k++;
	}

	*count = k;


	char ***textes = (char***)malloc(2*sizeof(char**));
	textes[0] = texte1_aligned;
	textes[1] = texte2_aligned;
	return textes; 
}

void align_texts(char* texte1, char* texte2){
	int n1 = count_occurences(texte1,'\n');
	char** liste1 = split(texte1,n1,"\n");
	int n2 = count_occurences(texte2,'\n');
	char** liste2 = split(texte2,n2,"\n");

	int** T = compute_distance_strings(liste1, n1, liste2, n2);
	printf("Distance entre les textes: %d, longueur du texte1: %ld, longueur du texte2: %ld\n",\
			T[n2+1][n1+1], strlen(texte1),strlen(texte2));
	int count;
	char*** result = get_alignement_texts(T, liste1, n1, liste2, n2, 0, &count);
	for(int i = count-1; i>=0;--i){
		affiche(result[0][i],result[1][i],80);
	}

	//Free results
	for(int i=0;i<count;++i){
		free(result[0][i]);
		free(result[1][i]);
	}
	free(result[0]);
	free(result[1]);
	free(result);

	//Free T
	for(int i=0;i<=n2+1;++i)
		free(T[i]);
	free(T);

	//Free liste1 et liste2
	for(int i=0;i<=n1;++i){
		free(liste1[i]);
	}
	free(liste1);

	for(int i=0;i<=n2;++i){
		free(liste2[i]);
	}
	free(liste2);

}

/* =============================================================== */
int main(int argc, char **argv)
	/* =============================================================== */
{
	char *texte1, *texte2; 

	printf("================================================================\n");
	printf(" 	                        Exercice 3                            \n");
	printf("================================================================\n");

	texte1 = readtextfile("texte1.txt");
	texte2 = readtextfile("texte2.txt");

	align_sentence(texte1, texte2);

	free(texte1);
	free(texte2);

	printf("================================================================\n");
	printf(" 	                        Exercice 4                            \n");
	printf("================================================================\n");

	texte1 = readtextfile("t1.txt");
	texte2 = readtextfile("t2.txt");

	align_texts(texte1, texte2);

	free(texte1);
	free(texte2);

}
