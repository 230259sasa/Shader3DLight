#pragma once
#include "Engine\GameObject.h"
#include "Engine\Texture.h"// ID3D11Buffer

const int MAX_MODEL(5);

struct CONSTBUFFER_STAGE {
    XMFLOAT4 lightPosition;//ŒõŒ¹ˆÊ’u
    XMFLOAT4 eyePosition;//Ž‹“_ˆÊ’u
};

class TestScene :
    public GameObject
{
    ID3D11Buffer* pConstantBuffer_;
    int hModel_[MAX_MODEL];
    void InitConstantBuffer();
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

