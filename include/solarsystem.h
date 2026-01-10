#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <vector>
#include "CelestialBody.h"
#include "Rasterizer.h"
#include "Camera.h"

/**
 * @class SolarSystem
 * @brief Gestionnaire du système solaire complet
 */
class SolarSystem {
private:
    std::vector<CelestialBody*> bodies;
    CelestialBody* sun;
    
public:
    SolarSystem();
    ~SolarSystem();
    
    /**
     * @brief Initialise le système avec le Soleil et les planètes
     */
    void initialize();
    
    /**
     * @brief Met à jour tous les corps célestes
     */
    void update(float deltaTime);
    
    /**
     * @brief Dessine le système complet
     */
     void draw(Rasterizer* rasterizer, Camera* camera);
    /**
     * @brief Dessine les orbites (SDL simple)
     */
    void drawOrbits(SDL_Renderer* renderer, Camera* camera, int screenWidth, int screenHeight);
    
    /**
     * @brief Obtenir tous les corps
     */
    const std::vector<CelestialBody*>& getBodies() const;
    
    /**
     * @brief Obtenir le soleil
     */
    CelestialBody* getSun() const;
    
    /**
     * @brief Obtenir le nombre total de triangles
     */
    int getTotalTriangleCount() const;
};

#endif // SOLARSYSTEM_H