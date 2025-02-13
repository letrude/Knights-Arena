/**
 * \file monstre.h
 * \author Gabriel COUDEL-KOUMBA
 * \date 11/12/24
 * \brief Déclaration des fonctions et structures relatives à l'utilisation des monstres
 */


#ifndef MONSTRE_H
#define MONSTRE_H

// Inclusions des en-têtes de la bibliothèque standard
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Définition des constantes
#define TAILLE_NOM 15
#define TAILLE_CONTEXTE 65

/* ====== Structures ====== */

// Définition de la structure Monstre
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Structure correspondant à un monstre
 * \param nom nom du monstre
 * \param niveau niveau du monstre(entre 1 et 3)
 * \param pv points de vie du monstre
 * \param pts_dgts points de dégâts qu'il peut infliger
 * \param nb_armes nombre d'armes qu'il peut manipuler
 */
typedef struct
{
    char nom[TAILLE_NOM];
    int niveau;
    int pv;
    int pts_dgts;
    int nb_armes;
} Monstre;

// Définition de la structure Maillon
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Structure correspondant à un maillon d'une File
 * \param monstre un monstre
 * \param suiv maillon suivant
 */
typedef struct Maillon
{
    Monstre monstre;
    struct Maillon *suiv;
} Maillon;

// Définition de la structure File
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Structure correspondant à une File
 * \param tete tete de la File
 * \param queue queue de la File
 */
typedef struct
{
    Maillon *tete;
    Maillon *queue;
} File;

//Déclaraction de la structure Pile
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Structure correspondant à un maillon d'une Pile
 * \param contexte le contexte du combat
 * \param grp1 1ère File de monstres
 * \param grp2 2e File de monstres
 * \param suiv maillon suivant de la Pile
 */
typedef struct MaillonPile
{
    char contexte[TAILLE_CONTEXTE];
    File *grp1;
    File *grp2;
    struct MaillonPile *suiv;
} MaillonPile, *Pile;

// Définition de type Booléen
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Structures correspondant au type Booléen
 * \param FALSE valant 0 
 * \param TRUE valant 1
 */
typedef enum
{
    FALSE,
    TRUE
} Booleen;

/* ====== Fonctions de bases des structures ====== */
// Déclaration des fonctions relatives à une File
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de savoir si une File est vide
 * \param f File de Monstre
 * \return 0 (FALSE) ou 1 (TRUE)
 */
Booleen fileEstVide(File *f);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction créant une File vide
 * \return une File vide
 */
File * creerFileVide(void);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction ajoutant un élément à la File
 * \param f File de Monstre
 * \param monstre Monstre que l'on souhaite ajouté
 */
void enfiler(File *f, Monstre monstre);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction supprimant l'élément en tête de la File
 * \param f File de Monstre
 */
void defiler(File *f);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de connaitre la taille d'une File
 * \param f File de Monstre
 * \return le nombre d'éléments comptés
 */
int longueurFile(File *f);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction renvoyant l'élément en tête de File
 * \param f File de Monstre
 * \return un Monstre
 */
Monstre *enTeteFile(File f);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant le détail d'un Monstre
 * \param m Monstre à afficher
 */
void afficherMonstre(Monstre *m);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant une File
 * \param f File de Monstre
 */
void afficherFile(File *f);

//Déclaration des fonctions relatives à une Pile
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de savoir si une Pile est vide
 * \param p Pile de structures
 * \return 0 (FALSE) ou 1 (TRUE)
 */
Booleen PileEstVide(Pile *p);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Focntion créant une Pile vide
 * \return une Pile vide
 */
Pile creerPileVide(void);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction ajoutant un élément à la Pile
 * \param p Pile de Files
 * \param contexte le contexte du combat
 */
void empiler(Pile *p, char *contexte);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction retirant un élément à la Pile
 * \param p Pile contenant des Files
 */
void depiler(Pile *p);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de connaitre la taille d'une Pile
 * \param p Pile contenant Files
 * \return le nombre d'éléments dans la Pile
 */
int longueurPile(Pile p);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant d'afficher le contenu d'une Pile
 * \param p Pile contenant des Files
 */
void afficherPile(Pile p);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction inversant l'ordre des éléments dans une Pile
 * \param p Pile contenant de Files
 */
void inverserPile(Pile *p);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction libérant totalement une Pile
 * \param p Pile contenant des Files
 */
void libererPile(Pile *p);
/* ====== Autres fonctions ====== */
// Déclaration des fonctions relatives à la manipulation des Monstres
/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction chargeant une partie choisie par l'utilisateur
 * \param p Pile contenant des Files
 * \param nbContexte Nombre de contexte de la partie (la taille logique de la Pile)
 * \return 1 si le chargement a marché, 0 sinon
 */
int chargerPartie(char *partie, Pile *p, int *nbContexte);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant à l'utilisateur de créer une partie
 * \param 
 */
void creerPartie(void);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de créer des groupes de monstres
 * \param groupe File de monstres
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlogNomMonstre taille logique du tableau de pointeurs
 */
void creerGroupeMonstre(File *groupe, char **tabNom, int tlogNomMonstre);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Menu de l'interface de création d'une partie
 * \return le choix fait par l'utilisateur
 */
int menuCreerPartie(void);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction sauvegardant la partie que l'on vient de créer
 * \param p Pile de File de Monstre
 * \param num numéro du fichier
 * \note Si un numéro de fichier existant est entrer, alors la sauvegarde écrasera le fichier existant
 */
void sauvegarderPartie(Pile *p, int num);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction lisant une ligne depuis un flot donné
 * \param flot Float à partir du quel on souhaite lire un monstre
 * \return un monstre lu
 */
Monstre lireMonstre(FILE *flot);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de créer un monstre
 * \param nomMonstre le nom du monstre choisi par l'utilisateur
 * \param niveau le niveau du monstre choisi par l'utilisateur
 * \return le monstre voulu par l'utilisateur
 */
Monstre creerMonstre(char *nomMonstre, int niveau);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de créer un monstre
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 * \return un monstre généré aléatoirement
 */
Monstre creerMonstreAleatoire(char **tabNom, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant la liste complète des noms de monstres disponibles
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void afficherNomMonstre(char **tabNom, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction ajoutant un nom de monstre à la liste
 * \param nomMonstre nom de monstre à ajouter dans la liste de nom disponibles
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void ajouterNomMonstre(char ***tabNom, char *nomMonstre, int *tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction choisissant aléatoirement un nom dans le fichier des noms disponibles
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 * \return le nom choisi
*/
char *genererNomMonstre(char **tabNom, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction chargeant les noms de monstres dans le fichier nom.txt
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void chargementNomMonstre(char ***tabNom, int *tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction sauvegardant les noms de monstres disponibles
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void sauvegarderNomMonstre(char **tabNom, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction libérant la mémoire alloué au tableau de pointeur
 * \param tabNom tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void libererNomMonstre(char **tabNom, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction chargeant les contextes
 * \param tlog taille logique
 * \return un tableau de pointeurs contenant les contextes
 */
char **chargementTabContexte(int *tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant les contextes disponibles
 * \param tabContexte tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void afficherContexte(char **tabContexte, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction libérant la mémoire alloué au tableau de pointeur
 * \param tabContexte tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 */
void libererContexte(char **tabContexte, int tlog);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction vérifiant si la chaîne est vide ou contient uniquement des espaces
 * \param str chaine à vérifier
 * \return 0 si la chaîne est composé d'autre chose qu'un espace ou 1 si la chaîne est vide ou composée uniquement d'espaces
 */
int estVide(char *str);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction recherchant si la chaîne de caractères est présente
 * \param str chaîne de caractères que l'on recherche
 * \param tabContexte tableau de pointeurs sur chaines de caractères
 * \param tlog taille logique
 * \return 1 si la chaîne est présente ou 0 sinon
 */
int estPresent(char *str, char **tabNom, int tlog);
#endif