#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include "helper/objmesh.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;
    std::unique_ptr<ObjMesh> mesh;
    std::unique_ptr<ObjMesh> mesh1;
    float tPrev;
    float lightAngle;
    float lightRotationSpeed;
    glm::vec4 lightPos;
    void setMatrices();
    void compile();
    void drawScene();
    void drawRoom(float rough, int metal, const glm::vec3& color);
    void drawModel();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
