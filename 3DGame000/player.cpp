/*==========================================================================================================================================================

														�v���C���[�̏���[player.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "model.h"
#include "billboard.h"

#define PLAYER_TEX_MAX			(10)		//���f���̃e�N�X�`���̍ő吔
#define PLAYER_SPEED			(0.5f)		//�v���C���[�̈ړ��X�s�[�h
#define MAX_SPEED				(4.0f)		//�v���C���[�̌��E�ړ���
#define CROSS_SPEED_COR			(0.9f)		//�΂ߕ����ւ̈ړ��ʕ␳�l

#define PLAYER_ROT_CORRECT_MOVE	(0.05f)		//�v���C���[�̐i�s�����ւ̌����C���̕␳�l

#define LIMIT_PLAYER_LEFT		(-600.0f)	//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_RIGHT		(600.0f)	//�v���C���[�̈ړ����E�n�_�@�E
#define LIMIT_PLAYER_CEELING	(100.0f)	//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_FLOAR		(0.0f)		//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_FRONT		(600.0f)	//�v���C���[�̈ړ����E�n�_�@��O
#define LIMIT_PLAYER_BACK		(-600.0f)	//�v���C���[�̈ړ����E�n�_�@��

//�v���C���[�̒e�̔��˂ɂ�����}�N��
#define BULLET_COLOR					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0)	//�e�̐F
#define PLAYER_BULLET_SPEED				(-5.0f)								//�e�̔��ˑ��x
#define PLAYER_BULLET_CORRECT_HEIGHT	(9.0f)								//�e�̔��ˈʒu�̕␳�l

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9			g_pTexturePlayer[PLAYER_TEX_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH					g_pMeshPlayer = NULL;					//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER				g_pBuffMatPlayer = NULL;				//�}�e���A���ւ̃|�C���^
DWORD						g_dwNumMatPlayer = ZERO;				//�}�e���A���̐�
Player						g_player;								//�v���C���[�̏��

//�v���C���[�̏���������
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	g_pMeshPlayer = NULL;						//���b�V��������
	g_pBuffMatPlayer = NULL;					//�}�e���A���ւ̃|�C���^������
	g_dwNumMatPlayer = ZERO;					//�}�e���A���̐�������
	ResetPlayer();								//�v���C���[�̏�񏉊���

	//X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/floating_car.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer, NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		if (pMat[nCntPlayer].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntPlayer].pTextureFilename, &g_pTexturePlayer[nCntPlayer]);
		}
	}

	//�e�̐ݒ�
	g_player.nIdxShadow = SetShadow();
}

//�v���C���[�̏I������
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	for (int nCntPlayer = START_CNT; nCntPlayer < PLAYER_TEX_MAX; nCntPlayer++)
	{
		if (g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//�v���C���[�̍X�V����
void UpdatePlayer(void)
{
	//�O��̈ʒu�X�V
	g_player.posOld = g_player.pos;

	//�v���C���[�̈ړ�����
	MovePlayer();

	//�v���C���[�̉�]����
	RotPlayer();

	//�ʒu�X�V
	g_player.pos += g_player.move;

	//�v���C���[�̈ʒu�C��
	FixPlayerPos();

	//���f���Ƃ̓����蔻��
	CollisionModel(&g_player.pos, &g_player.posOld);

	//�v���C���[�̃��Z�b�g����
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		ResetPlayer();
	}

	//�e����
	if (GetKeyboardPress(DIK_G) == true)
	{
		D3DXVECTOR3 BulletMove;

		BulletMove.x = sinf(g_player.rot.y) * PLAYER_BULLET_SPEED;
		BulletMove.z = cosf(g_player.rot.y) * PLAYER_BULLET_SPEED;
		BulletMove.y = F_ZERO;

		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYER_BULLET_CORRECT_HEIGHT, g_player.pos.z), BulletMove, BULLET_COLOR, 0.8f);
	}
	
	//�v���C���[�̉e�`��
	SetPositionShadow(g_player.nIdxShadow, g_player.pos);
}

//�v���C���[�̕`�揈��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	BillTest *pBill = GetAlphaTest();			//�r���{�[�h�̃��EZ�e�X�g�����擾

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntPlayer].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntPlayer]);

		//���f��(�p�[�c)�̕`��
		g_pMeshPlayer->DrawSubset(nCntPlayer);
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//�v���C���[�̏��
Player *GetPlayer(void)
{
	return &g_player;
}

//�v���C���[�̈ړ�����
void MovePlayer(void)
{
	D3DXVECTOR3 DirectionRot;			//�v���C���[�ƃJ�����̊p�x����i�s�������v�Z
	float fAngle;						//�p�x�����߂�
	float fMoveRot;						//�ړ�����
	Camera *pCamera = GetCamera();		//�J�����̏��擾

	DirectionRot = g_player.rot - pCamera->rot;	//�i�s�����v�Z

	//���Ɉړ��E�����]��
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		//�i�s�������p�x���������A�i�s�����̐^�t���傫��
		if (RIGHT + pCamera->rot.y >= DirectionRot.y && DirectionRot.y >= LEFT + pCamera->rot.y)
		{//�E����
			g_player.rot.y += PLAYER_ROT_CORRECT_MOVE;	//�v���C���[��i�s�����Ɍ�����

			//�p�x���i�s�������߂��Ă��܂���
			if (DirectionRot.y >= RIGHT - pCamera->rot.y)
			{
				g_player.rot.y = RIGHT - pCamera->rot.y;					//�i�s�����Ɍ�����
			}
		}
		
		else
		{
			g_player.rot.y -= PLAYER_ROT_CORRECT_MOVE;	//�v���C���[��i�s�����Ɍ�����

			//�p�x���i�s�������߂��Ă��܂���
			if (RIGHT - pCamera->rot.y >= DirectionRot.y && DirectionRot.y >= LEFT - pCamera->rot.y)
			{
				g_player.rot.y = RIGHT - pCamera->rot.y;					//�i�s�����Ɍ�����
			}
		}

		g_player.move.x += sinf(LEFT - pCamera->rot.y) * PLAYER_SPEED;	//X�����Ɉړ�
		g_player.move.z += sinf(DOWN - pCamera->rot.y) * PLAYER_SPEED;	//Z�����Ɉړ�
	}

	//�E�Ɉړ��E�����]��
	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		fMoveRot = (LEFT - pCamera->rot.y);								//�i�s�����v�Z

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//�v���C���[��i�s�����Ɍ�����

		g_player.move.x += sinf(RIGHT + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(UP - pCamera->rot.y) * PLAYER_SPEED;
	}

	//���Ɉړ��E�����]��
	if (GetKeyboardPress(DIK_UP) == true)
	{
		//�i�s�����̊Ԕ��΂��A�p�x����������	�p�x����������΍�����Ői�s�����Ɍ�������@�p�x���傫����ΉE����Ō�������
		if (g_player.rot.y <  DOWN || GetKeyboardPress(DIK_RIGHT) == true)
		{//�E����
			fMoveRot = (REVERSE_UP - pCamera->rot.y);								//�i�s�����v�Z
		}
		else
		{//������
			fMoveRot = (UP - pCamera->rot.y);								//�i�s�����v�Z
		}

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//�v���C���[��i�s�����Ɍ�����

		g_player.move.x += sinf(UP + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(RIGHT - pCamera->rot.y) * PLAYER_SPEED;

		//���ړ����Ă���
		if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{
			//�ړ��ʂ�␳����
			g_player.move.x *= CROSS_SPEED_COR;
			g_player.move.z *= CROSS_SPEED_COR;
		}
	}

	//��O�Ɉړ��E�����]��
	if (GetKeyboardPress(DIK_DOWN) == true)
	{
		fMoveRot = (DOWN - pCamera->rot.y);								//�i�s�����v�Z

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//�v���C���[��i�s�����Ɍ�����

		g_player.move.x += sinf(DOWN + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(LEFT - pCamera->rot.y) * PLAYER_SPEED;

		//���ړ����Ă���
		if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{
			//�ړ��ʂ�␳����
			g_player.move.x *= CROSS_SPEED_COR;
			g_player.move.z *= CROSS_SPEED_COR;
		}
	}

	//�㏸
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		g_player.move.y -= cosf(UP) * PLAYER_SPEED;
	}

	//���~
	if (GetKeyboardPress(DIK_TAB) == true)
	{
		g_player.move.y -= cosf(DOWN) * PLAYER_SPEED;
	}

	//�v���C���[�̈ړ��ʏC��
	FixPlayerMove();

	//�v���C���[�̈ړ���������
	InertiaPlayerMove();
}

//�v���C���[�̈ړ��ʏC��
void FixPlayerMove(void)
{
	//�v���C���[�̈ړ��ʂ�2.0f�ȏ�ɂȂ���
	if (g_player.move.x >= MAX_SPEED)
	{
		g_player.move.x = MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ� -2.0f�ȉ��ɂȂ���
	if (g_player.move.x <= -MAX_SPEED)
	{
		g_player.move.x = -MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ�2.0f�ȏ�ɂȂ���
	if (g_player.move.y >= MAX_SPEED)
	{
		g_player.move.y = MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ� -2.0f�ȉ��ɂȂ���
	if (g_player.move.y <= -MAX_SPEED)
	{
		g_player.move.y = -MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ�2.0f�ȏ�ɂȂ���
	if (g_player.move.z >= MAX_SPEED)
	{
		g_player.move.z = MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ� -2.0f�ȉ��ɂȂ���
	if (g_player.move.z <= -MAX_SPEED)
	{
		g_player.move.z = -MAX_SPEED;
	}
}

//�v���C���[�̈ړ���������
void InertiaPlayerMove(void)
{
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.1f;
	g_player.move.y += (F_ZERO - g_player.move.y) * 0.1f;
	g_player.move.z += (F_ZERO - g_player.move.z) * 0.1f;
}

//�v���C���[�̈ʒu�C��
void FixPlayerPos(void)
{
	//�u�ړ����E�n�_�@���v�𒴂���
	if (g_player.pos.x < LIMIT_PLAYER_LEFT)
	{
		g_player.pos.x = LIMIT_PLAYER_LEFT;
	}

	//�u�ړ����E�n�_�@�E�v�𒴂���
	if (g_player.pos.x > LIMIT_PLAYER_RIGHT)
	{
		g_player.pos.x = LIMIT_PLAYER_RIGHT;
	}

	//�u�ړ����E�n�_�@��v�𒴂���
	if (g_player.pos.y > LIMIT_PLAYER_CEELING)
	{
		g_player.pos.y = LIMIT_PLAYER_CEELING;
	}

	//�u�ړ����E�n�_�@���v�𒴂���
	if (g_player.pos.y < LIMIT_PLAYER_FLOAR)
	{
		g_player.pos.y = LIMIT_PLAYER_FLOAR;
	}

	//�u�ړ����E�n�_�@��O�v�𒴂���
	if (g_player.pos.z > LIMIT_PLAYER_FRONT)
	{
		g_player.pos.z = LIMIT_PLAYER_FRONT;
	}

	//�u�ړ����E�n�_�@���v�𒴂���
	if (g_player.pos.z < LIMIT_PLAYER_BACK)
	{
		g_player.pos.z = LIMIT_PLAYER_BACK;
	}
}

//�v���C���[�̉�]����
void RotPlayer(void)
{
	//������
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_player.rot.y += sinf(LEFT) * 0.1f;
	}

	//�E����
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_player.rot.y += sinf(RIGHT) * 0.1f;
	}

	/*==============================================
						�p�x�C��
	================================================*/
	//------------------------------
	//	�p�x��-3.14�𒴂��Ă��܂���
	//------------------------------
	// X �̊p�x����
	if (g_player.rot.x >= D3DX_PI)
	{
		g_player.rot.x -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Y �̊p�x����
	if (g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Z �̊p�x����
	if (g_player.rot.z >= D3DX_PI)
	{
		g_player.rot.z -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	//------------------------------
	//	�p�x��3.14�𒴂��Ă��܂���
	//------------------------------
	// X �̊p�x����
	if (g_player.rot.x <= -D3DX_PI)
	{
		g_player.rot.x += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Y �̊p�x����
	if (g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Z �̊p�x����
	if (g_player.rot.z <= -D3DX_PI)
	{
		g_player.rot.z += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}
}

//�v���C���[�̃��Z�b�g����
void ResetPlayer(void)
{
	g_player.pos	= ZERO_SET;		// �ʒu������
	g_player.posOld = ZERO_SET;		// �O��̈ʒu������
	g_player.move	= ZERO_SET;		// �ړ��ʏ�����
	g_player.rot	= ZERO_SET;		// ����������
}