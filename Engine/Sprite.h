#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
#include "Transform.h"
#include<string>


#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete[] p; p = nullptr;}


class Sprite
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
		XMMATRIX uvTrans;//テクスチャ座標の変換行列
		XMFLOAT4 bcolor;//テクスチャとの合成色
	};

	struct VERTEX
	{
		XMVECTOR position;	
		XMVECTOR uv;		
	};

protected:
	uint64_t vertexNum_;	
	std::vector<VERTEX> vertices_;	
	ID3D11Buffer* pVertexBuffer_;

	uint64_t indexNum;	
	std::vector<int> index_;
	ID3D11Buffer* pIndexBuffer_;

	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;	

	std::string filename_;
public:
	Sprite();
	Sprite(std::string filename);
	~Sprite();

	HRESULT Initialize();

	void Draw(Transform& transform);
	void Draw(Transform& transform, RECT rect,float alpha);

	void Release();

private:
	virtual void InitVertexData();		
	HRESULT CreateVertexBuffer();		

	virtual void InitIndexData();	
	HRESULT CreateIndexBuffer();

	HRESULT CreateConstantBuffer();	

	HRESULT LoadTexture();	
	HRESULT LoadTextureS(std::string filename);

	void PassDataToCB(XMMATRIX worldMatrix);
	void SetBufferToPipeline();
};