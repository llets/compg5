#ifndef PIPELINE_H
#define	PIPELINE_H

#include <glm/glm.hpp>
#include "structs.h"

class Pipeline
{
public:
    Pipeline()
    {
        m_scale      = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos   = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }
    
    void WorldPos(const glm::vec3& Pos)
    {
        m_worldPos = Pos;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(const ProjInfo& p)
    {
        m_persProjInfo = p;
    }

    void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    const glm::mat4& GetVPTrans();
    const glm::mat4& GetWVPTrans();
    const glm::mat4& GetWorldTrans();

private:
    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;

    ProjInfo m_persProjInfo;

    struct {
        glm::vec3 Pos;
        glm::vec3 Target;
        glm::vec3 Up;
    } m_camera;

    glm::mat4 m_WVPtransformation;
    glm::mat4 m_VPTtransformation;
    glm::mat4 m_WorldTransformation;
};
#endif