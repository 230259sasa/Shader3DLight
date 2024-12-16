#include "TestScene.h"
#include"Engine/Input.h"
#include"Engine\SceneManager.h"
#include"Engine\Model.h"
#include"Engine\Direct3D.h"
#include"Engine\Camera.h"


void TestScene::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTBUFFER_STAGE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
	}
}

TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

void TestScene::Initialize()
{
	hModel_[0] = Model::Load("Assets/phon.fbx");
	hModel_[1] = Model::Load("Assets/froer2.fbx");
	hModel_[2] = Model::Load("Assets/pp.fbx");

	InitConstantBuffer();
}

void TestScene::Update()
{
	transform_.rotate_.y += 0.5;
	if (Input::IsKey(DIK_A)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.x -= 0.1;
		Direct3D::SetGlobalLightVec(p);
	}
	else if (Input::IsKey(DIK_D)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.x += 0.1;
		Direct3D::SetGlobalLightVec(p);
	}
	else if (Input::IsKey(DIK_DOWN)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.y -= 0.1;
		Direct3D::SetGlobalLightVec(p);
	}
	else if (Input::IsKey(DIK_UP)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.y += 0.1;
		Direct3D::SetGlobalLightVec(p);
	}
	else if (Input::IsKey(DIK_W)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.z += 0.1;
		Direct3D::SetGlobalLightVec(p);
	}
	else if (Input::IsKey(DIK_S)) {
		XMFLOAT4 p = Direct3D::GetGlobalLightVec();
		p.z -= 0.1;
		Direct3D::SetGlobalLightVec(p);
	}

	//�R���X�^���g�o�b�t�@�̐ݒ�ƁA�V�F�[�_�[�ւ̃R���X�^���g�o�b�t�@�̃Z�b�g��������
	CONSTBUFFER_STAGE cb;
	cb.lightPosition = Direct3D::GetGlobalLightVec();
	XMStoreFloat4(&cb.eyePosition, Camera::GetPosition());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[
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
	t.position_.x = Direct3D::GetGlobalLightVec().x;
	t.position_.y = Direct3D::GetGlobalLightVec().y;
	t.position_.z = Direct3D::GetGlobalLightVec().z;
	t.scale_ = { 0.5,0.5,0.5 };
	Model::SetTransform(hModel_[2], t);
	Model::Draw(hModel_[2]);
	/*t.position_.x = 0;
	t.position_.y = -1;
	t.rotate_.y = 0;
	Model::SetTransform(hModel_[2], t);
	Model::Draw(hModel_[2]);*/
}

void TestScene::Release()
{
}
