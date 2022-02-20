#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {


class Rectangle {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(glm::vec3 origin,glm::vec3 width, glm::vec3 height);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Rectangle(glm::vec3 origin, glm::vec3 width, glm::vec3 height):
        m_nVertexCount(0) {
        build(origin, width, height); // Construction (voir le .cpp)
    }

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
    
}