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
	//SRT‚Ì‡”Ô‚Å‚©‚¯‚é‚Æƒ[ƒ‹ƒh•ÏŠ·‚É‚È‚é
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

	//Z,X,Y‚Ì‡”Ô‚Ås—ñ‚ğ‚©‚¯‚é‚ÆDirectX‚Ì‰ñ“]‚É‚È‚é
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
		return matScale_ * matRotate_ * matTranslate_;//s*r*t‚ğ‹‚ß‚é
	}
}

XMMATRIX Transform::GetNormalMatrix()
{
	return (matRotate_ * XMMatrixInverse(nullptr, matScale_));
}
