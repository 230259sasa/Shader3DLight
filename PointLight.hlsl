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
    float2 factor; //diffuseFactor
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
    //float4 color : COLOR; //�F�i���邳�j
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
    outData.normal = mul(normal, matNormal);
    outData.uv = uv;
    outData.eyev = eyePosition - mul(pos, matW);
    //outData.normal = wnormal;
    
    //float4 light = float4(1, 1, -1, 0); //�����x�N�g���̋t�x�N�g��
    
    //float4 light = normalize(lightVec - wos); //�P�ʃx�N�g����
    
    //outData.color = clamp(dot(normalize(wnormal), -light), 0, 1);
    //outData.color = normal;
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 light = lightVec * 1;
    float3 ambentSource = { 0.3, 0.3, 0.3 }; //�����̋���
    float3 diffuse;
    float3 ambient;
    float3 dir = normalize(light.xyz - inData.wpos.xyz); //�s�N�Z���ʒu�̃|���S����3�������W wpos
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