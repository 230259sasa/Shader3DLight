//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー
Texture2D g_nTexture : register(t1);

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
    bool isNormalMapped;
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
    float4 wpos : POSITION0;
    float2 uv : TEXCOORD; //UV座標
    float4 cos_alpha : COLOR; //色（明るさ）
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
    float4 Neyev : POSITION2; //ノーマルマップ用の接空間に変換された視線ベクトル
    float4 light : POSITION3;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    
    outData.pos = mul(pos, matWVP);
    outData.wpos = mul(pos, matW);
    outData.uv = uv.xy;
    //接戦、法線、従法線
    float3 tmp = cross(tangent.xyz, normal.xyz);
    float4 binormal = mul(tmp, matNormal);
    binormal = normalize(binormal);
    normal = mul(normal, matNormal);
    normal.w = 0;
    outData.normal = normalize(normal);
    tangent = mul(tangent, matNormal);
    tangent.w = 0;
    tangent = normalize(tangent);
    
    //視線ベクトル(ワールド座標)
    float4 posw = mul(pos, matW);
    outData.eyev = normalize(posw - eyePosition);
    
    //視線ベクトルを接空間に変換
    outData.Neyev.x = dot(outData.eyev,tangent);
    outData.Neyev.y = dot(outData.eyev,binormal);
    outData.Neyev.z = dot(outData.eyev,normal);
    outData.Neyev.w = 0;
    
    float4 light = lightVec;
    light.w = 0;
    light = normalize(light);
    outData.light.x = mul(light,tangent);
    outData.light.y = mul(light,binormal);
    outData.light.z = mul(light,normal);
    outData.light.w = 0;
    
    outData.cos_alpha = clamp(dot(outData.normal, light), 0, 1);
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = { 1.0, 1.0, 1.0, 1.0 };
    float4 ambentSource = { 0.9, 0.9, 0.9, 1.0 }; //環境光の強さ
    float4 diffuse;
    float4 ambient;
    
    float4 light = lightVec;
    float3 dir = normalize(light.xyz - inData.wpos.xyz); //ピクセル位置のポリゴンの3次元座標 wpos
    float color = saturate(dot(normalize(inData.normal.xyz), dir));
    float len = length(light.xyz - inData.wpos.xyz);
    float3 k = { 0.1f, 0.1f, 0.1f };
    float colA = 1.0 / (k.x + k.y * len + k.z * len * len); // 距離減衰
    float4 r = reflect(normalize(inData.normal), normalize(float4(-dir, 1)));
    float4 specular = pow(saturate(dot(r, normalize(inData.eyev))), shininess) * specularColor;
    
    if (isNormalMapped)
    {
        float4 nmap = g_nTexture.Sample(g_sampler, inData.uv)*2.0f - 1.0f;
        nmap = normalize(nmap);
        nmap.w = 0;
        float4 NL = clamp(dot(normalize(inData.light),nmap),0, 1);
        
        if (isTextured == false)
        {
            diffuse = diffuseColor * NL * factor.x;
            ambient = diffuseColor * ambentSource * factor.x;
        }
        else
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * NL * factor.x;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
        }
        return diffuse + ambient;
    }
    else
    {
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
        return diffuse + ambient + specular;
    }
    
    return diffuse + ambient + specular;
}