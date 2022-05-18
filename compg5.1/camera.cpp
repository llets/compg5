#include <GL/freeglut.h>
#include "camera.h"

const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

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
	glm::vec4 W = mult44(mult43(RotationQ, vector),ConjugateQ);

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
	glm::normalize(m_target);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
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

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = 360.0f - glm::degrees(asin(HTarget.z));
		}
		else
		{
			m_AngleH = 180.0f + glm::degrees(asin(HTarget.z));
		}
	}
	else
	{
		if (HTarget.x >= 0.0f)
		{
			m_AngleH = glm::degrees(asin(-HTarget.z));
		}
		else
		{
			m_AngleH = 90.0f + glm::degrees(asin(-HTarget.z));
		}
	}

	m_AngleV = -glm::degrees(asin(m_target.y));

	m_mousePos.x = m_windowWidth / 2;
	m_mousePos.y = m_windowHeight / 2;

	glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

bool Camera::OnKeyboard(int Key)
{
	bool Ret = false;

	switch (Key) {

	case GLUT_KEY_UP:
	{
		m_pos += (m_target * STEP_SCALE);
		Ret = true;
	}
	break;

	case GLUT_KEY_DOWN:
	{
		m_pos -= (m_target * STEP_SCALE);
		Ret = true;
	}
	break;

	case GLUT_KEY_LEFT:
	{
		glm::vec3 Left = glm::cross(m_target, m_up);
		glm::normalize(Left);
		Left *= STEP_SCALE;
		m_pos += Left;
		Ret = true;
	}
	break;

	case GLUT_KEY_RIGHT:
	{
		glm::vec3 Right = glm::cross(m_up, m_target);
		glm::normalize(Right);
		Right *= STEP_SCALE;
		m_pos += Right;
		Ret = true;
	}
	break;
	}

	return Ret;
}

void Camera::OnMouse(int x, int y)
{
	if ((x == m_mousePos.x) && (y == m_mousePos.y)) return;

	const int DeltaX = x - m_mousePos.x;
	const int DeltaY = y - m_mousePos.y;

	m_AngleH += (float)DeltaX / 20.0f;
	m_AngleV += (float)DeltaY / 20.0f;

	Update();
	glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

void Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (ShouldUpdate) {
		Update();
	}
}

void Camera::Update()
{
	const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	glm::vec3 View(1.0f, 0.0f, 0.0f);
	Rotate(View,m_AngleH, Vaxis);
	glm::normalize(View);

	// Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 Haxis = glm::cross(Vaxis, View);
	glm::normalize(Haxis);
	Rotate(View,m_AngleV, Haxis);

	m_target = View;
	glm::normalize(m_target);

	m_up = glm::cross(m_target, Haxis);
	glm::normalize(m_up);
}
