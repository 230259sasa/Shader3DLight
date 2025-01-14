//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

Texture2D g_toon_texture : register(t1); //テクスチャー
SamplerState g_toon_sampler : register(s1); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW;
    float4x4 matNormal; //法線をワールド座標に対応させる行列＝回転＊
    float4 diffuseColor; //拡散反射係数
    //float4 lightVec; //平行光源のベクトル
    float4 factor; //diffuseFactor
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTextured; //texが貼られているか
};

cbuffer gStage : register(b1)
{
    float4 lightVec;
    float4 eyePosition;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 cos_alpha : COLOR; //色（明るさ）
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL):SV_Position
{
    float4 outPos;
    
    normal.w = 0;
    normal = normalize(normal);
    outPos = pos + normal * 0.1;
    
    outPos = mul(outPos, matWVP);

    return outPos;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    return float4(0, 0, 0, 1.0);
}