#include "mainhandler.h"

mainhandler::mainhandler()
{
	m_pGameCamera = nullptr;
	m_pEffect = nullptr;
	m_scale = 0.0f;
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.7f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_pMesh1 = nullptr;
	m_pMesh2 = nullptr;
	m_pMesh3 = nullptr;
}

mainhandler::~mainhandler()
{
	delete (m_pEffect);
	delete (m_pGameCamera);
	delete (m_pMesh1);
	delete (m_pMesh2);
	delete (m_pMesh3);
}

bool mainhandler::Init()
{
	glm::vec3 Pos(3.0f, 7.0f, -10.0f);
	glm::vec3 Target(0.0f, -0.2f, 1.0f);
	glm::vec3 Up(0.0, 1.0f, 0.0f);

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	m_pEffect = new LightingTechnique();

	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pEffect->Enable();

	m_pEffect->SetColorTextureUnit(0);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);

	m_pMesh1 = new Mesh();
	if (!m_pMesh1->LoadMesh("C:/src/test/phoenix_ugv.md2")) {
		return false;
	}

	m_pMesh2 = new Mesh();
	if (!m_pMesh2->LoadMesh("C:/src/test/jeep.obj")) {
		return false;
	}

	m_pMesh3 = new Mesh();
	if (!m_pMesh3->LoadMesh("C:/src/test/hheli.obj")) {
		return false;
	}

	return true;
}

void mainhandler::Run()
{
	GLUTBackendRun(this);
}

void mainhandler::RenderSceneCB()
{
	m_scale += 0.01f;

	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.Rotate(0.0f, m_scale, 0.0f);
	p.SetPerspectiveProj(m_persProjInfo);

	p.Scale(0.1f, 0.1f, 0.1f);
	p.WorldPos(-6.0f, -2.0f, 10.0f);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh1->Render();

	p.Scale(0.01f, 0.01f, 0.01f);
	p.WorldPos(6.0f, -2.0f, 10.0f);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh2->Render();

	p.Scale(0.04f, 0.04f, 0.04f);
	p.WorldPos(0.0f, 6.0f, 10.0f);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh3->Render();

	glutSwapBuffers();
}

void mainhandler::IdleCB()
{
	RenderSceneCB();
}

void mainhandler::SpecialKeyboardCB(int Key, int x, int y)
{
	m_pGameCamera->OnKeyboard(Key);
}

void mainhandler::KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}

void mainhandler::PassiveMouseCB(int x, int y)
{
	m_pGameCamera->OnMouse(x, y);
}

void mainhandler::MouseCB(int Button, int State, int x, int y)
{
}