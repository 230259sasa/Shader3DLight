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
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 cos_alpha : COLOR; //色（明るさ）
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal: NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.wpos = mul(pos, matW);
    outData.uv = uv;
    
    //float4 light = float4(1, 1, -1, 0); //光源ベクトルの逆ベクトル
    float4 light = lightVec;
    light = normalize(light);//単位ベクトル化

    normal = mul(normal, matNormal);
    outData.normal = normal;
   // normal = normalize(normal);
   // normal.w = 0;
   //light.w = 0;
    
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);
    //outData.cos_alpha = saturate(dot(normalize(normal), 0));
    outData.eyev = eyePosition - mul(pos, matW);
    
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //return float4(65 / 255.0, 105 / 255.0, 225 / 255.0, 1); //ピクセルを塗る色
    //float4 myUv = { 0.125, 0.25, 0, 0 };
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 };
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv);
    //float cos_alpha = inData.cos_alpha;
    float4 ambentSource = { 0.7, 0.7, 0.7, 1.0 };//環境光の強さ
    float4 diffuse;
    float4 ambient;
    
    float3 dir = normalize(lightVec.xyz - inData.wpos.xyz);
    float4 r = reflect(normalize(inData.normal), normalize(float4(-dir, 1)));
    float4 specular = pow(saturate(dot(r, normalize(inData.eyev))), shininess) * specularColor;
    
    if (isTextured == false)
    {
       // return Id * diffuseColor * cos_alpha + Id * diffuseColor * ambentSource;
        diffuse = diffuseColor * inData.cos_alpha * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        //return Id * Kd * cos_alpha + Id * Kd * ambentSource;
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.cos_alpha * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;;
    }
    //return g_texture.Sample(g_sampler, myUv);
    return diffuse + ambient + specular;
}