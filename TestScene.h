#pragma once
#include "Engine\GameObject.h"

struct CONSTBUFFER_STAGE {
    XMFLOAT4 lightPosition;//�����ʒu
    XMFLOAT4 eyePosition;//���_�ʒu
};

class TestScene :
    public GameObject
{
    int hModel_[3];
    ID3D11Buffer* pConstantBuffer_;
    void InitConstantBuffer();
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

