#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "structs.h"

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

class mainhandler : public ICallbacks
{
public:
    mainhandler();
    ~mainhandler();
    bool Init();
    void Run();
    virtual void RenderSceneCB();
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);
private:
    LightingTechnique* m_pEffect;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    Mesh* m_pMesh1;
    Mesh* m_pMesh2;
    Mesh* m_pMesh3;
    ProjInfo m_persProjInfo;
};

