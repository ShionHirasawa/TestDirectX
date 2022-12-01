/*==========================================================================================================================================================

														�|���S������[polygon.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"

#define POLYGON_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define POLYGON_WIDTH	(80.0f)		//���T�C�Y
#define POLYGON_DEPTH	(800.0f)		//���s���T�C�Y

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;		//�e�N�X�`��
D3DXVECTOR3				g_posPolygon;					//�ʒu
D3DXVECTOR3				g_rotPolygon;					//����
D3DXMATRIX				g_mtxWorldPolygon;				//���[���h�}�g���b�N�X

//�|���S���̏���������
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	g_posPolygon = POLYGON_POS;		//�ʒu������
	g_rotPolygon = ZERO_SET;		//����������

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTexturePolygon);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = D3DXVECTOR3(g_posPolygon.x - POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z + POLYGON_DEPTH);
		pVtx[ONE].pos = D3DXVECTOR3(g_posPolygon.x  + POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z + POLYGON_DEPTH);
		pVtx[SEC].pos = D3DXVECTOR3(g_posPolygon.x  - POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z - POLYGON_DEPTH);
		pVtx[SAN].pos = D3DXVECTOR3(g_posPolygon.x  + POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z - POLYGON_DEPTH);
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
		pVtx[ONE].col =  RGBA_WHITE;
		pVtx[SEC].col =  RGBA_WHITE;
		pVtx[SAN].col =  RGBA_WHITE;
	}

	//�e�N�X�`���̍��W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(1.1f, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
		pVtx[SAN].tex = D3DXVECTOR2(1.1f, F_TEN);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//�|���S���̏I������
void UninitPolygon(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//�|���S���̍X�V����
void UpdatePolygon(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPolygon->Unlock();

}

//�|���S���̕`�揈��
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);
	
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffPolygon, ZERO, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	
}