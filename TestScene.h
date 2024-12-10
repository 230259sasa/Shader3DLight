#pragma once
#include "Engine\GameObject.h"
#include "Engine\Texture.h"// ID3D11Buffer

struct CONSTBUFFER_STAGE {
    XMFLOAT4 lightPosition;//光源位置
    XMFLOAT4 eyePosition;//視点位置
};

class TestScene :
    public GameObject
{
    ID3D11Buffer* pConstantBuffer_;
    int hModel_[3];
    void InitConstantBuffer();
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

