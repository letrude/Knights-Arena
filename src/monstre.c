#include "monstre.h"

/*---------------------- FILE ----------------------*/
Booleen fileEstVide(File *f)
{
    if (f->tete == NULL && f->queue == NULL)
        return TRUE;

    return FALSE;
}

File *creerFileVide(void)
{
    File *file;

    if ((file = (File *)malloc(sizeof(File))) == NULL)
    {
        perror("Erreur d'allocation dynamique pour la File");
        exit(1);
    }

    file->tete = NULL;
    file->queue = NULL;

    return file;
}

void enfiler(File *f, Monstre monstre)
{
    Maillon *m;

    if ((m = (Maillon *)malloc(sizeof(Maillon))) == NULL)
    {
        perror("Erreur d'allocation dynamique pour le Maillon de la File");
        exit(1);
    }

    strcpy(m->monstre.nom, monstre.nom);
    m->monstre.niveau = monstre.niveau;
    m->monstre.pv = monstre.pv;
    m->monstre.pts_dgts = monstre.pts_dgts;
    m->monstre.nb_armes = monstre.nb_armes;
    m->suiv = NULL;

    if (fileEstVide(f))
    {
        f->tete = m;
        f->queue = m;
    }

    f->queue->suiv = m;
    f->queue = m;
}

void defiler(File *f)
{
    if (fileEstVide(f))
    {
        fprintf(stderr, "Attention, vous essayez de supprimer un élément d'une File vide\n");
        return;
    }

    Maillon *m = f->tete;
    f->tete = f->tete->suiv;
    if (f->tete == NULL)
    {
        f->queue = NULL;
    }
    free(m);
}

int longueurFile(File *f)
{
    int cpt = 0;
    Maillon *temp = f->tete;

    while (temp != NULL)
    {
        cpt++;
        temp = temp->suiv;
    }
    return cpt;
}

Monstre *enTeteFile(File f)
{
    if (fileEstVide(&f))
    {
        fprintf(stderr, "Attention, vous essayez de regarder une File vide\n");
        return NULL;
    }
    return &(f.tete->monstre);
}

void afficherMonstre(Monstre *m)
{
    printf("Nom: %s\n", m->nom);
    printf("Niveau: %d\nPoints de vie: %d\n", m->niveau, m->pv);
    printf("Points de dégats: %d\n", m->pts_dgts);
    printf("Liste des armes: %d\n", m->nb_armes);
}

void afficherFile(File *f)
{
    Maillon *courant = f->tete;
    int index = 1;

    if (fileEstVide(f))
    {
        fprintf(stderr, "La file est vide.\n");
        return;
    }

    printf("File de Monstres :\n");
    while (courant != NULL)
    {
        printf("Monstre n°%d :\n", index++);
        afficherMonstre(&courant->monstre);
        printf("-------------------------------------------\n");
        courant = courant->suiv;
    }
}

/*---------------------- PILE ----------------------*/
Booleen PileEstVide(Pile *p)
{
    if (p == NULL)
        return TRUE;
    return FALSE;
}

Pile creerPileVide(void)
{
    return NULL;
}

void empiler(Pile *p, char *contexte)
{
    MaillonPile *nouv;

    if ((nouv = (MaillonPile *)malloc(sizeof(MaillonPile))) == NULL)
    {
        perror("Erreur d'allocation dynamique sur la Pile");
        exit(1);
    }

    strncpy(nouv->contexte, contexte, 64);
    nouv->contexte[64] = '\0';
    nouv->grp1 = creerFileVide();
    nouv->grp2 = creerFileVide();
    nouv->suiv = *p;

    *p = nouv;
}

void depiler(Pile *p)
{
    MaillonPile *temp = *p;

    if (PileEstVide(p))
    {
        fprintf(stderr, "Erreur : tentative de dépiler une pile vide.\n");
        return;
    }

    *p = (*p)->suiv;
    free(temp);
}

int longueurPile(Pile p)
{
    int cpt = 0;
    MaillonPile *courant = p;

    while (courant != NULL)
    {
        cpt++;
        courant = courant->suiv;
    }

    return cpt;
}

void afficherPile(Pile p)
{
    if (p == NULL)
    {
        fprintf(stderr, "La pile est vide.\n");
        return;
    }

    printf("Contenu de la pile :\n");
    MaillonPile *courant = p;
    while (courant != NULL)
    {
        printf("- Contexte : %s\n", courant->contexte);
        afficherFile(courant->grp1);
        afficherFile(courant->grp2);
        courant = courant->suiv;
    }
}

void inverserPile(Pile *p)
{
    Pile tmp = creerPileVide();
    while (*p != NULL)
    {
        MaillonPile *maillon = *p;
        *p = (*p)->suiv;

        maillon->suiv = tmp;
        tmp = maillon;
    }

    *p = tmp;
}

void libererPile(Pile *p)
{
    while (!PileEstVide(p))
    {
        depiler(p);
    }
}

/*---------------------- MANIPULATION DES STRUCTURES ----------------------*/
int chargerPartie(char *partie, Pile *p, int *nbContexte)
{
    FILE *fic;
    Monstre monstre;
    char contexte[70];
    int cpt;

    if ((fic = fopen(partie, "r")) == NULL)
    {
        return 0;
    }

    while (fgets(contexte, sizeof(contexte), fic))
    {
        contexte[strcspn(contexte, "\n")] = '\0';

        if (strlen(contexte) == 0)
            continue;
        empiler(p, contexte);

        if (fscanf(fic, "%d", &cpt) != 1)
        {
            fprintf(stderr, "Erreur de lecture du nombre de monstres pour le groupe 1 dans le contexte : %s\n", contexte);
            fclose(fic);
            exit(1);
        }
        while (fgetc(fic) != '\n' && !feof(fic))
            ; // évite d'avoir un caractère qui fait bug le chargement entre 2 lectures

        for (int i = 0; i < cpt; i++)
        {
            monstre = lireMonstre(fic);
            enfiler((*p)->grp1, monstre);
        }

        if (fscanf(fic, "%d", &cpt) != 1)
        {
            fprintf(stderr, "Erreur de lecture du nombre de monstres pour le groupe 2 dans le contexte : %s\n", contexte);
            fclose(fic);
            exit(1);
        }
        while (fgetc(fic) != '\n' && !feof(fic))
            ; // évite d'avoir un caractère qui fait bug le chargement entre 2 lectures

        for (int i = 0; i < cpt; i++)
        {
            monstre = lireMonstre(fic);
            enfiler((*p)->grp2, monstre);
        }
    }

    fclose(fic);
    return 1;
}

void creerPartie(void)
{
    int tlogContexte = 0, tlogNomMonstre = 0;
    int choix, choixContexte, num;
    char **tabNom = NULL;
    char **tabContexte = chargementTabContexte(&tlogContexte);
    char *contexte;
    Pile p;

    printf(" ________________________________________________________________________________________________________________________________\n");
    printf("|Bienvenue dans l'interface de création d'une partie                                                                             |\n");
    printf("|                                                                                                                                |\n");
    printf("|Quelques rappels importants:                                                                                                    |\n");
    printf("|\t- Un niveau correspond à 1 contexte (lieu de combat) et 2 groupes de monstres.                                           |\n");
    printf("|\t- Une partie n'a pas de limite fixe de niveaux.                                                                          |\n");
    printf("|\t- Un groupe de monstres contient au minimum 3 monstres.                                                                  |\n");
    printf("|\t- Il est possible de créer soit même les monstres, il faudra choisir un nom de la liste proposée et un niveau (de 1 à 3).|\n");
    printf("|\t- Il est aussi possible de générer des monstres aléatoirement (plus rapide pour créer une partie).                       |\n");
    printf("|\t- Selon l'ordre dont vous voulez que les monstres apparaissent il faut ou non inverser la pile (I).                      |\n");
    printf("|________________________________________________________________________________________________________________________________|\n");

    p = creerPileVide();

    while ((choix = menuCreerPartie()) != 9)
    {
        switch (choix)
        {
        case 1: // Ajouter un niveau
            chargementNomMonstre(&tabNom, &tlogNomMonstre);
            if (tlogContexte > 0)
            {
                printf("\nVeuillez choisir un contexte parmi la liste suivante:\n");
                afficherContexte(tabContexte, tlogContexte);

                do
                {
                    printf("N° du contexte: ");
                    fflush(stdout);

                    if (scanf("%d", &choixContexte) != 1 || choixContexte < 1 || choixContexte > 70)
                    {
                        fprintf(stderr, "Entrée invalide. Veuillez saisir un numéro valide.\n");
                        while (getchar() != '\n')
                            ; // Vider le tampon
                    }
                    else
                    {
                        while (getchar() != '\n')
                            ; // Vider le tampon après une entrée valide
                        break;
                    }
                } while (1);

                if ((contexte = malloc(TAILLE_CONTEXTE * sizeof(char))) == NULL)
                {
                    perror("Erreur d'allocation de mémoire");
                    exit(1);
                }
                strcpy(contexte, tabContexte[choixContexte - 1]);
                printf("Contexte %d sélectionné: %s\n", choixContexte, contexte);
                empiler(&p, contexte);

                printf("\nCréation du 1er groupe de monstre du niveau\n");
                creerGroupeMonstre(p->grp1, tabNom, tlogNomMonstre);

                printf("\nCréation du 2e groupe de monstre du niveau\n");
                creerGroupeMonstre(p->grp2, tabNom, tlogNomMonstre);
            }
            else
            {
                fprintf(stderr, "Aucun contexte disponible.\n");
            }
            break;

        case 2: // Afficher la partie
            afficherPile(p);
            break;

        case 3:
            inverserPile(&p);
            break;

        default:
            fprintf(stderr, "Choix non reconnu.\n");
        }
    }
    if (p != NULL) // Si la pile n'est pas vide
    {
        printf("Sélectionner un N° de partie (si vous entrez un N° existant, cela écrasera la partie existante): ");
        fflush(stdout);
        if (scanf("%d%*c", &num) == 1) // Lecture du numéro de partie
        {
            sauvegarderPartie(&p, num);
            printf("Fin de la création de partie. Sauvegarde...\n");
            printf("Sauvegarde effectuée.\n");
        }
        else
        {
            fprintf(stderr, "Entrée invalide. Sauvegarde annulée.\n");
        }
    }
    else // Si la pile est vide
    {
        printf("La pile est vide. Fin de la création de partie sans sauvegarde.\n");
    }

    libererNomMonstre(tabNom, tlogNomMonstre);
    libererContexte(tabContexte, tlogContexte);
    free(p);
}

void creerGroupeMonstre(File *groupe, char **tabNom, int tlogNomMonstre)
{
    int choixGen = 1, choixNbMonstre, quitterGrp = 0, cpt = 0;
    Monstre m;
    char *nom;
    int choixNomMonstre, niveau;

    printf("Souhaitez-vous générer les monstres automatiquement (1) ou manuellement (0) ? ");
    fflush(stdout); // Forcer l'affichage
    scanf("%d%*c", &choixGen);

    if (choixGen != 1)
    {
        while (!quitterGrp)
        {
            printf("Veuillez choisir un nom de monstre parmi la liste suivante:\n");
            fflush(stdout); // Forcer l'affichage
            afficherNomMonstre(tabNom, tlogNomMonstre);

            do
            {
                printf("N° du monstre: ");
                fflush(stdout); // Forcer l'affichage

                if (scanf("%d", &choixNomMonstre) != 1 || choixNomMonstre < 1 || choixNomMonstre > tlogNomMonstre)
                {
                    fprintf(stderr, "Entrée invalide. Veuillez saisir un numéro valide.\n");
                    fflush(stdout); // Forcer l'affichage
                    while (getchar() != '\n')
                        ;
                }
                else
                {
                    break;
                }
            } while (1);

            nom = malloc(TAILLE_NOM * sizeof(char));
            if (!nom)
            {
                perror("Erreur d'allocation mémoire");
                exit(1);
            }
            strcpy(nom, tabNom[choixNomMonstre - 1]);

            printf("Sélectionnez un niveau (1-3) : ");
            fflush(stdout); // Forcer l'affichage
            scanf("%d%*c", &niveau);
            if (niveau < 1 || niveau > 3)
            {
                fprintf(stderr, "Niveau invalide, génération aléatoire.\n");
                fflush(stdout); // Forcer l'affichage
                niveau = (rand() % 3) + 1;
            }

            m = creerMonstre(nom, niveau);
            enfiler(groupe, m);
            cpt++;

            printf("Quitter le groupe ? (1 = Oui, 0 = Non) : ");
            fflush(stdout); // Forcer l'affichage
            scanf("%d%*c", &quitterGrp);
        }

        while (cpt < 3)
        {
            m = creerMonstreAleatoire(tabNom, tlogNomMonstre);
            enfiler(groupe, m);
            cpt++;
        }
    }
    else
    {
        // Génération automatique
        printf("Nombre de monstres (>3) : ");
        fflush(stdout); // Forcer l'affichage
        scanf("%d%*c", &choixNbMonstre);
        if (choixNbMonstre < 3)
            choixNbMonstre = rand() % (10 - 3 + 1) + 3;

        for (int i = 0; i < choixNbMonstre; i++)
        {
            m = creerMonstreAleatoire(tabNom, tlogNomMonstre);
            enfiler(groupe, m);
        }
    }
}

int menuCreerPartie(void)
{
    char choix[10];

    while (1)
    {
        printf("\n");
        printf("\tA. Ajouter un niveau\n");
        printf("\tP. Afficher la partie\n");
        printf("\tI. Inversion de la pile\n");
        printf("\tQ. Quitter\n\n");
        printf("Votre choix: ");
        fflush(stdout); // force l'affichage

        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0';

        if (strlen(choix) == 1)
        {
            if (choix[0] == 'A' || choix[0] == 'a')
                return 1;
            else if (choix[0] == 'P' || choix[0] == 'p')
                return 2;
            else if (choix[0] == 'I' || choix[0] == 'i')
                return 3;
            else if (choix[0] == 'Q' || choix[0] == 'q')
                return 9;
            else
                fprintf(stderr, "Choix incorrect, veuillez resaisir.\n");
        }
        else
        {
            fprintf(stderr, "Choix incorrect, veuillez resaisir.\n");
        }
    }
}

void sauvegarderPartie(Pile *p, int num)
{
    FILE *fic;
    MaillonPile *courant = *p;

    char partie[] = "fichiers/parties/partie?.txt";

    // Remplace '?' avec la valeur num
    sprintf(&partie[strchr(partie, '?') - partie], "%d", num);
    strcat(partie, ".txt");

    printf("Nom de fichier généré : %s\n", partie);

    if ((fic = fopen(partie, "w")) == NULL)
    {
        perror("Erreur d'ouverture du fichier de sauvegarde");
        exit(1);
    }

    while (courant != NULL)
    {
        fprintf(fic, "%s\n", courant->contexte);

        int cpt = longueurFile(courant->grp1);
        fprintf(fic, "%d\n", cpt);
        Maillon *cour = courant->grp1->tete;

        for (int i = 0; i < cpt; i++)
        {
            fprintf(fic, "%s\n", cour->monstre.nom);
            fprintf(fic, "%d %d %d %d", cour->monstre.niveau, cour->monstre.pv, cour->monstre.pts_dgts, cour->monstre.nb_armes);
            cour = cour->suiv;
            if (i < cpt - 1)
                fprintf(fic, "\n");
        }

        cpt = longueurFile(courant->grp2);
        fprintf(fic, "\n%d\n", cpt);
        cour = courant->grp2->tete;

        for (int i = 0; i < cpt; i++)
        {
            fprintf(fic, "%s\n", cour->monstre.nom);
            fprintf(fic, "%d %d %d %d", cour->monstre.niveau, cour->monstre.pv, cour->monstre.pts_dgts, cour->monstre.nb_armes);
            cour = cour->suiv;
            if (i < cpt - 1)
                fprintf(fic, "\n");
        }

        if (courant->suiv != NULL)
            fprintf(fic, "\n");

        courant = courant->suiv;
    }

    fclose(fic);
}

/*---------------------- MANIPULATION DES MONSTRES ----------------------*/
Monstre lireMonstre(FILE *flot)
{
    Monstre monstre;

    fscanf(flot, "%s", monstre.nom);

    if (fscanf(flot, "%d %d %d %d", &monstre.niveau, &monstre.pv, &monstre.pts_dgts, &monstre.nb_armes) != 4)
    {
        fprintf(stderr, "Erreur lors de la lecture des attributs du monstre : %s\n", monstre.nom);
        monstre.niveau = 0;
        monstre.pv = 0;
        monstre.pts_dgts = 0;
        monstre.nb_armes = 0;
    }

    return monstre;
}

Monstre creerMonstre(char *nomMonstre, int niveau)
{
    Monstre monstre;

    strncpy(monstre.nom, nomMonstre, sizeof(monstre.nom) - 1);

    monstre.nom[sizeof(monstre.nom) - 1] = '\0';

    monstre.niveau = niveau;

    switch (monstre.niveau)
    {
    case 0:
        monstre.niveau = 1;
        monstre.pv = 4;
        monstre.pts_dgts = 1;
        monstre.nb_armes = 4;
        break;
    case 1:
        monstre.pv = 4;
        monstre.pts_dgts = 1;
        monstre.nb_armes = 4;
        break;
    case 2:
        monstre.pv = 6;
        monstre.pts_dgts = 1;
        monstre.nb_armes = 3;
        break;
    case 3:
        monstre.pv = 4;
        monstre.pts_dgts = 2;
        monstre.nb_armes = 5;
        break;
    }

    return monstre;
}

Monstre creerMonstreAleatoire(char **tabNom, int tlog)
{
    Monstre monstre;
    char *nom = genererNomMonstre(tabNom, tlog);

    strcpy(monstre.nom, nom);

    monstre.niveau = rand() % 4;

    switch (monstre.niveau)
    {
    case 0:
        monstre.niveau++;
    case 1:
        monstre.pv = 4;
        monstre.pts_dgts = 1;
        monstre.nb_armes = 4;
        break;
    case 2:
        monstre.pv = 6;
        monstre.pts_dgts = 1;
        monstre.nb_armes = 3;
        break;
    case 3:
        monstre.pv = 4;
        monstre.pts_dgts = 2;
        monstre.nb_armes = 5;
        break;
    default:
        fprintf(stderr, "Erreur dans la génération du monstre\n");
        printf("%d\n", monstre.niveau);
        exit(2);
    }

    return monstre;
}

void afficherNomMonstre(char **tabNom, int tlog)
{
    const int largeurColonne = 20;
    const int monstresParLigne = 3;

    for (int i = 0; i < tlog; i++)
    {
        printf("%-2d. %-*s", i + 1, largeurColonne, tabNom[i]);

        if ((i + 1) % monstresParLigne == 0 || i == tlog - 1)
        {
            printf("\n");
        }
    }
}

void ajouterNomMonstre(char ***tabNom, char *nomMonstre, int *tlog)
{
    char **temp;

    if (estPresent(nomMonstre, *tabNom, *tlog))
    {
        fprintf(stderr, "Nom de Monstre déjà présent.\n");
        return;
    }

    if ((temp = realloc(*tabNom, (*tlog + 1) * sizeof(char *))) == NULL)
    {
        perror("Erreur d'allocation de mémoire pour le tableau de noms");
        exit(1);
    }
    *tabNom = temp;

    if (((*tabNom)[*tlog] = malloc(TAILLE_NOM * sizeof(char))) == NULL)
    {
        perror("Erreur d'allocation de mémoire pour un nom");
        exit(1);
    }

    strncpy((*tabNom)[*tlog], nomMonstre, TAILLE_NOM);
    (*tabNom)[*tlog][14] = '\0';

    (*tlog)++;
}

void chargementNomMonstre(char ***tabNom, int *tlog)
{
    FILE *fic;

    if ((fic = fopen("fichiers/donnees/monstres/nom.txt", "r")) == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    fscanf(fic, "%d", tlog);

    if ((*tabNom = malloc(sizeof(char *) * (*tlog))) == NULL)
    {
        perror("Erreur d'allocation de mémoire pour les noms de monstres");
        fclose(fic);
        exit(1);
    }

    for (int i = 0; i < *tlog; i++)
    {
        if (((*tabNom)[i] = malloc(TAILLE_NOM * sizeof(char))) == NULL)
        {
            perror("Erreur d'allocation de mémoire pour un nom");
            fclose(fic);
            exit(1);
        }
        fscanf(fic, "%s", (*tabNom)[i]);
    }

    fclose(fic);
}

void sauvegarderNomMonstre(char **tabNom, int tlog)
{
    FILE *fic;

    if (tabNom == NULL || tlog <= 0)
    {
        fprintf(stderr, "Erreur : tableau de noms vide ou taille invalide.\n");
        return;
    }

    if ((fic = fopen("fichiers/donnees/monstres/nom.txt", "w")) == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    fprintf(fic, "%d\n", tlog);

    for (int i = 0; i < tlog; i++)
    {
        if (i == tlog - 1)
            fprintf(fic, "%s", tabNom[i]);
        else
            fprintf(fic, "%s\n", tabNom[i]);
    }

    fclose(fic);
}

char *genererNomMonstre(char **tabNom, int tlog)
{
    char *nom;
    if (tlog <= 0 || tabNom == NULL)
    {
        fprintf(stderr, "Tableau de noms vide ou taille logique invalide\n");
        exit(1);
    }

    int randomIndex = rand() % tlog;

    if ((nom = strdup(tabNom[randomIndex])) == NULL) // duplique le nom du tableau dans la variable locale
    {
        perror("Erreur d'allocation de mémoire pour le nom choisi");
        exit(1);
    }

    return nom;
}

void libererNomMonstre(char **tabNom, int tlog)
{
    for (int i = 0; i < tlog; i++)
    {
        free(tabNom[i]);
    }
    free(tabNom);
}

char **chargementTabContexte(int *tlog)
{
    FILE *fic;

    if ((fic = fopen("fichiers/donnees/contextes/contextes.txt", "r")) == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(1);
    }

    char **contexte = NULL;
    *tlog = 0;

    char line[256];
    while (fgets(line, sizeof(line), fic))
    {
        // Supprimer le saut de ligne
        line[strcspn(line, "\n")] = '\0';

        // Redimensionner le tableau pour accueillir une nouvelle description
        char **temp = realloc(contexte, (*tlog + 1) * sizeof(char *));
        if (!temp)
        {
            perror("Erreur de redimensionnement mémoire");
            fclose(fic);
            free(contexte);
            return NULL;
        }
        contexte = temp;

        // Copier la ligne dans le tableau
        contexte[*tlog] = strdup(line);
        if (!contexte[*tlog])
        {
            perror("Erreur de copie de ligne");
            fclose(fic);
            for (size_t i = 0; i < *tlog; i++)
            {
                free(contexte[i]);
            }
            free(contexte);
            return NULL;
        }
        (*tlog)++;
    }

    fclose(fic);
    return contexte;

    fclose(fic);
}

int estVide(char *str)
{
    while (*str)
    {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}

int estPresent(char *str, char **tabNom, int tlog)
{
    int gauche = 0;
    int droite = tlog - 1;

    while (gauche <= droite)
    {
        int milieu = gauche + (droite - gauche) / 2;
        int comparaison = strcmp(tabNom[milieu], str);

        if (comparaison == 0)
        {
            return 1;
        }
        else if (comparaison < 0)
        {
            gauche = milieu + 1;
        }
        else
        {
            droite = milieu - 1;
        }
    }

    return 0;
}

void afficherContexte(char **tabContexte, int tlog)
{
    int cpt = 1;
    for (int i = 0; i < tlog; i++)
    {
        if (estVide(tabContexte[i])) // Si la chaîne est vide ou ne contient que des espaces, on passe à la suivante
            continue;

        printf("%d. %s\n", cpt, tabContexte[i]);

        if (i == tlog - 1)
        {
            printf("\n");
        }
        cpt++;
    }
}

void libererContexte(char **tabContexte, int tlog)
{
    for (int i = 0; i < tlog; i++)
    {
        free(tabContexte[i]);
    }
    free(tabContexte);
}
