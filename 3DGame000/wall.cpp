/*==========================================================================================================================================================

															�ǂ̏���[wall.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "wall.h"

//�}�N����`
#define NUM_WALL		(4)				//�ǂ̐�
#define WALL_WIDTH		(600.0f)		//���T�C�Y
#define WALL_HEIGHT		(200.0f)		//�����T�C�Y
#define WALL_DEPTH		(600.0f)		//���s���T�C�Y
#define TEX_WIDTH		(6.0f)			//�e�N�X�`���̉����𑝂₷
#define TEX_HEIGHT		(2.0f)			//�e�N�X�`���̉����𑝂₷
#define WALL_ROT		(0.01f)			//�ǂ̉�]�l

//1�ڂ̕ǂ̃}�N��
#define WALL_POS_000	D3DXVECTOR3(0.0f, 0.0f, 600.0f)		//�ǂ̏ꏊ
#define WALL_ROT_000	D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//�ǂ̌���
#define WALL_TEX_000	"data/TEXTURE/wall000.jpg"			//�e�N�X�`���̃p�X

//2�ڂ̕ǂ̃}�N��
#define WALL_POS_001	D3DXVECTOR3(600.0f, 0.0f, 0.0f)		//�ǂ̏ꏊ
#define WALL_ROT_001	D3DXVECTOR3(0.0f, 1.57f, 0.0f)		//�ǂ̌���
#define WALL_TEX_001	"data/TEXTURE/wall001.jpg"			//�e�N�X�`���̃p�X

//3�ڂ̕ǂ̃}�N��
#define WALL_POS_002	D3DXVECTOR3(0.0f, 0.0f, -600.0f)	//�ǂ̏ꏊ
#define WALL_ROT_002	D3DXVECTOR3(0.0f, 3.14f, 0.0f)		//�ǂ̌���
#define WALL_TEX_002	"data/TEXTURE/wall001.jpg"			//�e�N�X�`���̃p�X

//4�ڂ̕ǂ̃}�N��
#define WALL_POS_003	D3DXVECTOR3(-600.0f, 0.0f, 0.0f)	//�ǂ̏ꏊ
#define WALL_ROT_003	D3DXVECTOR3(0.0f, -1.57f, 0.0f)		//�ǂ̌���
#define WALL_TEX_003	"data/TEXTURE/wall002.jpg"			//�e�N�X�`���̃p�X

//�ǂ̎�ލ\����
typedef enum
{
	WALLTYPE_D = ZERO,	//��
	WALLTYPE_R,			//�E
	WALLTYPE_F,			//��O
	WALLTYOE_L,			//��
	WALLTYPE_MAX
}WALLTYPE;

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureWall[NUM_WALL] = {};	//�e�N�X�`��
Wall					g_Wall[NUM_WALL];				//�ǂ̏��
float					g_fAngle = F_ZERO;				//��̊p�x

//�ǂ̏���������
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^	

	g_Wall[WALLTYPE_D].pos = WALL_POS_000;	/*�ʒu������*/	g_Wall[WALLTYPE_D].rot = WALL_ROT_000;	/*����������*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_000, &g_pTextureWall[WALLTYPE_D]);	//�e�N�X�`���̓ǂݍ���
	g_Wall[WALLTYPE_R].pos = WALL_POS_001;	/*�ʒu������*/	g_Wall[WALLTYPE_R].rot = WALL_ROT_001;	/*����������*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_001, &g_pTextureWall[WALLTYPE_R]);	//�e�N�X�`���̓ǂݍ���
	g_Wall[WALLTYPE_F].pos = WALL_POS_002;	/*�ʒu������*/	g_Wall[WALLTYPE_F].rot = WALL_ROT_002;	/*����������*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_002, &g_pTextureWall[WALLTYPE_F]);	//�e�N�X�`���̓ǂݍ���
	g_Wall[WALLTYOE_L].pos = WALL_POS_003;	/*�ʒu������*/	g_Wall[WALLTYOE_L].rot = WALL_ROT_003;	/*����������*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_003, &g_pTextureWall[WALLTYOE_L]);	//�e�N�X�`���̓ǂݍ���

	g_Wall[WALLTYPE_D].fWidth = WALL_WIDTH;	/*��������*/	g_Wall[WALLTYPE_D].fHeight = WALL_HEIGHT;	/*����������*/	g_Wall[WALLTYPE_D].fDepth = F_ZERO;			/*���s��������*/
	g_Wall[WALLTYPE_R].fWidth = F_ZERO;		/*��������*/	g_Wall[WALLTYPE_R].fHeight = WALL_HEIGHT;	/*����������*/	g_Wall[WALLTYPE_R].fDepth = WALL_DEPTH;		/*���s��������*/
	g_Wall[WALLTYPE_F].fWidth = -WALL_WIDTH;/*��������*/	g_Wall[WALLTYPE_F].fHeight = WALL_HEIGHT;	/*����������*/	g_Wall[WALLTYPE_F].fDepth = F_ZERO;			/*���s��������*/
	g_Wall[WALLTYOE_L].fWidth = F_ZERO;		/*��������*/	g_Wall[WALLTYOE_L].fHeight = WALL_HEIGHT;	/*����������*/	g_Wall[WALLTYOE_L].fDepth = -WALL_DEPTH;	/*���s��������*/

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += NUM_VERTEX)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VERTEX_LE_UP].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	g_Wall[nCntWall].fHeight,	+g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_RI_UP].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	g_Wall[nCntWall].fHeight,	-g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_LE_DO].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	NIL_F,						+g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_RI_DO].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	NIL_F,						-g_Wall[nCntWall].fDepth );
		}

		D3DXVECTOR3 Wallnor;
		switch (nCntWall)
		{
		case WALLTYPE_D:	//���̕�
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;

		case WALLTYPE_R:	//�E�̕�
			Wallnor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			break;
			
		case WALLTYPE_F:	//��O�̕�
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;

		case WALLTYOE_L:	//���̕�
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		}

		Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//nor�̐ݒ�
		{
			pVtx[VERTEX_LE_UP].nor = Wallnor;
			pVtx[VERTEX_RI_UP].nor = Wallnor;
			pVtx[VERTEX_LE_DO].nor = Wallnor;
			pVtx[VERTEX_RI_DO].nor = Wallnor;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VERTEX_LE_UP].col = RGBA_WHITE;
			pVtx[VERTEX_RI_UP].col = RGBA_WHITE;
			pVtx[VERTEX_LE_DO].col = RGBA_WHITE;
			pVtx[VERTEX_RI_DO].col = RGBA_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VERTEX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VERTEX_RI_UP].tex = D3DXVECTOR2(TEX_WIDTH, ORIGIN_POS);
			pVtx[VERTEX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, TEX_HEIGHT);
			pVtx[VERTEX_RI_DO].tex = D3DXVECTOR2(TEX_WIDTH,  TEX_HEIGHT);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂ̏I������
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		if (g_pTextureWall[nCntWall] != NULL)
		{
			g_pTextureWall[nCntWall]->Release();
			g_pTextureWall[nCntWall] = NULL;
		}
	}	

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//�ǂ̍X�V����
void UpdateWall(void)
{
	//�ǂ̉�]
	//RotWall();

	//�ǂ̌���������
	ResetRotWall();

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();
}

//�ǂ̉�]����
void RotWall(void)
{
	float fSpin = F_ZERO;		//��]�l

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		//�S�ǉ�]
		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			g_Wall[nCntWall].rot.y += WALL_ROT;		//��]�l���

			g_Wall[nCntWall].pos.x = sinf(D3DX_PI - g_Wall[nCntWall].rot.y) * 600.0f;	//�ǉ�]
			g_Wall[nCntWall].pos.z = cosf(D3DX_PI - g_Wall[nCntWall].rot.y) * 600.0f;	//�ǉ�]
		}
	}

	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_fAngle -= WALL_ROT;		//��]�l���

		//�S�ǉ�]
		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			g_Wall[nCntWall].pos.x = sinf(g_fAngle - (D3DX_PI * g_Wall[nCntWall].rot.y)) * 600.0f;	//�ǉ�]
			g_Wall[nCntWall].pos.z = cosf(g_fAngle - (D3DX_PI * g_Wall[nCntWall].rot.y)) * 600.0f;	//�ǉ�]
		}
	}

	//�ǂ̌����C��
	FixRotWall();
}

//�ǂ̌����C������
void FixRotWall(void)
{
	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//�ǂ̌����� 3.14�𒴂���
		if (g_Wall[nCntWall].rot.y >= D3DX_PI)
		{
			g_Wall[nCntWall].rot.y -= FIX_ROT;	//�����C��
		}

		//�ǂ̌����� -3.14�𒴂���
		if (g_Wall[nCntWall].rot.y <= -D3DX_PI)
		{
			g_Wall[nCntWall].rot.y += FIX_ROT;	//�����C��
		}
	}
}

//�ǂ̌���������
void ResetRotWall(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_Wall[WALLTYPE_D].pos = WALL_POS_000;	/*�ʒu������*/
		g_Wall[WALLTYPE_R].pos = WALL_POS_001;	/*�ʒu������*/
		g_Wall[WALLTYPE_F].pos = WALL_POS_002;	/*�ʒu������*/
		g_Wall[WALLTYOE_L].pos = WALL_POS_003;	/*�ʒu������*/

		g_Wall[WALLTYPE_D].rot = WALL_ROT_000;	/*����������*/
		g_Wall[WALLTYPE_R].rot = WALL_ROT_001;	/*����������*/
		g_Wall[WALLTYPE_F].rot = WALL_ROT_002;	/*����������*/
		g_Wall[WALLTYOE_L].rot = WALL_ROT_003;	/*����������*/
		g_fAngle = F_ZERO;
	}	
}

//�ǂ̕`�揈��
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffWall, ZERO, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureWall[nCntWall]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}
}

//�ǂ̓����蔻�菈��
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	bool bCollision = false;	//�����������ǂ����̔���
	float fVecAnswer;			//�O�όv�Z�̌��ʕϐ�
	D3DXVECTOR3 vecToPos;		//�I�u�W�F�N�g�̒��_ 0�Ԗڂ��猩���A�Ώۂ̈ʒu�ւ̊p�x
	D3DXVECTOR3 vecLine;		//�I�u�W�F�N�g�̒��_ 0�Ԗڂ���[�܂ł̃x�N�g��

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += NUM_VERTEX)
	{
		vecToPos	= *pPos - g_Wall[nCntWall].pos;								//�Ώۂ̈ʒu���璸�_0�Ԗڂ܂ł̃x�N�g���v�Z
		vecLine		= pVtx[VERTEX_RI_UP].pos - pVtx[VERTEX_LE_UP].pos;			//���̒��_����	���_0�Ԗڂ܂ł̃x�N�g���v�Z

		fVecAnswer	= (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);		//�Ώۂ̈ʒu�ւ̃x�N�g���v�Z

		//���݂̌����� "��"(��) �ł���
		if (fVecAnswer <= F_ZERO)
		{
			bCollision = true;	//��������
			break;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();

	//�����Ԃ�
	return bCollision;
}

//�ǂ̏��
Wall *GetWall(void)
{
	//�ǂ̏��̐擪�A�h���X��Ԃ�
	return &g_Wall[WALLTYPE_D];
}