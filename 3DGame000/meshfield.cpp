/*==========================================================================================================================================================

													���b�V���t�B�[���h����[meshfield.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshfield.h"

#define MESHFIELD_POS				D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define MESHFIELD_WIDTH				(600.0f)	//���T�C�Y
#define MESHFIELD_DEPTH				(600.0f)	//���s���T�C�Y
#define MESHFIELD_DIVISION_WIDTH	(3)			//���̕�����
#define MESHFIELD_DIVISION_DEPTH	(3)			//���s���̕�����

#define MESHFIELD_VERTEX			(9)			//���b�V���t�B�[���h�̒��_��
#define INDEX_VERTEX				(14)		//�C���f�b�N�X�o�b�t�@�̐�
#define SEPALATE_MESHFIELD			(12)		//���b�V���t�B�[���h�̕�����

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;		//�e�N�X�`��
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MeshField				g_MeshField;					//���b�V���t�B�[���h�̏��

//���b�V���t�B�[���h�̏���������
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	g_MeshField.pos = MESHFIELD_POS;			//�ʒu������
	g_MeshField.rot = ZERO_SET;					//����������

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�ؔ�000.png", &g_pTextureMeshField);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	for (int nCntMeshField = START_CNT; nCntMeshField < MESHFIELD_VERTEX; nCntMeshField++)
	{
		//���_���W�̐ݒ�
		pVtx[nCntMeshField].pos.x = -MESHFIELD_WIDTH * (ONE - (nCntMeshField % MESHFIELD_DIVISION_WIDTH));		//�J�E���^�[�� "���̕������Ŋ������]��" ���g���ĉ��̍��W������o��
		pVtx[nCntMeshField].pos.z = +MESHFIELD_DEPTH * (ONE - (nCntMeshField / MESHFIELD_DIVISION_WIDTH));		//�J�E���^�[�� "���̕������Ŋ���������" ���g���ĉ��̍��W������o��
		pVtx[nCntMeshField].pos.y = NIL_F;																		//���ʂȂ̂ŁAY���W�� 0

		//nor �̐ݒ�
		pVtx[nCntMeshField].nor = SET_NOR;
	
		//col �̐ݒ�
		pVtx[nCntMeshField].col = RGBA_WHITE;

		//�e�N�X�`�����W�̍��W
		pVtx[nCntMeshField].tex.x = HAL_F * (nCntMeshField % MESHFIELD_DIVISION_WIDTH);		//�J�E���^�[�� "���̕������Ŋ������]��" ���g���ĉ��̍��W������o��
		pVtx[nCntMeshField].tex.y = HAL_F * (nCntMeshField / MESHFIELD_DIVISION_DEPTH);		//�J�E���^�[�� "���̕������Ŋ���������" ���g���ĉ��̍��W������o��
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_VERTEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);

	//�C���f�b�N�X���̃o�b�t�@
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pIdx, ZERO);

	//���_�ԍ��f�[�^�̐ݒ�
					/*����*/		/*����*/			/*�E��*/
	/*���g*/		 pIdx[1] = 0;	 pIdx[3] = 1;	 pIdx[5] = 2;
	/*���g*/										 pIdx[6] = 2;
	 
	/*�^�񒆑g*/	 pIdx[0] = 3;	 pIdx[2] = 4;	 pIdx[4] = 5;
	/*�^�񒆑g*/	 pIdx[9] = 3;	pIdx[11] = 4;	pIdx[13] = 5;

	/*��O�g*/		 pIdx[7] = 6;	pIdx[10] = 7;	pIdx[12] = 8;
	/*��O�g*/		 pIdx[8] = 6;

	/*�C���f�b�N�X�ԍ��@��  0  1  2  3  4  5 | 6  7 | 8  9  10 11 12 13
			  ���_�ԍ�		3  0  4  1  5  2 | 2  6 | 6  3  7  4  8  5  */

	int nNumVtx = ZERO;		//���_�ԍ�
	for (int nCntIdx = START_CNT; nCntIdx < INDEX_VERTEX; nCntIdx++)
	{
		if (nCntIdx != 6 && nCntIdx != 7)
		{
			//�J�E���^�[�������̏ꍇ
			if (nCntIdx % SEC == ZERO)
			{
				//�C���f�b�N�X�ԍ��ݒ�
				pIdx[nCntIdx] = nNumVtx + MESHFIELD_DIVISION_WIDTH;		//�C���f�b�N�X�ԍ� = ��̒��_�ԍ� + ���̕�����
			}

			//�J�E���^�[����̏ꍇ
			else if (nCntIdx % SEC == ONE)
			{
				//�C���f�b�N�X�ԍ��ݒ�
				pIdx[nCntIdx] = nNumVtx;								//�C���f�b�N�X�ԍ� = ��̒��_�ԍ�

				//��̒��_�ԍ����Z
				nNumVtx++;
			}
		}

		//�k�ރ|���S���̍ŏ��̏d�����_�͂ЂƂO�̃C���f�b�N�X�ԍ��Ɠ���
		else if (nCntIdx == 6)
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - ONE];
		}

		//�k�ރ|���S���̍Ō�̏d�����_�͎��̃C���f�b�N�X�ԍ��Ɠ���
		else if (nCntIdx == 7)
		{
			pIdx[nCntIdx] = pIdx[nCntIdx + ONE];
		}
	}

	//�C���f�b�N�X�o�b�t�@�����b�N����
	g_pIdxBuffMeshField->Unlock();

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffMeshField->Unlock();
}

//���b�V���t�B�[���h�̏I������
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//���b�V���t�B�[���h�̍X�V����
void UpdateMeshField(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffMeshField->Unlock();

}

//���b�V���t�B�[���h�̕`�揈��
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshField.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);

	D3DXMatrixMultiply(&g_MeshField.mtxWorld, &g_MeshField.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);

	D3DXMatrixMultiply(&g_MeshField.mtxWorld, &g_MeshField.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshField, ZERO, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, ZERO, ZERO, MESHFIELD_VERTEX, ZERO, SEPALATE_MESHFIELD);
}