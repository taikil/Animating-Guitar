#include "Trapezoid.h"

using namespace ci;

Trapezoid::Trapezoid() {
    setup();
}
void Trapezoid::setup() {
    // Define vertices of the trapezoid
    std::vector<vec3> vertices = {
        vec3(100, 100, 0),   // Top-left
        vec3(300, 100, 0),   // Top-right
        vec3(250, 300, 0),   // Bottom-right
        vec3(150, 300, 0),   // Bottom-left

        vec3(120, 120, 100), // Top-left (extruded)
        vec3(280, 120, 100), // Top-right (extruded)
        vec3(240, 280, 100), // Bottom-right (extruded)
        vec3(180, 280, 100)  // Bottom-left (extruded)
    };

    // Define indices to form triangles
    std::vector<uint32_t> indices = {
        0, 1, 4,
        1, 5, 4,
        1, 2, 5,
        2, 6, 5,
        2, 3, 6,
        3, 7, 6,
        3, 0, 7,
        0, 4, 7,
        4, 5, 6,
        4, 6, 7,
        3, 2, 1,
        3, 1, 0
    };

    // Calculate normals
    std::vector<vec3> normals;
    for (size_t i = 0; i < vertices.size(); i += 3) {
        vec3 v0 = vertices[i];
        vec3 v1 = vertices[i + 1];
        vec3 v2 = vertices[i + 2];
        vec3 normal = normalize(cross(v1 - v0, v2 - v0));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }

    // Create a VboMesh
    gl::VboMesh::Layout layout;
    layout.attrib(geom::POSITION, 3);
    layout.attrib(geom::NORMAL, 3);
    mTrapezoidMesh = gl::VboMesh::create(vertices.size(), GL_TRIANGLES, { layout }, indices.size());
    mTrapezoidMesh->bufferAttrib(geom::POSITION, vertices.size() * sizeof(vec3), vertices.data());
    mTrapezoidMesh->bufferAttrib(geom::NORMAL, normals.size() * sizeof(vec3), normals.data());
    mTrapezoidMesh->bufferIndices(indices.size() * sizeof(uint32_t), indices.data());
}

void Trapezoid::draw() {
    gl::clear();
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::color(1, 0, 0); // Set color to red
    gl::draw(mTrapezoidMesh);
}

