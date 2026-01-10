#include "SolarSystem.h"
#include "Constants.h"
#include <cmath>

SolarSystem::SolarSystem() : sun(nullptr) {}

SolarSystem::~SolarSystem() {
    for (auto body : bodies) {
        delete body;
    }
    bodies.clear();
}

void SolarSystem::initialize() {
    // Créer le Soleil
    sun = new CelestialBody(
        "Soleil",
        Constants::SUN_RADIUS,
        0.0f,  // Pas d'orbite
        0.0f,  // Pas de mouvement orbital
        Constants::SUN_ROTATION_SPEED,
        Constants::SUN_COLOR,
        true   // C'est le soleil
    );
    bodies.push_back(sun);
    
    // Créer Mercure
    bodies.push_back(new CelestialBody(
        "Mercure",
        Constants::MERCURY_RADIUS,
        Constants::MERCURY_ORBIT,
        Constants::MERCURY_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::MERCURY_COLOR
    ));
    
    // Créer Vénus
    bodies.push_back(new CelestialBody(
        "Venus",
        Constants::VENUS_RADIUS,
        Constants::VENUS_ORBIT,
        Constants::VENUS_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::VENUS_COLOR
    ));
    
    // Créer la Terre
    bodies.push_back(new CelestialBody(
        "Terre",
        Constants::EARTH_RADIUS,
        Constants::EARTH_ORBIT,
        Constants::EARTH_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::EARTH_COLOR
    ));
    
    // Créer Mars
    bodies.push_back(new CelestialBody(
        "Mars",
        Constants::MARS_RADIUS,
        Constants::MARS_ORBIT,
        Constants::MARS_ORBITAL_SPEED,
        Constants::PLANET_ROTATION_SPEED,
        Constants::MARS_COLOR
    ));
}

void SolarSystem::update(float deltaTime) {
    for (auto body : bodies) {
        body->update(deltaTime);
    }
}

void SolarSystem::draw(Rasterizer* rasterizer, Camera* camera) {
    Matrix4x4 view = camera->getViewMatrix();
    Matrix4x4 projection = camera->getProjectionMatrix();
    
    // Dessiner chaque corps céleste
    for (auto body : bodies) {
        Matrix4x4 model = body->getModelMatrix();
        
        rasterizer->drawMesh(
            body->getMesh(),
            model,
            view,
            projection,
            body->getColor()
        );
    }
}

void SolarSystem::drawOrbits(SDL_Renderer* renderer, Camera* camera, int screenWidth, int screenHeight) {
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    
    SDL_SetRenderDrawColor(renderer, 
                          Constants::ORBIT_COLOR.r, 
                          Constants::ORBIT_COLOR.g, 
                          Constants::ORBIT_COLOR.b, 
                          Constants::ORBIT_COLOR.a);
    
    // Dessiner l'orbite de chaque planète
    for (const auto& body : bodies) {
        if (body->getIsSun()) continue;
        
        float radius = body->getOrbitalRadius();
        int segments = 128;
        
        for (int i = 0; i < segments; i++) {
            float angle1 = i * Constants::TWO_PI / segments;
            float angle2 = (i + 1) * Constants::TWO_PI / segments;
            
            Vector3D p1(std::cos(angle1) * radius, 0.0f, std::sin(angle1) * radius);
            Vector3D p2(std::cos(angle2) * radius, 0.0f, std::sin(angle2) * radius);
            
            Vector3D proj1 = viewProj.transformPoint(p1);
            Vector3D proj2 = viewProj.transformPoint(p2);
            
            // Convertir en coordonnées écran
            float x1 = (proj1.x + 1.0f) * 0.5f * screenWidth;
            float y1 = (1.0f - proj1.y) * 0.5f * screenHeight;
            float x2 = (proj2.x + 1.0f) * 0.5f * screenWidth;
            float y2 = (1.0f - proj2.y) * 0.5f * screenHeight;
            
            // Clip test basique
            if (x1 >= 0 && x1 < screenWidth && y1 >= 0 && y1 < screenHeight &&
                x2 >= 0 && x2 < screenWidth && y2 >= 0 && y2 < screenHeight) {
                SDL_RenderLine(renderer, x1, y1, x2, y2);
            }
        }
    }
}

const std::vector<CelestialBody*>& SolarSystem::getBodies() const {
    return bodies;
}

CelestialBody* SolarSystem::getSun() const {
    return sun;
}

int SolarSystem::getTotalTriangleCount() const {
    int total = 0;
    for (const auto& body : bodies) {
        total += body->getMesh()->getTriangleCount();
    }
    return total;
}