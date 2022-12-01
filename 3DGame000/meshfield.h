/*==========================================================================================================================================================

													���b�V���t�B�[���h����[meshfield.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MESHFIELD_H
#define _MESHFIELD_H

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//���[���h�}�g���b�N�X
}MeshField;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif