#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include<vector>
#include"Transform.h"
#include"Quad.h"
using namespace DirectX;

//コンスタントバッファー:　アプリ側から、シェーダーに毎フレーム渡したい情報



class Sprite:
	public Quad
{
public:
	Sprite() :Quad() {}//親のコンストラクタを呼ぶ
	~Sprite();
	HRESULT Initialize() override;
	HRESULT Initialize(std::string _s);
	void InitVertexData() override;
	void InitIndexData() override;
};

