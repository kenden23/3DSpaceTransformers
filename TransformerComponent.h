#ifndef __TRANSFORMER_COMPONENT_H__
#define __TRANSFORMER_COMPONENT_H__

#include "glm.hpp"

class TransformerComponent
{
public:
	TransformerComponent();
	~TransformerComponent();

	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 &pos);
	glm::vec3 &GetPosition();
	void MoveTo(float dx, float dy, float dz);
	void MoveTo(glm::vec3 &deltaPos);

	void SetScale(float xScale, float yScale = 1.f, float zScale = 1.f);
	void SetScale(float xyzScale);
	void SetScaleX(float xScale);
	void SetScaleY(float yScale);
	void SetScaleZ(float zScale);
	void SetScaleMat(glm::mat4 &aScaleMat);

	float GetRotate();
	void SetRotate(float angle);
	void RotateTo(float dtAngle);

	glm::mat4 &GetWorldMat();
	glm::mat4 GetWorldInverseMat();

protected:
	virtual void SetWorldMat(glm::mat4 &aWorldMat) = 0;
public:

	void SetFacingNormal(glm::vec3 &aNormal);
	glm::vec3 &GetFacingNormal() { return m_vecFacingNormal; };
	
	// set the model matrix, local coordinate of model, for the best, normally is a unit matrix
	void SetMatModel(glm::mat4 &aMat);
	glm::mat4 &GetMatModel() { return m_matModel; }

	void SetOriginalFacingNormal(glm::vec3 &aNormal);
	glm::vec3 &GetOriginalFacingNormal() { return m_vecOriginalFacingNormal; }

	void MirrorYZPlane();
	void MirrorXZPlane();
	void MirrorXYPlane();

protected:
	void UpdateWorldMat();
	bool IsVec3DEqual(glm::vec3 &a, glm::vec3 &b);
	void CalMatToNormal();

public:
	float GetScaleX() { return m_matScale[0][0]; }
	float GetScaleY() { return m_matScale[1][1]; }
	float GetScaleZ() { return m_matScale[2][2]; }
protected:
	glm::mat4 m_matScale, m_matMove;

	glm::mat4 m_matToNormal;
	glm::vec3 m_vecFacingNormal;
	
	glm::mat4 m_matModel;
	glm::mat4 m_matInverseModel;

	glm::mat4 m_matMirror;

	glm::vec3 m_vecOriginalFacingNormal;

	bool m_bMatTotalDirty;
	glm::mat4 m_matTotal;

	float m_fRotateAngle;
	glm::mat4 m_matRotAroundNormal;

};

#endif