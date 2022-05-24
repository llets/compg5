#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "engine_common.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "ds_geom_pass_tech.h"
#include "ds_point_light_pass_tech.h"
#include "ds_dir_light_pass_tech.h"
#include "null_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "gbuffer.h"
#include "lights_common.h"
#include "structs.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

class mainhandler : public ICallbacks
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    void DSGeometryPass();
    void DSStencilPass(unsigned int PointLightIndex);
    void DSPointLightPass(unsigned int PointLightIndex);
    void DSDirectionalLightPass();
    void DSFinalPass();
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);

private:

    float CalcPointLightBSphere(const PointLight& Light);
    void InitLights();
    void InitBoxPositions();
    void CalcFPS();
    void RenderFPS();

    DSGeomPassTech m_DSGeomPassTech;
    DSPointLightPassTech m_DSPointLightPassTech;
    DSDirLightPassTech m_DSDirLightPassTech;
    NullTechnique m_nullTech;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    DirectionalLight m_dirLight;
    PointLight m_pointLight[3];
    Mesh m_box;
    Mesh m_bsphere;
    Mesh m_quad;
    ProjInfo m_persProjInfo;
    int m_time;
    int m_frameCount;
    float m_fps;
    GBuffer m_gbuffer;
    glm::vec3 m_boxPositions[5];
};