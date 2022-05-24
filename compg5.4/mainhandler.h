#pragma once
#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

#define NUM_ROWS 50
#define NUM_COLS 20
#define NUM_INSTANCES NUM_ROWS * NUM_COLS

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "mainhandler.h"

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
    void CalcPositions();

    LightingTechnique* m_pEffect;
    Camera* m_pGameCamera;
    float m_scale;
    DirectionalLight m_directionalLight;
    Mesh* m_pMesh;
    ProjInfo m_persProjInfo;
    int m_time;
    int m_frameCount;
    float m_fps;
    glm::vec3 m_positions[NUM_INSTANCES];
    float m_velocity[NUM_INSTANCES];
};