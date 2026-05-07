# Jeu du Pissenlit - SDL3 / C++

## Technologies utilisées

- C++
- SDL3
- Git / GitHub
- CLion
- CMake

## Concepts utilisés

Dans ce projet, j’ai utilisé :

- une grille 2D pour représenter le terrain de jeu
- un `enum` pour gérer les états des cases
- des événements SDL pour gérer la souris et le clavier
- une boucle principale de jeu (`game loop`)
- des fonctions séparées pour organiser la logique du projet
- une propagation directionnelle pour le déplacement des graines

## Structure du code

Le projet est séparé en plusieurs fonctions principales :

- `resetGame()` : réinitialise la partie
- `isGridFull()` : vérifie la condition de victoire
- `getRandomUnusedDirection()` : choisit une direction aléatoire
- `spreadSeeds()` : applique la propagation des graines
- `drawGame()` : dessine la grille et les éléments
- `updateWindowTitle()` : met à jour les informations de la fenêtre

## Représentation des cases
Chaque case possède un état :

```cpp
EMPTY
SEED
DANDELION
----------------------------------------------------------------------------------------------------
J’ai utilisé un enum afin de rendre le code plus clair et plus facile à maintenir.
Algorithme principal
Le joueur plante un pissenlit avec la souris.
Le vent choisit ensuite une direction aléatoire parmi 8 directions possibles.
Tous les pissenlits présents propagent des graines dans cette direction jusqu’au bord de la grille.
Pour éviter les erreurs pendant la propagation, j’utilise une copie temporaire de la grille (newGrid) avant de mettre à jour la grille principale.
Affichage
Le rendu graphique est réalisé avec SDL3 :

dessin de la grille

couleurs différentes selon l’état des cases

affichage des graines et des pissenlits avec des formes simples

mise à jour dynamique du titre de la fenêtre

Contrôles:
clic gauche : planter un pissenlit
touche R : recommencer une partie



















