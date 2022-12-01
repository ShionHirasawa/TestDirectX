/*==========================================================================================================================================================

															�e�̏���[shadow.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "input.h"
#include "color.h"
#include "shadow.h"
#include "player.h"

#define SHADOW_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define SHADOW_WIDTH	(10.0f)							//���T�C�Y
#define SHADOW_DEPTH	(10.0f)							//���s���T�C�Y
#define SHADOW_ALPTH	(200)							//�e�̃��l
#define MAX_SHADOW		(512)							//�e�̍ő吔

//�e�̍\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	int				aColor;		//�e�̃��l
	float			fWidht;		//��
	float			fDepth;		//���s��
	bool			bSet;		//�Ώۂ̉e�����܂��Ă��邩�ǂ���
	bool			bUse;		//�g���Ă��邩�ǂ���
}Shadow;

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;		//�e�N�X�`��
Shadow g_aShadow[MAX_SHADOW];							//�e�̏��

//�|���S���̏���������
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = SHADOW_POS;			//�ʒu������
		g_aShadow[nCntShadow].rot = ZERO_SET;			//����������
		g_aShadow[nCntShadow].fWidht = SHADOW_WIDTH;	//��������
		g_aShadow[nCntShadow].fDepth = SHADOW_DEPTH;	//���s��������
		g_aShadow[nCntShadow].aColor = SHADOW_ALPTH;	//�e�̃��l������
		g_aShadow[nCntShadow].bSet = false;				//�g�p����Ă��Ȃ�
		g_aShadow[nCntShadow].bUse = false;				//�g�p����Ă��Ȃ�

		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
			pVtx[ONE].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
			pVtx[SEC].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
			pVtx[SAN].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
		}

		//nor�̐ݒ�
		{
			pVtx[ZERO].nor = SET_NOR;
			pVtx[ONE].nor = SET_NOR;
			pVtx[SEC].nor = SET_NOR;
			pVtx[SAN].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//�|���S���̏I������
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//�|���S���̍X�V����
void UpdateShadow(void)
{
	//�|�C���^��ݒ�
	//VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	//g_pVtxBuffShadow->Unlock();

}

//�|���S���̕`�揈��
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffShadow, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�e�̐ݒ菈��
int SetShadow(void)
{
	int nCntShadow = MINUS_ONE;

	for (int mCntShadow = START_CNT; mCntShadow < MAX_SHADOW; mCntShadow++)
	{
		nCntShadow++;

		if (g_aShadow[mCntShadow].bUse == false && g_aShadow[mCntShadow].bSet == false)
		{
			g_aShadow[mCntShadow].pos = SHADOW_POS;	//�ʒu������
			g_aShadow[mCntShadow].rot = ZERO_SET;	//����������
			g_aShadow[mCntShadow].bSet = true;		//�Ώۂ����܂���
			break;
		}
	}

	return nCntShadow;
}

//�e�̈ʒu�ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.y = F_ZERO;

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += NUM_VERTEX * nIdxShadow;		//���_���W���C���f�b�N�X���i�߂�

	g_aShadow[nIdxShadow].fWidht = pos.y / 20.0f + SHADOW_WIDTH;		//������
	g_aShadow[nIdxShadow].fDepth = pos.y / 20.0f + SHADOW_DEPTH;		//���s������
	g_aShadow[nIdxShadow].aColor = SHADOW_ALPTH - (int)(pos.y * 1.3f);	//���l����	
	g_aShadow[nIdxShadow].bUse = true;									//�g�p����

	//���_�J���[�̐ݒ�
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//�e�̎g�p�𖳂���
void BreakShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}