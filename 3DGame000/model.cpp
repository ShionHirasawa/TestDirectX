/*==========================================================================================================================================================

														モデル処理[model.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "model.h"
#include "shadow.h"

#define MODEL_TEX			(10)		//モデルのテクスチャの最大数
#define NUM_MODEL			(2)			//モデルの総数
#define MINPOS				(80000.0f)	//最小位置の初期化に使う値。　最小を入れるために大きい値を入れて後で上書き。
#define MAXPOS				(-80000.0f)	//最大位置の初期化に使う値。　最大を入れるために小さい値を入れて後で上書き。
#define INIT_MIN_POS		(D3DXVECTOR3(MINPOS, MINPOS, MINPOS))	//最小位置の初期化に用いる数値
#define INIT_MAX_POS		(D3DXVECTOR3(MAXPOS, MAXPOS, MAXPOS))	//最小位置の初期化に用いる数値

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureModel[MODEL_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH				g_pMeshModel;		// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatModel[MODELTYPE_MAX];		// マテリアルへのポインタ
DWORD					g_dwNumMatModel[MODELTYPE_MAX];		// マテリアルの数
Model					g_Model[MODELTYPE_MAX];				// モデルの情報

// Xファイル名
const char *c_apFileNameModel[MODELTYPE_MAX] = 
{
	"data/MODEL/木箱.x",					//木箱のファイル名
	"data/MODEL/木箱群.x",					//木箱群のファイル名
};

// モデルの位置
const D3DXVECTOR3 c_StartModel[MODELTYPE_MAX] = 
{
	D3DXVECTOR3(100.0f, NIL_F, 0.0f),	//木箱の初期位置
	D3DXVECTOR3(-100.0f, NIL_F, 0.0f),	//木箱群の初期位置
};

//----------------------------------------------------
//				モデルの初期化処理
//----------------------------------------------------
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	g_pMeshModel = NULL;						//メッシュ初期化

	//モデルの情報初期化
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{		
		g_Model[nCntModel].StartPos		= c_StartModel[nCntModel];	//モデルの初期位置設定
		g_pBuffMatModel[nCntModel]		= NULL;						//マテリアルへのポインタ初期化
		g_dwNumMatModel[nCntModel]		= ZERO;						//マテリアルの数初期化
		g_Model[nCntModel].MinPos		= INIT_MIN_POS;				//最小位置の初期化
		g_Model[nCntModel].MaxPos		= INIT_MAX_POS;				//最大位置の初期化
		g_Model[nCntModel].nIdxShadow	= SetShadow();				//影の設定

		// Xファイル読み込み
		D3DXLoadMeshFromX(c_apFileNameModel[nCntModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatModel[nCntModel], NULL, &g_dwNumMatModel[nCntModel], &g_pMeshModel);

		//モデルのテクスチャ設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntModel]; nCntMat++)
		{
			//マテリアルデータへのポインタを取得
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntModel]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureModel[nCntMat]);
			}
		}
	}

	//位置と向き初期化
	ResetModel();

	//最小・最大位置設定
	SearchModel_MinAndMax_Pos();
}

//当たり判定に使用する最小・最大値設定処理
void SearchModel_MinAndMax_Pos(void)
{
	int		nNumVtx;		//頂点数
	DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
	BYTE	*pVtxBuff;		//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel->GetFVF());

	//頂点バッファをロック
	g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//当たり判定に使う頂点の最小・最大位置を取得
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		// 現在格納されている最小・最大値を頂点座標で塗り替えるかどうか判定
		for (int nCntVtx = START_CNT; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//頂点座標の代入

			//------------------------------
			//			Xの判定
			//------------------------------
			// 最小値判定
			if (vtx.x <= g_Model[nCntModel].MinPos.x)
			{
				g_Model[nCntModel].MinPos.x = vtx.x;		//最小値代入
			}

			// 最大値判定
			else if (g_Model[nCntModel].MaxPos.x <= vtx.x)
			{
				g_Model[nCntModel].MaxPos.x = vtx.x;		//最大値代入
			}

			//------------------------------
			//			Yの判定
			//------------------------------
			// 最小値判定
			if (vtx.y <= g_Model[nCntModel].MinPos.y)
			{
				g_Model[nCntModel].MinPos.y = vtx.y;		//最小値代入
			}

			// 最大値判定
			else if (g_Model[nCntModel].MaxPos.y <= vtx.y)
			{
				g_Model[nCntModel].MaxPos.y = vtx.y;		//最大値代入
			}

			//------------------------------
			//			Zの判定
			//------------------------------
			// 最小値判定
			if (vtx.z <= g_Model[nCntModel].MinPos.z)
			{
				g_Model[nCntModel].MinPos.z = vtx.z;		//最小値代入
			}

			// 最大値判定
			else if (g_Model[nCntModel].MaxPos.z <= vtx.z)
			{
				g_Model[nCntModel].MaxPos.z = vtx.z;		//最大値代入
			}

			//頂点フォーマットのサイズ分、ポインタを進める
			pVtxBuff += dwSizeFVF;
		}
	}

	//頂点バッファをアンロック
	g_pMeshModel->UnlockVertexBuffer();
}


//----------------------------------------------------
//					モデルの終了処理
//----------------------------------------------------
void UninitModel(void)
{
	//メッシュ破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//テクスチャ破棄
		if (g_pTextureModel[nCntModel] != NULL)
		{
			g_pTextureModel[nCntModel]->Release();
			g_pTextureModel[nCntModel] = NULL;
		}

		//マテリアル破棄
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}
}


//----------------------------------------------------
//					モデルの更新処理
//----------------------------------------------------
void UpdateModel(void)
{
	//モデルの移動処理
	//MoveModel();

	//モデルの回転処理
	//RotModel();

	//モデルのリセット処理
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		ResetModel();
	}

	//影の描画位置設定
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		SetPositionShadow(g_Model[nCntModel].nIdxShadow, g_Model[nCntModel].pos);
	}
}


//----------------------------------------------------
//					モデルの描画処理
//----------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);

		D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);

		D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntModel]->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntModel]; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureModel[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshModel->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//モデルの移動処理
void MoveModel(void)
{
	/*if (GetKeyboardPress(DIK_A) == true)
	{
		g_Model.pos.x += sinf(D3DX_PI * LEFT) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_Model.pos.x += sinf(D3DX_PI * RIGHT) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{
		g_Model.pos.z -= cosf(D3DX_PI * UP) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_Model.pos.z -= cosf(D3DX_PI * DOWN) * 1.0f;
	}

	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		g_Model.pos.y -= cosf(D3DX_PI * UP) * 1.0f;
	}

	else if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_Model.pos.y -= cosf(D3DX_PI * DOWN) * 1.0f;
	}*/
}

//モデルの回転処理
void RotModel(void)
{
	/*if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_Model.rot.y += sinf(D3DX_PI * LEFT) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_Model.rot.y += sinf(D3DX_PI * RIGHT) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Model.rot.x += cosf(D3DX_PI * UP) * 0.1f;
	}

	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Model.rot.x += cosf(D3DX_PI * DOWN) * 0.1f;
	}*/
}

//モデルのリセット処理
void ResetModel(void)
{
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		g_Model[nCntModel].pos = g_Model[nCntModel].StartPos;	//位置初期化
		g_Model[nCntModel].rot = ZERO_SET;						//向き初期化
	}
}

//モデルの当たり判定
void CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold)
{
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		// Xの最小値と最大値の範囲内にいる
		if (g_Model[nCntModel].MinPos.x <= pPos->x && pPos->x <= g_Model[nCntModel].MaxPos.x)
		{
			// Zの最大値の当たり判定
			ColliModel_ZMax(pPos, pPosold, nCntModel);

			// Zの最小値の当たり判定
			ColliModel_ZMin(pPos, pPosold, nCntModel);
		}
	}
}

//Zの最大値を使った当たり判定
void ColliModel_ZMax(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel)
{	
	if (pPosold->z <= g_Model[nCntModel].MinPos.z &&	//前回の位置が対象のモデルの Zの最小位置のより小さく
		g_Model[nCntModel].MinPos.z <= pPos->z)			//今回の位置が対象のモデルの Zの最小位置のより大きい
	{
		pPos->z = g_Model[nCntModel].MinPos.z;			//今回の位置を対象のモデルの Zの位置にする
	}
}

//Zの最小値を使った当たり判定
void ColliModel_ZMin(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel)
{
	if (g_Model[nCntModel].MaxPos.z <= pPosold->z &&	//前回の位置が対象のモデルの Zの最大位置のより大きく
		pPos->z <= g_Model[nCntModel].MaxPos.z)			//今回の位置が対象のモデルの Zの最大位置のより小さい
	{
		pPos->z = g_Model[nCntModel].MaxPos.z;			//今回の位置を対象のモデルの Zの位置にする
	}
}