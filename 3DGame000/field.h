/*==========================================================================================================================================================

													�t�B�[���h(�n��)����[field.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _FIELD_H_
#define _FIELD_H_

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//���[���h�}�g���b�N�X
}Field;

void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

#endif 