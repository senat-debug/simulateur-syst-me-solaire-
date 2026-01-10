#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <SDL3/SDL.h>
#include <vector>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Mesh.h"

/**
 * @class Rasterizer
 * @brief Moteur de rendu 3D software (sans OpenGL)
 * 
 * Fonctionnalités :
 * - Projection 3D -> 2D
 * - Z-Buffer pour gestion de profondeur
 * - Backface culling
 * - Remplissage de triangles
 * - Éclairage diffuse
 */
class Rasterizer {
private:
    SDL_Renderer* renderer;
    int width;
    int height;
    
    // Z-Buffer pour gestion de profondeur
    std::vector<float> zBuffer;
    
    // Éclairage
    Vector3D lightDirection;
    SDL_Color lightColor;
    float ambientStrength;
    
    // Mode wireframe
    bool wireframeMode;
    
    /**
     * @brief Structure pour un vertex projeté
     */
    struct ProjectedVertex {
        float x, y, z;  // Position écran + profondeur
        Vector3D normal;
        float u, v;
        bool visible;
    };
    
    /**
     * @brief Nettoie le Z-Buffer
     */
    void clearZBuffer();
    
    /**
     * @brief Test de profondeur
     */
    bool depthTest(int x, int y, float depth);
    
    /**
     * @brief Dessine une ligne (algorithme de Bresenham)
     */
    void drawLine(int x0, int y0, int x1, int y1, SDL_Color color);
    
    /**
     * @brief Remplit un triangle
     */
    void fillTriangle(const ProjectedVertex& v0, 
                     const ProjectedVertex& v1, 
                     const ProjectedVertex& v2,
                     SDL_Color baseColor);
    
    /**
     * @brief Calcule l'éclairage pour une normale
     */
    SDL_Color calculateLighting(const Vector3D& normal, SDL_Color baseColor);
    
    /**
     * @brief Projette un vertex 3D en 2D
     */
    ProjectedVertex projectVertex(const Vertex& vertex, const Matrix4x4& mvp);
    
public:
    Rasterizer(SDL_Renderer* renderer, int width, int height);
    ~Rasterizer();
    
    /**
     * @brief Nettoie l'écran et le z-buffer
     */
    void clear();
    
    /**
     * @brief Redimensionne le rasterizer
     */
    void resize(int width, int height);
    
    /**
     * @brief Configure la direction de la lumière
     */
    void setLightDirection(const Vector3D& dir);
    
    /**
     * @brief Dessine un mesh complet
     */
    void drawMesh(const Mesh* mesh,
                  const Matrix4x4& model,
                  const Matrix4x4& view,
                  const Matrix4x4& projection,
                  SDL_Color color);
    
    /**
     * @brief Toggle wireframe mode
     */
    void setWireframeMode(bool enabled);
    bool isWireframeMode() const;
    
    /**
     * @brief Stats de rendu
     */
    int getLastTriangleCount() const;
    
private:
    int lastTriangleCount;
    int culledTriangleCount;
};

#endif // RASTERIZER_H