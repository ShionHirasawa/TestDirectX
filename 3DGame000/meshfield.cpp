/*==========================================================================================================================================================

													メッシュフィールド処理[meshfield.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshfield.h"

#define MESHFIELD_POS				D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define MESHFIELD_WIDTH				(600.0f)	//幅サイズ
#define MESHFIELD_DEPTH				(600.0f)	//奥行きサイズ
#define MESHFIELD_DIVISION_WIDTH	(3)			//横の分割数
#define MESHFIELD_DIVISION_DEPTH	(3)			//奥行きの分割数

#define MESHFIELD_VERTEX			(9)			//メッシュフィールドの頂点数
#define INDEX_VERTEX				(14)		//インデックスバッファの数
#define SEPALATE_MESHFIELD			(12)		//メッシュフィールドの分割数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;		//テクスチャ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
MeshField				g_MeshField;					//メッシュフィールドの情報

//メッシュフィールドの初期化処理
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	g_MeshField.pos = MESHFIELD_POS;			//位置初期化
	g_MeshField.rot = ZERO_SET;					//向き初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/木箱000.png", &g_pTextureMeshField);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	for (int nCntMeshField = START_CNT; nCntMeshField < MESHFIELD_VERTEX; nCntMeshField++)
	{
		//頂点座標の設定
		pVtx[nCntMeshField].pos.x = -MESHFIELD_WIDTH * (ONE - (nCntMeshField % MESHFIELD_DIVISION_WIDTH));		//カウンターを "横の分割数で割った余り" を使って横の座標を割り出す
		pVtx[nCntMeshField].pos.z = +MESHFIELD_DEPTH * (ONE - (nCntMeshField / MESHFIELD_DIVISION_WIDTH));		//カウンターを "奥の分割数で割った答え" を使って奥の座標を割り出す
		pVtx[nCntMeshField].pos.y = NIL_F;																		//平面なので、Y座標は 0

		//nor の設定
		pVtx[nCntMeshField].nor = SET_NOR;
	
		//col の設定
		pVtx[nCntMeshField].col = RGBA_WHITE;

		//テクスチャ座標の座標
		pVtx[nCntMeshField].tex.x = HAL_F * (nCntMeshField % MESHFIELD_DIVISION_WIDTH);		//カウンターを "横の分割数で割った余り" を使って横の座標を割り出す
		pVtx[nCntMeshField].tex.y = HAL_F * (nCntMeshField / MESHFIELD_DIVISION_DEPTH);		//カウンターを "奥の分割数で割った答え" を使って奥の座標を割り出す
	}

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_VERTEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);

	//インデックス情報のバッファ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pIdx, ZERO);

	//頂点番号データの設定
					/*左側*/		/*中央*/			/*右側*/
	/*奥組*/		 pIdx[1] = 0;	 pIdx[3] = 1;	 pIdx[5] = 2;
	/*奥組*/										 pIdx[6] = 2;
	 
	/*真ん中組*/	 pIdx[0] = 3;	 pIdx[2] = 4;	 pIdx[4] = 5;
	/*真ん中組*/	 pIdx[9] = 3;	pIdx[11] = 4;	pIdx[13] = 5;

	/*手前組*/		 pIdx[7] = 6;	pIdx[10] = 7;	pIdx[12] = 8;
	/*手前組*/		 pIdx[8] = 6;

	/*インデックス番号　→  0  1  2  3  4  5 | 6  7 | 8  9  10 11 12 13
			  頂点番号		3  0  4  1  5  2 | 2  6 | 6  3  7  4  8  5  */

	int nNumVtx = ZERO;		//頂点番号
	for (int nCntIdx = START_CNT; nCntIdx < INDEX_VERTEX; nCntIdx++)
	{
		if (nCntIdx != 6 && nCntIdx != 7)
		{
			//カウンターが偶数の場合
			if (nCntIdx % SEC == ZERO)
			{
				//インデックス番号設定
				pIdx[nCntIdx] = nNumVtx + MESHFIELD_DIVISION_WIDTH;		//インデックス番号 = 基準の頂点番号 + 幅の分割数
			}

			//カウンターが奇数の場合
			else if (nCntIdx % SEC == ONE)
			{
				//インデックス番号設定
				pIdx[nCntIdx] = nNumVtx;								//インデックス番号 = 基準の頂点番号

				//基準の頂点番号加算
				nNumVtx++;
			}
		}

		//縮退ポリゴンの最初の重複頂点はひとつ前のインデックス番号と同じ
		else if (nCntIdx == 6)
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - ONE];
		}

		//縮退ポリゴンの最後の重複頂点は次のインデックス番号と同じ
		else if (nCntIdx == 7)
		{
			pIdx[nCntIdx] = pIdx[nCntIdx + ONE];
		}
	}

	//インデックスバッファをロックする
	g_pIdxBuffMeshField->Unlock();

	//頂点バッファをロックする
	g_pVtxBuffMeshField->Unlock();
}

//メッシュフィールドの終了処理
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//メッシュフィールドの更新処理
void UpdateMeshField(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	g_pVtxBuffMeshField->Unlock();

}

//メッシュフィールドの描画処理
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshField.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);

	D3DXMatrixMultiply(&g_MeshField.mtxWorld, &g_MeshField.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);

	D3DXMatrixMultiply(&g_MeshField.mtxWorld, &g_MeshField.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshField.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshField, ZERO, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, ZERO, ZERO, MESHFIELD_VERTEX, ZERO, SEPALATE_MESHFIELD);
}