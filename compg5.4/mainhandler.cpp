#include "mainhandler.h"

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

#define NUM_ROWS 50
#define NUM_COLS 20
#define NUM_INSTANCES NUM_ROWS * NUM_COLS

float RandomFloat()
{
	float Max = RAND_MAX;
	return ((float)rand() / Max);
}

mainhandler::mainhandler()
{
	m_pGameCamera = nullptr;
	m_pEffect = nullptr;
	m_scale = 0.0f;
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_pMesh = nullptr;
	m_frameCount = 0;
	m_fps = 0.0f;
}

mainhandler::~mainhandler()
{
	delete (m_pEffect);
	delete (m_pGameCamera);
	delete (m_pMesh);
}

bool mainhandler::Init()
{
	glm::vec3 Pos(7.0f, 3.0f, 0.0f);
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
	m_pEffect->SetColor(0, glm::vec4(1.0f, 0.5f, 0.5f, 0.0f));
	m_pEffect->SetColor(1, glm::vec4(0.5f, 1.0f, 1.0f, 0.0f));
	m_pEffect->SetColor(2, glm::vec4(1.0f, 0.5f, 1.0f, 0.0f));
	m_pEffect->SetColor(3, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

	m_pMesh = new Mesh();

	if (!m_pMesh->LoadMesh("C:/src/test/spider.obj")) {
		return false;
	}

	m_time = glutGet(GLUT_ELAPSED_TIME);

	CalcPositions();

	return true;
}

void mainhandler::Run()
{
	GLUTBackendRun(this);
}

void mainhandler::RenderSceneCB()
{
	CalcFPS();

	m_scale += 0.005f;

	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pEffect->Enable();
	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Rotate(0.0f, 90.0f, 0.0f);
	p.Scale(0.005f, 0.005f, 0.005f);

	glm::mat4 WVPMatrics[NUM_INSTANCES];
	glm::mat4 WorldMatrices[NUM_INSTANCES];

	for (unsigned int i = 0; i < NUM_INSTANCES; i++) {
		glm::vec3 Pos(m_positions[i]);
		Pos.y += sinf(m_scale) * m_velocity[i];
		p.WorldPos(Pos);
		WVPMatrics[i] = glm::transpose(p.GetWVPTrans());
		WorldMatrices[i] = glm::transpose(p.GetWorldTrans());
	}

	m_pMesh->Render(NUM_INSTANCES, WVPMatrics, WorldMatrices);

	RenderFPS();

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

void mainhandler::CalcFPS()
{
	m_frameCount++;

	int time = glutGet(GLUT_ELAPSED_TIME);

	if (time - m_time > 1000) {
		m_fps = (float)m_frameCount * 1000.0f / (time - m_time);
		m_time = time;
		m_frameCount = 0;
	}
}

void mainhandler::RenderFPS()
{
	char text[32];
	snprintf(text, sizeof(text), "FPS: %.2f", m_fps);
}

void mainhandler::CalcPositions()
{
	for (unsigned int i = 0; i < NUM_ROWS; i++) {
		for (unsigned int j = 0; j < NUM_COLS; j++) {
			unsigned int Index = i * NUM_COLS + j;
			m_positions[Index].x = (float)j;
			m_positions[Index].y = RandomFloat() * 5.0f;
			m_positions[Index].z = (float)i;
			m_velocity[Index] = RandomFloat();
			if (i & 1) {
				m_velocity[Index] *= (-1.0f);
			}
		}
	}
}