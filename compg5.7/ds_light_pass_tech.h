#ifndef DS_LIGHT_PASS_TECH_H
#define	DS_LIGHT_PASS_TECH_H

#include <string>
#include <glm/glm.hpp>

#include "technique.h"

class DSLightPassTech : public Technique {
public:

    DSLightPassTech();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);
    void SetPositionTextureUnit(unsigned int TextureUnit);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetNormalTextureUnit(unsigned int TextureUnit);
    void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
    void SetScreenSize(unsigned int Width, unsigned int Height);

private:
#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF
    GLuint m_WVPLocation;
    GLuint m_posTextureUnitLocation;
    GLuint m_normalTextureUnitLocation;
    GLuint m_colorTextureUnitLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_screenSizeLocation;
};
#endif