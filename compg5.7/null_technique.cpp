#include <limits.h>
#include <string.h>

#include "null_technique.h"

NullTechnique::NullTechnique()
{
}

bool NullTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/null_technique37.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/null_technique37.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

    if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

void NullTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)&WVP);
}