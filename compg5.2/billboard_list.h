#ifndef BILLBOARD_LIST_H
#define	BILLBOARD_LIST_H

#include <string>
#include <glm/glm.hpp>

#include "texture.h"
#include "billboard_technique.h"

class BillboardList
{
public:
    BillboardList();    
    ~BillboardList();
    
    bool Init(const std::string& TexFilename);
    
    void Render(const glm::mat4& VP, const glm::vec3& CameraPos);

private:
    void CreatePositionBuffer();
    
    GLuint m_VB;
    Texture* m_pTexture;
    BillboardTechnique m_technique;
};

#endif