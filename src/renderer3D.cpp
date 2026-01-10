#include "Renderer3D.h"
#include "Constants.h"
#include <cmath>
#include <random>

Renderer3D::Renderer3D(SDL_Renderer* renderer, int width, int height)
    : sdlRenderer(renderer)
    , screenWidth(width)
    , screenHeight(height)
    , showOrbits(true)
    , showStars(false)
    , showInfo(true) {
    
    rasterizer = new Rasterizer(renderer, width, height);
    generateStars();
}

Renderer3D::~Renderer3D() {
    delete rasterizer;
}

void Renderer3D::generateStars() {
    stars.clear();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-500.0f, 500.0f);
    std::uniform_real_distribution<float> brightDist(0.3f, 1.0f);
    std::uniform_real_distribution<float> sizeDist(0.5f, 2.0f);
    
    for (int i = 0; i < Constants::MAX_STARS; i++) {
        Star star;
        star.position = Vector3D(
            posDist(gen),
            posDist(gen),
            posDist(gen)
        );
        star.brightness = brightDist(gen);
        star.size = sizeDist(gen);
        stars.push_back(star);
    }
}

void Renderer3D::renderStars(Camera* camera) {
    if (!showStars) return;
    
    Matrix4x4 viewProj = camera->getViewProjectionMatrix();
    
    for (const auto& star : stars) {
        Vector3D projected = viewProj.transformPoint(star.position);
        
        // Convertir en coordonnées écran
        float x = (projected.x + 1.0f) * 0.5f * screenWidth;
        float y = (1.0f - projected.y) * 0.5f * screenHeight;
        
        // Vérifier si visible
        if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
            Uint8 brightness = (Uint8)(255 * star.brightness);
            SDL_SetRenderDrawColor(sdlRenderer, brightness, brightness, brightness, 255);
            
            // Dessiner l'étoile (petit carré ou point)
            if (star.size > 1.5f) {
                SDL_FRect rect = {x - star.size/2, y - star.size/2, star.size, star.size};
                SDL_RenderFillRect(sdlRenderer, &rect);
            } else {
                SDL_RenderPoint(sdlRenderer, x, y);
            }
        }
    }
}

void Renderer3D::clear() {
    rasterizer->clear();
}

void Renderer3D::present() {
    SDL_RenderPresent(sdlRenderer);
}

void Renderer3D::resize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    rasterizer->resize(width, height);
}

void Renderer3D::render(SolarSystem* solarSystem, Camera* camera) {
    // 1. Fond étoilé
    renderStars(camera);
    
    // 2. Orbites
    if (showOrbits) {
        solarSystem->drawOrbits(sdlRenderer, camera, screenWidth, screenHeight);
    }
    
    // 3. Corps célestes
    solarSystem->draw(rasterizer, camera);
}

void Renderer3D::renderInfo(int fps, int triangles, float simSpeed, CelestialBody* selectedBody) {
    if (!showInfo) return;
    
    // Dessiner un fond semi-transparent pour le texte
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 180);
    SDL_FRect bgRect = {10, 10, 250, 120};
    SDL_RenderFillRect(sdlRenderer, &bgRect);
    
    // Bordure
    SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255);
    SDL_RenderRect(sdlRenderer, &bgRect);
    
    // Note: Pour afficher du texte, il faudrait SDL_ttf ou dessiner caractère par caractère
    // Pour ce projet, on affiche juste des indicateurs visuels
    
    // Indicateur FPS (barre colorée)
    float fpsRatio = fps / 60.0f;
    if (fpsRatio > 1.0f) fpsRatio = 1.0f;
    
    Uint8 green = (Uint8)(255 * fpsRatio);
    Uint8 red = (Uint8)(255 * (1.0f - fpsRatio));
    SDL_SetRenderDrawColor(sdlRenderer, red, green, 0, 255);
    SDL_FRect fpsBar = {20, 20, 230 * fpsRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &fpsBar);
    
    // Indicateur triangles (barre)
    float triRatio = triangles / 3000.0f;
    if (triRatio > 1.0f) triRatio = 1.0f;
    
    SDL_SetRenderDrawColor(sdlRenderer, 100, 200, 255, 255);
    SDL_FRect triBar = {20, 40, 230 * triRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &triBar);
    
    // Indicateur vitesse simulation
    float speedRatio = simSpeed / Constants::MAX_SIMULATION_SPEED;
    SDL_SetRenderDrawColor(sdlRenderer, 255, 200, 0, 255);
    SDL_FRect speedBar = {20, 60, 230 * speedRatio, 10};
    SDL_RenderFillRect(sdlRenderer, &speedBar);
    
    // Points pour représenter les valeurs
    for (int i = 0; i < 5; i++) {
        SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255);
        SDL_FRect dot = {20.0f + i * 50, 80, 5, 5};
        SDL_RenderFillRect(sdlRenderer, &dot);
    }
}

void Renderer3D::drawText(const std::string& text, int x, int y) {
    // Fonction placeholder pour le texte
    // Dans un vrai projet, utiliser SDL_ttf
    (void)text;
    (void)x;
    (void)y;
}