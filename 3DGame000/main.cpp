/*==========================================================================================================================================================

															���C������[main.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "color.h"
#include "polygon.h"
#include "particle.h"
#include "model.h"
#include "shadow.h"
#include "player.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "meshfield.h"

//#include "game.h"
//#include "bg.h"
//#include "title.h"
//#include "door.h"
//#include "block.h"
//#include "fade.h"
//#include "loading.h"
//#include "sound.h"

//�}�N����`
#define CLASS_NAME		"WindowClass"			//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"3D�Q�[��"				//�E�C���h�E�̖��O�i�L���v�V�����ɕ\���j

#define HALF_TIME		(500)					//0.5�b
#define SEN_TIMES		(1000)					//���l��1000�{�ɂ���Ƃ��Ɏg�p
#define ONE_FLAME		(1000 / 60)				//�P�t���[��
#define MAX_MOJI		(256)					//�f�o�b�O�p�Ɏg�p�����s�̕����̌��E��

//�v���C���[�h�\����
typedef enum
{
	PLAYMODE_MODEL = ZERO,	//���f������\
	PLAYMODE_FIELD,			//�n�ʂ̑���\
	PLAYMODE_WALL,			//�ǂ̑���\
	PLAYMODE_CAMERA,		//�J��������\
	PLAYMODE_MAX
}PLAYMODE;

//�O���[�o���ϐ�
LPDIRECT3D9			g_pD3D = NULL;				//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT			g_pFont = NULL;				//�t�H���g�ւ̃|�C���^
int					g_nCountFPS = ZERO;			//FPS�J�E���^
bool				g_bFPS = true;				//FPS��\�����邩�ǂ���
bool				bPause = false;				//�|�[�Y
MODE g_mode = MODE_LOADING;						//���݂̃��[�h
#if _DEBUG
	PLAYMODE g_PlayMode = PLAYMODE_MODEL;		//�v���C���[�h�ݒ�
#else
	PLAYMODE g_PlayMode = PLAYMODE_MAX;			//�v���C���[�h�ݒ�
#endif

//==========================================================================================================================
//                                                     ���C���֐�
//==========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//���ݎ���
	DWORD dwExecLastTime;						//�Ō�ɏ�����������
	DWORD dwFrameCount;							//�t���[���J�E���g
	DWORD dwFPSLastTime;						//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�C���h�E�̃X�^�C��
		WindowProc,								//�E�C���h�E�v���V�[�W��
		NULL,									//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		NULL,									//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),			//�^�X�N�o�[�̃A�C�R��
		LoadIcon(NULL,IDC_ARROW),				//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + ONE),			//�N���C�A���g�̈�̔w�i�F
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),			//�t�@�C���̃A�C�R��
	};

	HWND hWnd;									//�E�C���h�E�n���h���i���ʎq�j
	MSG  mSg;									//���b�Z�[�W���i�[����ϐ�

	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };	//��ʃT�C�Y�̍\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(ZERO,						//�g���E�C���h�E�X�^�C��
		CLASS_NAME,									//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,								//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,						//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,								//�E�C���h�E�̍���w���W
		CW_USEDEFAULT,								//�E�C���h�E�̍���x���W
		(rect.right - rect.left),					//�E�C���h�E�̕�
		(rect.bottom - rect.top),					//�E�C���h�E�̍���
		NULL,										//�e�E�C���h�E�̃n���h��
		NULL,										//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,									//�C���X�^���g�n���h��
		NULL);										//�E�C���h�E�쐬�f�[�^

													//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);						//�E�C���h�E�̕\����Ԃ�ݒ�

	UpdateWindow(hWnd);								//�N���C�A���g�̈���X�V

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s������
		return MINUS_ONE;
	}

	//����\��ݒ�
	timeBeginPeriod(ONE);

	//�`�揈���֘A�̕ϐ��̏�����
	dwCurrentTime = ZERO;
	dwExecLastTime = timeGetTime();

	//FPS�֘A�̕ϐ��̏�����
	dwFrameCount = ZERO;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (ONE)
	{
		if (PeekMessage(&mSg, NULL, ZERO, ZERO, PM_REMOVE) != ZERO)
		{//windows�̏���
			if (mSg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&mSg);					//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&mSg);					//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= HALF_TIME)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * SEN_TIMES) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = ZERO;				//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60����1�b�o��
				//�����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;		

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				//�t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)mSg.wParam;

	return ZERO;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:										//�E�C���h�E�j���̃��b�Z�[�W
															//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(ZERO);
		break;

	case WM_KEYDOWN:										//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:										//[ESC]�L�[�������ꂽ
															//�I�����邩�̊m�F
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				MessageBox(NULL, "�܂����Ă��������ˁ`", "������", MB_OK);
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;		//�v���[���e�[�V�����p�����[�^

	//DirectX3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�����̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	/*===========================================================================
							�e��I�u�W�F�N�g�̏���������						
	============================================================================*/
	//�����_�[�X�e�[�g
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//�e�N�X�`���̌J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, ZERO, ZERO, ZERO, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	InitCamera();		//�J�����̏���������
	InitLight();		//���C�g�̏���������
	InitShadow();		//�e�̏���������
	InitWall();			//�ǂ̏���������
	InitPlayer();		//�v���C���[�̏���������
	InitParticle();		//�p�[�e�B�N���̏���������
	InitBillboard();	//�r���{�[�h�̏���������
	InitBullet();		//�e�̏���������
	InitExplosion();	//�����̏���������
	InitEffect();		//�G�t�F�N�g�̏���������
	InitMeshField();	//���b�V���t�B�[���h�̏���������
	InitModel();		//���f���̏���������
	//InitPolygon();	//�|���S���̏���������

	//�T�E���h�̏���������
	//InitSound(hWnd);

	//�t�F�[�h�̐ݒ�
	//InitFade(g_mode);

	return S_OK;
}

//�I������
void Uninit(void)
{
	//����\��߂�
	timeEndPeriod(ONE);

	/*======================================================================
							�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	//UninitGame();		//�Q�[���I��
	UninitKeyboard();	//�L�[�{�[�h�̏I������
	UninitCamera();		//�J�����̏I������
	UninitLight();		//���C�g�̏I������
	UninitWall();		//�ǂ̏I������
	UninitParticle();	//�p�[�e�B�N���̏I������
	UninitShadow();		//�e�̏I������
	UninitPlayer();		//�v���C���[�̏I������
	UninitBillboard();	//�r���{�[�h�̏I������
	UninitBullet();		//�e�̏I������
	UninitExplosion();	//�����̏I������
	UninitEffect();		//�G�t�F�N�g�̏I������
	UninitMeshField();	//���b�V���t�B�[���h�̏I������
	UninitModel();		//���f���̏I������
	//UninitPolygon();	//�|���S���̏I������
	//StopSound();		//�T�E���h�̒�~
	//UninitSound();	//�T�E���h�̏I������
	//UninitFade();		//�t�F�[�h�̏I������
	XInputEnable(false);//Xinput�����

	//�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//�X�V����
void Update(void)
{
	//FADE fadeState = GetFade();		//�t�F�[�h��Ԏ擾

	UpdateKeyboard();			//�L�[�{�[�h�̍X�V����

	if (bPause == false)
	{
		//�v���C���[�h�ɂ��X�V��������
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			UpdatePlayer();		//�v���C���[�̍X�V����
			UpdateModel();		//���f���̍X�V����
			break;

		case PLAYMODE_FIELD:
			UpdateMeshField();		//���b�V���t�B�[���h�̍X�V����
			break;

		case PLAYMODE_CAMERA:
			UpdateCamera();			//�J�����̍X�V����
			break;

		case PLAYMODE_WALL:
			UpdateWall();			//�ǂ̍X�V����
			break;

			//�S���X�V
		case PLAYMODE_MAX:
			UpdateCamera();			//�J�����̍X�V����
			UpdatePlayer();			//�v���C���[�̍X�V����
			UpdateMeshField();		//���b�V���t�B�[���h�̍X�V����
			UpdateWall();			//�ǂ̍X�V����
			break;
		}

		UpdateBullet();			//�e�̍X�V����
		UpdateExplosion();		//�����̍X�V����
		UpdateEffect();			//�G�t�F�N�g�̍X�V����
		UpdateParticle();		//�p�[�e�B�N���̍X�V����
		UpdateShadow();			//�e�̍X�V����
	}
	else
	{
		UpdateCamera();
	}

	UpdateBillboard();		//�r���{�[�h�̍X�V����
	UpdateLight();			//���C�g�̍X�V����

#if _DEBUG
	//�v���C���[�h�ύX
	PlayMode();
#endif

	if (GetKeyboardPress(DIK_BACKSPACE) == true || GetKeyboardTrigger(DIK_P) == true)
	{
		bPause = bPause ? false : true;
	}
#if 0

	if (fadeState == FADE_NONE)
	{
		switch (g_mode)
		{
		case MODE_LOADING:		//���[�f�B���O��ʂ̍X�V
			UpdateLoading();
			break;
		case MODE_GAME:			//�Q�[����ʂ̍X�V
			UpdateGame();
			break;
		}
	}

	//�t�F�[�h�̍X�V����
	//UpdateFade();
#endif
}

//�`�揈��
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(ZERO, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), RGBA_BLACK, F_TEN, ZERO);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		/*======================================================================
								�e��I�u�W�F�N�g�̕`�揈��
		========================================================================*/
#if 0
		switch (g_mode)
		{
		case MODE_LOADING:		//���[�f�B���O��ʕ`��
			DrawLoading();
			break;

		case MODE_GAME:			//�Q�[����ʕ`��
			DrawGame();
			break;
		}

		//�t�F�[�h�̕`�揈��
		DrawFade();
#endif
		SetCamera();		//�J�����̐ݒ菈��
		DrawWall();			//�ǂ̕`�揈��
		DrawMeshField();	//���b�V���t�B�[���h�̍X�V����
		DrawParticle();		//�p�[�e�B�N���̕`�揈��
		DrawEffect();		//�G�t�F�N�g�̕`�揈��
		DrawParticle();		//�p�[�e�B�N���̕`�揈��
		DrawShadow();		//�e�̕`�揈��
		//DrawBillboard();	//�r���{�[�h�̕`�揈��
		DrawBullet();		//�e�̕`�揈��
		DrawExplosion();	//�����̕`�揈��
		DrawPlayer();		//�v���C���[�̕`�揈��
		DrawModel();		//���f���̕`�揈��

		//FPS�\��������
		if (g_bFPS == true)
		{
			DrawFPS();			//FPS�`�揈��
		}

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//FPS�\�� ON / OFF
	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bFPS = g_bFPS ? false : true;
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//FPS�̕\��
void DrawFPS(void)
{
#if /*_DEBUG*/1
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	BillTest *pAlphaTest = GetAlphaTest();		//���e�X�g�̒l��\��

	wsprintf(&aStr[ZERO], "\n\n--- [ ������@ ] ---    FPS:%d\n[ F2 ] : �v���C���[�h�ύX\n[ F6 ] : �v���C���[�h : �S��", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//���e�X�g�̏�ԕ\��
	if (pAlphaTest->bAlphaTest == true)
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n���e�X�g�̒l:%d\n���e�X�g:�L��", pAlphaTest->AlphaTest);
	}
	else
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n���e�X�g�̒l:%d\n���e�X�g:����", pAlphaTest->AlphaTest);
	}

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//Z�e�X�g�̏�ԕ\��
	if (pAlphaTest->bZTest == true)
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                Z�e�X�g:�L��");
	}
	else
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                Z�e�X�g:����");
	}

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//�v���C���[�h�`��
	switch (g_PlayMode)
	{
	case PLAYMODE_MODEL:
		DrawFPSPlayer();	//�v���C���[�̏��\��
		break;

	case PLAYMODE_FIELD:
		DrawFPSField();		//�n�ʂ̏��\��
		break;

	case PLAYMODE_WALL:
		DrawFPSWall();		//�ǂ̏��\��
		break;

	case PLAYMODE_CAMERA:
		DrawFPSCamera();	//�J�����̏��\��
		break;

	case PLAYMODE_MAX:
		DrawFPSALL();	//�S�̏��\��
		break;
	}
	
#endif
}

//�v���C���[�̏��\��
void DrawFPSPlayer(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Player *pPlayer = GetPlayer();
	int X, Y, Z, x, y, z;

	wsprintf(&aStr[ZERO], "�v���C���[�h�F���f��\n\n\n\n\n\n--- [ �ړ� ] ---\n [ �� ] : ��\n [ �� ] : �E\n [ �� ] : ��\n [ �� ] : ��O\n[ Y ]    : �㏸\n[ N ]  : ���~");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-------------------------------
			�v���C���[�̈ړ���
	--------------------------------*/

	X = pPlayer->move.x;
	Y = pPlayer->move.y;
	Z = pPlayer->move.z;

	x = fabsf(pPlayer->move.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->move.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->move.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �v���C���[�̈ړ��� ] ---\n    X �̈ړ��� : %d.%d \n    Y �̈ړ��� : %d.%d \n    Z �̈ړ��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pPlayer->rot.x / 1;
	Y = pPlayer->rot.y / 1;
	Z = pPlayer->rot.z / 1;

	x = fabsf(pPlayer->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->rot.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ L SHIFT ] : ����]\n[ R SHIFT ] : �E��]\n[ ENTER ]    : ���f���ʒu���Z�b�g\n\n\n\n\n\n\n--- [ �v���C���[�̌��� ] ---\n    X �̌��� : %d.%d \n    Y �̌��� : %d.%d \n    Z �̌��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//�t�B�[���h���\��
void DrawFPSField(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "�v���C���[�h�F�t�B�[���h\n\n\n\n\n\n���ݑ���s��");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//�Ǐ��\��
void DrawFPSWall(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Wall *pWall = GetWall();	//�ǂ̏��
	int X, Y, Z, x, y, z;

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "�v���C���[�h�F��\n\n\n\n\n\n�� / �E��] : �� / ��");

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//�ǂ̃|�C���^�����̕ǂ�
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n--- [ �� 1�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//�ǂ̃|�C���^�����̕ǂ�
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �� 2�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//�ǂ̃|�C���^�����̕ǂ�
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �� 3�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);

	//�ǂ̏��\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �� 4�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//�J�������\��
void DrawFPSCamera(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Camera *pCamera = GetCamera();
	int X, Y, Z, x, y, z;

	//�O�㍶�E�ړ� �㏸ ���~ ���@�\��
	wsprintf(&aStr[ZERO], "�v���C���[�h�F�J����\n\n\n\n\n\n A / D : ���E�ړ�\n W / S : �O��ړ�\n\n Q / E : �����_����\n Z / C : ���_����\n\n  ���_�㏸ ���~ I / K\n\n�����_�㏸ ���~ U / J\n\n  �����㏸ ���~ SPACE / L SHIFT\n\nENTER : �J�����ʒu���Z�b�g");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

#if _DEBUG
	X = pCamera->rot.x / 1;
	Y = pCamera->rot.y / 1;
	Z = pCamera->rot.z / 1;

	x = fabsf(pCamera->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->rot.z * 100.0f) - fabsf(Z * 100.0f);

	//�J�����̌����\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �J�����̌��� ] ---\n    X �̌��� : %d.%d \n    Y �̌��� : %d.%d \n    Z �̌��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->posV.x / 1;
	Y = pCamera->posV.y / 1;
	Z = pCamera->posV.z / 1;

	x = fabsf(pCamera->posV.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->posV.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->posV.z * 100.0f) - fabsf(Z * 100.0f);

	//���_�̍��W �\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ ���_�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->posR.x / 1;
	Y = pCamera->posR.y / 1;
	Z = pCamera->posR.z / 1;

	x = fabsf(pCamera->posR.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->posR.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->posR.z * 100.0f) - fabsf(Z * 100.0f);

	//�����_�̍��W �\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �����_�̍��W ] --- \n    X �̈ʒu : %d.%d \n    Y �̈ʒu : %d.%d \n    Z �̈ʒu : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->fLength / ONE;
	Y = X * 100.0f;
	Z = pCamera->fLength * 100.0f;
	x = Z - Y;

	//�����_�̍��W �\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ ���_�Ԃ̋��� ] ---\n����:%d.%d", X, x);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
#endif

}

//�S�̏��\��
void DrawFPSALL(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Camera *pCamera = GetCamera();
	Player *pPlayer = GetPlayer();
	int X, Y, Z, x, y, z;

	/*-----------------------------
	�v���C���[�̌���
	------------------------------*/
	X = pPlayer->rot.x;
	Y = pPlayer->rot.y;
	Z = pPlayer->rot.z;

	x = fabsf(pPlayer->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->rot.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "�v���C���[�h�F���ׂ�\n\n\n\n\n\n--- [ �v���C���[�̌��� ] ---\n    X �̌��� : %d.%d \n    Y �̌��� : %d.%d \n    Z �̌��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
		�v���C���[�̈ړ���
	------------------------------*/

	X = pPlayer->move.x;
	Y = pPlayer->move.y;
	Z = pPlayer->move.z;

	x = fabsf(pPlayer->move.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->move.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->move.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "�v���C���[�h�F���ׂ�\n\n\n\n\n\n\n\n\n\n\n--- [ �v���C���[�̈ړ��� ] ---\n    X �̈ړ��� : %d.%d \n    Y �̈ړ��� : %d.%d \n    Z �̈ړ��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			�J�����̌���
	------------------------------*/

	X = pCamera->rot.x / 1,
	Y = pCamera->rot.y / 1,
	Z = pCamera->rot.z / 1;

	x = fabsf(pCamera->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->rot.z * 100.0f) - fabsf(Z * 100.0f);

	//�J�����̌����\��
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �J�����̌��� ] ---\n    X �̌��� : %d.%d \n    Y �̌��� : %d.%d \n    Z �̌��� : %d.%d", X, x, Y, y, Z, z);

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			�v���C���[�������
	------------------------------*/
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �v���C���[�̑������ ] ---\n[ �ړ� ] : �� / �� / �� / ��\n[ �㏸ ] : SPACE\n[ ���~ ] : L SHIFT");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
				�Ǐ]����
	------------------------------*/

	if (pCamera->bHorming == false)
	{
		/*-----------------------------
				�J�����������
		------------------------------*/
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �J�����̑������ ] ---\n[  ���_  �㏸ / ���~ ] : I / K\n[ �����_ �㏸ / ���~ ] : U / J\n[  ���_  ���� ] : Z / C\n[ �����_ ���� ] : Q / E\n\n\n\n\n\n\n[ F7 ] : �Ǐ]�ύX     ���� :OFF");
	}
	else
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ �J�����̑������ ] ---\n[  ���_  ���� ] : Z / C\n\n\n\n\n\n\n\n\n\n[ F7 ] : �Ǐ]�ύX     ���� :ON");
	}


	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			���ʑ������
	------------------------------*/

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ ���ʑ��� ] ---\n[ �㏸ ] : SPACE\n[ ���~ ] : TAB\n[ ������ ] : ENTER");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//���[�h�̐ݒ�
void SetMode(MODE mode)
{
#if 0
	//���݂̉�ʁi���[�h�j�̏I������
	switch (g_mode)
	{
	case MODE_LOADING:		//���[�f�B���O��ʏI��
		UninitLoading();
		StopSound(SOUND_LABEL_TETRIS);	//�e�g���X���y�I��
		break;
	case MODE_GAME:			//�Q�[����ʏI��
		UninitGame();
		StopSound(SOUND_LABEL_MORUDAW);	//�����_�E�I��
		break;
	}

	//�V������ʁi���[�h�j�̏���������
	switch (mode)
	{
	case MODE_LOADING:		//���[�f�B���O��ʏ�����
		InitLoading();
		PlaySound(SOUND_LABEL_TETRIS);	//�e�g���X���y�J�n
		break;
	case MODE_GAME:			//�Q�[����ʏ�����
		InitGame();
		PlaySound(SOUND_LABEL_MORUDAW);	//�����_�E�J�n
		break;
	}

	g_mode = mode;		//���݂̃��[�h��؂�ւ���
#endif
}

//���[�h�̎擾
MODE GetMode(void)
{
	return g_mode;
}

//�v���C���[�h�ύX
void PlayMode(void)
{
	//�v���C���[���[�h�ύX
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			g_PlayMode = PLAYMODE_FIELD;	//���f���@���@�t�B�[���h
			break;

		case PLAYMODE_FIELD:
			g_PlayMode = PLAYMODE_WALL;		//�t�B�[���h�@���@��
			break;

		case PLAYMODE_WALL:
			g_PlayMode = PLAYMODE_CAMERA;	//�ǁ@���@�J����
			break;

		case PLAYMODE_CAMERA:
			g_PlayMode = PLAYMODE_MAX;		//�J�����@���@�S��
			break;

		case PLAYMODE_MAX:
			g_PlayMode = PLAYMODE_MODEL;	//�S�́@���@���f��
			break;
		}
	}

	//�v���C���[�h��S�̂�
	if (GetKeyboardTrigger(DIK_F6) == true)
	{
		g_PlayMode = PLAYMODE_MAX;		//�v���C���[�h �S��
	}
}