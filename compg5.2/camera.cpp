#include <GL/freeglut.h>
#include "camera.h"

const static int MARGIN = 10;
static float EDGE_STEP = 1.0f;

glm::mat4 InitTranslationTransf(float x, float y, float z)
{
	glm::mat4 m;
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return m;
}

glm::mat4 InitTranslationTransf(const glm::vec3& Pos)
{
	return InitTranslationTransf(Pos.x, Pos.y, Pos.z);
}

glm::mat4 InitCameraTransf(const glm::vec3& Target, const glm::vec3& Up)
{
	glm::vec3 N = Target;
	glm::normalize(N);

	glm::vec3 U;
	U = glm::cross(Up, N);
	glm::normalize(U);

	glm::vec3 V = glm::cross(N,U);
	glm::mat4 m;
	m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
	m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
	m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
	return m;
}

glm::mat4 InitCameraTransf(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
	glm::mat4 CameraTranslation;
	CameraTranslation = InitTranslationTransf(-Pos.x, -Pos.y, -Pos.z);

	glm::mat4 CameraRotateTrans;
	CameraRotateTrans = InitCameraTransf(Target, Up);

	return CameraTranslation * CameraRotateTrans;
}

glm::vec4 mult44(const glm::vec4& l, const glm::vec4& r)
{
	const float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	const float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
	const float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
	const float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

	glm::vec4 ret(x, y, z, w);

	return ret;
}

glm::vec4 mult43(const glm::vec4& q, const glm::vec3& v)
{
	const float w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);
	const float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
	const float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
	const float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

	glm::vec4 ret(x, y, z, w);

	return ret;
}

void Rotate(glm::vec3& vector, float Angle, const glm::vec3& Axe)
{
	const float SinHalfAngle = sinf(glm::radians(Angle / 2));
	const float CosHalfAngle = cosf(glm::radians(Angle / 2));

	const float Rx = Axe.x * SinHalfAngle;
	const float Ry = Axe.y * SinHalfAngle;
	const float Rz = Axe.z * SinHalfAngle;
	const float Rw = CosHalfAngle;
	glm::vec4 RotationQ(Rx, Ry, Rz, Rw);

	glm::vec4 ConjugateQ = glm::vec4(-RotationQ.x, -RotationQ.y, -RotationQ.z, RotationQ.w);
	glm::vec4 W = mult44(mult43(RotationQ, vector), ConjugateQ);

	vector.x = W.x;
	vector.y = W.y;
	vector.z = W.z;
}

Camera::Camera(int WindowWidth, int WindowHeight)
{
	m_windowWidth = WindowWidth;
	m_windowHeight = WindowHeight;
	m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_target = glm::vec3(0.0f, 0.0f, 1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const  glm::vec3& Pos, const  glm::vec3& Target, const glm::vec3& Up)
{
	m_windowWidth = WindowWidth;
	m_windowHeight = WindowHeight;
	m_pos = Pos;

	m_target = Target;
	glm::normalize(m_target);

	m_up = Up;
	glm::normalize(m_up);

	Init();
}

void Camera::Init()
{
	glm::vec3 HTarget(m_target.x, 0.0, m_target.z);
	glm::normalize(HTarget);

	float Angle = glm::degrees(asin(abs(HTarget.z)));

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = 360.0f - Angle;
		}
		else
		{
			m_AngleH = 180.0f + Angle;
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = Angle;
		}
		else
		{
			m_AngleH = 180.0f - Angle;
		}
	}

	m_AngleV = -glm::degrees(asin(m_target.y));

	m_OnUpperEdge = false;
	m_OnLowerEdge = false;
	m_OnLeftEdge = false;
	m_OnRightEdge = false;
	m_mousePos.x = m_windowWidth / 2;
	m_mousePos.y = m_windowHeight / 2;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void Camera::OnKeyboard(unsigned char Key)
{
	switch (Key) {

	case GLUT_KEY_UP:
		m_pos += (m_target * m_speed);
		break;

	case GLUT_KEY_DOWN:
		m_pos -= (m_target * m_speed);
		break;

	case GLUT_KEY_LEFT:
	{
		glm::vec3 Left = glm::cross(m_target, m_up);
		glm::normalize(Left);
		Left *= m_speed;
		m_pos += Left;
	}
	break;

	case GLUT_KEY_RIGHT:
	{
		glm::vec3 Right = glm::cross(m_up, m_target);
		glm::normalize(Right);
		Right *= m_speed;
		m_pos += Right;
	}
	break;

	case GLUT_KEY_PAGE_UP:
		m_pos.y += m_speed;
		break;

	case GLUT_KEY_PAGE_DOWN:
		m_pos.y -= m_speed;
		break;

	case '+':
		m_speed += 0.1f;
		printf("Speed changed to %f\n", m_speed);
		break;

	case '-':
		m_speed -= 0.1f;
		if (m_speed < 0.1f) {
			m_speed = 0.1f;
		}
		printf("Speed changed to %f\n", m_speed);
		break;
	}
}

void Camera::OnMouse(int x, int y)
{
	int DeltaX = x - m_mousePos.x;
	int DeltaY = y - m_mousePos.y;

	m_mousePos.x = x;
	m_mousePos.y = y;

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 50.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			m_OnLeftEdge = true;
		}
		else if (x >= (m_windowWidth - MARGIN)) {
			m_OnRightEdge = true;
		}
	}
	else {
		m_OnLeftEdge = false;
		m_OnRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			m_OnUpperEdge = true;
		}
		else if (y >= (m_windowHeight - MARGIN)) {
			m_OnLowerEdge = true;
		}
	}
	else {
		m_OnUpperEdge = false;
		m_OnLowerEdge = false;
	}

	Update();
}

void Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (m_OnLeftEdge) {
		m_AngleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (m_OnRightEdge) {
		m_AngleH += EDGE_STEP;
		ShouldUpdate = true;
	}

	if (m_OnUpperEdge) {
		if (m_AngleV > -90.0f) {
			m_AngleV -= EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	else if (m_OnLowerEdge) {
		if (m_AngleV < 90.0f) {
			m_AngleV += EDGE_STEP;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		Update();
	}
}

void Camera::Update()
{
	glm::vec3 Yaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	glm::vec3 View(1.0f, 0.0f, 0.0f);
	Rotate(View, m_AngleH, Yaxis);
	glm::normalize(View);

	// Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 U = glm::cross(Yaxis, View);
	glm::normalize(U);
	Rotate(View, m_AngleV, U);

	m_target = View;
	glm::normalize(m_target);

	m_up = glm::cross(m_target, U);
	glm::normalize(m_up);
}

glm::mat4 Camera::GetMatrix()
{
	glm::mat4 CameraTransformation;
	CameraTransformation = InitCameraTransf(m_pos, m_target, m_up);

	return CameraTransformation;
}
