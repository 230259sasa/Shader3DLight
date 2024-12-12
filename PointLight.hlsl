//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

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
    float2 factor; //diffuseFactor
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
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    //float4 color : COLOR; //色（明るさ）
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
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
    outData.wpos = mul(pos, matW);
    outData.normal = mul(normal, matNormal);
    outData.uv = uv;
    outData.eyev = eyePosition - mul(pos, matW);
    //outData.normal = wnormal;
    
    //float4 light = float4(1, 1, -1, 0); //光源ベクトルの逆ベクトル
    
    //float4 light = normalize(lightVec - wos); //単位ベクトル化
    
    //outData.color = clamp(dot(normalize(wnormal), -light), 0, 1);
    //outData.color = normal;
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 light = lightVec * 1;
    float3 ambentSource = { 0.3, 0.3, 0.3 }; //環境光の強さ
    float3 diffuse;
    float3 ambient;
    float3 dir = normalize(light.xyz - inData.wpos.xyz); //ピクセル位置のポリゴンの3次元座標 wpos
    inData.normal.z = 0;
    float3 color = saturate(dot(normalize(inData.normal.xyz), dir));
    float len = length(light.xyz - inData.wpos.xyz);
    float3 k = { 0.1f, 0.1f, 0.1f };
    float colA = 1.0 / (k.x + k.y * len + k.z * len * len);
    
    
    float3 r = reflect(normalize(inData.normal), normalize(float4((float2)-dir,0, 0)));
    float specular = pow(saturate(dot(r, normalize((float3)inData.eyev))),shininess) * specularColor;

    
    if (isTextured == false)
    {
        diffuse = diffuseColor.xyz * color * factor.x;
        ambient = diffuseColor.xyz * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv).xyz * color * colA * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv).xyz * ambentSource * factor.x;
    }
    
    return float4(diffuse + ambient + specular, 1.0f);
}