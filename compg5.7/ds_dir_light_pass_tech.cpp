#include <limits.h>
#include <string.h>

#include "ds_dir_light_pass_tech.h"

DSDirLightPassTech::DSDirLightPassTech()
{
}

bool DSDirLightPassTech::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/light_pass37.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/dir_light_pass37.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");

    if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return DSLightPassTech::Init();
}

void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight& Light)
{
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    glm::vec3 Direction = Light.Direction;
    glm::normalize(Direction);
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}