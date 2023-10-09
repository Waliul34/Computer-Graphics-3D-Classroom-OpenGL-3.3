#ifndef directionalLight_h
#define directionalLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class DirectionalLight {
public:
    glm::vec3 direction;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    int lightNumber;

    DirectionalLight(glm::vec3 dir, glm::vec4 amb, glm::vec4 diff, glm::vec4 spec, int num)
    {
        direction = dir;
        ambient = amb;
        diffuse = diff;
        specular = spec;
        lightNumber = num;
    }

    void setUpLight(Shader& lightingShader)
    {
        lightingShader.use();
        lightingShader.setVec4("directionalLight.ambient", ambient * ambientOn * isOn);
        lightingShader.setVec4("directionalLight.diffuse", diffuse * diffuseOn * isOn);
        lightingShader.setVec4("directionalLight.specular", specular * specularOn * isOn);
        lightingShader.setVec3("directionalLight.direction", direction);
    }

    void turnOff()
    {
        isOn = 0.0;
    }
    void turnOn()
    {
        isOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }

private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
    float isOn = 1.0;
};

#endif /* directionalLight_h */