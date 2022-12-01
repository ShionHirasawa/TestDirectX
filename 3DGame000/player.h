/*==========================================================================================================================================================

														�v���C���[�̏���[player.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3		pos;					//�ʒu
	D3DXVECTOR3		posOld;					//�O��̈ʒu
	D3DXVECTOR3		move;					//�ړ���
	D3DXVECTOR3		rot;					//����
	D3DXMATRIX		mtxWorld;				//���[���h�}�g���b�N�X
	int				nIdxShadow = MINUS_ONE;	//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
}Player;

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

void MovePlayer(void);
void FixPlayerMove(void);
void InertiaPlayerMove(void);	// Inertia �́u�����v�Ƃ����Ӗ�
void FixPlayerPos(void);
void RotPlayer(void);
void ResetPlayer(void);

#endif 