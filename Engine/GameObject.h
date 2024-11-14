#pragma once
#include<string>
#include<list>
#include"Transform.h"

class SphereCollider;

class GameObject
{
private:
	bool isDead_;
protected:
	std::list<GameObject*> childList_;
	Transform	transform_;
	GameObject*	pParent_;
	std::string	objectName_;
	SphereCollider* pCollider_;
public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	void KillMe();

	//inline’è‹`
	void SetPosition(XMFLOAT3 position) {
		transform_.position_ = position;
	}
	void SetPosition(float x, float y, float z) {
		SetPosition(XMFLOAT3(x, y, z));
	}
	void SetScale(float x, float y, float z) {
		transform_.scale_ = { x,y,z };
	}
	void SetRotateX(float x) { transform_.rotate_.x = x; }
	void SetRotateY(float y) { transform_.rotate_.x = y; }
	void SetRotateZ(float z) { transform_.rotate_.x = z; }

	XMFLOAT3 GetPosition() { return transform_.position_; }

	GameObject* FindObject(std::string objName);
	GameObject* GetRootJob();
	GameObject* FindChildObject(std::string objName);

	void AddCollider(SphereCollider* pColl);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);
	virtual void OnCollision(GameObject* pTarget) {};

	//template <typename T>‚Æ“¯‚¶‹@”\
	template <class T>
	T* Instantiate(GameObject* pParent)
	{
		T* pTmp = new T(pParent);
		if (pTmp != nullptr)
		{
			pTmp->Initialize();
			pParent->childList_.push_back(pTmp);
		}
		return pTmp;
	}
};
