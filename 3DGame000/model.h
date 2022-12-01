/*==========================================================================================================================================================

															モデル処理[model.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MODEL_H_
#define _MODEL_H_

//モデルの構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		StartPos;	// 初期位置
	D3DXVECTOR3		MinPos;		// 最小の位置
	D3DXVECTOR3		MaxPos;		// 最大の位置
	D3DXVECTOR3		rot;		// 向き
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	int				nIdxShadow;	// 対象の影のインデックス(番号)
}Model;

//モデルの種類
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