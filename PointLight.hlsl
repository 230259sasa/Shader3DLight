//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //�@�������[���h���W�ɑΉ�������s�񁁉�]��
    float4 diffuseColor; //�g�U���ˌW��
    float4 lightVec; //���s�����̃x�N�g��
    float2 factor; //diffuseFactor
    bool isTextured; //tex���\���Ă��邩
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 color : COLOR; //�F�i���邳�j
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
    outData.uv = uv;
    
    //float4 light = float4(1, 1, -1, 0); //�����x�N�g���̋t�x�N�g��
    normal = mul(normal, matW);
    normal.w = 0;
    normal = normalize(normal);
    
    float4 light = lightVec - outData.pos;
    light = normalize(light); //�P�ʃx�N�g����
    
    outData.color = clamp(dot(normal, light), 0, 1);
    //outData.color = normal;
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambentSource = { 0.5, 0.5, 0.5, 1.0 }; //�����̋���
    float4 diffuse;
    float4 ambient;
    
    //float4 light = lightVec - inData.pos;
    //light = normalize(light); //�P�ʃx�N�g����
    //ambentSource = clamp(dot(inData.color, light), 0, 1);
    ambentSource = inData.color;
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.color * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;
    }
    return diffuseColor + ambient;
}