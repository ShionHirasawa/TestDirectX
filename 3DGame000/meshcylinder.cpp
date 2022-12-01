/*==========================================================================================================================================================

													���b�V���V�����_�[����[meshcylinder.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshcylinder.h"

#define CYLINDER_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)	//�����ʒu
#define CYLINDER_WIDTH		(600.0f)						//���T�C�Y
#define CYLINDER_DEPTH		(600.0f)						//���s���T�C�Y
#define MESHFIELD_VERTEX	(14)							//���b�V���t�B�[���h�̒��_��
#define SEPALATE_MESH		(12)							//���b�V���t�B�[���h�̕�����

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder = NULL;		//�e�N�X�`��
MeshCylinder			g_MeshCylinder;						//���b�V���V�����_�[�̏��

//���b�V���V�����_�[�̏���������
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	g_MeshCylinder.pos = CYLINDER_POS;	//�ʒu������
	g_MeshCylinder.rot = ZERO_SET;		//����������

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�ؔ�000.png", &g_pTextureMeshCylinder);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshCylinder, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[0].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(NIL_F, NIL_F, +CYLINDER_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[5].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[8].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[9].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[10].pos = D3DXVECTOR3(NIL_F, NIL_F, -CYLINDER_DEPTH);
		pVtx[11].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[12].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[13].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, NIL_F);
	}

	//nor�̐ݒ�
	{
		pVtx[0].nor = SET_NOR;
		pVtx[1].nor = SET_NOR;
		pVtx[2].nor = SET_NOR;
		pVtx[3].nor = SET_NOR;
		pVtx[4].nor = SET_NOR;
		pVtx[5].nor = SET_NOR;
		pVtx[6].nor = SET_NOR;
		pVtx[7].nor = SET_NOR;
		pVtx[8].nor = SET_NOR;
		pVtx[9].nor = SET_NOR;
		pVtx[10].nor = SET_NOR;
		pVtx[11].nor = SET_NOR;
		pVtx[12].nor = SET_NOR;
		pVtx[13].nor = SET_NOR;
	}

	//���_�J���[�̐ݒ�
	{
		pVtx[0].col = RGBA_WHITE;
		pVtx[1].col = RGBA_WHITE;
		pVtx[2].col = RGBA_WHITE;
		pVtx[3].col = RGBA_WHITE;
		pVtx[4].col = RGBA_WHITE;
		pVtx[5].col = RGBA_WHITE;
		pVtx[6].col = RGBA_WHITE;
		pVtx[7].col = RGBA_WHITE;
		pVtx[8].col = RGBA_WHITE;
		pVtx[9].col = RGBA_WHITE;
		pVtx[10].col = RGBA_WHITE;
		pVtx[11].col = RGBA_WHITE;
		pVtx[12].col = RGBA_WHITE;
		pVtx[13].col = RGBA_WHITE;
	}

	//�e�N�X�`���̍��W
	{
		//�e�N�X�`�����W  0.0f   0.0f		//�e�N�X�`�����W  0.5f   0.0f		//�e�N�X�`�����W  1.0f   0.0f
		pVtx[1].tex = ORIGIN_TEX;			pVtx[3].tex = HA_ZE_TEX;			pVtx[5].tex = MA_ZE_TEX;
		pVtx[6].tex = MA_ZE_TEX;

		//�e�N�X�`�����W  0.0f   0.5f		//�e�N�X�`�����W  0.5f   0.5f		//�e�N�X�`�����W  1.0f   0.5f
		pVtx[0].tex = ZE_HA_TEX;			pVtx[2].tex = HALF_TEX;				pVtx[4].tex = MA_HA_TEX;
		pVtx[9].tex = ZE_HA_TEX;			pVtx[11].tex = HALF_TEX;			pVtx[13].tex = MA_HA_TEX;

		//�e�N�X�`�����W  0.0f   1.0f		//�e�N�X�`�����W  0.5f   1.0f		//�e�N�X�`�����W  1.0f   1.0f
		pVtx[7].tex = ZE_MA_TEX;			pVtx[10].tex = HA_MA_TEX;			pVtx[12].tex = LIMIT_TEX;
		pVtx[8].tex = ZE_MA_TEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();
}

//���b�V���V�����_�[�̏I������
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
}

//���b�V���V�����_�[�̍X�V����
void UpdateMeshCylinder(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

}

//���b�V���V�����_�[�̕`�揈��
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_MeshCylinder.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder.rot.y, g_MeshCylinder.rot.x, g_MeshCylinder.rot.z);

	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder.pos.x, g_MeshCylinder.pos.y, g_MeshCylinder.pos.z);

	D3DXMatrixMultiply(&g_MeshCylinder.mtxWorld, &g_MeshCylinder.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshCylinder, ZERO, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEPALATE_MESH);
}