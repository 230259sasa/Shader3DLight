#include "TestScene.h"
#include"Engine/Input.h"
#include"Engine\SceneManager.h"
#include"Engine\Model.h"
#include"Engine\Direct3D.h"

TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

void TestScene::Initialize()
{
	hModel_[0] = Model::Load("Assets/ss.fbx");
	hModel_[1] = Model::Load("Assets/ps.fbx");
	hModel_[2] = Model::Load("Assets/froer.fbx");
}

void TestScene::Update()
{
	transform_.rotate_.y += 0.5;
	if (Input::IsKey(DIK_A)) {
		XMFLOAT4 l = { -5,0,-10,0 };
		Direct3D::SetGlobalLightVec(l);
	}
	else if (Input::IsKey(DIK_D)) {
		XMFLOAT4 l = { 0,-10,0,0 };
		Direct3D::SetGlobalLightVec(l);
	}
	else if (Input::IsKey(DIK_S)) {
		XMFLOAT4 l = { 0,10,0,0 };
		Direct3D::SetGlobalLightVec(l);
	}
}

void TestScene::Draw()
{
	Transform t;
	t = transform_;
	for (int i = 0; i < 2; i++) {
		t.position_.x += 2 * i;
		Model::SetTransform(hModel_[i], t);
		Model::Draw(hModel_[i]);
	}

	t.position_.x = 0;
	t.position_.y = -1;
	t.rotate_.y = 0;
	Model::SetTransform(hModel_[2], t);
	Model::Draw(hModel_[2]);
}

void TestScene::Release()
{
}
