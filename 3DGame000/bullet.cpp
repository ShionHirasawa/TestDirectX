/*==========================================================================================================================================================

															�e����[bullet.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "bullet.h"
#include "billboard.h"
#include "color.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "wall.h"

//�}�N����`
#define MAX_BULLET			(256)		//�e�̍ő吔
#define BULLET_HP			(400)		//�e�̗̑�
#define FANGLE_CORRECT		(6.28f)		//�p�x�̏C���Ɏg�p
#define DAMAGE				(1)			//�e�̍U����
#define BULLET_SIZE			(4.0f)		//�e�̕�
#define BULLET_LOMIT_POS_X	(600.0f)	//�e�̌��E���W�@X����
#define BULLET_LOMIT_POS_Z	(600.0f)	//�e�̌��E���W�@Z����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Bullet					g_aBullet[MAX_BULLET];			//�e�̏��

//�e�̏���������
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos				= ZERO_SET;		//�ʒu������
		g_aBullet[nCntBullet].posOld			= ZERO_SET;		//�O��̈ʒu������
		g_aBullet[nCntBullet].move				= ZERO_SET;		//�ړ��ʏ�����
		g_aBullet[nCntBullet].nLife				= BULLET_HP;	//����������
		g_aBullet[nCntBullet].EffectAlphaColor	= F_ZERO;		//�e�̋O�Ղ̃��l���w�肷��ϐ�
		g_aBullet[nCntBullet].bUse				= false;		//�g�p���Ă��Ȃ���Ԃɂ���

		g_aBullet[nCntBullet].nIdxShadow = SetShadow();		//�e�̐ݒ�
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE,  BULLET_SIZE, F_ZERO);
			pVtx[1].pos = D3DXVECTOR3( BULLET_SIZE,  BULLET_SIZE, F_ZERO);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, F_ZERO);
			pVtx[3].pos = D3DXVECTOR3( BULLET_SIZE, -BULLET_SIZE, F_ZERO);
		}

		//rhw�̐ݒ�
		{
			pVtx[0].nor = SET_NOR;
			pVtx[1].nor = SET_NOR;
			pVtx[2].nor = SET_NOR;
			pVtx[3].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = XCOL_WHITE;
			pVtx[1].col = XCOL_WHITE;
			pVtx[2].col = XCOL_WHITE;
			pVtx[3].col = XCOL_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̏I������
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//�e�̍X�V����
void UpdateBullet(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

			//�O��̈ʒu�X�V
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//�e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			pVtx[ZERO].col = g_aBullet[nCntBullet].col;
			pVtx[ONE].col = g_aBullet[nCntBullet].col;
			pVtx[SEC].col = g_aBullet[nCntBullet].col;
			pVtx[SAN].col = g_aBullet[nCntBullet].col;

			//���������炷
			g_aBullet[nCntBullet].nLife--;

			//�e����ʊO�ɏo���ꍇ
			if (CollisionWall(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld) == true ||
				g_aBullet[nCntBullet].nLife <= ZERO)
			{
				g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//�e�`��
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			//�G�t�F�N�g�`��
			SetEffect(g_aBullet[nCntBullet].pos, XCOL_YELLOW, 10.0f, 60);

			//�g���Ȃ��Ȃ����e�̏�����
			if (g_aBullet[nCntBullet].bUse == false)
			{
				//�����ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos, XCOL_WHITE);

				//�p�[�e�B�N���ݒ�
				if (GetKeyboardPress(DIK_B) == true)
				{
					//SetParticle(g_aBullet[nCntBullet].pos, SET_PARTICLE_NORMAL);
				}
				else
				{
					//SetParticle(g_aBullet[nCntBullet].pos, SET_PARTICLE_EXPLOSION);
				}

				BreakShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e�̎g�p��j������
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̕`�揈��
void DrawBullet(void)
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
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aBullet[nCntBullet].mtxWorld._41 = F_ZERO;		//����
			g_aBullet[nCntBullet].mtxWorld._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_aBullet[nCntBullet].mtxWorld._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * NUM_VERTEX, SEC);
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

//�e�̐ݒ菈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float Alpha)
{
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			//�e�̏���ݒ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].EffectAlphaColor = Alpha;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//�g�p���Ă����Ԃɂ���
			break;
		}
	}
}