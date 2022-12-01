/*==========================================================================================================================================================

															ライド処理[light.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "light.h"

//マクロ定義
#define NUM_LIGHT		(3)			//ライトの数
#define LIGHT_VEC_0		D3DXVECTOR3(0.4f, -0.8f, 0.2f)		//ライトNo.1の方向
#define LIGHT_VEC_1		D3DXVECTOR3(0.0f, 0.8f, 0.0f)		//ライトNo.2の方向
#define LIGHT_VEC_2		D3DXVECTOR3(0.2f, -0.8f, -0.4f)		//ライトNo.3の方向

//グローバル変数
D3DLIGHT9 g_light[NUM_LIGHT];	//ライトの情報

//ライトの初期化処理
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXVECTOR3 vecDir[NUM_LIGHT];				//設定用方向ベクトル

	//ライトの情報をクリアする
	for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
	{
		switch (nCntLight)
		{
		case 0:
			vecDir[0] = LIGHT_VEC_0;					//ライトNo.0の方向
			break;

		case 1:
			vecDir[1] = LIGHT_VEC_1;					//ライトNo.1の方向
			break;

		case 2:
			vecDir[2] = LIGHT_VEC_2;					//ライトNo.2の方向
			break;
		}
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//ベクトルを正規化する
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//ライトの終了処理
void UninitLight(void)
{

}

//ライトの更新処理
void UpdateLight(void)
{

}