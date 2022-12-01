/*==========================================================================================================================================================

														�r���{�[�h����[billboard.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//�r���{�[�h�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//���[���h�}�g���b�N�X
}Billboard;

//�r���{�[�h�̃��e�X�g�ȂǕ`��Ɋւ����̂̍\����
typedef struct
{
	int AlphaTest;
	bool bAlphaTest;
	bool bZTest;
}BillTest;

void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
BillTest *GetAlphaTest(void);

#endif 