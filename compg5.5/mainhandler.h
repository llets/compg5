#pragma once

#define WINDOW_WIDTH  600  
#define WINDOW_HEIGHT 600

#include <string>
#include <Magick++.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "ds_geom_pass_tech.h"
#include "glut_backend.h"
#include "mesh.h"
#include "gbuffer.h"

class mainhandler : public ICallbacks
{
public:

    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    void DSGeometryPass();
    void DSLightPass();
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);

private:
    void CalcFPS();
    void RenderFPS();

    DSGeomPassTech m_DSGeomPassTech;
    Camera* m_pGameCamera;
    float m_scale;
    Mesh m_mesh;
    ProjInfo m_persProjInfo;
    int m_time;
    int m_frameCount;
    float m_fps;
    GBuffer m_gbuffer;
};