/*==========================================================================================================================================================

														���f������[model.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "model.h"
#include "shadow.h"

#define MODEL_TEX			(10)		//���f���̃e�N�X�`���̍ő吔
#define NUM_MODEL			(2)			//���f���̑���
#define MINPOS				(80000.0f)	//�ŏ��ʒu�̏������Ɏg���l�B�@�ŏ������邽�߂ɑ傫���l�����Č�ŏ㏑���B
#define MAXPOS				(-80000.0f)	//�ő�ʒu�̏������Ɏg���l�B�@�ő�����邽�߂ɏ������l�����Č�ŏ㏑���B
#define INIT_MIN_POS		(D3DXVECTOR3(MINPOS, MINPOS, MINPOS))	//�ŏ��ʒu�̏������ɗp���鐔�l
#define INIT_MAX_POS		(D3DXVECTOR3(MAXPOS, MAXPOS, MAXPOS))	//�ŏ��ʒu�̏������ɗp���鐔�l

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureModel[MODEL_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshModel;		// ���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatModel[MODELTYPE_MAX];		// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatModel[MODELTYPE_MAX];		// �}�e���A���̐�
Model					g_Model[MODELTYPE_MAX];				// ���f���̏��

// X�t�@�C����
const char *c_apFileNameModel[MODELTYPE_MAX] = 
{
	"data/MODEL/�ؔ�.x",					//�ؔ��̃t�@�C����
	"data/MODEL/�ؔ��Q.x",					//�ؔ��Q�̃t�@�C����
};

// ���f���̈ʒu
const D3DXVECTOR3 c_StartModel[MODELTYPE_MAX] = 
{
	D3DXVECTOR3(100.0f, NIL_F, 0.0f),	//�ؔ��̏����ʒu
	D3DXVECTOR3(-100.0f, NIL_F, 0.0f),	//�ؔ��Q�̏����ʒu
};

//----------------------------------------------------
//				���f���̏���������
//----------------------------------------------------
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	g_pMeshModel = NULL;						//���b�V��������

	//���f���̏�񏉊���
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{		
		g_Model[nCntModel].StartPos		= c_StartModel[nCntModel];	//���f���̏����ʒu�ݒ�
		g_pBuffMatModel[nCntModel]		= NULL;						//�}�e���A���ւ̃|�C���^������
		g_dwNumMatModel[nCntModel]		= ZERO;						//�}�e���A���̐�������
		g_Model[nCntModel].MinPos		= INIT_MIN_POS;				//�ŏ��ʒu�̏�����
		g_Model[nCntModel].MaxPos		= INIT_MAX_POS;				//�ő�ʒu�̏�����
		g_Model[nCntModel].nIdxShadow	= SetShadow();				//�e�̐ݒ�

		// X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX(c_apFileNameModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatModel[nCntModel], NULL, &g_dwNumMatModel[nCntModel], &g_pMeshModel);

		//���f���̃e�N�X�`���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntModel]; nCntMat++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntModel]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureModel[nCntMat]);
			}
		}
	}

	//�ʒu�ƌ���������
	ResetModel();

	//�ŏ��E�ő�ʒu�ݒ�
	SearchModel_MinAndMax_Pos();
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchModel_MinAndMax_Pos(void)
{
	int		nNumVtx;		//���_��
	DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = g_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		// ���݊i�[����Ă���ŏ��E�ő�l�𒸓_���W�œh��ւ��邩�ǂ�������
		for (int nCntVtx = START_CNT; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//���_���W�̑��

			//------------------------------
			//			X�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.x <= g_Model[nCntModel].MinPos.x)
			{
				g_Model[nCntModel].MinPos.x = vtx.x;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_Model[nCntModel].MaxPos.x <= vtx.x)
			{
				g_Model[nCntModel].MaxPos.x = vtx.x;		//�ő�l���
			}

			//------------------------------
			//			Y�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.y <= g_Model[nCntModel].MinPos.y)
			{
				g_Model[nCntModel].MinPos.y = vtx.y;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_Model[nCntModel].MaxPos.y <= vtx.y)
			{
				g_Model[nCntModel].MaxPos.y = vtx.y;		//�ő�l���
			}

			//------------------------------
			//			Z�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.z <= g_Model[nCntModel].MinPos.z)
			{
				g_Model[nCntModel].MinPos.z = vtx.z;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_Model[nCntModel].MaxPos.z <= vtx.z)
			{
				g_Model[nCntModel].MaxPos.z = vtx.z;		//�ő�l���
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshModel->UnlockVertexBuffer();
}


//----------------------------------------------------
//					���f���̏I������
//----------------------------------------------------
void UninitModel(void)
{
	//���b�V���j��
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//�e�N�X�`���j��
		if (g_pTextureModel[nCntModel] != NULL)
		{
			g_pTextureModel[nCntModel]->Release();
			g_pTextureModel[nCntModel] = NULL;
		}

		//�}�e���A���j��
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}
}


//----------------------------------------------------
//					���f���̍X�V����
//----------------------------------------------------
void UpdateModel(void)
{
	//���f���̈ړ�����
	//MoveModel();

	//���f���̉�]����
	//RotModel();

	//���f���̃��Z�b�g����
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		ResetModel();
	}

	//�e�̕`��ʒu�ݒ�
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		SetPositionShadow(g_Model[nCntModel].nIdxShadow, g_Model[nCntModel].pos);
	}
}


//----------------------------------------------------
//					���f���̕`�揈��
//----------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);

		D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);

		D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntModel]->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntModel]; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureModel[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshModel->DrawSubset(nCntMat);
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//���f���̈ړ�����
void MoveModel(void)
{
	/*if (GetKeyboardPress(DIK_A) == true)
	{
		g_Model.pos.x += sinf(D3DX_PI * LEFT) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_Model.pos.x += sinf(D3DX_PI * RIGHT) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_Model.pos.z -= cosf(D3DX_PI * UP) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_Model.pos.z -= cosf(D3DX_PI * DOWN) * 1.0f;
	}

	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		g_Model.pos.y -= cosf(D3DX_PI * UP) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_Model.pos.y -= cosf(D3DX_PI * DOWN) * 1.0f;
	}*/
}

//���f���̉�]����
void RotModel(void)
{
	/*if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_Model.rot.y += sinf(D3DX_PI * LEFT) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_Model.rot.y += sinf(D3DX_PI * RIGHT) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Model.rot.x += cosf(D3DX_PI * UP) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Model.rot.x += cosf(D3DX_PI * DOWN) * 0.1f;
	}*/
}

//���f���̃��Z�b�g����
void ResetModel(void)
{
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		g_Model[nCntModel].pos = g_Model[nCntModel].StartPos;	//�ʒu������
		g_Model[nCntModel].rot = ZERO_SET;						//����������
	}
}

//���f���̓����蔻��
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold)
{
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		// X�̍ŏ��l�ƍő�l�͈͓̔��ɂ���
		if (g_Model[nCntModel].MinPos.x <= pPos->x && pPos->x <= g_Model[nCntModel].MaxPos.x)
		{
			// Z�̍ő�l�̓����蔻��
			ColliModel_ZMax(pPos, pPosold, nCntModel);

			// Z�̍ŏ��l�̓����蔻��
			ColliModel_ZMin(pPos, pPosold, nCntModel);
		}
	}
}

//Z�̍ő�l���g���������蔻��
void ColliModel_ZMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel)
{	
	if (pPosold->z <= g_Model[nCntModel].MinPos.z &&	//�O��̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂�菬����
		g_Model[nCntModel].MinPos.z <= pPos->z)			//����̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂��傫��
	{
		pPos->z = g_Model[nCntModel].MinPos.z;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
	}
}

//Z�̍ŏ��l���g���������蔻��
void ColliModel_ZMin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel)
{
	if (g_Model[nCntModel].MaxPos.z <= pPosold->z &&	//�O��̈ʒu���Ώۂ̃��f���� Z�̍ő�ʒu�̂��傫��
		pPos->z <= g_Model[nCntModel].MaxPos.z)			//����̈ʒu���Ώۂ̃��f���� Z�̍ő�ʒu�̂�菬����
	{
		pPos->z = g_Model[nCntModel].MaxPos.z;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
	}
}