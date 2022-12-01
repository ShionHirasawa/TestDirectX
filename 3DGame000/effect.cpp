/*==========================================================================================================================================================

														�G�t�F�N�g����[effect.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "effect.h"
#include "shadow.h"
#include "color.h"
#include "billboard.h"

//�}�N����`
#define MAX_EFFECT		(20000)		//�G�t�F�N�g�̍ő吔
#define EFFECT_SIZE		(10.0f)		//�G�t�F�N�g�̑傫��
#define EFFECT_LIFE		(50)		//�G�t�F�N�g�̎���

//�G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	D3DXMATRIX	mtxWorld;	//���[���h�}�g���b�N�X
	float fRadius;			//���a(�傫��)
	int nLife;				//����(�\������)
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Effect					g_aEffect[MAX_EFFECT];			//�G�t�F�N�g�̏��

//�G�t�F�N�g�̏���������
void InitEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�G�t�F�N�g�̏��̏�����
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos		= ZERO_SET;			//�ʒu�̏�����
		g_aEffect[nCntEffect].col		= XCOL_WHITE;		//�F�̏�����
		g_aEffect[nCntEffect].fRadius	= F_ZERO;			//���a(�傫��)�̏�����
		g_aEffect[nCntEffect].nLife		= EFFECT_LIFE;		//�����̏�����
		g_aEffect[nCntEffect].bUse		= false;			//�g�p���Ă��Ȃ���Ԃɂ���

		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
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
			pVtx[ZERO].col = XCOL_YELLOW;
			pVtx[ONE].col = XCOL_YELLOW;
			pVtx[SEC].col = XCOL_YELLOW;
			pVtx[SAN].col = XCOL_YELLOW;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2( MAX_POS,    ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2( ORIGIN_POS, MAX_POS);
			pVtx[SAN].tex = D3DXVECTOR2( MAX_POS,    MAX_POS);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̏I������
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//�G�t�F�N�g�̍X�V����
void UpdateEffect(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			 //�����Ƒ傫�������炵�Ă���
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].fRadius >= F_ZERO)
			{
				g_aEffect[nCntEffect].fRadius -= 0.2f;
			}

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fRadius / 200.0f;

			if (g_aEffect[nCntEffect].nLife <= ZERO || g_aEffect[nCntEffect].fRadius < F_ZERO)
			{
				g_aEffect[nCntEffect].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[ZERO].col = g_aEffect[nCntEffect].col;
				pVtx[ONE].col = g_aEffect[nCntEffect].col;
				pVtx[SEC].col = g_aEffect[nCntEffect].col;
				pVtx[SAN].col = g_aEffect[nCntEffect].col;
			}

		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̕`�揈��
void DrawEffect(void)
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

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	//�r���{�[�h�̕`��ݒ�
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aEffect[nCntEffect].mtxWorld._41 = F_ZERO;		//����
			g_aEffect[nCntEffect].mtxWorld._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_aEffect[nCntEffect].mtxWorld._43 = F_ZERO;		//����

																	//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffEffect, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * NUM_VERTEX, SEC);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//�G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�

			//�G�t�F�N�g���̐ݒ�
			g_aEffect[nCntEffect].pos = pos;			//�ʒu���
			g_aEffect[nCntEffect].col = col;			//�F�ʑ��
			g_aEffect[nCntEffect].fRadius = fRadius;	//���a���
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;	//�������
			g_aEffect[nCntEffect].bUse = true;			//�g�p���Ă����Ԃɂ���

			//���_���̐ݒ�
			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[ZERO].col = g_aEffect[nCntEffect].col;
				pVtx[ONE].col  = g_aEffect[nCntEffect].col;
				pVtx[SEC].col  = g_aEffect[nCntEffect].col;
				pVtx[SAN].col  = g_aEffect[nCntEffect].col;
			}

			break;
		}
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();
}