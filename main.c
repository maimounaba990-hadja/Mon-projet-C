#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_R 10
#define MAX_D 10
#define MAX_C 10
#define MAX_CV 10
#define MAX_BO 10
#define MAX_CAND 10

int estNombre(char saisie[]) {
    for(int i=0; saisie[i]!='\0'; i++)
        if(!isdigit(saisie[i])) return 0;
    return 1;
}

int lireEntier(char message[]) {
    char saisie[20];
    int val;
    while(1) {
        printf("%s", message);
        scanf("%s", saisie);
        if(estNombre(saisie)) {
            val = atoi(saisie);
            if(val >= 0) return val;
            else printf("Erreur : nombre negatif !\n");
        } else printf("Erreur : chiffres uniquement !\n");
    }
}

void trierCandidatsCroissant(int voix[], char noms[][50], int nc) {
    int tmp;
    char tmpNom[50];
    for(int i=0;i<nc-1;i++)
        for(int j=i+1;j<nc;j++)
            if(voix[i] > voix[j]) {
                tmp = voix[i];
                voix[i] = voix[j];
                voix[j] = tmp;

                strcpy(tmpNom, noms[i]);
                strcpy(noms[i], noms[j]);
                strcpy(noms[j], tmpNom);
            }
}

double calculerTPN(int nv, int ne) {
    return (ne > 0) ? (nv * 100.0) / ne : 0;
}

void top3Zones(int freq[], char noms[][50], int n) {
    int idx[MAX_R];
    for(int i=0;i<n;i++) idx[i]=i;

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(freq[idx[i]] < freq[idx[j]]) {
                int t=idx[i]; idx[i]=idx[j]; idx[j]=t;
            }

    printf("\nTop 3 zones (bulletins nuls) :\n");
    for(int i=0;i<3 && i<n;i++)
        printf("%s : %d\n", noms[idx[i]], freq[idx[i]]);
}

int main() {

    int r,d,c,cv,bo,nc,ne;
    int i,j,k,l,m,p;
    int bn,sve,nv,voix;

    int totalNV_National=0, totalBN_National=0;

    int VoixBO[MAX_CAND], VoixCommune[MAX_CAND];
    int VoixDept[MAX_CAND], TotalVoix[MAX_CAND];
    int TotalBN_Region[MAX_R];

    char nomRegion[MAX_R][50];
    char nomDept[MAX_D][50];
    char nomCommune[MAX_C][50];
    char nomCandidat[MAX_CAND][50];

    system("cls");
    r = lireEntier("Nombre de regions : ");
    nc = lireEntier("Nombre de candidats : ");
    ne = lireEntier("Nombre d'electeurs inscrits : ");

    for(p=0;p<nc;p++) {
        printf("Nom du candidat %d : ", p+1);
        scanf("%s", nomCandidat[p]);
        TotalVoix[p] = 0;
    }

    for(i=0;i<r;i++) {
        system("cls");
        TotalBN_Region[i]=0;

        printf("=== REGION %d ===\n", i+1);
        printf("Nom de la region : ");
        scanf("%s", nomRegion[i]);

        d = lireEntier("Nombre de departements : ");
        for(p=0;p<nc;p++) VoixDept[p]=0;

        for(j=0;j<d;j++) {
            system("cls");
            printf("Region : %s\n", nomRegion[i]);
            printf("Nom du departement : ");
            scanf("%s", nomDept[j]);

            c = lireEntier("Nombre de communes : ");
            for(p=0;p<nc;p++) VoixCommune[p]=0;

            for(k=0;k<c;k++) {
                system("cls");
                printf("Departement : %s\n", nomDept[j]);
                printf("Nom de la commune : ");
                scanf("%s", nomCommune[k]);

                cv = lireEntier("Nombre de centres : ");
                for(p=0;p<nc;p++) VoixBO[p]=0;

                for(l=0;l<cv;l++) {
                    system("cls");
                    printf("Commune : %s\n", nomCommune[k]);
                    bo = lireEntier("Nombre de bureaux : ");

                    for(m=0;m<bo;m++) {
                        printf("\n===========================\n");
                        printf("Bureau de vote %d\n", m+1);
                        printf("===========================\n");

                        while(1) {
                            bn = lireEntier("Bulletins nuls : ");
                            sve = lireEntier("Suffrages valablement exprimes : ");
                            nv = bn + sve;

                            if(nv > ne)
                                printf("ERREUR : nv (%d) > inscrits (%d)\n", nv, ne);
                            else break;
                        }

                        totalNV_National += nv;
                        totalBN_National += bn;
                        TotalBN_Region[i] += bn;

                        for(p=0;p<nc;p++) {
                            char msg[80];
                            sprintf(msg,"Voix de %s : ", nomCandidat[p]);
                            voix = lireEntier(msg);
                            VoixBO[p] += voix;
                            TotalVoix[p] += voix;
                        }
                    }
                }

                for(p=0;p<nc;p++) VoixCommune[p] += VoixBO[p];
                trierCandidatsCroissant(VoixCommune, nomCandidat, nc);

                printf("\nClassement commune %s :\n", nomCommune[k]);
                for(p=0;p<nc;p++)
                    printf("%s : %d voix\n", nomCandidat[p], VoixCommune[p]);

                system("pause");
            }

            for(p=0;p<nc;p++) VoixDept[p] += VoixCommune[p];
        }

        trierCandidatsCroissant(VoixDept, nomCandidat, nc);
        printf("\nClassement region %s :\n", nomRegion[i]);
        for(p=0;p<nc;p++)
            printf("%s : %d voix\n", nomCandidat[p], VoixDept[p]);

        system("pause");
    }

    system("cls");
    trierCandidatsCroissant(TotalVoix, nomCandidat, nc);
    printf("=== RESULTATS NATIONAUX ===\n");
    for(p=0;p<nc;p++)
        printf("%s : %d voix\n", nomCandidat[p], TotalVoix[p]);

    printf("\nTaux de participation national : %.2f %%\n",
           calculerTPN(totalNV_National, ne));

    top3Zones(TotalBN_Region, nomRegion, r);

    return 0;
}
