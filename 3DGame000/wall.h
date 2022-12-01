/*==========================================================================================================================================================

															�ǂ̏���[wall.cpp]
															Author:���V����

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

//�ǂ̏��\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	float			fWidth;		//��
	float			fHeight;	//����
	float			fDepth;		//���s��
}Wall;

void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
void RotWall(void);
void FixRotWall(void);
void ResetRotWall(void);
Wall *GetWall(void);

#endif 