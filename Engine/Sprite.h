#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include<vector>
#include"Transform.h"
#include"Quad.h"
using namespace DirectX;

//�R���X�^���g�o�b�t�@�[:�@�A�v��������A�V�F�[�_�[�ɖ��t���[���n���������



class Sprite:
	public Quad
{
public:
	Sprite() :Quad() {}//�e�̃R���X�g���N�^���Ă�
	~Sprite();
	HRESULT Initialize() override;
	HRESULT Initialize(std::string _s);
	void InitVertexData() override;
	void InitIndexData() override;
};

