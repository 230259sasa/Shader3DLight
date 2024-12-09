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
		XMFLOAT2 factor;
	};
	//コンスタントバッファー:　アプリ側から、シェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			//スクリーン変換マトリクス
		XMMATRIX	matW;			//ワールド変換
		XMMATRIX	matNormal;		//法線ワールド変換用
		XMFLOAT4	diffuseColor;	//色(RGBの拡散反射係数)
		//XMFLOAT4	globalLightVec;	//光源位置
		XMFLOAT2	diffuseFactor;	//拡散光の反射係数
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

