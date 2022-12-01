/*==========================================================================================================================================================

														�p�[�e�B�N������[parthicle.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "particle.h"
#include "billboard.h"
#include "color.h"

//�}�N����`
#define MAX_PARTICLE				(5000)		//�p�[�e�B�N���̍ő吔
#define PARTICLE_LIFE				(60)		//�p�[�e�B�N���̎���

#define MAX_RADIUS					(10.0f)		//���a�̏����l
#define MINUS_RADIUS				(0.1f)		//���a�̏k���l

//�p�[�e�B�N���̔����Ɏg���ړ������̃}�N��
#define ANGLE						(629)		//�p�[�e�B�N�����ړ��������
#define FIX_ANGLE					(314)		//ANGLE�̊p�x���C��

#define TIP_UP						(3)		//���ˏグ��ړ���
#define MIN_TIP						(2)		//�Œ���̈ړ���
#define FIX_TIP_UP					(3)			//TIP_UP�̔{���C��
#define FIX_FLOAT					(100.0f)	//�p�x��float�^�ɏC��
#define MOVE_PARTICLE				(2.0f)		//�p�[�e�B�N���̊�{�ړ���
#define PARTICLE_NUM_EXPLOSION		(30)		//�����I�ȃp�[�e�B�N���̔�����

//�p�[�e�B�N���̏d�̓}�N��
#define GRAVITY_PARTICLE			(-4.0f)		//�p�[�e�B�N���̏d�͉����x
#define GRAVITY_PART_COR			(0.2f)		//�p�[�e�B�N���̏d�͌W��
#define BOUND_PARTICLE_COR			(-0.9f)		//�p�[�e�B�N���̃o�E���h�␳�l

#define REFLECT_PARTICEL			(-1.0f)		//���˕Ԃ��W��

#define PARTICLE_LIMIT_WALL			(600.0f)	//�ǂ̈ʒu
#define PARTICLE_LIMIT_FLOOR		(000.0f)	//���̍��W

//�p�[�e�B�N���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu(�����ʒu)
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR	col;		//�F
	D3DXMATRIX	mtxWorld;	//���[���h�}�g���b�N�X
	int			nLife;		//����(��������)
	float		fRadius;	//�p�[�e�B�N���̔��a
	bool		bUse;		//�g�p���Ă��邩�ǂ���
}Particle;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Particle				g_aParticle[MAX_PARTICLE];	//�p�[�e�B�N���̏��

//�p�[�e�B�N���̏�����
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle000.jpg", &g_pTextureParticle);	//�����p�[�e�B�N��
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�p�[�e�B�N���̏��̏�����
		g_aParticle[nCntParticle].pos		= ZERO_SET;			//�ʒu�̏�����
		g_aParticle[nCntParticle].move		= ZERO_SET;			//�ړ��ʂ̏�����
		g_aParticle[nCntParticle].nLife		= PARTICLE_LIFE;	//�����̏�����
		g_aParticle[nCntParticle].fRadius	= MAX_RADIUS;		//���a�̏�����
		g_aParticle[nCntParticle].bUse		= false;			//�g�p���Ă��Ȃ���Ԃɂ���

		//���_���W (���a���g����) �̐ݒ�
		SetParticleRadius(nCntParticle);

		//rhw�̐ݒ�
		{
			pVtx[0].nor = SET_NOR;
			pVtx[1].nor = SET_NOR;
			pVtx[2].nor = SET_NOR;
			pVtx[3].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = PARTICLE_COL;
			pVtx[1].col = PARTICLE_COL;
			pVtx[2].col = PARTICLE_COL;
			pVtx[3].col = PARTICLE_COL;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = ORIGIN_TEX;
			pVtx[1].tex = MA_ZE_TEX;
			pVtx[2].tex = ZE_MA_TEX;
			pVtx[3].tex = LIMIT_TEX;
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//�p�[�e�B�N���̏I������
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//�p�[�e�B�N���̍X�V����
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//�p�[�e�B�N�����ړ�
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//�p�[�e�B�N���ɏd�͂�������
			g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * GRAVITY_PART_COR;

			//�p�[�e�B�N���� �ǁE���ɓ�������������
			BoundParticle(nCntParticle);

			//���a���k��
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//���a��0�ɂȂ���
			if (g_aParticle[nCntParticle].fRadius <= F_ZERO)
			{
				g_aParticle[nCntParticle].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//���a���g���Ē��_���W��ݒ�
			SetParticleRadius(nCntParticle);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//�p�[�e�B�N���̕`�揈��
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p
	BillTest *pBill = GetAlphaTest();			//�r���{�[�h�̃��EZ�e�X�g�����擾

	//���e�X�g��L���ɂ���
	if (pBill->bAlphaTest == true)
	{
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
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aParticle[nCntParticle].mtxWorld, NULL, &mtxView);	//�t�s������߂�
			g_aParticle[nCntParticle].mtxWorld._41 = F_ZERO;		//����
			g_aParticle[nCntParticle].mtxWorld._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_aParticle[nCntParticle].mtxWorld._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffParticle, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureParticle);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * NUM_VERTEX, SEC);
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

//�p�[�e�B�N���̐ݒ菈��
void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType)
{
	//�����p�^�[������
	switch (nType)
	{
	case SET_PARTICLE_NORMAL:		//�p�[�e�B�N���ʏ픭��
		SetParticleNor(pos);
		break;

	case SET_PARTICLE_EXPLOSION:
		SetParticleExplosion(pos);	//�p�[�e�B�N����������
		break;
	}
}

//�ʏ픭���̃p�[�e�B�N���ݒ�
void SetParticleNor(D3DXVECTOR3 pos)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//�p�[�e�B�N�����ړ�����p�x��ݒ�

			fAngle.x = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	//���ꂼ��̕����̊p�x������
			fAngle.y = (float)(rand() % TIP_UP + MIN_TIP);				//���ꂼ��̕����̊p�x������
			fAngle.z = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	//���ꂼ��̕����̊p�x������

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//�p�[�e�B�N���̈ړ���

																			//�p�[�e�B�N�����̐ݒ�
			g_aParticle[nCntParticle].pos = pos;							//�ʒu
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				//����
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					//���a
			g_aParticle[nCntParticle].bUse = true;							//�g�p���Ă����Ԃɂ���

			break;
		}
	}
}

//���������̃p�[�e�B�N���ݒ�
void SetParticleExplosion(D3DXVECTOR3 pos)
{
	//�����ʃJ�E���g
	int nCntBirth = ZERO;

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//�p�[�e�B�N�����ړ�����p�x��ݒ�
				
			fAngle.x = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	//���ꂼ��̕����̊p�x������
			fAngle.y = (float)(rand() %  TIP_UP +  MIN_TIP  );				//���ꂼ��̕����̊p�x������
			fAngle.z = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	//���ꂼ��̕����̊p�x������

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//�p�[�e�B�N���̈ړ���

			//�p�[�e�B�N�����̐ݒ�
			g_aParticle[nCntParticle].pos = pos;								//�ʒu
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			//�ړ��ʌv�Z
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;					//����
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;						//���a
			g_aParticle[nCntParticle].bUse = true;								//�g�p���Ă����Ԃɂ���

			//�����ʑ���
			nCntBirth++;

			//�����ʂ��K��ʂ𒴂�����I��
			if (nCntBirth >= PARTICLE_NUM_EXPLOSION)
			{
				break;
			}
		}
	}
}

//�p�[�e�B�N�����ǂɓ����������ɒ��˕Ԃ�
void BoundParticle(int nCntParticle)
{
	//X���W�̔���
	if (g_aParticle[nCntParticle].pos.x >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.x <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.x *= REFLECT_PARTICEL;		//�ړ��ʂ��t�]������
	}

	//Y���W�̔���
	if (g_aParticle[nCntParticle].pos.y <= PARTICLE_LIMIT_FLOOR)
	{//�o�E���h������
		g_aParticle[nCntParticle].move.y = (GRAVITY_PARTICLE + g_aParticle[nCntParticle].move.y) * BOUND_PARTICLE_COR;
	}

	//Z���W�̔���
	if (g_aParticle[nCntParticle].pos.z >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.z <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.z *= REFLECT_PARTICEL;		//�ړ��ʂ��t�]������
	}
}

//�p�[�e�B�N���̔��a����
void SetParticleRadius(int nCntParticle)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += nCntParticle * NUM_VERTEX;	//���_�o�b�t�@���p�[�e�B�N���̔ԍ��̐����₷

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[ONE].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[SEC].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[SAN].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}