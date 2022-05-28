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
	m_pGameCamera = NULL;
	m_pEffect = NULL;
	m_scale = 0.0f;
	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.55f;
	m_directionalLight.DiffuseIntensity = 0.9f;
	m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_frameCount = 0;
	m_fps = 0.0f;

	m_position = Vector3f(0.0f, 0.0f, 6.0f);
}

mainhandler::~mainhandler()
{
	SAFE_DELETE(m_pEffect);
	SAFE_DELETE(m_pGameCamera);
}

bool mainhandler::Init()
{
	Vector3f Pos(0.0f, 3.0f, -1.0f);
	Vector3f Target(0.0f, 0.0f, 1.0f);
	Vector3f Up(0.0, 1.0f, 0.0f);

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

	m_pEffect = new SkinningTechnique();

	if (!m_pEffect->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pEffect->Enable();

	m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_pEffect->SetDirectionalLight(m_directionalLight);
	m_pEffect->SetMatSpecularIntensity(0.0f);
	m_pEffect->SetMatSpecularPower(0);

	if (!m_mesh.LoadMesh("C:/src/test/boblampclean.md5mesh")) {
		printf("Mesh load failed\n");
		return false;
	}
	m_glutTime = glutGet(GLUT_ELAPSED_TIME);
	m_startTime = GetCurrentTimeMillis();

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

	vector<Matrix4f> Transforms;

	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;

	m_mesh.BoneTransform(RunningTime, Transforms);

	for (uint i = 0; i < Transforms.size(); i++) {
		m_pEffect->SetBoneTransform(i, Transforms[i]);
	}

	m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Scale(0.1f, 0.1f, 0.1f);

	Vector3f Pos(m_position);
	p.WorldPos(Pos);
	p.Rotate(270.0f, 180.0f, 0.0f);
	m_pEffect->SetWVP(p.GetWVPTrans());
	m_pEffect->SetWorldMatrix(p.GetWorldTrans());

	m_mesh.Render();

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

	if (time - m_glutTime > 1000) {
		m_fps = (float)m_frameCount * 1000.0f / (time - m_glutTime);
		m_glutTime = time;
		m_frameCount = 0;
	}
}

void mainhandler::RenderFPS()
{
	char text[32];
	ZERO_MEM(text);
	SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
}