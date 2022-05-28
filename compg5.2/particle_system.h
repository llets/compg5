#pragma once

#include <GL/glew.h>
#include "ps_update_technique.h"
#include "random_texture.h"
#include "billboard_technique.h"
#include "texture.h"

class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem();
    bool InitParticleSystem(const glm::vec3& Pos);
    void Render(int DeltaTimeMillis, const glm::mat4& VP, const glm::vec3& CameraPos);

private:

    void UpdateParticles(int DeltaTimeMillis);
    void RenderParticles(const glm::mat4& VP, const glm::vec3& CameraPos);
    bool m_isFirst;
    unsigned int m_currVB;
    unsigned int m_currTFB;
    GLuint m_particleBuffer[2];
    GLuint m_transformFeedback[2];
    PSUpdateTechnique m_updateTechnique;
    BillboardTechnique m_billboardTechnique;
    RandomTexture m_randomTexture;
    Texture* m_pTexture;
    int m_time;
};

