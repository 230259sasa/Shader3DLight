#pragma once
#include "Direct3D.h"
#include<fbxsdk.h>
#include <DirectXMath.h>
#include "Texture.h"
#include<vector>
#include"Transform.h"
#include<filesystem>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class FBX
{
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};
	//�R���X�^���g�o�b�t�@�[:�@�A�v��������A�V�F�[�_�[�ɖ��t���[���n���������
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4 diffuseColor;
		int isTextured;
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //�m�[�}���ǉ��i�@���x�N�g���j
	};

	ID3D11Buffer* pVertexBuffer_;//���_�o�b�t�@�p������
	ID3D11Buffer** pIndexBuffer_;//�C���f�b�N�X�o�b�t�@�p������
	ID3D11Buffer* pConstantBuffer_;//�R���X�^���g�o�b�t�@�p������
	std::vector<MATERIAL> pMaterialList_;
	std::vector<int> indexCount_;
	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	//int indexNum_;
	int materialCount_;	//�}�e���A���̌�
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
public:
	FBX();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
};

