#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include<vector>
#include"Transform.h"
using namespace DirectX;

//�R���X�^���g�o�b�t�@�[:�@�A�v��������A�V�F�[�_�[�ɖ��t���[���n���������
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;
};

//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal; //�m�[�}���ǉ��i�@���x�N�g���j
};

class Quad
{
protected:
	int vertexNum_;
	std::vector<VERTEX> vertices_;
	int indexNum_;
	std::vector<int> index_;

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;    //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	Texture* pTexture_;
public:
	Quad();
	virtual ~Quad();
	virtual HRESULT Initialize();
	//void Draw();
	void Draw(Transform& transform);
	void Release();
protected:
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture(std::string fileName);

	void PassDataToCB(Transform& transform);
	void SetBufferToPipeline();
};

