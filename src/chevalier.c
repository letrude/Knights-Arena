#include "chevalier.h"

Chevalier initialiser_chevalier(char *nom)
{
    Chevalier chevalier;

    strncpy(chevalier.nom, nom, sizeof(chevalier.nom) - 1);

    chevalier.nom[sizeof(chevalier.nom) - 1] = '\0';
    chevalier.niveau = 0;
    chevalier.xp = 0;
    chevalier.pv = 20;
    chevalier.dgts_infliges = 0;
    chevalier.nb_parties = 0;
    chevalier.tab_scores = NULL;

    return chevalier;
}

void afficher_chevalier(Chevalier chevalier)
{
    printf("\n---------- Chevalier ----------\n");
    printf("Nom : %s\n", chevalier.nom);
    printf("Niveau : %d\n", chevalier.niveau);
    printf("Expérience : %d\n", chevalier.xp);
    printf("Points de vie : %d\n", chevalier.pv);
    printf("Dégâts infligés : %d\n", chevalier.dgts_infliges);
    printf("Nombre de parties jouées : %d\n", chevalier.nb_parties);

    if (chevalier.nb_parties > 0)
    {
        printf("Scores : ");

        for (int i = 0; i < chevalier.nb_parties; i++)
        {
            printf("%d ", chevalier.tab_scores[i]);
        }

        printf("\n");
        printf("Meilleur score : %d\n", score_max(chevalier));
        printf("Pire score : %d\n", score_min(chevalier));
        printf("Score moyen : %.2f\n", score_moyen(chevalier));
    }

    else
    {
        printf("Aucun score enregistré.\n");
    }

    printf("-------------------------------\n");
}

void sauvegarder_chevalier(Chevalier *chevalier)
{
    FILE *f;
    int size, nb_chevaliers, tlog = 0, indice = 0;
    Chevalier **tab, chevalier_comparer;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "ab")) == NULL)
    {
        perror("Erreur ");
        liberer_chevalier(chevalier);
        exit(1);
    }

    size = ftell(f);

    if (size == 0)
    {
        tlog++;

        fwrite(&tlog, sizeof(int), 1, f);

        fwrite(chevalier, sizeof(Chevalier) - sizeof(int *), 1, f);
        fwrite(chevalier->tab_scores, sizeof(int), chevalier->nb_parties, f);

        fclose(f);

        liberer_chevalier(chevalier);

        return;
    }

    if (chevalier_existe(chevalier->nom) == 0)
    {
        fwrite(chevalier, sizeof(Chevalier) - sizeof(int *), 1, f);
        fwrite(chevalier->tab_scores, sizeof(int), chevalier->nb_parties, f);

        fclose(f);

        nb_chevaliers = nombre_de_chevaliers();
        nb_chevaliers++;

        if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "r+b")) == NULL)
        {
            perror("Erreur ");
            liberer_chevalier(chevalier);
            exit(1);
        }

        fseek(f, 0, SEEK_SET);
        fwrite(&nb_chevaliers, sizeof(int), 1, f);

        fclose(f);

        liberer_chevalier(chevalier);

        return;
    }

    fclose(f);

    chevalier_comparer = chevalier_recuperation(chevalier->nom);

    if (chevalier->nb_parties == chevalier_comparer.nb_parties)
    {
        liberer_chevalier(chevalier);
        liberer_chevalier(&chevalier_comparer);

        return;
    }

    tab = charger_tab_chevalier(&tlog);

    for (; strcmp(chevalier->nom, tab[indice]->nom) != 0; indice++)
    {
    }

    liberer_chevalier(tab[indice]);

    for (; indice < tlog - 1; indice++)
    {
        tab[indice] = tab[indice + 1];
    }

    if ((tab[tlog - 1] = (Chevalier *)malloc(sizeof(Chevalier))) == NULL)
    {
        perror("Erreur ");
        liberer_chevalier(chevalier);
        exit(1);
    }

    tab[tlog - 1] = chevalier;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "wb")) == NULL)
    {
        perror("Erreur ");
        liberer_chevalier(chevalier);
        exit(1);
    }

    fwrite(&tlog, sizeof(int), 1, f);

    for (int i = 0; i < tlog; i++)
    {
        fwrite(tab[i], sizeof(Chevalier) - sizeof(int *), 1, f);
        fwrite(tab[i]->tab_scores, sizeof(int), tab[i]->nb_parties, f);
    }

    fclose(f);

    liberer_chevalier(chevalier);
}

Chevalier chevalier_recuperation(char *nom)
{
    FILE *f;
    Chevalier chevalier;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "rb")) == NULL)
    {
        perror("Erreur ");
        exit(1);
    }

    fseek(f, sizeof(int), SEEK_CUR);

    while (fread(&chevalier, sizeof(Chevalier) - sizeof(int *), 1, f))
    {
        if (strcmp(chevalier.nom, nom) == 0)
        {
            if (chevalier.nb_parties == 0)
            {
                chevalier.tab_scores = NULL;
            }
            else if ((chevalier.tab_scores = (int *)malloc(sizeof(int) * chevalier.nb_parties)) == NULL)
            {
                perror("Erreur ");
                fclose(f);
                exit(1);
            }

            fread(chevalier.tab_scores, sizeof(int), chevalier.nb_parties, f);
            fclose(f);

            return chevalier;
        }

        fseek(f, sizeof(int) * chevalier.nb_parties, SEEK_CUR);
    }

    fclose(f);
    chevalier = initialiser_chevalier(nom);

    return chevalier;
}

void liberer_chevalier(Chevalier *chevalier)
{
    if (chevalier->nb_parties==0) {
        return;
    }
    free(chevalier->tab_scores);
    chevalier->tab_scores = NULL;
}

int chevalier_existe(char *nom)
{
    FILE *f;
    Chevalier chevalier;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "rb")) == NULL)
    {
        perror("Erreur ");
        exit(1);
    }

    fseek(f, sizeof(int), SEEK_CUR);

    while (fread(&chevalier, sizeof(Chevalier) - sizeof(int *), 1, f))
    {
        if (strcmp(chevalier.nom, nom) == 0)
        {
            fclose(f);
            return 1;
        }

        fseek(f, sizeof(int) * chevalier.nb_parties, SEEK_CUR);
    }

    fclose(f);

    return 0;
}

int nombre_de_chevaliers()
{
    FILE *f;
    int res;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "rb")) == NULL)
    {
        perror("Erreur ");
        exit(1);
    }

    fread(&res, 1, sizeof(int), f);

    return res;
}

Chevalier **charger_tab_chevalier(int *tlog)
{
    FILE *f;
    Chevalier **tab;

    if ((f = fopen("fichiers/donnees/chevalier/chevalier.don", "rb")) == NULL)
    {
        perror("Erreur ");
        exit(1);
    }

    fread(tlog, sizeof(int), 1, f);

    if ((tab = (Chevalier **)malloc(sizeof(Chevalier *) * (*tlog))) == NULL)
    {
        perror("Erreur ");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < *tlog; i++)
    {
        if ((tab[i] = (Chevalier *)malloc(sizeof(Chevalier))) == NULL)
        {
            perror("Erreur ");
            fclose(f);
            exit(1);
        }
        fread(tab[i], sizeof(Chevalier) - sizeof(int *), 1, f);

        if ((tab[i]->tab_scores = (int *)malloc(sizeof(int) * tab[i]->nb_parties)) == NULL)
        {
            perror("Erreur ");
            fclose(f);
            exit(1);
        }

        fread(tab[i]->tab_scores, sizeof(int), tab[i]->nb_parties, f);
    }

    fclose(f);

    return tab;
}

void liberer_tab_chevalier(Chevalier **tab, int tlog)
{
    for (int i = 0; i < tlog; i++)
    {
        liberer_chevalier(tab[i]);
        free(tab[i]);
        tab[i] = NULL;
    }
    free(tab);
}

void affiche_tab_chevalier(Chevalier **tab, int tlog)
{
    for (int i = 0; i < tlog; i++)
    {
        afficher_chevalier(*tab[i]);
    }
}

float score_moyen(Chevalier chevalier)
{
    float moyenne = 0;

    for (int i = 0; i < chevalier.nb_parties; i++)
    {
        moyenne += chevalier.tab_scores[i];
    }

    moyenne = moyenne / chevalier.nb_parties;
    return moyenne;
}

int score_min(Chevalier chevalier)
{
    int min;

    if (chevalier.nb_parties == 0)
    {
        return 0;
    }

    min = chevalier.tab_scores[0];

    for (int i = 0; i < chevalier.nb_parties; i++)
    {
        if (chevalier.tab_scores[i] < min)
        {
            min = chevalier.tab_scores[i];
        }
    }

    return min;
}

int score_max(Chevalier chevalier)
{
    int max;

    if (chevalier.nb_parties == 0)
    {
        return 0;
    }

    max = chevalier.tab_scores[0];

    for (int i = 0; i < chevalier.nb_parties; i++)
    {
        if (chevalier.tab_scores[i] > max)
        {
            max = chevalier.tab_scores[i];
        }
    }

    return max;
}

void ajouter_degats(Chevalier *chevalier, int degats)
{
    chevalier->dgts_infliges += degats;
}

void baisser_pv(Chevalier *chevalier, int degats)
{
    chevalier->pv -= degats;
}

void tri_chevalier_rapide_nom(Chevalier **tab, int tlog)
{
    int petit = 0, grand = tlog - 2;
    Chevalier *temp, *pivot;

    if (tlog <= 1)
    {
        return;
    }

    pivot = tab[tlog - 1];

    while (petit <= grand)
    {
        while (petit < tlog - 1 && strcmp(tab[petit]->nom, pivot->nom) <= 0)
        {
            petit++;
        }

        while (grand >= 0 && strcmp(tab[grand]->nom, pivot->nom) >= 0)
        {
            grand--;
        }

        if (petit < grand)
        {
            temp = tab[petit];
            tab[petit] = tab[grand];
            tab[grand] = temp;
        }
    }

    tab[tlog - 1] = tab[petit];
    tab[petit] = pivot;

    tri_chevalier_rapide_nom(tab, petit);
    tri_chevalier_rapide_nom(tab + petit + 1, tlog - petit - 1);
}

void tri_chevalier_fusion_score(Chevalier **tab, int deb, int fin, int tlog)
{
    int milieu = deb + (fin - deb) / 2;
    if (deb < fin)
    {
        tri_chevalier_fusion_score(tab, deb, milieu, tlog);
        tri_chevalier_fusion_score(tab, milieu + 1, fin, tlog);
        fusion(tab, deb, milieu, fin, tlog);
    }
}

void fusion(Chevalier **tab, int deb, int milieu, int fin, int tlog)
{
    int taille_gauche = milieu - deb + 1, taille_droite = fin - milieu;
    int i = 0, j = 0, k = deb;
    Chevalier **gauche, **droite;

    if ((gauche = (Chevalier **)malloc(taille_gauche * sizeof(Chevalier *))) == NULL)
    {
        perror("Erreur ");
        liberer_tab_chevalier(tab, tlog);
        exit(1);
    }

    if ((droite = (Chevalier **)malloc(taille_droite * sizeof(Chevalier *))) == NULL)
    {
        perror("Erreur ");
        liberer_tab_chevalier(tab, tlog);
        free(gauche);
        exit(1);
    }

    for (; i < taille_gauche; i++)
    {
        gauche[i] = tab[deb + i];
    }

    for (i = 0; i < taille_droite; i++)
    {
        droite[i] = tab[milieu + 1 + i];
    }

    i = 0;

    while (i < taille_gauche && j < taille_droite)
    {
        if (score_max(*gauche[i]) >= score_max(*droite[j]))
        {
            tab[k] = gauche[i];
            i++;
        }

        else
        {
            tab[k] = droite[j];
            j++;
        }
        k++;
    }

    while (i < taille_gauche)
    {
        tab[k] = gauche[i];
        i++;
        k++;
    }

    while (j < taille_droite)
    {
        tab[k] = droite[j];
        j++;
        k++;
    }

    free(gauche);
    free(droite);
}

void monte_de_niveau(Chevalier *chevalier)
{
    chevalier->niveau++;
    chevalier->xp = 0;
    printf("Vous êtes passé au niveau %d\n\n", chevalier->niveau);
}

void ajouter_xp(Chevalier *chevalier, int xp)
{
    chevalier->xp += xp;
    printf("Vous avez gagné %d points d'expérience !\n\n", xp);
}

int puissance(int niveau, float facteur)
{
    if (niveau == 1)
    {
        return facteur;
    }
    return facteur * puissance(niveau - 1, facteur);
}

void test_gain_de_niveau(Chevalier *chevalier)
{
    if (chevalier->xp >= 100 * (puissance(chevalier->niveau + 1, 1.5)))
    {
        monte_de_niveau(chevalier);
        return;
    }
}