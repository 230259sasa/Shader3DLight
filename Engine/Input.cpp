#include "Input.h"

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256];    //�O�t���[���ł̊e�L�[�̏��
	//�}�E�X
	LPDIRECTINPUTDEVICE8	pMouseDevice;	//�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState;				//�}�E�X�̏��
	DIMOUSESTATE prevMouseState;			//�O�t���[���̃}�E�X�̏��
	XMVECTOR mousePosition;

	void Initialize(HWND hWnd)
	{
		DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		//�}�E�X
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}

	void Update()
	{
		memcpy(&prevKeyState, &keyState, sizeof(keyState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
		//�}�E�X
		pMouseDevice->Acquire();
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	bool IsKey(int keyCode)
	{
		if ((keyState[keyCode] & 0x80) != 0)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if ((keyState[keyCode] & 0x80) != 0 &&
			(prevKeyState[keyCode] & 0x80) == 0)
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if ((keyState[keyCode] & 0x80) == 0 &&
			(prevKeyState[keyCode] & 0x80) != 0)
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		SAFE_RELEASE(pMouseDevice);
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
	}

	/////////////////////////////�@�}�E�X���擾�@//////////////////////////////////

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	//�}�E�X�̃{�^����������Ă��邩���ׂ�
	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�i�������ςȂ��͖����j
	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//�}�E�X�̃{�^�����������������ׂ�
	bool IsMouseButtonUp(int buttonCode)
	{
		//�������ĂȂ��āA�O��͉����Ă�
		if (!IsMouseButton(buttonCode) && prevMouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}
}