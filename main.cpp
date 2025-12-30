#include <SDL3/SDL.h>
#include <iostream>
#include <iomanip>
#include "Constants.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "SolarSystem.h"

/**
 * @file main.cpp
 * @brief APPLICATION FINALE - Simulateur de Système Solaire 3D
 */

void printSeparator() {
    std::cout << "========================================" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "\n";
    printSeparator();
    std::cout << "  SOLAR SYSTEM SIMULATOR 3D" << std::endl;
    std::cout << "  Software Renderer - SDL3 Only" << std::endl;
    printSeparator();
    std::cout << "\n";
    
    // Initialisation SDL3
    std::cout << "=== Initialisation ===" << std::endl;
    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERREUR] SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "[OK] SDL3 initialise" << std::endl;
    
    // Créer la fenêtre
    SDL_Window* window = SDL_CreateWindow(
        Constants::WINDOW_TITLE,
        Constants::WINDOW_WIDTH,
        Constants::WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "[ERREUR] Creation fenetre: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    std::cout << "[OK] Fenetre creee: " << Constants::WINDOW_WIDTH 
              << "x" << Constants::WINDOW_HEIGHT << std::endl;
    
    // Créer le renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "[ERREUR] Creation renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "[OK] Renderer cree" << std::endl;
    
    // Créer le rasterizer
    Rasterizer* rasterizer = new Rasterizer(
        renderer,
        Constants::WINDOW_WIDTH,
        Constants::WINDOW_HEIGHT
    );
    std::cout << "[OK] Rasterizer 3D initialise" << std::endl;
    
    // Configurer la lumière (depuis le soleil)
    rasterizer->setLightDirection(Vector3D(0.0f, 0.0f, 1.0f));
    
    // Créer la caméra
    float aspectRatio = (float)Constants::WINDOW_WIDTH / Constants::WINDOW_HEIGHT;
    Camera* camera = new Camera(aspectRatio, Constants::CAMERA_FOV);
    std::cout << "[OK] Camera configuree" << std::endl;
    
    // Créer le système solaire
    SolarSystem* solarSystem = new SolarSystem();
    solarSystem->initialize();
    std::cout << "[OK] Systeme solaire cree:" << std::endl;
    std::cout << "    - " << solarSystem->getBodies().size() << " corps celestes" << std::endl;
    std::cout << "    - " << solarSystem->getTotalTriangleCount() << " triangles au total" << std::endl;
    
    printSeparator();
    std::cout << "[SUCCES] INITIALISATION COMPLETE !" << std::endl;
    printSeparator();
    
    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "  SOURIS :" << std::endl;
    std::cout << "    - Clic gauche + Glisser : Rotation camera" << std::endl;
    std::cout << "    - Molette : Zoom avant/arriere" << std::endl;
    std::cout << "\n  CLAVIER :" << std::endl;
    std::cout << "    - ESPACE : Pause / Play" << std::endl;
    std::cout << "    - W : Toggle Wireframe" << std::endl;
    std::cout << "    - O : Toggle Orbites" << std::endl;
    std::cout << "    - + / - : Vitesse simulation" << std::endl;
    std::cout << "    - R : Reset camera" << std::endl;
    std::cout << "    - ESC : Quitter" << std::endl;
    std::cout << "\n";
    
    // Variables de contrôle
    bool running = true;
    bool isPaused = false;
    bool showOrbits = true;
    float simulationSpeed = Constants::DEFAULT_SIMULATION_SPEED;
    
    // Variables souris
    bool isDragging = false;
    int lastMouseX = 0;
    int lastMouseY = 0;
    
    // Timing
    SDL_Event event;
    Uint64 lastTime = SDL_GetTicks();
    int frameCount = 0;
    float fpsTimer = 0.0f;
    int currentFPS = 0;
    
    // Boucle principale
    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // === ÉVÉNEMENTS ===
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    
                    case SDLK_SPACE:
                        isPaused = !isPaused;
                        std::cout << (isPaused ? "[PAUSE]" : "[PLAY]") << std::endl;
                        break;
                    
                    case SDLK_W:
                        rasterizer->setWireframeMode(!rasterizer->isWireframeMode());
                        std::cout << (rasterizer->isWireframeMode() ? "[WIREFRAME ON]" : "[WIREFRAME OFF]") << std::endl;
                        break;
                    
                    case SDLK_O:
                        showOrbits = !showOrbits;
                        std::cout << (showOrbits ? "[ORBITES ON]" : "[ORBITES OFF]") << std::endl;
                        break;
                    
                    case SDLK_EQUALS:
                    case SDLK_PLUS:
                        simulationSpeed += 0.1f;
                        if (simulationSpeed > Constants::MAX_SIMULATION_SPEED) {
                            simulationSpeed = Constants::MAX_SIMULATION_SPEED;
                        }
                        std::cout << "[VITESSE] " << std::fixed << std::setprecision(1) 
                                  << simulationSpeed << "x" << std::endl;
                        break;
                    
                    case SDLK_MINUS:
                        simulationSpeed -= 0.1f;
                        if (simulationSpeed < Constants::MIN_SIMULATION_SPEED) {
                            simulationSpeed = Constants::MIN_SIMULATION_SPEED;
                        }
                        std::cout << "[VITESSE] " << std::fixed << std::setprecision(1) 
                                  << simulationSpeed << "x" << std::endl;
                        break;
                    
                    case SDLK_R:
                        camera->setDistance(Constants::CAMERA_DISTANCE);
                        std::cout << "[RESET CAMERA]" << std::endl;
                        break;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    lastMouseX = (int)event.button.x;
                    lastMouseY = (int)event.button.y;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                if (isDragging) {
                    int deltaX = (int)event.motion.x - lastMouseX;
                    int deltaY = (int)event.motion.y - lastMouseY;
                    
                    camera->rotate(
                        deltaX * Constants::CAMERA_ROTATION_SPEED,
                        -deltaY * Constants::CAMERA_ROTATION_SPEED
                    );
                    
                    lastMouseX = (int)event.motion.x;
                    lastMouseY = (int)event.motion.y;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                camera->zoom(-event.wheel.y * Constants::CAMERA_ZOOM_SPEED);
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                int newWidth = event.window.data1;
                int newHeight = event.window.data2;
                rasterizer->resize(newWidth, newHeight);
                camera->setAspectRatio((float)newWidth / newHeight);
            }
        }
        
        // === MISE À JOUR ===
        if (!isPaused) {
            solarSystem->update(deltaTime * simulationSpeed);
        }
        
        camera->update();
        
        // === RENDU ===
        rasterizer->clear();
        
        // Dessiner les orbites (avant les planètes pour qu'elles soient derrière)
        if (showOrbits) {
            solarSystem->drawOrbits(renderer, camera, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
        }
        
        // Dessiner le système solaire
        solarSystem->draw(rasterizer, camera);
        
        SDL_RenderPresent(renderer);
        
        // === FPS ===
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f) {
            currentFPS = frameCount;
            
            std::cout << "FPS: " << currentFPS 
                      << " | Triangles: " << rasterizer->getLastTriangleCount()
                      << " | Vitesse: " << std::fixed << std::setprecision(1) << simulationSpeed << "x"
                      << " | Zoom: " << (int)camera->getDistance()
                      << std::endl;
            
            frameCount = 0;
            fpsTimer = 0.0f;
        }
        
        // Limiter le framerate
        SDL_Delay(Constants::FRAME_DELAY_MS);
    }
    
    // === NETTOYAGE ===
    std::cout << "\n=== Nettoyage ===" << std::endl;
    
    delete solarSystem;
    std::cout << "[OK] Systeme solaire detruit" << std::endl;
    
    delete camera;
    std::cout << "[OK] Camera detruite" << std::endl;
    
    delete rasterizer;
    std::cout << "[OK] Rasterizer detruit" << std::endl;
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "[OK] SDL3 ferme" << std::endl;
    
    printSeparator();
    std::cout << "  MERCI D'AVOIR UTILISE LE SIMULATEUR !" << std::endl;
    printSeparator();
    std::cout << "\n";
    
    return 0;
}