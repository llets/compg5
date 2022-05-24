#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include <glm/glm.hpp>
#include "technique.h"

class DSGeomPassTech : public Technique {
public:

    DSGeomPassTech();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);
    void SetWorldMatrix(const glm::mat4& WVP);
    void SetColorTextureUnit(unsigned int TextureUnit);

private:
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF
    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorTextureUnitLocation;
};
#endif