#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct PGMImage
{
    char pgmType[3];
    int **data;
    int width;
    int height;
    int maxValue;
} PGMImage;

void printImageDetails(PGMImage *pgm, const char *filename);
bool openPGM(PGMImage *pgm, const char *filename);
void limirizacaoPGM(const char *filename, const PGMImage *pgm);
void negativoPGM(const char *filename, const PGMImage *pgm);
void histogramaPGM(const char *filename, const PGMImage *pgm);
void inicializar();
void ignoreComments(FILE* fp);

int main(int argc, char *argv[])
{
	inicializar();
  
    return 0;
}

void inicializar(){
	char filename[500];
	char otherFiles[500];
	char file[500];
	printf("Digite o nome do arquivo(MAXIMO 400 caracteres):\n");
	scanf(" %[^\n]%*c",&file);
	
	strcat(strcpy(otherFiles, file), ".pgm");
	PGMImage *pgm = malloc(sizeof(PGMImage));

    printf("Arquivo: %s\n", otherFiles);

    if (openPGM(pgm, otherFiles))
    {
    	strcat(strcpy(filename, file), "Limiarizacao.pgm");
        limirizacaoPGM(filename, pgm);
        printf("\nLimiarizacao completa: %sn\nNegativo comecando...\n",filename);
        filename[0]='\0';
        
        strcat(strcpy(filename, file), "Negativo.pgm");
        negativoPGM(filename, pgm);
        printf("Negativo completa: %s\n\nHistograma comecando...\n",filename);
        filename[0]='\0';
        
        strcat(strcpy(filename, file), "Histograma.txt");
        histogramaPGM(filename, pgm);
        printf("Histograma completo: %s",filename);
        filename[0]='\0';
    }

}

void histogramaPGM(const char *filename, const PGMImage *pgm){
	FILE *txtFile;
	int pix[255]={0};
	txtFile=fopen(filename,"w");
	if (txtFile == NULL)
    {
        perror("Nao foi possivel abrir");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < pgm->width; i++)
    {
        for (int j = 0; j < pgm->height; j++)
        {
            pix[pgm->data[i][j]]++;
        }
    }
    for(int i=0;i<=pgm->maxValue;i++){
    	fprintf(txtFile,"Tonalidade: %d -> Pixels: %d \n",i,pix[i]);
	}
	fclose(txtFile);
    
}

void negativoPGM(const char *filename, const PGMImage *pgm)
{
    FILE *pgmFile;
    char *number;
    int ad = 0, x = 0, outro = 0;
    int **ado;
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL)
    {
        perror("Nao foi possivel abrir");
        exit(EXIT_FAILURE);
    }

    fprintf(pgmFile, "P2\n");
    fprintf(pgmFile, "%d %d\n", pgm->width, pgm->height);
    fprintf(pgmFile, "%d\n", pgm->maxValue);
    for (int i = 0; i < pgm->width; i++)
    {
        for (int j = 0; j < pgm->height; j++)
        {
            fprintf(pgmFile, "%d ", 255-pgm->data[i][j]);
        }
        fprintf(pgmFile, "\n");
    }
    fclose(pgmFile);
}

void limirizacaoPGM(const char *filename, const PGMImage *pgm)
{
    FILE *pgmFile;
    char *number;
    int ad = 0, x = 0, outro = 0;
    int **ado;
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL)
    {
        perror("Nao foi possivel abrir");
        exit(EXIT_FAILURE);
    }

    fprintf(pgmFile, "P2\n");
    fprintf(pgmFile, "%d %d\n", pgm->width, pgm->height);
    fprintf(pgmFile, "%d\n", pgm->maxValue);
    for (int i = 0; i < pgm->width; i++)
    {
        for (int j = 0; j < pgm->height; j++)
        {
            if (pgm->data[i][j] > 150)
            {
                fprintf(pgmFile, "%d ", 255);
            }
            else
            {
                fprintf(pgmFile, "%d ", 0);
            }
        }
        fprintf(pgmFile, "\n");
    }
    fclose(pgmFile);
}

bool openPGM(PGMImage *pgm, const char *filename)
{
    FILE *pgmfile = fopen(filename, "rb");

    if (pgmfile == NULL)
    {
        printf("Arquivo nao existe\n");
        return false;
    }
	ignoreComments(pgmfile);
    fscanf(pgmfile, "%s", pgm->pgmType);
    
	ignoreComments(pgmfile);
    fscanf(pgmfile, "%d %d", &(pgm->width), &(pgm->height));
    
	ignoreComments(pgmfile);
    fscanf(pgmfile, "%d", &(pgm->maxValue));
    
	ignoreComments(pgmfile);
    pgm->data = malloc(pgm->height * sizeof(int *));



    for (int i = 0; i < pgm->height; i++){
		pgm->data[i] = malloc(pgm->width * sizeof(int *));
		
        if (pgm->data[i] == NULL)
        {
            printf("erro malloc\n");
            exit(1);
        }
        
        for (int j = 0; j < pgm->width; j++)
        {
            fscanf(pgmfile, "%d ", &pgm->data[i][j]);
        }
    }
    fclose(pgmfile);

    return true;
}

void ignoreComments(FILE* fp)
{
    int ch;
    char line[100];

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        ignoreComments(fp);
    }
    else
        fseek(fp, -1, SEEK_CUR);
}
