#include <limits.h>
#include <string.h>

#include "ds_geom_pass_tech.h"

DSGeomPassTech::DSGeomPassTech()
{
}

bool DSGeomPassTech::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "C:/src/test/shaders/geometry_pass36.vs")) {
		return false;
	}


	if (!AddShader(GL_FRAGMENT_SHADER, "C:/src/test/shaders/geometry_pass36.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_colorTextureUnitLocation = GetUniformLocation("gColorMap");

	if (m_WVPLocation == 0xFFFFFFFF ||
		m_WorldMatrixLocation == 0xFFFFFFFF ||
		m_colorTextureUnitLocation == 0xFFFFFFFF) {
		return false;
	}

	return true;
}


void DSGeomPassTech::SetWVP(const glm::mat4& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)&WVP);
}


void DSGeomPassTech::SetWorldMatrix(const glm::mat4& WorldInverse)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)&WorldInverse);
}


void DSGeomPassTech::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureUnitLocation, TextureUnit);
}