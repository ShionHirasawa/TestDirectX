/*==========================================================================================================================================================

															壁の処理[wall.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "wall.h"

//マクロ定義
#define NUM_WALL		(4)				//壁の数
#define WALL_WIDTH		(600.0f)		//幅サイズ
#define WALL_HEIGHT		(200.0f)		//高さサイズ
#define WALL_DEPTH		(600.0f)		//奥行きサイズ
#define TEX_WIDTH		(6.0f)			//テクスチャの横幅を増やす
#define TEX_HEIGHT		(2.0f)			//テクスチャの横幅を増やす
#define WALL_ROT		(0.01f)			//壁の回転値

//1つ目の壁のマクロ
#define WALL_POS_000	D3DXVECTOR3(0.0f, 0.0f, 600.0f)		//壁の場所
#define WALL_ROT_000	D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//壁の向き
#define WALL_TEX_000	"data/TEXTURE/wall000.jpg"			//テクスチャのパス

//2つ目の壁のマクロ
#define WALL_POS_001	D3DXVECTOR3(600.0f, 0.0f, 0.0f)		//壁の場所
#define WALL_ROT_001	D3DXVECTOR3(0.0f, 1.57f, 0.0f)		//壁の向き
#define WALL_TEX_001	"data/TEXTURE/wall001.jpg"			//テクスチャのパス

//3つ目の壁のマクロ
#define WALL_POS_002	D3DXVECTOR3(0.0f, 0.0f, -600.0f)	//壁の場所
#define WALL_ROT_002	D3DXVECTOR3(0.0f, 3.14f, 0.0f)		//壁の向き
#define WALL_TEX_002	"data/TEXTURE/wall001.jpg"			//テクスチャのパス

//4つ目の壁のマクロ
#define WALL_POS_003	D3DXVECTOR3(-600.0f, 0.0f, 0.0f)	//壁の場所
#define WALL_ROT_003	D3DXVECTOR3(0.0f, -1.57f, 0.0f)		//壁の向き
#define WALL_TEX_003	"data/TEXTURE/wall002.jpg"			//テクスチャのパス

//壁の種類構造体
typedef enum
{
	WALLTYPE_D = ZERO,	//奥
	WALLTYPE_R,			//右
	WALLTYPE_F,			//手前
	WALLTYOE_L,			//左
	WALLTYPE_MAX
}WALLTYPE;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureWall[NUM_WALL] = {};	//テクスチャ
Wall					g_Wall[NUM_WALL];				//壁の情報
float					g_fAngle = F_ZERO;				//基準の角度

//壁の初期化処理
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ	

	g_Wall[WALLTYPE_D].pos = WALL_POS_000;	/*位置初期化*/	g_Wall[WALLTYPE_D].rot = WALL_ROT_000;	/*向き初期化*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_000, &g_pTextureWall[WALLTYPE_D]);	//テクスチャの読み込み
	g_Wall[WALLTYPE_R].pos = WALL_POS_001;	/*位置初期化*/	g_Wall[WALLTYPE_R].rot = WALL_ROT_001;	/*向き初期化*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_001, &g_pTextureWall[WALLTYPE_R]);	//テクスチャの読み込み
	g_Wall[WALLTYPE_F].pos = WALL_POS_002;	/*位置初期化*/	g_Wall[WALLTYPE_F].rot = WALL_ROT_002;	/*向き初期化*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_002, &g_pTextureWall[WALLTYPE_F]);	//テクスチャの読み込み
	g_Wall[WALLTYOE_L].pos = WALL_POS_003;	/*位置初期化*/	g_Wall[WALLTYOE_L].rot = WALL_ROT_003;	/*向き初期化*/	D3DXCreateTextureFromFile(pDevice, WALL_TEX_003, &g_pTextureWall[WALLTYOE_L]);	//テクスチャの読み込み

	g_Wall[WALLTYPE_D].fWidth = WALL_WIDTH;	/*幅初期化*/	g_Wall[WALLTYPE_D].fHeight = WALL_HEIGHT;	/*高さ初期化*/	g_Wall[WALLTYPE_D].fDepth = F_ZERO;			/*奥行き初期化*/
	g_Wall[WALLTYPE_R].fWidth = F_ZERO;		/*幅初期化*/	g_Wall[WALLTYPE_R].fHeight = WALL_HEIGHT;	/*高さ初期化*/	g_Wall[WALLTYPE_R].fDepth = WALL_DEPTH;		/*奥行き初期化*/
	g_Wall[WALLTYPE_F].fWidth = -WALL_WIDTH;/*幅初期化*/	g_Wall[WALLTYPE_F].fHeight = WALL_HEIGHT;	/*高さ初期化*/	g_Wall[WALLTYPE_F].fDepth = F_ZERO;			/*奥行き初期化*/
	g_Wall[WALLTYOE_L].fWidth = F_ZERO;		/*幅初期化*/	g_Wall[WALLTYOE_L].fHeight = WALL_HEIGHT;	/*高さ初期化*/	g_Wall[WALLTYOE_L].fDepth = -WALL_DEPTH;	/*奥行き初期化*/

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * NUM_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += NUM_VERTEX)
	{
		//頂点座標の設定
		{
			pVtx[VERTEX_LE_UP].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	g_Wall[nCntWall].fHeight,	+g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_RI_UP].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	g_Wall[nCntWall].fHeight,	-g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_LE_DO].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	NIL_F,						+g_Wall[nCntWall].fDepth);
			pVtx[VERTEX_RI_DO].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	NIL_F,						-g_Wall[nCntWall].fDepth );
		}

		D3DXVECTOR3 Wallnor;
		switch (nCntWall)
		{
		case WALLTYPE_D:	//奥の壁
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;

		case WALLTYPE_R:	//右の壁
			Wallnor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			break;
			
		case WALLTYPE_F:	//手前の壁
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;

		case WALLTYOE_L:	//左の壁
			Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		}

		Wallnor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//norの設定
		{
			pVtx[VERTEX_LE_UP].nor = Wallnor;
			pVtx[VERTEX_RI_UP].nor = Wallnor;
			pVtx[VERTEX_LE_DO].nor = Wallnor;
			pVtx[VERTEX_RI_DO].nor = Wallnor;
		}

		//頂点カラーの設定
		{
			pVtx[VERTEX_LE_UP].col = RGBA_WHITE;
			pVtx[VERTEX_RI_UP].col = RGBA_WHITE;
			pVtx[VERTEX_LE_DO].col = RGBA_WHITE;
			pVtx[VERTEX_RI_DO].col = RGBA_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VERTEX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VERTEX_RI_UP].tex = D3DXVECTOR2(TEX_WIDTH, ORIGIN_POS);
			pVtx[VERTEX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, TEX_HEIGHT);
			pVtx[VERTEX_RI_DO].tex = D3DXVECTOR2(TEX_WIDTH,  TEX_HEIGHT);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();
}

//壁の終了処理
void UninitWall(void)
{
	//テクスチャの破棄
	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		if (g_pTextureWall[nCntWall] != NULL)
		{
			g_pTextureWall[nCntWall]->Release();
			g_pTextureWall[nCntWall] = NULL;
		}
	}	

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//壁の更新処理
void UpdateWall(void)
{
	//壁の回転
	//RotWall();

	//壁の向き初期化
	ResetRotWall();

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();
}

//壁の回転処理
void RotWall(void)
{
	float fSpin = F_ZERO;		//回転値

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		//全壁回転
		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			g_Wall[nCntWall].rot.y += WALL_ROT;		//回転値代入

			g_Wall[nCntWall].pos.x = sinf(D3DX_PI - g_Wall[nCntWall].rot.y) * 600.0f;	//壁回転
			g_Wall[nCntWall].pos.z = cosf(D3DX_PI - g_Wall[nCntWall].rot.y) * 600.0f;	//壁回転
		}
	}

	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_fAngle -= WALL_ROT;		//回転値代入

		//全壁回転
		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			g_Wall[nCntWall].pos.x = sinf(g_fAngle - (D3DX_PI * g_Wall[nCntWall].rot.y)) * 600.0f;	//壁回転
			g_Wall[nCntWall].pos.z = cosf(g_fAngle - (D3DX_PI * g_Wall[nCntWall].rot.y)) * 600.0f;	//壁回転
		}
	}

	//壁の向き修正
	FixRotWall();
}

//壁の向き修正処理
void FixRotWall(void)
{
	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//壁の向きが 3.14を超えた
		if (g_Wall[nCntWall].rot.y >= D3DX_PI)
		{
			g_Wall[nCntWall].rot.y -= FIX_ROT;	//向き修正
		}

		//壁の向きが -3.14を超えた
		if (g_Wall[nCntWall].rot.y <= -D3DX_PI)
		{
			g_Wall[nCntWall].rot.y += FIX_ROT;	//向き修正
		}
	}
}

//壁の向き初期化
void ResetRotWall(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_Wall[WALLTYPE_D].pos = WALL_POS_000;	/*位置初期化*/
		g_Wall[WALLTYPE_R].pos = WALL_POS_001;	/*位置初期化*/
		g_Wall[WALLTYPE_F].pos = WALL_POS_002;	/*位置初期化*/
		g_Wall[WALLTYOE_L].pos = WALL_POS_003;	/*位置初期化*/

		g_Wall[WALLTYPE_D].rot = WALL_ROT_000;	/*向き初期化*/
		g_Wall[WALLTYPE_R].rot = WALL_ROT_001;	/*向き初期化*/
		g_Wall[WALLTYPE_F].rot = WALL_ROT_002;	/*向き初期化*/
		g_Wall[WALLTYOE_L].rot = WALL_ROT_003;	/*向き初期化*/
		g_fAngle = F_ZERO;
	}	
}

//壁の描画処理
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffWall, ZERO, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureWall[nCntWall]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}
}

//壁の当たり判定処理
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	bool bCollision = false;	//当たったかどうかの判定
	float fVecAnswer;			//外積計算の結果変数
	D3DXVECTOR3 vecToPos;		//オブジェクトの頂点 0番目から見た、対象の位置への角度
	D3DXVECTOR3 vecLine;		//オブジェクトの頂点 0番目から端までのベクトル

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += NUM_VERTEX)
	{
		vecToPos	= *pPos - g_Wall[nCntWall].pos;								//対象の位置から頂点0番目までのベクトル計算
		vecLine		= pVtx[VERTEX_RI_UP].pos - pVtx[VERTEX_LE_UP].pos;			//次の頂点から	頂点0番目までのベクトル計算

		fVecAnswer	= (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);		//対象の位置へのベクトル計算

		//現在の向きが "負"(左) である
		if (fVecAnswer <= F_ZERO)
		{
			bCollision = true;	//当たった
			break;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();

	//判定を返す
	return bCollision;
}

//壁の情報
Wall *GetWall(void)
{
	//壁の情報の先頭アドレスを返す
	return &g_Wall[WALLTYPE_D];
}