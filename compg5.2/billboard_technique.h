#ifndef BILLBOARD_TECHNIQUE_H
#define	BILLBOARD_TECHNIQUE_H

#include "technique.h"
#include <glm/glm.hpp>

class BillboardTechnique : public Technique
{
public:

    BillboardTechnique();
    virtual bool Init();
    void SetVP(const glm::mat4& VP);
    void SetCameraPosition(const glm::vec3& Pos);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetBillboardSize(float BillboardSize);

private:

    GLuint m_VPLocation;
    GLuint m_cameraPosLocation;
    GLuint m_colorMapLocation;
    GLuint m_billboardSizeLocation;
};

#endif