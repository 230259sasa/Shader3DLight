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
	hModel_ = Model::Load("Assets/pp.fbx");
}

void TestScene::Update()
{
	transform_.rotate_.y += 0.5;
}

void TestScene::Draw()
{
	Model::SetTransform(hModel_,transform_);
	Model::Draw(hModel_);
}

void TestScene::Release()
{
}
