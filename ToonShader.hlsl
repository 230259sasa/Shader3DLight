//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW;
    float4x4 matNormal; //�@�������[���h���W�ɑΉ�������s�񁁉�]��
    float4 diffuseColor; //�g�U���ˌW��
    //float4 lightVec; //���s�����̃x�N�g��
    float4 factor; //diffuseFactor
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTextured; //tex���\���Ă��邩
};

cbuffer gStage : register(b1)
{
    float4 lightVec;
    float4 eyePosition;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 cos_alpha : COLOR; //�F�i���邳�j
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.wpos = mul(pos, matW);
    outData.uv = uv;
    
    //float4 light = float4(1, 1, -1, 0); //�����x�N�g���̋t�x�N�g��
    float4 light = lightVec;
    light = normalize(light); //�P�ʃx�N�g����

    normal = mul(normal, matNormal);
    outData.normal = normal;
   // normal = normalize(normal);
   // normal.w = 0;
   //light.w = 0;
    
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);
    //outData.cos_alpha = saturate(dot(normalize(normal), 0));
    outData.eyev = eyePosition - mul(pos, matW);
    
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    //return float4(65 / 255.0, 105 / 255.0, 225 / 255.0, 1); //�s�N�Z����h��F
    //float4 myUv = { 0.125, 0.25, 0, 0 };
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 };
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv);
    //float cos_alpha = inData.cos_alpha;
    float4 ambentSource = { 0.7, 0.7, 0.7, 1.0 }; //�����̋���
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