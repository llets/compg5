#include "mainhandler.h"
#include "windows.h"
#include <winsock.h>
#include <ctime>

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;
	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

static long long GetCurrentTimeMillis()
{
	timeval t;
	gettimeofday(&t, NULL);
	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
}

mainhandler::mainhandler()
{
	m_pLightingTechnique = nullptr;
	m_pGameCamera = nullptr;
	m_pGround = nullptr;
	m_pTexture = nullptr;
	m_pNormalMap = nullptr;

	m_dirLight.AmbientIntensity = 0.2f;
	m_dirLight.DiffuseIntensity = 0.8f;
	m_dirLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_dirLight.Direction = glm::vec3(1.0f, 0.0f, 0.0f);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_currentTimeMillis = GetCurrentTimeMillis();
}

mainhandler::~mainhandler()
{
	delete (m_pLightingTechnique);
	delete (m_pGameCamera);
	delete (m_pGround);
	delete (m_pTexture);
	delete (m_pNormalMap);
}

bool mainhandler::Init()
{
	glm::vec3 Pos(0.0f, 0.4f, -0.5f);
	glm::vec3 Target(0.0f, 0.2f, 1.0f);
	glm::vec3 Up(0.0, 1.0f, 0.0f);

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	m_pLightingTechnique = new LightingTechnique();

	if (!m_pLightingTechnique->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetColorTextureUnit(0);
	m_pLightingTechnique->SetNormalMapTextureUnit(2);

	m_pGround = new Mesh();
	if (!m_pGround->LoadMesh("C:/src/test/quad.obj")) {
		return false;
	}

	m_pTexture = new Texture(GL_TEXTURE_2D, "C:/src/test/bricks.jpg");
	if (!m_pTexture->Load()) {
		return false;
	}
	m_pTexture->Bind(GL_TEXTURE0);

	m_pNormalMap = new Texture(GL_TEXTURE_2D, "C:/src/test/normal_map.jpg");

	if (!m_pNormalMap->Load()) {
		return false;
	}

	glm::vec3 ParticleSystemPos = glm::vec3(0.0f, 0.0f, 1.0f);

	return m_particleSystem.InitParticleSystem(ParticleSystemPos);
}

void mainhandler::Run()
{
	GLUTBackendRun(this);
}

void mainhandler::RenderSceneCB()
{
	long long TimeNowMillis = GetCurrentTimeMillis();
	assert(TimeNowMillis >= m_currentTimeMillis);
	unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
	m_currentTimeMillis = TimeNowMillis;
	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pLightingTechnique->Enable();

	m_pTexture->Bind(GL_TEXTURE0);
	m_pNormalMap->Bind(GL_TEXTURE2);

	Pipeline p;
	p.Scale(20.0f, 20.0f, 1.0f);
	p.Rotate(90.0f, 0.0, 0.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

	m_pGround->Render();
	m_particleSystem.Render(DeltaTimeMillis, p.GetVPTrans(), m_pGameCamera->GetPos());

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
