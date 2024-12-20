#include "Transform.h"

Transform::Transform()
	:position_(0, 0, 0),
	rotate_(0, 0, 0),
	scale_(1, 1, 1),
	matTranslate_(XMMatrixIdentity()),
	matRotate_(XMMatrixIdentity()),
	matScale_(XMMatrixIdentity()),
	pParent_(nullptr)
{
	/*float x = XMVectorGetX(position_);
	float y = XMVectorGetY(position_);
	float z = XMVectorGetZ(position_);
	float w = XMVectorGetW(position_);*/
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
	//SRTの順番でかけるとワールド変換になる
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

	//Z,X,Yの順番で行列をかけるとDirectXの回転になる
	//digree0>radians XMConvertToRadians()
	XMMATRIX rx = XMMatrixRotationX(XMConvertToRadians(rotate_.x));
	XMMATRIX ry = XMMatrixRotationY(XMConvertToRadians(rotate_.y));
	XMMATRIX rz = XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
	matRotate_ = rz * rx * ry;

	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
	if (pParent_ != nullptr) {
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	else {
		return matScale_ * matRotate_ * matTranslate_;//s*r*tを求める
	}
}

XMMATRIX Transform::GetNormalMatrix()
{
	return (matRotate_ * XMMatrixInverse(nullptr, matScale_));
}
