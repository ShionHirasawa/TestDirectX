/*==========================================================================================================================================================

													���b�V���V�����_�[����[meshcylinder.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MESHCYLINDER_H
#define _MESHCYLINDER_H

#include "main.h"

//���b�V���V�����_�[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//���[���h�}�g���b�N�X
}MeshCylinder;

//�v���g�^�C�v�錾
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif