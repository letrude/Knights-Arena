#include "menu.h"

void menuPrincipal(void)
{
    int choix = 0, tlog = 0, num, pts;
    char nom[32], nomRecherche[32];
    char nomPartie[] = "fichiers/parties/partie?.txt";
    Chevalier chevalier, **tabChevalier, chevalierRecherche;

    printf("Bienvenue sur le jeu !\n\n");
    print_rouge();
    printf("Pour une meilleure expérience veuillez jouer en plein écran !\n\n");
    print_reset();

    while (1)
    {
        printf("------------- Menu -------------\n");
        printf("1. jouer une partie prédéfinie\n");
        printf("2. créer une nouvelle partie\n");
        printf("3. afficher la liste des joueurs triée par nom\n");
        printf("4. afficher la liste des joueurs triée par meilleur score\n");
        printf("5. afficher les statistiques d'un joueur\n");
        printf("9. Quitter\n");
        printf("--------------------------------\n");
        printf("choix: ");
        fflush(stdout);
        while (scanf(" %d%*c", &choix) != 1)
        {
            fprintf(stderr, "Choix non reconnu !\n");
            printf("choix: ");
            while (getchar() != '\n')
                ;
        }

        switch (choix)
        {
        case 1:
            printf("Numéro du fichier correspondant à la partie: ");
            fflush(stdout);
            while (scanf("%d%*c", &num) != 1)
            {
                fprintf(stderr, "Choix non reconnu !\n");
                printf("Numéro du fichier correspondant à la partie: ");
                while (getchar() != '\n')
                    ;
            }
            printf("Pseudo du joueur: ");
            printf("Quel est votre nom de chevalier ?\nSeul les 30 premiers caractères seront pris en compte !\n> ");
            fflush(stdout);

            fgets(nom, 32, stdin);
            nom[strlen(nom) - 1] = '\0';
            chevalier = chevalier_recuperation(nom);

            afficher_chevalier(chevalier);
            printf("\nBienvenue Sir %s.\n", chevalier.nom);

            chevalier.nb_parties++;

            sprintf(&nomPartie[strchr(nomPartie, '?') - nomPartie], "%d", num);
            strcat(nomPartie, ".txt");           // get nomPartie
            pts = partie(nomPartie, &chevalier); // jeu d'une partie

            if (pts < 0)
            {
                break;
            }
            
            if ((chevalier.tab_scores = (int *)realloc(chevalier.tab_scores, sizeof(int) * chevalier.nb_parties)) == NULL)
            {
                perror("Erreur ");
                exit(1);
            }

            chevalier.tab_scores[chevalier.nb_parties - 1] = pts;
            afficher_chevalier(chevalier);
            sauvegarder_chevalier(&chevalier);
            while (getchar() != '\n')
                ;
            strcpy(nomPartie,"fichiers/parties/partie?.txt");
            break;
        case 2:
            creerPartie();
            break;
        case 3:
            printf("Affichage de la liste des joueurs triée par nom\n");
            tabChevalier = charger_tab_chevalier(&tlog);
            tri_chevalier_rapide_nom(tabChevalier, tlog);
            affiche_tab_chevalier(tabChevalier, tlog);
            liberer_tab_chevalier(tabChevalier, tlog);
            break;
        case 4:
            printf("Affichage de la liste des joueurs triée par meilleur score\n");
            tabChevalier = charger_tab_chevalier(&tlog);
            tri_chevalier_fusion_score(tabChevalier, 0, tlog - 1, tlog);
            affiche_tab_chevalier(tabChevalier, tlog);
            choix = 0;
            liberer_tab_chevalier(tabChevalier, tlog);
            break;
        case 5:
            printf("Entrez le nom du chevalier que vous recherchez !\n>");
            fflush(stdout);
            fgets(nomRecherche, 32, stdin);
            nomRecherche[strlen(nomRecherche) - 1] = '\0';

            if (chevalier_existe(nomRecherche) == 0)
            {
                fprintf(stderr, "Ce chevalier n'existe pas !\n");
                break;
            }

            chevalierRecherche = chevalier_recuperation(nomRecherche);
            afficher_chevalier(chevalierRecherche);
            liberer_chevalier(&chevalierRecherche);
            break;
        case 9:
            printf("Au revoir et à bientôt !\n");
            return;
        default:
            fprintf(stderr, "Choix non reconnu !\n");
        }
    }
}

int partie(char *partie, Chevalier *chevalier)
{
    int tlogContexte, pts = 0;
    Pile p = creerPileVide();

    if (!chargerPartie(partie, &p, &tlogContexte))
    {
        fprintf(stderr, "Aucun fichier trouvé, le numéro entré est trop grand !\n");
        return -1;
    }

    inverserPile(&p);

    while (p != NULL && chevalier->pv > 0)
    {
        print_bleu();
        printf("Contexte: %s\n", p->contexte);
        print_reset();
        atkGrp1(p->grp1, chevalier, &pts);
        atkGrp2(p->grp2, chevalier, &pts);

        depiler(&p);
    }

    if (chevalier->pv <= 0)
    {
        chevalier->pv = 20 + 1 * (chevalier->niveau / 5);
        printf("%s est mort. Les monstres ont gagné.\n", chevalier->nom);
    }

    else
        printf("Tous les monstres ont été vaincus. %s a triomphé !\n", chevalier->nom);

    return pts;
}

void afficherEtatCombat(File *groupe, Chevalier *chevalier)
{
    print_bleu();
    printf("Le monstre %s(%dpv, %datk) accoure et se prépare à t'attaquer %s(%dpv, 1atk)\n",
           groupe->tete->monstre.nom, groupe->tete->monstre.pv, groupe->tete->monstre.pts_dgts,
           chevalier->nom, chevalier->pv);
    print_reset();
    fflush(stdout);
}

void afficherEtatCombat2(Monstre *monstre, Chevalier *chevalier)
{
    print_bleu();
    printf("Le monstre %s(%dpv, %datk) accoure et se prépare à t'attaquer %s(%dpv, 1atk)\n",
           monstre->nom, monstre->pv, monstre->pts_dgts,
           chevalier->nom, chevalier->pv);
    print_reset();
    fflush(stdout);
}

char choixArme(Chevalier *chevalier, int pts)
{
    char choix;
    Booleen choixValide = FALSE;

    do
    {
        printf("%s (%dpts) choisis ton arme parmi P, F, C: ", chevalier->nom, pts);
        fflush(stdout);
        scanf(" %c", &choix);

        choix = toupper(choix);

        if (choix == 'P' || choix == 'F' || choix == 'C')
        {
            choixValide = TRUE;
        }

        else
        {
            fprintf(stderr,"Choix invalide. Veuillez saisir P, F ou C.\n");
        }

    } while (!choixValide);

    return choix;
}

void egalite(Chevalier *chevalier, Monstre monstre)
{
    print_orange();
    printf(" Aucun de %s(%dpv) et %s(%dpv) ne gagne l'attaque\n\n", chevalier->nom, chevalier->pv, monstre.nom, monstre.pv);
    print_reset();
    fflush(stdout);
}

int victoire(Chevalier *chevalier, File *groupe, int multi)
{
    int pts = 0;
    Monstre *monstre = &groupe->tete->monstre;
    print_vert();
    monstre->pv -= 1;
    pts = 10;
    printf(" %s(%dpv) gagne l'attaque contre %s(%dpv)\t+10pts\n", chevalier->nom, chevalier->pv, monstre->nom, monstre->pv);
    ajouter_degats(chevalier, 1);
    ajouter_xp(chevalier, 5);
    test_gain_de_niveau(chevalier);

    if (monstre->pv <= 0)
    {
        pts += multi * monstre->niveau;
        printf("%s meurt sous le coup de l'attaque\t+%dpts\n\n", monstre->nom, multi * monstre->niveau);
        defiler(groupe);
        ajouter_xp(chevalier, 5);
        test_gain_de_niveau(chevalier);
    }

    print_reset();
    fflush(stdout);
    return pts;
}

void defaite(Chevalier *chevalier, Monstre monstre)
{
    print_rouge();
    baisser_pv(chevalier, monstre.pts_dgts);
    printf(" %s(%dpv) perd l'attaque contre %s(%dpv)\n\n", chevalier->nom, chevalier->pv, monstre.nom, monstre.pv);
    print_reset();
    fflush(stdout);
}

Booleen estVictoire(char choix, char atkMonstre)
{
    if ((choix == 'P' && atkMonstre == 'C') ||
        (choix == 'F' && atkMonstre == 'P') ||
        (choix == 'C' && atkMonstre == 'F') ||
        (atkMonstre == 'O'))
    {
        return TRUE;
    }
    return FALSE;
}

void atkGrp1(File *groupe, Chevalier *chevalier, int *pts)
{
    char choix, atkMonstre;
    char tabAtk[] = {'P', 'F', 'C', 'O', '#'};
    int index;
    int tabSize = sizeof(tabAtk) / sizeof(tabAtk[0]);

    while (!fileEstVide(groupe) && chevalier->pv > 0)
    {
        afficherEtatCombat(groupe, chevalier);

        choix = choixArme(chevalier, *pts);

        if (groupe->tete->monstre.niveau == 1)
            index = rand() % (tabSize - 1);

        else if (groupe->tete->monstre.niveau == 2)
            index = rand() % (tabSize - 2);

        else
            index = rand() % tabSize;

        atkMonstre = tabAtk[index];

        printf("%s(%c) attaque %s(%c)\n", chevalier->nom, choix, groupe->tete->monstre.nom, atkMonstre);
        fflush(stdout);

        if (choix == atkMonstre)
        {
            egalite(chevalier, groupe->tete->monstre);
        }

        else if (estVictoire(choix, atkMonstre))
        {
            *pts += victoire(chevalier, groupe, PTS_GRP1);
        }

        else
        {
            defaite(chevalier, groupe->tete->monstre);
        }
    }
}

void atkGrp2(File *groupe, Chevalier *chevalier, int *pts)
{
    char choix;
    char atkMonstre;
    char tabAtk[] = {'P', 'F', 'C', 'O', '#'};
    int tabSize = sizeof(tabAtk) / sizeof(tabAtk[0]);
    int index;

    while (!fileEstVide(groupe) && chevalier->pv > 0)
    {
        Maillon *courant = groupe->tete;

        while (courant != NULL && chevalier->pv > 0)
        {
            Monstre *monstre = &courant->monstre;

            afficherEtatCombat2(monstre, chevalier);

            choix = choixArme(chevalier, *pts);
            if (groupe->tete->monstre.niveau == 1)
                index = rand() % (tabSize - 1);

            else if (groupe->tete->monstre.niveau == 2)
                index = rand() % (tabSize - 2);

            else
                index = rand() % tabSize;

            atkMonstre = tabAtk[index];

            printf("%s(%c) attaque %s(%c)\n", chevalier->nom, choix, monstre->nom, atkMonstre);
            fflush(stdout);

            if (choix == atkMonstre)
            {
                egalite(chevalier, *monstre);
            }

            else if (estVictoire(choix, atkMonstre))
            {
                *pts += victoire(chevalier, groupe, PTS_GRP2);

                if (monstre->pv <= 0)
                {
                    printf("%s a été éliminé !\n", monstre->nom);
                    courant = courant->suiv;
                    defiler(groupe);
                    continue;
                }
            }
            
            else
            {
                defaite(chevalier, *monstre);
            }

            courant = courant->suiv;
        }
    }
}
