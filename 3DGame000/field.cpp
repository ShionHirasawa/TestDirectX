/*==========================================================================================================================================================

													�t�B�[���h(�n��)����[field.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "field.h"

#define FIELD_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define FIELD_WIDTH		(600.0f)	//���T�C�Y
#define FIELD_DEPTH		(600.0f)	//���s���T�C�Y

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureField = NULL;		//�e�N�X�`��
Field					g_Field;					//�t�B�[���h�̏��

//�t�B�[���h�̏���������
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	g_Field.pos = FIELD_POS;	//�ʒu������
	g_Field.rot = ZERO_SET;		//����������

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTextureField);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = D3DXVECTOR3(g_Field.pos.x - FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z + FIELD_DEPTH);
		pVtx[ONE].pos =  D3DXVECTOR3(g_Field.pos.x + FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z + FIELD_DEPTH);
		pVtx[SEC].pos =  D3DXVECTOR3(g_Field.pos.x - FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z - FIELD_DEPTH);
		pVtx[SAN].pos =  D3DXVECTOR3(g_Field.pos.x + FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z - FIELD_DEPTH);
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
		pVtx[ZERO].col = RGBA_WHITE;
		pVtx[ONE].col = RGBA_WHITE;
		pVtx[SEC].col = RGBA_WHITE;
		pVtx[SAN].col = RGBA_WHITE;
	}

	//�e�N�X�`���̍��W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
		pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffField->Unlock();
}

//�t�B�[���h�̏I������
void UninitField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//�t�B�[���h�̍X�V����
void UpdateField(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffField->Unlock();

}

//�t�B�[���h�̕`�揈��
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);

	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);

	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Field.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffField, ZERO, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureField);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}