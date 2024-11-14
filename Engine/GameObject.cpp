#include "GameObject.h"
#include"Direct3D.h"
#include"SphereCollider.h"

GameObject::GameObject()
	:pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent),objectName_(name),isDead_(false),pCollider_(nullptr)
{
	if (parent != nullptr) {
		this->transform_.pParent_ = &(parent->transform_);
	}
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
		(*itr)->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->isDead_)
		{
			(*itr)->ReleaseSub();
			////(*itr)->Release();
			//delete (*itr);
			//(*itr) = nullptr;
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr : childList_) {
		itr->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();
	for (auto itr : childList_) {
		itr->ReleaseSub();
	}
}

void GameObject::KillMe()
{
	isDead_ = true;
}

GameObject* GameObject::FindObject(std::string objName)
{
	/*GameObject* rootJob = GetRootJob();
	GameObject* res = rootJob->FindChildObject(objName);
	return res;*/
	return GetRootJob()->FindChildObject(objName);
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr) {
		return this;
	}
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindChildObject(std::string objName)
{
	if (this->objectName_ == objName) {
		return this;
	}
	else {
		for (auto itr : childList_) {
			GameObject* obj = itr->FindChildObject(objName);
			if (obj != nullptr);
			return obj;
		}
	}
	return nullptr;
}

void GameObject::AddCollider(SphereCollider* pColl)
{
	pCollider_ = pColl;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (this->pCollider_ == nullptr || pTarget->pCollider_ == nullptr || this == pTarget)
		return;

	XMVECTOR me = XMLoadFloat3(&(transform_.position_));
	XMVECTOR ta = XMLoadFloat3(&(pTarget->transform_.position_));
	XMVECTOR d = XMVector3Length(me - ta);
	float dist = XMVectorGetX(d);
	//“ñ‚Â‚Ì”¼Œa‚ð‡‚í‚¹‚½’l
	float rDist = pTarget->pCollider_->GetRadius() + this->pCollider_->GetRadius();
	if (dist <= rDist) {
		OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (this->pCollider_ == nullptr)
		return;
	if(pTarget->pCollider_ != nullptr)
	Collision(pTarget);

	for (auto& itr : pTarget->childList_) {
		RoundRobin(itr);
	}
}
