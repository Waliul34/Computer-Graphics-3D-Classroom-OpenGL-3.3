#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    float Kc;
    float Kl;
    float Kq;
    int lightNumber;

    SpotLight(glm::vec3 pos, glm::vec3 dir, float cuOff, float oCuOff, glm::vec4 amb, glm::vec4 diff, glm::vec4 spec, float constant, float linear, float quadratic, int num)
    {
        position = pos;
        direction = dir;
        cutOff = cuOff;
        outerCutOff = oCuOff;
        ambient = amb;
        diffuse = diff;
        specular = spec;
        Kc = constant;
        Kl = linear;
        Kq = quadratic;
        lightNumber = num;
    }

    void setUpLight(Shader& lightingShader)
    {
        lightingShader.use();
        lightingShader.setVec4("spotLight.ambient", ambient * ambientOn * isOn);
        lightingShader.setVec4("spotLight.diffuse", diffuse * diffuseOn * isOn);
        lightingShader.setVec4("spotLight.specular", specular * specularOn * isOn);
        lightingShader.setVec3("spotLight.position", position);
        lightingShader.setVec3("spotLight.direction", direction);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));
        lightingShader.setFloat("spotLight.Kc", Kc);
        lightingShader.setFloat("spotLight.Kl", Kl);
        lightingShader.setFloat("spotLight.Kq", Kq);
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

#endif /* spotLight_h */