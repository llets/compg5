#include "mainhandler.h"

mainhandler::mainhandler()
{
	m_pGameCamera = nullptr;
	m_scale = 0.0f;

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_frameCount = 0;
	m_fps = 0.0f;

	m_time = glutGet(GLUT_ELAPSED_TIME);
}

mainhandler::~mainhandler()
{
	delete (m_pGameCamera);
}

bool mainhandler::Init()
{
	if (!m_gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return false;
	}

	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!m_DSGeomPassTech.Init()) {
		printf("Error initializing DSGeomPassTech\n");
		return false;
	}

	m_DSGeomPassTech.Enable();
	m_DSGeomPassTech.SetColorTextureUnit(0);

	if (!m_mesh.LoadMesh("C:/src/test/phoenix_ugv.md2")) {
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
	CalcFPS();

	m_scale += 0.05f;

	m_pGameCamera->OnRender();

	DSGeometryPass();
	DSLightPass();

	RenderFPS();

	glutSwapBuffers();
}

void mainhandler::DSGeometryPass()
{
	m_DSGeomPassTech.Enable();

	m_gbuffer.BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Pipeline p;
	p.Scale(0.1f, 0.1f, 0.1f);
	p.Rotate(0.0f, m_scale, 0.0f);
	p.WorldPos(-0.8f, -1.0f, 12.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	m_DSGeomPassTech.SetWVP(p.GetWVPTrans());
	m_DSGeomPassTech.SetWorldMatrix(p.GetWorldTrans());
	m_mesh.Render();
}

void mainhandler::DSLightPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_gbuffer.BindForReading();

	GLint HalfWidth = (GLint)(WINDOW_WIDTH / 2.0f);
	GLint HalfHeight = (GLint)(WINDOW_HEIGHT / 2.0f);

	m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
	memset(text, 0, sizeof(text));
	snprintf(text, sizeof(text), "FPS: %.2f", m_fps);
}