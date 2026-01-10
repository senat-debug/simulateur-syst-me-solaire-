#include "celestialbody.h"
#include "constants.h"
#include <cmath>

CelestialBody::CelestialBody(const std::string& name,
                             float visualRadius,
                             float orbitalRadius,
                             float orbitalSpeed,
                             float rotationSpeed,
                             SDL_Color color,
                             bool isSun)
    : name(name)
    , visualRadius(visualRadius)
    , orbitalRadius(orbitalRadius)
    , orbitalSpeed(orbitalSpeed)
    , rotationSpeed(rotationSpeed)
    , currentAngle(0.0f)
    , currentRotation(0.0f)
    , color(color)
    , isSun(isSun) {
    
    // Créer la géométrie
    if (isSun) {
        sphereMesh = new Sphere(visualRadius, Constants::SUN_SECTORS, Constants::SUN_STACKS);
    } else {
        sphereMesh = new Sphere(visualRadius, Constants::SPHERE_SECTORS, Constants::SPHERE_STACKS);
    }
    
    // Position initiale
    position.x = std::cos(currentAngle) * orbitalRadius;
    position.y = 0.0f;
    position.z = std::sin(currentAngle) * orbitalRadius;
    
    rotation = Vector3D::zero();
}

CelestialBody::~CelestialBody() {
    delete sphereMesh;
}

void CelestialBody::update(float deltaTime) {
    if (!isSun) {
        // 1. Mise à jour de l'angle orbital
        currentAngle += orbitalSpeed * deltaTime;
        
        // 2. Calculer la nouvelle position basée sur le nouvel angle
        position.x = std::cos(currentAngle) * orbitalRadius;
        position.z = std::sin(currentAngle) * orbitalRadius;
        position.y = 0.0f; // Les orbites restent sur le plan XZ
    }
    
    // 3. Rotation propre de la planète sur elle-même
    currentRotation += rotationSpeed * deltaTime;
    rotation.y = currentRotation;
}

Matrix4x4 CelestialBody::getModelMatrix() const {
    // Créer les matrices individuelles
    Matrix4x4 translation = Matrix4x4::translation(position.x, position.y, position.z);
    Matrix4x4 rotationMatrix = Matrix4x4::rotationY(rotation.y);
    
    // L'ordre est crucial : Rotation d'abord (autour de l'origine locale), puis Translation
    return translation * rotationMatrix;
}

const Sphere* CelestialBody::getMesh() const {
    return sphereMesh;
}

std::string CelestialBody::getName() const {
    return name;
}

Vector3D CelestialBody::getPosition() const {
    return position;
}

SDL_Color CelestialBody::getColor() const {
    return color;
}

bool CelestialBody::getIsSun() const {
    return isSun;
}

float CelestialBody::getOrbitalRadius() const {
    return orbitalRadius;
}

float CelestialBody::getCurrentAngle() const {
    return currentAngle;
}

float CelestialBody::getVisualRadius() const {
    return visualRadius;
}