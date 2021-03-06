#include "billboard_list.h"

#define NUM_ROWS 10
#define NUM_COLUMNS 10

BillboardList::BillboardList()
{
    m_pTexture = NULL;
    m_VB = 0xFFFFFFFF;
}

BillboardList::~BillboardList()
{
    delete m_pTexture;
    
    if (m_VB != 0xFFFFFFFF)
    {
        glDeleteBuffers(1, &m_VB);
    }
}
 
bool BillboardList::Init(const std::string& TexFilename)
{
    m_pTexture = new Texture(GL_TEXTURE_2D, TexFilename.c_str());   
    if (!m_pTexture->Load()) {
        return false;
    }
    CreatePositionBuffer();
    if (!m_technique.Init()) {
        return false;
    }
    return true;
}

void BillboardList::CreatePositionBuffer()
{    
    glm::vec3 Positions[NUM_ROWS * NUM_COLUMNS];
    
    for (unsigned int j = 0 ; j < NUM_ROWS ; j++) {
        for (unsigned int i = 0 ; i < NUM_COLUMNS ; i++) {
            glm::vec3 Pos((float)i, 0.0f, (float)j);            
            Positions[j * NUM_COLUMNS + i] = Pos;
        }
    }
    glGenBuffers(1, &m_VB);
  	glBindBuffer(GL_ARRAY_BUFFER, m_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), &Positions[0], GL_STATIC_DRAW);
}

void BillboardList::Render(const glm::mat4& VP, const glm::vec3& CameraPos)
{
    m_technique.Enable();
    m_technique.SetVP(VP);
    m_technique.SetCameraPosition(CameraPos);
    m_pTexture->Bind(GL_TEXTURE0);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_VB);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position
    
    glDrawArrays(GL_POINTS, 0, NUM_ROWS * NUM_COLUMNS);
    
    glDisableVertexAttribArray(0);
}