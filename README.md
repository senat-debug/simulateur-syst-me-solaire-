# 🌌 Simulateur Système Solaire 3D

[![C++](https://img.shields.io/badge/Language-C++17-blue.svg)](https://isocpp.org/)
[![Library](https://img.shields.io/badge/Library-SDL3-green.svg)](https://www.libsdl.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](#)

Un simulateur de système solaire interactif développé en **C++17** pur. Ce projet implémente son propre **moteur de rendu logiciel (Software Renderer)** pour afficher des corps célestes en 3D sans utiliser de bibliothèques graphiques externes comme OpenGL ou DirectX.



## 🚀 Fonctionnalités principales

* **Moteur de Rendu Software** : Implémentation complète de la rasterisation de triangles avec gestion du **Z-buffer** pour la profondeur.
* **Physique & Orbites** : Simulation de trajectoires circulaires pour le Soleil et 4 planètes (Mercure, Vénus, Terre, Mars).
* **Texturing** : Mapping de textures PNG sur des sphères générées dynamiquement.
* **Éclairage Dynamique** : Modèle d'éclairage diffuse basé sur la position centrale du Soleil.
* **Caméra Interactive** : Caméra orbitale contrôlable à la souris permettant de zoomer et de pivoter autour du système.

## 🛠️ Technologies utilisées

* **Langage** : C++17.
* **Framework Graphique** : **SDL3** (Simple DirectMedia Layer) pour la gestion de la fenêtre et des pixels.
* **Mathématiques 3D** : Bibliothèque personnalisée pour les vecteurs (`Vector3D`) et les matrices (`Matrix4x4`).
* **Chargement d'Images** : `stb_image` pour le décodage des textures planétaires.

## 📁 Structure du Projet

```text
SIMULATEUR SYSTEME SOLAIRE 3D/
├── assets/               # Textures des planètes (sun.png, earth.png...)
├── include/              # Fichiers d'en-tête (.h)
│   ├── Application.h     # Gestion de la boucle de jeu
│   ├── constants.h       # Constantes physiques et réglages
│   └── Matrix4x4.h       # Calculs de transformation 3D
├── src/                  # Fichiers sources (.cpp)
│   ├── main.cpp          # Point d'entrée de l'application
│   ├── rasterizer.cpp    # Algorithme de dessin des triangles
│   └── Sphere.cpp        # Algorithme de génération de la sphère
└── CMakeLists.txt        # Configuration de compilation CMake
