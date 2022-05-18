#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "billboard_list.h"
#include "structs.h"

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

class mainhandler : public ICallbacks
{
private:
	LightingTechnique* m_pLightingTechnique;
	Camera* m_pGameCamera;
	DirectionalLight m_dirLight;
	Mesh* m_pGround;
	Texture* m_pTexture;
	Texture* m_pNormalMap;
	ProjInfo m_persProjInfo;
	BillboardList m_billboardList;
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
};

