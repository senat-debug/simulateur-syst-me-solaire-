#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3D.h"

/**
 * @class Matrix4x4
 * @brief Matrice 4x4 pour les transformations 3D
 * 
 * Représentation d'une matrice 4x4 en row-major order
 * Utilisée pour :
 * - Transformations géométriques (translation, rotation, scale)
 * - Matrices de vue (caméra)
 * - Matrices de projection (perspective)
 */
class Matrix4x4 {
private:
    float m[4][4];  // Matrice 4x4 en row-major
    
public:
    // ======================
    // CONSTRUCTEURS
    // ======================
    
    /**
     * @brief Constructeur par défaut (matrice identité)
     */
    Matrix4x4();
    
    /**
     * @brief Constructeur avec tableau
     */
    Matrix4x4(float values[4][4]);
    
    // ======================
    // MATRICES SPÉCIALES
    // ======================
    
    /**
     * @brief Crée une matrice identité
     */
    static Matrix4x4 identity();
    
    /**
     * @brief Crée une matrice de translation
     * @param x, y, z Déplacements sur chaque axe
     */
    static Matrix4x4 translation(float x, float y, float z);
    
    /**
     * @brief Crée une matrice de rotation autour de l'axe X
     * @param angle Angle en radians
     */
    static Matrix4x4 rotationX(float angle);
    
    /**
     * @brief Crée une matrice de rotation autour de l'axe Y
     * @param angle Angle en radians
     */
    static Matrix4x4 rotationY(float angle);
    
    /**
     * @brief Crée une matrice de rotation autour de l'axe Z
     * @param angle Angle en radians
     */
    static Matrix4x4 rotationZ(float angle);
    
    /**
     * @brief Crée une matrice de mise à l'échelle
     * @param sx, sy, sz Facteurs d'échelle sur chaque axe
     */
    static Matrix4x4 scale(float sx, float sy, float sz);
    
    /**
     * @brief Crée une matrice de vue (caméra)
     * @param eye Position de la caméra
     * @param target Point regardé
     * @param up Vecteur "haut" de la caméra
     */
    static Matrix4x4 lookAt(const Vector3D& eye, 
                            const Vector3D& target, 
                            const Vector3D& up);
    
    /**
     * @brief Crée une matrice de projection perspective
     * @param fov Champ de vision en degrés
     * @param aspect Ratio largeur/hauteur
     * @param near Plan proche
     * @param far Plan lointain
     */
    static Matrix4x4 perspective(float fov, float aspect, float near, float far);
    
    // ======================
    // OPÉRATEURS
    // ======================
    
    /**
     * @brief Multiplication de matrices
     */
    Matrix4x4 operator*(const Matrix4x4& other) const;
    
    /**
     * @brief Multiplication matrice * vecteur (transformation de point)
     * @return Vecteur transformé avec division perspective si w != 1
     */
    Vector3D operator*(const Vector3D& vec) const;
    
    // ======================
    // TRANSFORMATIONS
    // ======================
    
    /**
     * @brief Transforme un point 3D (avec translation)
     * @param point Point à transformer
     * @return Point transformé (avec division perspective)
     */
    Vector3D transformPoint(const Vector3D& point) const;
    
    /**
     * @brief Transforme un vecteur 3D (sans translation)
     * @param vec Vecteur à transformer
     * @return Vecteur transformé (normales, directions)
     */
    Vector3D transformVector(const Vector3D& vec) const;
    
    // ======================
    // ACCÈS AUX ÉLÉMENTS
    // ======================
    
    /**
     * @brief Accès en lecture/écriture
     */
    float& at(int row, int col);
    
    /**
     * @brief Accès en lecture seule
     */
    float at(int row, int col) const;
    
    /**
     * @brief Obtenir un pointeur vers les données (pour OpenGL si besoin)
     */
    const float* data() const;
    
    // ======================
    // UTILITAIRES
    // ======================
    
    /**
     * @brief Affiche la matrice (pour debug)
     */
    void print() const;
    
    /**
     * @brief Transpose la matrice
     */
    Matrix4x4 transposed() const;
    
    /**
     * @brief Calcule l'inverse (simplifié pour transformations affines)
     */
    Matrix4x4 inverse() const;
};

#endif // MATRIX4X4_H