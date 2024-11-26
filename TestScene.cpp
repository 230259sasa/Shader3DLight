#include "TestScene.h"
#include"Engine/Input.h"
#include"Engine\SceneManager.h"
#include"Engine\Model.h"

TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

void TestScene::Initialize()
{
	hModel_[0] = Model::Load("Assets/ss.fbx");
	hModel_[1] = Model::Load("Assets/ps.fbx");
}

void TestScene::Update()
{
	transform_.rotate_.y += 0.5;
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
}

void TestScene::Release()
{
}
