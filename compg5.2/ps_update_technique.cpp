#include "ps_update_technique.h"

PSUpdateTechnique::PSUpdateTechnique()
{

}

bool PSUpdateTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/ps_update28.vs")) {
        return false;
    }

    if (!AddShader(GL_GEOMETRY_SHADER, "C:/src/test/shaders/ps_update28.gs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/ps_update28.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    const GLchar* Varyings[4];
    Varyings[0] = "Type1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";
    Varyings[3] = "Age1";

    glTransformFeedbackVaryings(m_shaderProg, 4, Varyings, GL_INTERLEAVED_ATTRIBS);

    if (!Finalize()) {
        return false;
    }

    m_deltaTimeMillisLocation = GetUniformLocation("gDeltaTimeMillis");
    m_randomTextureLocation = GetUniformLocation("gRandomTexture");
    m_timeLocation = GetUniformLocation("gTime");
    m_launcherLifetimeLocation = GetUniformLocation("gLauncherLifetime");
    m_shellLifetimeLocation = GetUniformLocation("gShellLifetime");
    m_secondaryShellLifetimeLocation = GetUniformLocation("gSecondaryShellLifetime");

    if (m_deltaTimeMillisLocation == 0xFFFFFFFF ||
        m_timeLocation == 0xFFFFFFFF ||
        m_randomTextureLocation == 0xFFFFFFFF ||
        m_launcherLifetimeLocation == 0xFFFFFFFF ||
        m_shellLifetimeLocation == 0xFFFFFFFF ||
        m_secondaryShellLifetimeLocation == 0xFFFFFFFF) {
        return false;
    }

    return true;
}

void PSUpdateTechnique::SetDeltaTimeMillis(unsigned int DeltaTimeMillis)
{
    glUniform1f(m_deltaTimeMillisLocation, (float)DeltaTimeMillis);
}

void PSUpdateTechnique::SetTime(int Time)
{
    glUniform1f(m_timeLocation, (float)Time);
}

void PSUpdateTechnique::SetRandomTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_randomTextureLocation, TextureUnit);
}

void PSUpdateTechnique::SetLauncherLifetime(float Lifetime)
{
    glUniform1f(m_launcherLifetimeLocation, Lifetime);
}

void PSUpdateTechnique::SetShellLifetime(float Lifetime)
{
    glUniform1f(m_shellLifetimeLocation, Lifetime);
}

void PSUpdateTechnique::SetSecondaryShellLifetime(float Lifetime)
{
    glUniform1f(m_secondaryShellLifetimeLocation, Lifetime);
}