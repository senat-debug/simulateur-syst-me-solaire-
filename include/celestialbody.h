#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <SDL3/SDL.h>
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Sphere.h"

/**
 * @class CelestialBody
 * @brief Représente un corps céleste (planète ou soleil)
 */
class CelestialBody {
private:
    std::string name;
    
    // Transformation
    Vector3D position;
    Vector3D rotation;
    float visualRadius;
    
    // Orbite
    float orbitalRadius;     // Distance au centre (soleil)
    float orbitalSpeed;      // Vitesse orbitale (rad/s)
    float rotationSpeed;     // Vitesse rotation propre (rad/s)
    float currentAngle;      // Angle actuel sur l'orbite
    float currentRotation;   // Rotation actuelle sur lui-même
    
    // Apparence
    SDL_Color color;
    bool isSun;
    
    // Géométrie
    Sphere* sphereMesh;
    
public:
    CelestialBody(const std::string& name,
                  float visualRadius,
                  float orbitalRadius,
                  float orbitalSpeed,
                  float rotationSpeed,
                  SDL_Color color,
                  bool isSun = false);
    
    ~CelestialBody();
    
    /**
     * @brief Met à jour la position et rotation
     */
    void update(float deltaTime);
    
    /**
     * @brief Calcule la matrice de transformation model
     */
    Matrix4x4 getModelMatrix() const;
    
    /**
     * @brief Obtenir le mesh
     */
    const Sphere* getMesh() const;
    
    // Getters
    std::string getName() const;
    Vector3D getPosition() const;
    SDL_Color getColor() const;
    bool getIsSun() const;
    float getOrbitalRadius() const;
    float getCurrentAngle() const;
    float getVisualRadius() const;
};

#endif // CELESTIALBODY_H