#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Rectangle.hpp"

namespace glimac {

void Rectangle::build(glm::vec3 o, glm::vec3 w,glm::vec3 h) {

    std::vector<ShapeVertex> data;

    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            ShapeVertex vertex;
            vertex.texCoords.x = i;
            vertex.texCoords.y = 1-j;
            vertex.normal = glm::normalize(glm::cross(w,h));
            vertex.position = o+w*(float)i+h*(float)j;
            data.push_back(vertex);
        }
    }
        
    m_nVertexCount = 6;

    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[2]);
    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[2]);
    m_Vertices.push_back(data[3]);

}

}
