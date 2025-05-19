# 🚴‍♂️ Bike Race Game – Projet C++ avec SFML

Projet réalisé dans le cadre du module **Programmation Orientée Objet (POO)** à l’Université **Cadi Ayyad**, Faculté des Sciences Semlalia – Marrakech.

## 🎓 Informations générales

- **Année universitaire** : 2024–2025  
- **Encadrante** : Mme R. Hannane  
- **Binôme** :
  - Amine Rihan – 2130072 – INF04 – grp 03
  - Adham El Warari – 2130071 – INF04 – grp 01  
- **Date de remise** : 10/05/2025

---

## 🎮 Description du jeu

Un jeu de course à vélo mono-joueur développé en C++ avec la bibliothèque **SFML**.  
L’objectif est simple : éviter les obstacles (murs) tout en progressant sur la piste.  
Le jeu inclut :
- Un menu interactif (Jouer, À propos, Quitter)
- Un système de score et de chronométrage
- Un écran de fin avec possibilité de **rejouer**
- Une ambiance sonore en boucle
- Des éléments de décor (arbres, étoiles, etc.)

---

## 🔧 Fonctionnalités

### Fonctionnelles
- Contrôle du vélo via les touches directionnelles
- Collision avec murs = fin de partie
- Bonus à collecter
- Musique de fond

### Non fonctionnelles
- Interface fluide (60 fps)
- Responsive à la taille de la fenêtre
- Navigation intuitive (souris + clavier)

---

## 🧰 Technologies et outils utilisés

- **Langage** : C++
- **Bibliothèque** : [SFML 2.5.1](https://www.sfml-dev.org)
- **IDE** : Code::Blocks 20.03
- **Compilateur** : MinGW g++
- **Système d’exploitation** : Windows 11

---

## 🧱 Architecture du projet

projet/
├── main.cpp
├── BikeGameSFML.cpp
├── BikeGameSFML.hpp
├── assets/
│ ├── images/
│ ├── fonts/
│ └── sounds/
├── bin/
├── obj/
├── projetCourseBike.cbp
└── RAPPORT PROJET C++.pdf



---

## 🧠 Concepts orientés objet utilisés

- **Encapsulation** : données protégées, accès via méthodes
- **Abstraction** : logique cachée derrière des interfaces simples
- **Responsabilité unique** : chaque classe a un rôle clair
- **Réutilisabilité** : classes réutilisables dans d'autres projets
- **Modélisation par états** : gestion claire des phases du jeu

---

## 📈 Améliorations possibles

- Ajout de niveaux avec difficulté progressive
- Système de sauvegarde des scores
- Graphismes plus riches (sprites/textures)
- Effets sonores dynamiques
- Tutoriel interactif pour les nouveaux joueurs

---

## 📷 Aperçus

### Menu principal  
Interface intuitive avec boutons "Jouer", "À propos", "Quitter"

### Fenêtre de jeu  
Moto sur une piste avec obstacles, chronomètre en haut à droite

### Écran "À propos"  
Affiche les noms du binôme et les objectifs du jeu

### Écran de fin  
Affiche le **score final** et le **temps total** écoulé

---


## 📌 Auteurs

Développé par :
- **Amine Rihan**
- **Adham El Warari**

---

