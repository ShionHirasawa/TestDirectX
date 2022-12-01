/*==========================================================================================================================================================

														�J��������[camera.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXVECTOR3 rot;			//����
	float		fLength;		//���_�ƒ����_�̋���
	D3DXMATRIX	mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxview;		//�r���[�}�g���b�N�X
	bool		bHorming;		//�J�������ǔ����邩�ۂ�
}Camera;

void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

//�J�����̈ړ�����
void MoveCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void MovePosV(void);
void MovePosR(void);
void SpinPosV(void);
void SpinPosR(void);
void ResetCameraPos(void);
void FixCameraRot(void);
void SetCamera(void);
Camera *GetCamera(void);
void LengthCamera(void);

#endif 