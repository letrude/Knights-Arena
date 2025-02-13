# Chevalier ⚔️ vs. Monstres 🐉

## Présentation
Le projet consistait à développer un jeu vidéo de combat entre un chevalier et des monstres, avec une mécanique basée sur le duel "Pierre/Feuille/Ciseaux" en 1 mois en binôme. Le chevalier affronte deux groupes de monstres dans chaque partie. Dans le 1er groupe les monstres attaquent un par un, et le chevalier les affronte successivement. Dans le 2e groupe ils attaquent de manière quasi-simultanée.

## Documentation

### Fichier PDF
#### Lien vers le fichier dans le dépôt code first
[Rapport pdf](https://codefirst.iut.uca.fr/git/gabriel.coudel-koumba/sae_1.02/src/branch/master/doc/rapport.pdf)

> Commandes à entrer dans le terminal depuis la racine du répertoire<br>
> Il est possible de remplacer evince par le visionneur pdf de votre choix

#### Ouvrir avec un lecteur pdf

``` evince doc/rapport.pdf ``` 

### Documentation doxygen
#### Générer la documentation 
``` make doc ```
#### Ouvrir la page de documentation
> Il est possible de remplacer firefox par le navigateur de votre choix

``` firefox doc/html/index.html``` <span style="color: orange">⚠ uniquement si ``` make doc ``` a été fait</span><br>

## Pour commencer

> Commandes à entrer dans le terminal depuis la racine du répertoire

### Compiler
``` make ```

### Éxecuter le projet
``` ./bin/exe ``` <span style="color: orange">⚠ depuis la racine du répertoire</span><br>
* #### En étant dans ``` bin ``` 
``` cd bin/ ``` (<span style="color: orange">⚠ depuis la racine du répertoire</span>) puis ``` ./exe ``` 

* #### En utilisant ``` install ```
``` make install ``` (copie l'exécutable dans le répertoire parent soit la racine du répertoire) puis ``` ./exe ``` : 

### Nettoyer le projet

* #### En utilisant ``` clean ```
``` make clean ``` : supprime ``` bin ```, ``` obj ``` et la documentation doxygen générée

* #### En utilisant ``` uninstall ```
``` make uninstall ``` : supprime la copie de l'éxecutable  (<span style="color: orange">⚠ uniquement si utilisation de ``` install ```</span>)

## Comment contribuer ?
Si vous souhaitez contribuer à ce projet, veuillez d'abord discuter de la modification que vous souhaitez apporter via un ticket, un mail ou toute autre méthode avec les propriétaires de ce dépôt avant d'apporter une modification.

## Auteurs
* **Gabriel COUDEL-KOUMBA** - *Travail initial* - [Phantom-Whisper](https://github.com/Phantom-Whisper)
* **Ange GRIMAUD** - *Travail initial* - [letrude](https://github.com/letrude)
