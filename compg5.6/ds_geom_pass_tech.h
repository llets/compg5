#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "technique.h"
#include <glm/glm.hpp>

class DSGeomPassTech : public Technique {
public:

    DSGeomPassTech();

    virtual bool Init();

    void SetWVP(const glm::mat4& WVP);
    void SetWorldMatrix(const glm::mat4& WVP);
	void SetColorTextureUnit(unsigned int TextureUnit);

private:

    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
	GLuint m_colorTextureUnitLocation;
};


#endif