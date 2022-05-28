#include "billboard_technique.h"

BillboardTechnique::BillboardTechnique()
{
}

bool BillboardTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/billboard28.vs")) {
        return false;
    }

    if (!AddShader(GL_GEOMETRY_SHADER, "C:/src/test/shaders/billboard28.gs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/billboard28.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_VPLocation = GetUniformLocation("gVP");
    m_cameraPosLocation = GetUniformLocation("gCameraPos");
    m_colorMapLocation = GetUniformLocation("gColorMap");
    m_billboardSizeLocation = GetUniformLocation("gBillboardSize");

    if (m_VPLocation == 0xFFFFFFFF ||
        m_cameraPosLocation == 0xFFFFFFFF ||
        m_billboardSizeLocation == 0xFFFFFFFF ||
        m_colorMapLocation == 0xFFFFFFFF) {
        return false;
    }

    return  (glGetError() == GL_NO_ERROR);
}

void BillboardTechnique::SetVP(const glm::mat4& VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)&VP);
}

void BillboardTechnique::SetCameraPosition(const glm::vec3& Pos)
{
    glUniform3f(m_cameraPosLocation, Pos.x, Pos.y, Pos.z);
}

void BillboardTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorMapLocation, TextureUnit);
}

void BillboardTechnique::SetBillboardSize(float BillboardSize)
{
    glUniform1f(m_billboardSizeLocation, BillboardSize);
}