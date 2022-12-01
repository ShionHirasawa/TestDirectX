/*==========================================================================================================================================================

														エフェクト処理[effect.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "effect.h"
#include "shadow.h"
#include "color.h"
#include "billboard.h"

//マクロ定義
#define MAX_EFFECT		(20000)		//エフェクトの最大数
#define EFFECT_SIZE		(10.0f)		//エフェクトの大きさ
#define EFFECT_LIFE		(50)		//エフェクトの寿命

//エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	float fRadius;			//半径(大きさ)
	int nLife;				//寿命(表示時間)
	bool bUse;				//使用しているかどうか
}Effect;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//頂点バッファへのポインタ
Effect					g_aEffect[MAX_EFFECT];			//エフェクトの情報

//エフェクトの初期化処理
void InitEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//エフェクトの情報の初期化
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos		= ZERO_SET;			//位置の初期化
		g_aEffect[nCntEffect].col		= XCOL_WHITE;		//色の初期化
		g_aEffect[nCntEffect].fRadius	= F_ZERO;			//半径(大きさ)の初期化
		g_aEffect[nCntEffect].nLife		= EFFECT_LIFE;		//寿命の初期化
		g_aEffect[nCntEffect].bUse		= false;			//使用していない状態にする

		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
		}

		//rhwの設定
		{
			pVtx[ZERO].nor = SET_NOR;
			pVtx[ONE].nor = SET_NOR;
			pVtx[SEC].nor = SET_NOR;
			pVtx[SAN].nor = SET_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = XCOL_YELLOW;
			pVtx[ONE].col = XCOL_YELLOW;
			pVtx[SEC].col = XCOL_YELLOW;
			pVtx[SAN].col = XCOL_YELLOW;
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2( MAX_POS,    ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2( ORIGIN_POS, MAX_POS);
			pVtx[SAN].tex = D3DXVECTOR2( MAX_POS,    MAX_POS);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの終了処理
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//エフェクトの更新処理
void UpdateEffect(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			 //寿命と大きさを減らしていく
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].fRadius >= F_ZERO)
			{
				g_aEffect[nCntEffect].fRadius -= 0.2f;
			}

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fRadius / 200.0f;

			if (g_aEffect[nCntEffect].nLife <= ZERO || g_aEffect[nCntEffect].fRadius < F_ZERO)
			{
				g_aEffect[nCntEffect].bUse = false;		//使用していない状態にする
			}

			//頂点座標の設定
			{
				pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//頂点カラーの設定
			{
				pVtx[ZERO].col = g_aEffect[nCntEffect].col;
				pVtx[ONE].col = g_aEffect[nCntEffect].col;
				pVtx[SEC].col = g_aEffect[nCntEffect].col;
				pVtx[SAN].col = g_aEffect[nCntEffect].col;
			}

		}

		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用
	BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

	if (pBill->bAlphaTest == true)
	{
		//αテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
	}

	//Zテストを無効にする
	if (pBill->bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファの有効無効設定
	}

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	//ビルボードの描画設定
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aEffect[nCntEffect].mtxWorld._41 = F_ZERO;		//→↓
			g_aEffect[nCntEffect].mtxWorld._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_aEffect[nCntEffect].mtxWorld._43 = F_ZERO;		//→↑

																	//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffEffect, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureEffect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * NUM_VERTEX, SEC);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない

			//エフェクト情報の設定
			g_aEffect[nCntEffect].pos = pos;			//位置代入
			g_aEffect[nCntEffect].col = col;			//色彩代入
			g_aEffect[nCntEffect].fRadius = fRadius;	//半径代入
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;	//寿命代入
			g_aEffect[nCntEffect].bUse = true;			//使用している状態にする

			//頂点情報の設定
			//頂点座標の設定
			{
				pVtx[ZERO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius,  g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3( -g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//頂点カラーの設定
			{
				pVtx[ZERO].col = g_aEffect[nCntEffect].col;
				pVtx[ONE].col  = g_aEffect[nCntEffect].col;
				pVtx[SEC].col  = g_aEffect[nCntEffect].col;
				pVtx[SAN].col  = g_aEffect[nCntEffect].col;
			}

			break;
		}
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffEffect->Unlock();
}