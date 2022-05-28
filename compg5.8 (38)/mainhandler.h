#pragma once

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <winsock.h>
#include <ctime>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "skinning_technique.h"
#include "glut_backend.h"
#include "mesh.h"

using namespace std;

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
    virtual void IdleCB();
    virtual void SpecialKeyboardCB(int Key, int x, int y);
    virtual void KeyboardCB(unsigned char Key, int x, int y);
    virtual void PassiveMouseCB(int x, int y);
    virtual void MouseCB(int Button, int State, int x, int y);

private:

    void CalcFPS();
    void RenderFPS();

    SkinningTechnique* m_pEffect;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    Mesh m_mesh;
    Vector3f m_position;
    PersProjInfo m_persProjInfo;
    int m_glutTime;
    long long m_startTime;
    int m_frameCount;
    float m_fps;
};