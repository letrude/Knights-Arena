/**
 * \file chevalier.h
 * \author Ange GRIMAUD
 * \date 11/12/2024
 * \brief Fonctions permettant de gérer les chevaliers
 */


#ifndef CHEVALIER_H
#define CHEVALIER_H

// Inclusion des en-têtes de la bibliothèque standard
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>



/**
 * \author Ange GRIMAUD
 * \brief La structure correspondant au joueur
 * \param nom Nom du chevalier
 * \param niveau niveau du chevalier
 * \param xp expérience posséder par le joueur, qui indique quand il va monter de niveau
 * \param pv points de vie du chevalier
 * \param dgts_infliges les dégats totaux infligés de toutes les parties
 * \param nb_parties le nombres de parties du chevalier, qui sert aussi de taille logique du tableau des scores
 * \param tab_scores un tableau dynamique contenant les scores de toutes les parties qu'a effectué le joueur
 */
typedef struct
{
    char nom[31];
    int niveau;
    int xp;
    int pv;
    int dgts_infliges;
    int nb_parties;
    int *tab_scores;
} Chevalier;


/**
 * \author Ange GRIMAUD
 * \brief Initialise un chevalier avec les valeurs par défaut
 * \param nom Nom du chevalier
 * \return Un chevalier initialisé
 */
Chevalier initialiser_chevalier(char *nom);


/**
 * \author Ange GRIMAUD
 * \brief Affiche les informations du chevalier
 * \param chevalier Le chevalier à afficher
 */
void afficher_chevalier(Chevalier chevalier);


/**
 * \author Ange GRIMAUD
 * \brief Sauvegarde un chevalier dans un fichier
 * \param chevalier Pointeur vers le chevalier à sauvegarder
 */
void sauvegarder_chevalier(Chevalier *chevalier);


/**
 * \author Ange GRIMAUD
 * \brief Charge un chevalier depuis un fichier
 * \param nom Nom du chevalier à charger
 * \return Le chevalier chargé ou un nouveau si inexistant
 */
Chevalier chevalier_recuperation(char *nom);


/**
 * \author Ange GRIMAUD
 * \brief Libère les ressources allouées pour un chevalier
 * \param chevalier Pointeur vers le chevalier
 */
void liberer_chevalier(Chevalier *chevalier);


/**
 * \author Ange GRIMAUD
 * \brief Recherche un chevalier depuis un fichier
 * \param nom Nom du chevalier à rechercher
 * \return 1 si le chevalier est trouvé, 0 sinon
 */
int chevalier_existe(char *nom);


/**
 * \author Ange GRIMAUD
 * \brief Donne le nombre de chevaliers présents dans le fichier
 * \return le nombre de chevaliers dans le fichier
 */
int nombre_de_chevaliers();


/**
 * \author Ange GRIMAUD
 * \brief charge le fichier dans un tableau dynamique
 * \param tlog la taille logique du tableau dynamique, qui sera aussi sa taille physique
 * \return l'adresse du tableau dynamique contenant des pointeurs sur des structures Chevalier
 */
Chevalier **charger_tab_chevalier(int *tlog);


/**
 * \author Ange GRIMAUD
 * \brief Libère les ressources allouées pour le tableau dynamique de pointeurs sur une structure Chevalier
 * \param tab tab le tableau dynamique de pointeur sur la structure Chevalier
 * \param tlog la taille logique du tableau dynamique, qui est aussi sa taille physique
 */
void liberer_tab_chevalier(Chevalier **tab, int tlog);


/**
 * \author Ange GRIMAUD
 * \brief affiche tous les chevaliers chargés dans le tableau
 * \param tab le tableau dynamique de pointeur sur la structure Chevalier
 * \param tlog la taille logique du tableau dynamique, qui est aussi sa taille physique
 */
void affiche_tab_chevalier(Chevalier **tab, int tlog);


/**
 * \author Ange GRIMAUD
 * \brief calcule le score moyen du chevalier
 * \param chevalier un chevalier donné
 * \return le score moyen du chevalier
 */
float score_moyen(Chevalier chevalier);


/**
 * \author Ange GRIMAUD
 * \brief calcule le score minimum du chevalier
 * \param chevalier un chevalier donné
 * \return le score minimum du chevalier
 */
int score_min(Chevalier chevalier);


/**
 * \author Ange GRIMAUD
 * \brief calcule le score maximum du chevalier
 * \param chevalier un chevalier donné
 * \return le score maximum du chevalier
 */
int score_max(Chevalier chevalier);


/**
 * \author Ange GRIMAUD
 * \brief augmente le nombre de dégat total du chevalier
 * \param chevalier un chevalier donné
 * \param degats un nombre de dégats donné
 */
void ajouter_degats(Chevalier *chevalier, int degats);


/**
 * \author Ange GRIMAUD
 * \brief baisse le nombre de pv du chevalier
 * \param chevalier un chevalier donné
 * \param degats un nombre de dégats donné
 */
void baisser_pv(Chevalier *chevalier, int degats);


/**
 * \author Ange GRIMAUD
 * \brief tri rapide sur le tableau dynamique dans l'ordre lexicographique croissant, avec d'abord les majuscules et après les minuscules
 * \param tab le tableau dynamique de pointeur sur la structure Chevalier
 * \param tlog la taille logique du tableau dynamique, qui est aussi sa taille physique
 */
void tri_chevalier_rapide_nom(Chevalier **tab, int tlog);


/**
 * \author Ange GRIMAUD
 * \brief tri fusion sur le tableau dynamique dans l'ordre croissant du meilleur score, partitionne le tableau
 * \param tab le tableau dynamique de pointeur sur la structure Chevalier
 * \param deb indice de début, par défaut mettre 0
 * \param fin indice de fin, par défaut mettre tlog-1
 * \param tlog la taille logique du tableau dynamique, qui est aussi sa taille physique
 */
void tri_chevalier_fusion_score(Chevalier **tab, int deb, int fin, int tlog);


/**
 * \author Ange GRIMAUD
 * \brief tri fusion sur le tableau dynamique dans l'ordre croissant du meilleur score, fusionne le tableau
 * \param tab le tableau dynamique de pointeur sur la structure Chevalier
 * \param deb indice de début
 * \param milieu indice du milieu
 * \param fin indice de fin
 * \param tlog la taille logique du tableau dynamique, qui est aussi sa taille physique
 */
void fusion(Chevalier **tab, int deb, int milieu, int fin, int tlog);


/**
 * \author Ange GRIMAUD
 * \brief augmente le niveau du chevalier
 * \param chevalier un chevalier donné
 */
void monte_de_niveau(Chevalier *chevalier);


/**
 * \author Ange GRIMAUD
 * \brief augmente l'expérience du chevalier
 * \param chevalier un chevalier donné
 * \param xp un nombre d'expérience donné
 */
void ajouter_xp(Chevalier *chevalier, int xp);


/**
 * \author Ange GRIMAUD
 * \brief calculer un nombre à une certaine puissance
 * \param niveau la puissance
 * \param xp le nombre
 * \return le résultat du calcul de la puissance
 */
int puissance(int niveau, float facteur);


/**
 * \author Ange GRIMAUD
 * \brief test si le chevalier peut monter de niveau, le fais monter s'il le peut
 * \param chevalier un chevalier donné
 */
void test_gain_de_niveau(Chevalier *chevalier);


#endif