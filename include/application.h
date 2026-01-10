#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL3/SDL.h>
#include "Renderer3D.h"
#include "Camera.h"
#include "SolarSystem.h"

/**
 * @class Application
 * @brief Classe principale de l'application
 * 
 * Gère :
 * - Initialisation SDL
 * - Boucle de jeu
 * - Événements
 * - Timing
 */
class Application {
private:
    // SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    // Composants principaux
    Renderer3D* renderer3D;
    Camera* camera;
    SolarSystem* solarSystem;
    
    // État
    bool isRunning;
    bool isPaused;
    float simulationSpeed;
    
    // Timing
    Uint64 lastFrameTime;
    int frameCount;
    float fpsTimer;
    int currentFPS;
    
    // Contrôles souris
    bool isDragging;
    int lastMouseX;
    int lastMouseY;
    
    // Sélection
    CelestialBody* selectedBody;
    
    // Dimensions
    int screenWidth;
    int screenHeight;
    
    /**
     * @brief Initialise SDL et crée la fenêtre
     */
    bool initializeSDL();
    
    /**
     * @brief Initialise les composants du projet
     */
    bool initializeComponents();
    
    /**
     * @brief Gère les événements SDL
     */
    void handleEvents();
    
    /**
     * @brief Met à jour la simulation
     */
    void update(float deltaTime);
    
    /**
     * @brief Effectue le rendu
     */
    void render();
    
    /**
     * @brief Nettoie les ressources
     */
    void cleanup();
    
    /**
     * @brief Affiche les contrôles dans la console
     */
    void printControls();
    
public:
    Application();
    ~Application();
    
    /**
     * @brief Initialise l'application
     */
    bool initialize();
    
    /**
     * @brief Lance la boucle principale
     */
    void run();
};

#endif // APPLICATION_H