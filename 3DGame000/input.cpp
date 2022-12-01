/*==========================================================================================================================================================

												�L�[�{�[�h�E�R���g���[���[����[main.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "input.h"

//�}�N����`
#define NUM_KEY_MAX		(256)		//�L�[�̍ő吔
#define NUM_PAD_MAX		(14)		//�Q�[���p�b�h�̍ő�{�^����

//�O���[�o���ϐ��錾
LPDIRECTINPUT8		 g_pInput = NULL;			//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^

//�L�[�{�[�h
BYTE g_aKeyState[NUM_KEY_MAX];					//�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//�L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			//�L�[�{�[�h�̃����[�X���

//�R���g���[���[
WORD g_wGamePadStateTrigger;					//�Q�[���p�b�h�̃g���K�[���
WORD g_wGamePadStateRelease;					//�Q�[���p�b�h�̃����[�X���
XINPUT_STATE g_xInput;							//�R���g���[���[�̏��
bool g_bUseGamepad;								//�Q�[���p�b�h���g���Ă��邩�ǂ���

//�L�[�{�[�h�̏���������
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void* *)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//�L�[�{�[�h�̏I������
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//�L�[�{�[�h�ւ̃A�N�Z�X����j��
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//�L�[�{�[�h�̍X�V����
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	XINPUT_STATE xInput;			//�Q�[���p�b�h�̓��͏��

									//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = START_CNT; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (aKeyState[nCntKey] ^ g_aKeyState[nCntKey]) & g_aKeyState[nCntKey];//�L�[�{�[�h�̃����[�X����ۑ�����

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//�L�[�{�[�h�̃g���K�[����ۑ�����

			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//�L�[�{�[�h�̃v���X����ۑ�����
		}
	}

	else
	{
		//�L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}

	//�Q�[���p�b�h������擾
	if (XInputGetState(0, &xInput) == ERROR_SUCCESS)
	{
		g_bUseGamepad = true;
	}
	else
	{
		g_bUseGamepad = false;
	}

	//�Q�[���p�b�h�̃����[�X����ۑ�����
	g_wGamePadStateRelease = (xInput.Gamepad.wButtons ^ g_xInput.Gamepad.wButtons) & g_xInput.Gamepad.wButtons;

	//�Q�[���p�b�h�̃g���K�[����ۑ�����
	g_wGamePadStateTrigger = (g_xInput.Gamepad.wButtons ^ xInput.Gamepad.wButtons) & xInput.Gamepad.wButtons;

	//�Q�[���p�b�h�̃v���X����ۑ�����
	g_xInput.Gamepad.wButtons = xInput.Gamepad.wButtons;
}

//�L�[�{�[�h�̃v���X�����擾
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;		//�R�����Z�q
}

//�L�[�{�[�h�̃g���K�[�����擾
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//�L�[�{�[�h�̃����[�X�����擾
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

XINPUT_STATE *GetXInputState(void)
{
	return &g_xInput;
}

//�Q�[���p�b�h�̃v���X���
bool GetGamePadPress(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_xInput.Gamepad.wButtons & nPad ? true : false;
	}
	else
	{
		return false;
	}
}

//�Q�[���p�b�h�̃g���K�[���
bool GetGamePadTrigger(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_wGamePadStateTrigger & nPad ? true : false;
	}
	else
	{
		return false;
	}
}

//�Q�[���p�b�h�̃����[�X���
bool GetGamePadRelease(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_wGamePadStateRelease & nPad ? true : false;
	}
	else
	{
		return false;
	}
}