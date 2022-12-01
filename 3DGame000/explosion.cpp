/*==========================================================================================================================================================

															��������[explosion.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "explosion.h"
#include "shadow.h"
#include "color.h"
#include "billboard.h"

//�}�N����`
#define MAX_EXPLOSION		(128)		//�����̍ő吔
#define EXPLOSION_SIZE		(10.0f)		//�����̃T�C�Y
#define EXPLO_ANIM_STEP		(6)			//�����̃A�j���[�V�����ω��J�E���^
#define EXPLO_ANIM_MAX		(8)			//�����̃A�j���[�V�����ő�l
#define EXPLO_TEX_WIDTH		(0.125f)	//�����̃e�N�X�`���̕�

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXCOLOR	col;			//�F
	D3DXMATRIX	mtxWorld;		//���[���h�}�g���b�N�X
	int			nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int			nPatternAnim;	//�A�j���[�V�����p�^�[��No
	bool		bUse;			//�g�p���Ă��邩�ǂ���
}Explosion;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Explosion				g_aExplosion[MAX_EXPLOSION];	//�����̏��

//�����̏���������
void InitExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�����̏��̏�����
	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = ZERO_SET;				//�ʒu�̏�����
		g_aExplosion[nCntExplosion].col = XCOL_WHITE;			//�F�̏�����
		g_aExplosion[nCntExplosion].nCounterAnim = ZERO;		//�A�j���[�V�����J�E���^�[�̏�����
		g_aExplosion[nCntExplosion].nPatternAnim = ZERO;		//�A�j���[�V�����p�^�[��No�̏�����
		g_aExplosion[nCntExplosion].bUse = false;				//�g�p���Ă��Ȃ���Ԃɂ���

		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(-EXPLOSION_SIZE,  EXPLOSION_SIZE, F_ZERO);
			pVtx[ONE].pos = D3DXVECTOR3( EXPLOSION_SIZE,  EXPLOSION_SIZE, F_ZERO);
			pVtx[SEC].pos = D3DXVECTOR3(-EXPLOSION_SIZE, -EXPLOSION_SIZE, F_ZERO);
			pVtx[SAN].pos = D3DXVECTOR3( EXPLOSION_SIZE, -EXPLOSION_SIZE, F_ZERO);
		}

		//rhw�̐ݒ�
		{
			pVtx[ZERO].nor = SET_NOR;
			pVtx[ONE].nor = SET_NOR;
			pVtx[SEC].nor = SET_NOR;
			pVtx[SAN].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = XCOL_WHITE;
			pVtx[ONE].col = XCOL_WHITE;
			pVtx[SEC].col = XCOL_WHITE;
			pVtx[SAN].col = XCOL_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[SAN].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�����̏I������
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//�����̍X�V����
void UpdateExplosion(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���
			//�A�j���[�V�����J�E���^�X�V
			g_aExplosion[nCntExplosion].nCounterAnim++;

			//�J�E���^�[���X�V�J�E���g�Ɠ����ɂȂ���
			if (g_aExplosion[nCntExplosion].nCounterAnim == EXPLO_ANIM_STEP)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;			//�p�^�[��No.�X�V

				g_aExplosion[nCntExplosion].nCounterAnim = ZERO;	//�J�E���^�[������

				//�p�^�[��No.���ő�l�ȏ�ɂȂ���
				if (g_aExplosion[nCntExplosion].nPatternAnim >= EXPLO_ANIM_MAX)
				{
					g_aExplosion[nCntExplosion].nPatternAnim = ZERO;	//�p�^�[��No.������

					g_aExplosion[nCntExplosion].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
				}
			}

			//�e�N�X�`�����W�̍X�V
			pVtx[ZERO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, ORIGIN_POS);
			pVtx[ONE].tex =  D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, ORIGIN_POS);
			pVtx[SEC].tex =  D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, MAX_POS);
			pVtx[SAN].tex =  D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, MAX_POS);
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//�����̕`�揈��
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p
	BillTest *pBill = GetAlphaTest();			//�r���{�[�h�̃��EZ�e�X�g�����擾

	if (pBill->bAlphaTest == true)
	{
		//���e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
	}

	//Z�e�X�g�𖳌��ɂ���
	if (pBill->bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@�̗L�������ݒ�
	}

	//�r���{�[�h�̕`��ݒ�
	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCntExplosion].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aExplosion[nCntExplosion].mtxWorld._41 = F_ZERO;		//����
			g_aExplosion[nCntExplosion].mtxWorld._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_aExplosion[nCntExplosion].mtxWorld._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);

			D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffExplosion, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureExplosion);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * NUM_VERTEX, SEC);
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

															//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//�����̐ݒ菈��
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�������g�p����Ă��Ȃ�

		 //�������̐ݒ�
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = ZERO;
			g_aExplosion[nCntExplosion].nPatternAnim = ZERO;

			//���_���̐ݒ�
			//���_�J���[�̐ݒ�
			{
				pVtx[ZERO].col = g_aExplosion[nCntExplosion].col;
				pVtx[ONE].col = g_aExplosion[nCntExplosion].col;
				pVtx[SEC].col = g_aExplosion[nCntExplosion].col;
				pVtx[SAN].col = g_aExplosion[nCntExplosion].col;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[ZERO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim,				  ORIGIN_POS);
				pVtx[ONE].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, ORIGIN_POS);
				pVtx[SEC].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim,					  MAX_POS);
				pVtx[SAN].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, MAX_POS);
			}

			g_aExplosion[nCntExplosion].bUse = true;			//�g�p���Ă����Ԃɂ���
			break;
		}
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffExplosion->Unlock();
}