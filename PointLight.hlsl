//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //法線をワールド座標に対応させる行列＝回転＊
    float4 diffuseColor; //拡散反射係数
    float4 lightVec; //平行光源のベクトル
    float2 factor; //diffuseFactor
    bool isTextured; //texが貼られているか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 cos_alpha : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    //float4 light = float4(1, 1, -1, 0); //光源ベクトルの逆ベクトル
    float4 light = lightVec;
    light = normalize(light); //単位ベクトル化

    normal = mul(normal, matW);
    normal = normalize(normal);
    normal.w = 0;
    light.w = 0;
    
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambentSource = { 0.5, 0.5, 0.5, 1.0 }; //環境光の強さ
    float4 diffuse;
    float4 ambient;
    float4 dir;
    
    dir = lightVec - inData.pos;
    dir *= -1;
    ambentSource = saturate(dot(normalize(inData.cos_alpha), dir));
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.cos_alpha * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.cos_alpha * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
    }
    return diffuseColor + ambient;
    
    
}