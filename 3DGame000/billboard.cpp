/*==========================================================================================================================================================

														ビルボード処理[billboard.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "billboard.h"

#define NUM_BILLBOARD		(2)									//ビルボードの数
#define BILLBOARD_POS		D3DXVECTOR3(-200.0f, 0.0f, 0.0f)	//位置
#define BILLBOARD_WIDTH		(80.0f)								//幅サイズ
#define BILLBOARD_HEIGHT	(180.0f)							//奥行きサイズ
#define BILLBOARD_NOR		D3DXVECTOR3(0.0f, 0.0f, -1.0f)		//nor の値
#define MAX_ALPHA			(255)								//αテストの最大値

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureBillboard = NULL;		//テクスチャ
Billboard				g_Billboard[NUM_BILLBOARD];		//ビルボードの情報
BillTest				g_BillTest;						//αテストなどの情報

//ビルボードの初期化処理
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tree000.png", &g_pTextureBillboard);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_BILLBOARD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBill = START_CNT; nCntBill < NUM_BILLBOARD; nCntBill++, pVtx += NUM_VERTEX)
	{
		g_Billboard[nCntBill].pos = BILLBOARD_POS;	//位置初期化
		g_Billboard[nCntBill].rot = ZERO_SET;		//向き初期化
		g_Billboard[nCntBill].pos.x += nCntBill * 400.0f;

		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(-BILLBOARD_WIDTH,  BILLBOARD_HEIGHT, g_Billboard[nCntBill].pos.z);
			pVtx[ONE].pos =  D3DXVECTOR3( BILLBOARD_WIDTH,  BILLBOARD_HEIGHT, g_Billboard[nCntBill].pos.z);
			pVtx[SEC].pos =  D3DXVECTOR3(-BILLBOARD_WIDTH,       F_ZERO     , g_Billboard[nCntBill].pos.z);
			pVtx[SAN].pos =  D3DXVECTOR3( BILLBOARD_WIDTH,       F_ZERO     , g_Billboard[nCntBill].pos.z);
		}

		//norの設定
		{
			pVtx[ZERO].nor = SET_NOR;
			pVtx[ONE].nor =  SET_NOR;
			pVtx[SEC].nor =  SET_NOR;
			pVtx[SAN].nor =  SET_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = RGBA_WHITE;
			pVtx[ONE].col = RGBA_WHITE;
			pVtx[SEC].col = RGBA_WHITE;
			pVtx[SAN].col = RGBA_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffBillboard->Unlock();

	g_BillTest.AlphaTest = ZERO;	//αテストの値初期化
	g_BillTest.bAlphaTest = true;	//αテストが行われているか
	g_BillTest.bZTest = true;		//Zテストが行われているか
}	

//ビルボードの終了処理
void UninitBillboard(void)
{
	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//ビルボードの更新処理
void UpdateBillboard(void)
{
	////ポインタを設定
	//VERTEX_3D *pVtx;

	////頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffBillboard->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	////頂点バッファをロックする
	//g_pVtxBuffBillboard->Unlock();

	//αテストの値を増加
	if (GetKeyboardPress(DIK_9) == true)
	{
		g_BillTest.AlphaTest++;

		//255を超えた
		if (g_BillTest.AlphaTest > MAX_ALPHA)
		{
			g_BillTest.AlphaTest = MAX_ALPHA;		//255に戻す
		}
	}

	//αテストの値を減少
	if (GetKeyboardPress(DIK_0) == true)
	{
		g_BillTest.AlphaTest--;

		//ゼロ未満になった
		if (g_BillTest.AlphaTest < ZERO)
		{
			g_BillTest.AlphaTest = ZERO;		//ゼロに戻す
		}
	}

	//αテストの有効 / 無効切り替え
	if (GetKeyboardTrigger(DIK_F3) == true)
	{
		g_BillTest.bAlphaTest = g_BillTest.bAlphaTest ? false : true;
	}

	//Zテストの有効 / 無効切り替え
	if (GetKeyboardTrigger(DIK_F4) == true)
	{
		g_BillTest.bZTest = g_BillTest.bZTest ? false : true;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Billboard[ZERO].pos.x -= 1.0f;

		if (g_Billboard[ZERO].pos.x <= -600.0f)
		{
			g_Billboard[ZERO].pos.x = -600.0f;
		}
	}

	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Billboard[ZERO].pos.x += 1.0f;

		if (g_Billboard[ZERO].pos.x >= 600.0f)
		{
			g_Billboard[ZERO].pos.x = 600.0f;
		}
	}
}

//ビルボードの描画処理
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	//αテストを有効にする
	if (g_BillTest.bAlphaTest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, g_BillTest.AlphaTest);
	}

	//Zテストを無効にする
	if (g_BillTest.bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファの有効無効設定
	}	

	//ビルボードの描画設定
	for (int nCntBill = START_CNT; nCntBill < NUM_BILLBOARD; nCntBill++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Billboard[nCntBill].mtxWorld);

		//ビューマトリックスをGet
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&g_Billboard[nCntBill].mtxWorld, NULL, &mtxView);	//逆行列を求める
		g_Billboard[nCntBill].mtxWorld._41 = F_ZERO;		//→↓
		g_Billboard[nCntBill].mtxWorld._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
		g_Billboard[nCntBill].mtxWorld._43 = F_ZERO;		//→↑

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBill].pos.x, g_Billboard[nCntBill].pos.y, g_Billboard[nCntBill].pos.z);

		D3DXMatrixMultiply(&g_Billboard[nCntBill].mtxWorld, &g_Billboard[nCntBill].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCntBill].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffBillboard, ZERO, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureBillboard);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBill * NUM_VERTEX, SEC);
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_BillTest.AlphaTest);
}

BillTest *GetAlphaTest(void)
{
	return &g_BillTest;
}