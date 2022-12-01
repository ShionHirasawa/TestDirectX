/*==========================================================================================================================================================

															���f������[model.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MODEL_H_
#define _MODEL_H_

//���f���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		StartPos;	// �����ʒu
	D3DXVECTOR3		MinPos;		// �ŏ��̈ʒu
	D3DXVECTOR3		MaxPos;		// �ő�̈ʒu
	D3DXVECTOR3		rot;		// ����
	D3DXMATRIX		mtxWorld;	// ���[���h�}�g���b�N�X
	int				nIdxShadow;	// �Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
}Model;

//���f���̎��
typedef enum
{
	MODELTYPE_WOODEN_BLOCK = ZERO,
	MODELTYPE_WOODEN_BLOCKs,
	MODELTYPE_MAX
}MODELTYPE;

void InitModel(void);
void SearchModel_MinAndMax_Pos(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

void MoveModel(void);
void RotModel(void);
void ResetModel(void);
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold);
void ColliModel_ZMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel);
void ColliModel_ZMin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel);

#endif 