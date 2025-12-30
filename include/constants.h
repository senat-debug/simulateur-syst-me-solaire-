#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL3/SDL.h>

/**
 * @file Constants.h
 * @brief Toutes les constantes du projet en un seul endroit
 */

namespace Constants {
    
    // ======================
    // FENÊTRE
    // ======================
    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Solar System Simulator 3D";
    
    // ======================
    // FPS ET TIMING
    // ======================
    constexpr int TARGET_FPS = 60;
    constexpr float FIXED_TIMESTEP = 1.0f / TARGET_FPS;
    constexpr int FRAME_DELAY_MS = 1000 / TARGET_FPS;  // ~16ms
    
    // ======================
    // MATHÉMATIQUES
    // ======================
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    constexpr float HALF_PI = PI / 2.0f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON = 0.0001f;  // Pour comparaisons float
    
    // ======================
    // CAMÉRA
    // ======================
    constexpr float CAMERA_FOV = 60.0f;              // Field of view en degrés
    constexpr float CAMERA_NEAR = 0.1f;              // Plan proche
    constexpr float CAMERA_FAR = 1000.0f;            // Plan lointain
    constexpr float CAMERA_DISTANCE = 300.0f;        // Distance initiale
    constexpr float CAMERA_MIN_DISTANCE = 100.0f;    // Zoom max
    constexpr float CAMERA_MAX_DISTANCE = 600.0f;    // Zoom min
    constexpr float CAMERA_ROTATION_SPEED = 0.005f;  // Sensibilité souris
    constexpr float CAMERA_ZOOM_SPEED = 10.0f;       // Vitesse zoom molette
    
    // ======================
    // QUALITÉ DES SPHÈRES
    // ======================
    // IMPORTANT : Valeurs optimisées pour software rendering
    constexpr int SPHERE_SECTORS = 18;   // Subdivisions horizontales
    constexpr int SPHERE_STACKS = 12;    // Subdivisions verticales
    // Total triangles par sphère : sectors * stacks * 2 = 432
    
    // Pour le Soleil (plus de détails)
    constexpr int SUN_SECTORS = 24;
    constexpr int SUN_STACKS = 16;
    
    // ======================
    // TAILLES DES CORPS CÉLESTES
    // ======================
    constexpr float SUN_RADIUS = 15.0f;
    constexpr float MERCURY_RADIUS = 3.0f;
    constexpr float VENUS_RADIUS = 5.0f;
    constexpr float EARTH_RADIUS = 5.5f;
    constexpr float MARS_RADIUS = 4.0f;
    
    // ======================
    // DISTANCES ORBITALES
    // ======================
    constexpr float MERCURY_ORBIT = 50.0f;
    constexpr float VENUS_ORBIT = 80.0f;
    constexpr float EARTH_ORBIT = 110.0f;
    constexpr float MARS_ORBIT = 150.0f;
    
    // ======================
    // VITESSES ORBITALES (rad/s)
    // ======================
    constexpr float MERCURY_ORBITAL_SPEED = 1.5f;
    constexpr float VENUS_ORBITAL_SPEED = 1.2f;
    constexpr float EARTH_ORBITAL_SPEED = 1.0f;
    constexpr float MARS_ORBITAL_SPEED = 0.8f;
    
    // ======================
    // VITESSES DE ROTATION (rad/s)
    // ======================
    constexpr float SUN_ROTATION_SPEED = 0.1f;
    constexpr float PLANET_ROTATION_SPEED = 2.0f;
    
    // ======================
    // COULEURS (RGBA)
    // ======================
    inline SDL_Color makeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        return SDL_Color{r, g, b, a};
    }
    
    const SDL_Color SUN_COLOR = {255, 200, 50, 255};
    const SDL_Color MERCURY_COLOR = {169, 169, 169, 255};
    const SDL_Color VENUS_COLOR = {255, 198, 73, 255};
    const SDL_Color EARTH_COLOR = {100, 149, 237, 255};
    const SDL_Color MARS_COLOR = {193, 68, 14, 255};
    const SDL_Color ORBIT_COLOR = {100, 100, 100, 80};
    const SDL_Color BACKGROUND_COLOR = {0, 0, 0, 255};
    const SDL_Color WHITE = {255, 255, 255, 255};
    
    // ======================
    // ÉCLAIRAGE
    // ======================
    const SDL_Color LIGHT_COLOR = {255, 255, 255, 255};
    constexpr float AMBIENT_STRENGTH = 0.2f;   // Lumière ambiante
    constexpr float DIFFUSE_STRENGTH = 0.8f;   // Lumière diffuse
    
    // ======================
    // SIMULATION
    // ======================
    constexpr float DEFAULT_SIMULATION_SPEED = 1.0f;
    constexpr float MIN_SIMULATION_SPEED = 0.1f;
    constexpr float MAX_SIMULATION_SPEED = 5.0f;
    
    // ======================
    // RENDU
    // ======================
    constexpr int MAX_STARS = 500;  // Nombre d'étoiles de fond
    
} // namespace Constants

#endif // CONSTANTS_H