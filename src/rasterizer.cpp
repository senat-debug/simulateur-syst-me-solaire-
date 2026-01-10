#include "Rasterizer.h"
#include "constants.h"
#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer(SDL_Renderer* renderer, int width, int height)
    : renderer(renderer)
    , width(width)
    , height(height)
    , lightDirection(0.0f, 0.0f, 1.0f)
    , lightColor{255, 255, 255, 255}
    , ambientStrength(Constants::AMBIENT_STRENGTH)
    , wireframeMode(false)
    , lastTriangleCount(0)
    , culledTriangleCount(0) {
    
    zBuffer.resize(width * height, 1e10f);
    lightDirection.normalize();
}

Rasterizer::~Rasterizer() {
    zBuffer.clear();
}

void Rasterizer::clearZBuffer() {
    std::fill(zBuffer.begin(), zBuffer.end(), 1e10f);
}

void Rasterizer::clear() {
    clearZBuffer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Rasterizer::resize(int w, int h) {
    width = w;
    height = h;
    zBuffer.resize(width * height, 1e10f);
}

bool Rasterizer::depthTest(int x, int y, float depth) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    int index = y * width + x;
    if (depth < zBuffer[index]) {
        zBuffer[index] = depth;
        return true;
    }
    
    return false;
}

void Rasterizer::setLightDirection(const Vector3D& dir) {
    lightDirection = dir.normalized();
}

SDL_Color Rasterizer::calculateLighting(const Vector3D& normal, SDL_Color baseColor) {
    // Ambient
    float ambient = ambientStrength;
    
    // Diffuse
    float diffuse = std::max(0.0f, -normal.dot(lightDirection));
    
    float brightness = ambient + diffuse * Constants::DIFFUSE_STRENGTH;
    brightness = std::min(1.0f, brightness);
    
    return SDL_Color{
        (Uint8)(baseColor.r * brightness),
        (Uint8)(baseColor.g * brightness),
        (Uint8)(baseColor.b * brightness),
        baseColor.a
    };
}

Rasterizer::ProjectedVertex Rasterizer::projectVertex(const Vertex& vertex, const Matrix4x4& mvp) {
    ProjectedVertex result;
    
    // Transformer le point
    Vector3D projected = mvp.transformPoint(vertex.position);
    
    // Convertir en coordonnées écran
    result.x = (projected.x + 1.0f) * 0.5f * width;
    result.y = (1.0f - projected.y) * 0.5f * height;
    result.z = projected.z;
    result.normal = vertex.normal;
    result.u = vertex.u;
    result.v = vertex.v;
    result.visible = (result.x >= -100 && result.x <= width + 100 && 
                     result.y >= -100 && result.y <= height + 100);
    
    return result;
}

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    
    int maxSteps = std::max(width, height) * 2;
    int steps = 0;
    
    while (steps++ < maxSteps) {
        SDL_RenderPoint(renderer, x0, y0);
        
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Rasterizer::fillTriangle(const ProjectedVertex& v0,
                               const ProjectedVertex& v1,
                               const ProjectedVertex& v2,
                               SDL_Color baseColor) {
    // Trier les vertices par Y
    ProjectedVertex verts[3] = {v0, v1, v2};
    if (verts[0].y > verts[1].y) std::swap(verts[0], verts[1]);
    if (verts[1].y > verts[2].y) std::swap(verts[1], verts[2]);
    if (verts[0].y > verts[1].y) std::swap(verts[0], verts[1]);
    
    // Normale moyenne pour l'éclairage
    Vector3D avgNormal = (v0.normal + v1.normal + v2.normal) * (1.0f / 3.0f);
    avgNormal.normalize();
    SDL_Color litColor = calculateLighting(avgNormal, baseColor);
    
    // Scanline rasterization (simplifié)
    int minY = std::max(0, (int)verts[0].y);
    int maxY = std::min(height - 1, (int)verts[2].y);
    
    for (int y = minY; y <= maxY; y++) {
        // Trouver les intersections avec les bords du triangle
        float t1 = (verts[2].y - verts[0].y) > 0.001f ? 
                   (y - verts[0].y) / (verts[2].y - verts[0].y) : 0.0f;
        
        float x1 = verts[0].x + t1 * (verts[2].x - verts[0].x);
        float z1 = verts[0].z + t1 * (verts[2].z - verts[0].z);
        
        float x2, z2;
        if (y < verts[1].y) {
            float t2 = (verts[1].y - verts[0].y) > 0.001f ? 
                       (y - verts[0].y) / (verts[1].y - verts[0].y) : 0.0f;
            x2 = verts[0].x + t2 * (verts[1].x - verts[0].x);
            z2 = verts[0].z + t2 * (verts[1].z - verts[0].z);
        } else {
            float t2 = (verts[2].y - verts[1].y) > 0.001f ? 
                       (y - verts[1].y) / (verts[2].y - verts[1].y) : 0.0f;
            x2 = verts[1].x + t2 * (verts[2].x - verts[1].x);
            z2 = verts[1].z + t2 * (verts[2].z - verts[1].z);
        }
        
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(z1, z2);
        }
        
        int startX = std::max(0, (int)x1);
        int endX = std::min(width - 1, (int)x2);
        
        for (int x = startX; x <= endX; x++) {
            float t = (x2 - x1) > 0.001f ? (x - x1) / (x2 - x1) : 0.0f;
            float z = z1 + t * (z2 - z1);
            
            if (depthTest(x, y, z)) {
                SDL_SetRenderDrawColor(renderer, litColor.r, litColor.g, litColor.b, litColor.a);
                SDL_RenderPoint(renderer, x, y);
            }
        }
    }
}

void Rasterizer::drawMesh(const Mesh* mesh,
                          const Matrix4x4& model,
                          const Matrix4x4& view,
                          const Matrix4x4& projection,
                          SDL_Color color) {
    
    Matrix4x4 mvp = projection * view * model;
    
    const std::vector<Vertex>& vertices = mesh->getVertices();
    const std::vector<int>& indices = mesh->getIndices();
    
    // Projeter tous les vertices
    std::vector<ProjectedVertex> projectedVerts;
    projectedVerts.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        // Transformer la normale
        Vector3D transformedNormal = model.transformVector(vertex.normal);
        transformedNormal.normalize();
        
        Vertex transformedVertex = vertex;
        transformedVertex.normal = transformedNormal;
        
        projectedVerts.push_back(projectVertex(transformedVertex, mvp));
    }
    
    // Dessiner tous les triangles
    lastTriangleCount = 0;
    culledTriangleCount = 0;
    
    for (size_t i = 0; i < indices.size(); i += 3) {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];
        
        const ProjectedVertex& p0 = projectedVerts[i0];
        const ProjectedVertex& p1 = projectedVerts[i1];
        const ProjectedVertex& p2 = projectedVerts[i2];
        
        // Backface culling
        float cross = (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
        if (cross < 0) {
            culledTriangleCount++;
            continue;
        }
        
        if (wireframeMode) {
            // Mode fil de fer
            SDL_Color wireColor = {0, 255, 0, 255};
            if (p0.visible && p1.visible) drawLine((int)p0.x, (int)p0.y, (int)p1.x, (int)p1.y, wireColor);
            if (p1.visible && p2.visible) drawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, wireColor);
            if (p2.visible && p0.visible) drawLine((int)p2.x, (int)p2.y, (int)p0.x, (int)p0.y, wireColor);
        } else {
            // Mode rempli avec éclairage
            fillTriangle(p0, p1, p2, color);
        }
        
        lastTriangleCount++;
    }
}

void Rasterizer::setWireframeMode(bool enabled) {
    wireframeMode = enabled;
}

bool Rasterizer::isWireframeMode() const {
    return wireframeMode;
}

int Rasterizer::getLastTriangleCount() const {
    return lastTriangleCount;
}