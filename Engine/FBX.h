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
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse; 
		XMFLOAT4 specular; //鏡面反射係数
		XMFLOAT4 shininess;//鏡面反射のパラメータ
		XMFLOAT4 ambient; //環境光の反射係数（色？
		XMFLOAT4 factor; //スカラ
	};
	//コンスタントバッファー:　アプリ側から、シェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			//スクリーン変換マトリクス
		XMMATRIX	matW;			//ワールド変換
		XMMATRIX	matNormal;		//法線ワールド変換用
		XMFLOAT4	diffuseColor;	//色(RGBの拡散反射係数)
		//XMFLOAT4	globalLightVec;	//光源位置
		XMFLOAT4	diffuseFactor;	//拡散光の反射係数
		XMFLOAT4	ambientColor;
		XMFLOAT4	specularColor;
		XMFLOAT4	shininess;
		int isTextured;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加（法線ベクトル）
	};

	RENDER_STATE state_;

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
	void	Update();//deletesiro
};

