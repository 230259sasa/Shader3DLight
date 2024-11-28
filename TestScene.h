#pragma once
#include "Engine\GameObject.h"
class TestScene :
    public GameObject
{
    int hModel_[3];
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    
};

