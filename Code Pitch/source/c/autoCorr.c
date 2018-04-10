//Includes
#include "fonctions.h"


//Variables globales
short echant[TAILLE_TRAME];
long resultAutoCorrC[TAILLE_TRAME];
long resultAutoCorrASM[TAILLE_TRAME];


void initAutoCorr(short entree[TAILLE_TRAME])
{
    int i = 0;

    //Initialisation des entrées de l'auto-corrélation et copie de l'échantillon en entrée
    while(i < TAILLE_TRAME)
    {
        echant[i] = entree[i];
        i++;
    }
}


void autoCorrC(void)
{
    //Initialisation des compteurs
    int i;
    int k;

    //Calcul de l'auto-corrélation
       for(k = -TAILLE_TRAME+1; k <= 0; k++)
       {
           resultAutoCorrC[k+TAILLE_TRAME-1] = 0;
           for (i = 0; i < TAILLE_TRAME-abs(k); i++)
                {
                    //if (k <= 0)
                        resultAutoCorrC[k+TAILLE_TRAME-1] = resultAutoCorrC[k+TAILLE_TRAME-1] + echant[i] * echant[i-k];
                    //else
                       // resultAutoCorrC[k+TAILLE_TRAME-1] = resultAutoCorrC[k+TAILLE_TRAME-1] + echant[i+k] * echant[i];
                }
       }
}

void planDeTest(short entree[TAILLE_TRAME])
{
        initAutoCorr(entree);
        autoCorrC();
        autoCorrASM(echant, resultAutoCorrASM);

        int i = 0;
        int j = 0;

        while (i < TAILLE_TRAME && j == 0)
        {
            if (resultAutoCorrC[i] != resultAutoCorrASM[i])
                j = 1;
            i++;
        }

        if (j == 0)
            printf("Test Réussi\n");
        else
            printf("Test Échoué\n");
}


