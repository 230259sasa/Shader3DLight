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
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};
	//コンスタントバッファー:　アプリ側から、シェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4 diffuseColor;
		int isTextured;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加（法線ベクトル）
	};

	ID3D11Buffer* pVertexBuffer_;//頂点バッファ用メモリ
	ID3D11Buffer** pIndexBuffer_;//インデックスバッファ用メモリ
	ID3D11Buffer* pConstantBuffer_;//コンスタントバッファ用メモリ
	std::vector<MATERIAL> pMaterialList_;
	std::vector<int> indexCount_;
	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	//int indexNum_;
	int materialCount_;	//マテリアルの個数
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

