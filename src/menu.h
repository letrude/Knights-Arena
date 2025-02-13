/**
 * \file menu.h
 * \author Ange GRIMAUD/Gabriel COUDEL-KOUMBA
 * \date 11/12/2024
 * \brief Fonctions permettant de gérer les chevaliers
 */


#ifndef MENU_H
#define MENU_H

#include "chevalier.h"
#include "monstre.h"
#include "couleur.h"

// Inclusion des en-têtes de la bibliothèque standard
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PTS_GRP1 50
#define PTS_GRP2 100


/**
 * \author Ange GRIMAUD & Gabriel COUDEL-KOUMBA
 * \brief Menu du jeu
 */
void menuPrincipal(void);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction déroulant une partie
 * \param partie nom du fichier de la partie que l'on souhaite jouer
 * \param joueur nom du joueur
 * \return le nombre de points obtenus par le joueur
 */
int partie(char *partie, Chevalier *chevalier);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant l'état d'un combat
 * \param groupe File de Monstres
 * \param chevalier le joueur
 */
void afficherEtatCombat(File *groupe, Chevalier *chevalier);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction affichant l'état d'un combat
 * \param monstre le monstre à combattre
 * \param chevalier le joueur
 */
void afficherEtatCombat2(Monstre *monstre, Chevalier *chevalier);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant au joueur de choisir son coup
 * \param chevalier le joueur
 * \param pts les points du joueur
 * \return le coup du joueur
 */
char choixArme(Chevalier *chevalier, int pts);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de savoir s'il y a égalité
 * \param chevalier le joueur
 * \param monstre le monstre à combattre
 */
void egalite(Chevalier *chevalier, Monstre monstre);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant d'effectuer les modifications suite à une victoire au coup du joueur
 * \param chevalier le joueur
 * \param groupe File de Monstres
 * \param multi multiplicateur de score
 * \return le score fait par le joueur
 */
int victoire(Chevalier *chevalier, File *groupe, int multi);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant d'effectuer les modifications suite à une défaite au coup du joueur 
 * \param chevalier le joueur
 * \param monstre le monstre à combattre
 */
void defaite(Chevalier *chevalier, Monstre monstre);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction permettant de savoir si le joueur a gagné
 * \param choix coup du joueur
 * \param atkMonstre coup du Monstre
 * \return TRUE si le coup du joueur bat le coup du monstre, FALSE sinon
 */
Booleen estVictoire(char choix, char atkMonstre);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction gérant le 1er groupe de Monstre d'un Maillon de Pile
 * \param groupe File de Monstres
 * \param chevalier le joueur
 * \param pts le score de la partie du joueur
 */
void atkGrp1(File *groupe, Chevalier *chevalier, int *pts);

/**
 * \author Gabriel COUDEL-KOUMBA
 * \brief Fonction gérant le 2e groupe de Monstre d'un Maillon de Pile
 * \param groupe File de Monstres
 * \param chevalier le joueur
 * \param pts le score de la partie du joueur
 */
void atkGrp2(File *groupe, Chevalier *chevalier, int *pts);
#endif