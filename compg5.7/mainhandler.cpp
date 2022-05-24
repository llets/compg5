#include "mainhandler.h"

mainhandler::mainhandler()
{
	m_pGameCamera = NULL;
	m_scale = 0.0f;

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;

	m_frameCount = 0;
	m_fps = 0.0f;

	InitLights();
	InitBoxPositions();

	m_time = glutGet(GLUT_ELAPSED_TIME);
}


mainhandler::~mainhandler()
{
	delete(m_pGameCamera);
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
	m_DSGeomPassTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	if (!m_DSPointLightPassTech.Init()) {
		printf("Error initializing DSPointLightPassTech\n");
		return false;
	}

	m_DSPointLightPassTech.Enable();

	m_DSPointLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSPointLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSPointLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSPointLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!m_DSDirLightPassTech.Init()) {
		printf("Error initializing DSDirLightPassTech\n");
		return false;
	}

	m_DSDirLightPassTech.Enable();

	m_DSDirLightPassTech.SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSDirLightPassTech.SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSDirLightPassTech.SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSDirLightPassTech.SetDirectionalLight(m_dirLight);
	m_DSDirLightPassTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glm::mat4 WVP;
	WVP = glm::mat4(1.0f);
	m_DSDirLightPassTech.SetWVP(WVP);

	if (!m_nullTech.Init()) {
		return false;
	}

	if (!m_quad.LoadMesh("C:/src/test/quad.obj")) {
		return false;
	}

	if (!m_box.LoadMesh("C:/src/test/box.obj")) {
		return false;
	}

	if (!m_bsphere.LoadMesh("C:/src/test/sphere.obj")) {
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

	m_gbuffer.StartFrame();

	DSGeometryPass();

	glEnable(GL_STENCIL_TEST);

	for (unsigned int i = 0; i < (sizeof(m_pointLight) / sizeof(m_pointLight[0])); i++) {
		DSStencilPass(i);
		DSPointLightPass(i);
	}

	glDisable(GL_STENCIL_TEST);

	DSDirectionalLightPass();

	DSFinalPass();

	RenderFPS();

	glutSwapBuffers();
}

void mainhandler::DSGeometryPass()
{
	m_DSGeomPassTech.Enable();

	m_gbuffer.BindForGeomPass();

	// Only the geometry pass updates the depth buffer
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	Pipeline p;
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	p.Rotate(0.0f, m_scale, 0.0f);

	for (unsigned int i = 0; i < (sizeof(m_boxPositions) / sizeof(m_boxPositions[0])); i++) {
		p.WorldPos(m_boxPositions[i]);
		m_DSGeomPassTech.SetWVP(p.GetWVPTrans());
		m_DSGeomPassTech.SetWorldMatrix(p.GetWorldTrans());
		m_box.Render();
	}

	glDepthMask(GL_FALSE);
}

void mainhandler::DSStencilPass(unsigned int PointLightIndex)
{
	m_nullTech.Enable();

	m_gbuffer.BindForStencilPass();
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	Pipeline p;
	p.WorldPos(m_pointLight[PointLightIndex].Position);
	float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
	p.Scale(BBoxScale, BBoxScale, BBoxScale);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_nullTech.SetWVP(p.GetWVPTrans());
	m_bsphere.Render();
}

void mainhandler::DSPointLightPass(unsigned int PointLightIndex)
{
	m_gbuffer.BindForLightPass();

	m_DSPointLightPassTech.Enable();
	m_DSPointLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	Pipeline p;
	p.WorldPos(m_pointLight[PointLightIndex].Position);
	float BBoxScale = CalcPointLightBSphere(m_pointLight[PointLightIndex]);
	p.Scale(BBoxScale, BBoxScale, BBoxScale);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	m_DSPointLightPassTech.SetWVP(p.GetWVPTrans());
	m_DSPointLightPassTech.SetPointLight(m_pointLight[PointLightIndex]);
	m_bsphere.Render();
	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void mainhandler::DSDirectionalLightPass()
{
	m_gbuffer.BindForLightPass();

	m_DSDirLightPassTech.Enable();
	m_DSDirLightPassTech.SetEyeWorldPos(m_pGameCamera->GetPos());

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_quad.Render();

	glDisable(GL_BLEND);
}

void mainhandler::DSFinalPass()
{
	m_gbuffer.BindForFinalPass();
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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

float mainhandler::CalcPointLightBSphere(const PointLight& Light)
{
	float MaxChannel = fmax(fmax(Light.Color.x, Light.Color.y), Light.Color.z);

	float ret = (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.DiffuseIntensity)))
		/
		2 * Light.Attenuation.Exp;

	return ret;
}

void mainhandler::InitLights()
{
	m_spotLight.AmbientIntensity = 0.0f;
	m_spotLight.DiffuseIntensity = 0.9f;
	m_spotLight.Color = COLOR_WHITE;
	m_spotLight.Attenuation.Linear = 0.01f;
	m_spotLight.Position = glm::vec3(-20.0, 20.0, 5.0f);
	m_spotLight.Direction = glm::vec3(1.0f, -1.0f, 0.0f);
	m_spotLight.Cutoff = 20.0f;

	m_dirLight.AmbientIntensity = 0.1f;
	m_dirLight.Color = COLOR_CYAN;
	m_dirLight.DiffuseIntensity = 0.5f;
	m_dirLight.Direction = glm::vec3(1.0f, 0.0f, 0.0f);

	m_pointLight[0].DiffuseIntensity = 0.2f;
	m_pointLight[0].Color = COLOR_GREEN;
	m_pointLight[0].Position = glm::vec3(0.0f, 1.5f, 5.0f);
	m_pointLight[0].Attenuation.Constant = 0.0f;
	m_pointLight[0].Attenuation.Linear = 0.0f;
	m_pointLight[0].Attenuation.Exp = 0.3f;

	m_pointLight[1].DiffuseIntensity = 0.2f;
	m_pointLight[1].Color = COLOR_RED;
	m_pointLight[1].Position = glm::vec3(2.0f, 0.0f, 5.0f);
	m_pointLight[1].Attenuation.Constant = 0.0f;
	m_pointLight[1].Attenuation.Linear = 0.0f;
	m_pointLight[1].Attenuation.Exp = 0.3f;

	m_pointLight[2].DiffuseIntensity = 0.2f;
	m_pointLight[2].Color = COLOR_BLUE;
	m_pointLight[2].Position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_pointLight[2].Attenuation.Constant = 0.0f;
	m_pointLight[2].Attenuation.Linear = 0.0f;
	m_pointLight[2].Attenuation.Exp = 0.3f;
}

void mainhandler::InitBoxPositions()
{
	m_boxPositions[0] = glm::vec3(0.0f, 0.0f, 5.0f);
	m_boxPositions[1] = glm::vec3(6.0f, 1.0f, 10.0f);
	m_boxPositions[2] = glm::vec3(-5.0f, -1.0f, 12.0f);
	m_boxPositions[3] = glm::vec3(4.0f, 4.0f, 15.0f);
	m_boxPositions[4] = glm::vec3(-4.0f, 2.0f, 20.0f);
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