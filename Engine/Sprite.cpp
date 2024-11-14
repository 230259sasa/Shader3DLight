#include "Sprite.h"
#include<filesystem>

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Initialize()
{
	return E_NOTIMPL;
}

HRESULT Sprite::Initialize(std::string _s)
{
	InitVertexData();
	if (FAILED(CreateVertexBuffer())) {
		return E_FAIL;
	}

	InitIndexData();
	if (FAILED(CreateIndexBuffer())) {
		return E_FAIL;
	}

	if (FAILED(CreateConstantBuffer())) {
		return E_FAIL;
	}
	if (FAILED(LoadTexture(_s))) {
		return E_FAIL;
	}

	return S_OK;
}

void Sprite::InitVertexData()
{
	vertices_ =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0, 0.0, 0.0, 0.0)}, 	// 四角形の頂点（左上）, UV
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0, 0.0, 0.0, 0.0)},	// 四角形の頂点（右上）, UV
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0, 1.0, 0.0, 0.0)},	// 四角形の頂点（右下）, UV
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0, 1.0, 0.0, 0.0)}	// 四角形の頂点（左下）, UV
	};
	vertexNum_ = vertices_.size();
}

void Sprite::InitIndexData()
{
	index_ = { 0,2,3, 0,1,2 };
	indexNum_ = index_.size();
}
