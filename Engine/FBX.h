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
	enum RENDER_STATE {
		RENDER_3D,
		RENDER_POINT
	};
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse; 
		XMFLOAT4 specular; //���ʔ��ˌW��
		XMFLOAT4 shininess;//���ʔ��˂̃p�����[�^
		XMFLOAT4 ambient; //�����̔��ˌW���i�F�H
		XMFLOAT4 factor; //�X�J��
	};
	//�R���X�^���g�o�b�t�@�[:�@�A�v��������A�V�F�[�_�[�ɖ��t���[���n���������
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			//�X�N���[���ϊ��}�g���N�X
		XMMATRIX	matW;			//���[���h�ϊ�
		XMMATRIX	matNormal;		//�@�����[���h�ϊ��p
		XMFLOAT4	diffuseColor;	//�F(RGB�̊g�U���ˌW��)
		//XMFLOAT4	globalLightVec;	//�����ʒu
		XMFLOAT4	diffuseFactor;	//�g�U���̔��ˌW��
		XMFLOAT4	ambientColor;
		XMFLOAT4	specularColor;
		XMFLOAT4	shininess;
		int isTextured;
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //�m�[�}���ǉ��i�@���x�N�g���j
	};

	RENDER_STATE state_;

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
	void	Update();//deletesiro
};

