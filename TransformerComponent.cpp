#include "TransformerComponent.h"

#include "gtc/matrix_transform.hpp"

//#include "../Geometry/GeoAlgo.h"

TransformerComponent::TransformerComponent() :m_matScale(1)
, m_matModel(1), m_matInverseModel(1)
, m_matToNormal(1), m_vecFacingNormal(0.f, 0.f, 1.f)
, m_matRotAroundNormal(1), m_matMove(1)
, m_matTotal(1), m_bMatTotalDirty(false)
, m_matMirror(1)
, m_vecOriginalFacingNormal(0.f, 0.f, 1.f)
{

}

TransformerComponent::~TransformerComponent()
{

}

void TransformerComponent::SetPosition(float x, float y, float z)
{
	m_matMove[3][0] = x;
	m_matMove[3][1] = y;
	m_matMove[3][2] = z;
	UpdateWorldMat();
}

void TransformerComponent::SetPosition(glm::vec3 &pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

glm::vec3 & TransformerComponent::GetPosition()
{
	return glm::vec3(m_matMove[3][0], m_matMove[3][1], m_matMove[3][2]);
}

void TransformerComponent::MoveTo(float dx, float dy, float dz)
{
	m_matMove[3][0] += dx;
	m_matMove[3][1] += dy;
	m_matMove[3][2] += dz;
	UpdateWorldMat();
}

void TransformerComponent::MoveTo(glm::vec3 &deltaPos)
{
	m_matMove[3][0] += deltaPos.x;
	m_matMove[3][1] += deltaPos.y;
	m_matMove[3][2] += deltaPos.z;
	UpdateWorldMat();
}

void TransformerComponent::SetScale(float xScale, float yScale /*= 1.f*/, float zScale /*= 1.f*/)
{
	m_matScale[0][0] = xScale;
	m_matScale[1][1] = yScale;
	m_matScale[2][2] = zScale;
	UpdateWorldMat();
}

void TransformerComponent::SetScale(float xyzScale)
{
	SetScale(xyzScale, xyzScale, xyzScale);
}

void TransformerComponent::SetScaleX(float xScale)
{
	m_matScale[0][0] = xScale;
	UpdateWorldMat();
}

void TransformerComponent::SetScaleY(float yScale)
{
	m_matScale[1][1] = yScale;
	UpdateWorldMat();
}

void TransformerComponent::SetScaleZ(float zScale)
{
	m_matScale[2][2] = zScale;
	UpdateWorldMat();
}

void TransformerComponent::SetScaleMat(glm::mat4 &aScaleMat)
{
	SetScale(aScaleMat[0][0], aScaleMat[1][1], aScaleMat[2][2]);
}

float TransformerComponent::GetRotate()
{
	return m_fRotateAngle;
}

void TransformerComponent::SetRotate(float angle)
{
	angle -= m_fRotateAngle;
	RotateTo(angle);
}

void TransformerComponent::RotateTo(float dtAngle)
{
	m_fRotateAngle += dtAngle;
	m_matRotAroundNormal = glm::rotate(m_matRotAroundNormal, dtAngle, m_vecFacingNormal);
	UpdateWorldMat();
}

glm::mat4 & TransformerComponent::GetWorldMat()
{
	if (m_bMatTotalDirty)
	{
		m_matTotal = m_matInverseModel * m_matScale * m_matMirror * m_matRotAroundNormal * m_matToNormal * m_matMove;
		m_bMatTotalDirty = true;
	}
	return m_matTotal;
}

glm::mat4 TransformerComponent::GetWorldInverseMat()
{
	auto wm = GetWorldMat();
	return glm::inverse(wm);
}

// do not use this interface, otherwise, can not break this mat to other mats, only for inner use
// override this function, so that set the real model to the real position, otherwise all are meaningless
void TransformerComponent::SetWorldMat(glm::mat4 &aWorldMat)
{
	m_matTotal = aWorldMat;
}

void TransformerComponent::SetFacingNormal(glm::vec3 &aNormal)
{
	auto tNor = glm::normalize(aNormal);
	if (IsVec3DEqual(tNor, m_vecFacingNormal))
	{
		return;
	}
	m_vecFacingNormal = tNor;
	CalMatToNormal();
	UpdateWorldMat();
}

void TransformerComponent::SetMatModel(glm::mat4 &aMat)
{
	m_matModel = aMat;
	m_matInverseModel = glm::inverse(m_matModel);
	m_bMatTotalDirty = true;
}

void TransformerComponent::SetOriginalFacingNormal(glm::vec3 &aNormal)
{
	m_vecOriginalFacingNormal = aNormal;
}

void TransformerComponent::MirrorYZPlane()
{
	m_matMirror[0] *= -1.f;
	UpdateWorldMat();
}

void TransformerComponent::MirrorXZPlane()
{
	m_matMirror[1] *= -1.f;
	UpdateWorldMat();
}

void TransformerComponent::MirrorXYPlane()
{
	m_matMirror[2] *= -1.f;
	UpdateWorldMat();
}

void TransformerComponent::UpdateWorldMat()
{
	m_bMatTotalDirty = true;
	SetWorldMat(GetWorldMat());
}

bool TransformerComponent::IsVec3DEqual(glm::vec3 &a, glm::vec3 &b)
{
	float dif = 1E-5f;
	return fabs(a.x - b.x) < dif && fabs(a.y - b.y) < dif && fabs(a.z - b.z) < dif;
}

void TransformerComponent::CalMatToNormal()
{
	//GeoAlgo::CalRotMatBetween(m_matToNormal, m_vecOriginalFacingNormal, m_vecFacingNormal);
}
