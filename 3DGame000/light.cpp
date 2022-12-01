/*==========================================================================================================================================================

															���C�h����[light.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "light.h"

//�}�N����`
#define NUM_LIGHT		(3)			//���C�g�̐�
#define LIGHT_VEC_0		D3DXVECTOR3(0.4f, -0.8f, 0.2f)		//���C�gNo.1�̕���
#define LIGHT_VEC_1		D3DXVECTOR3(0.0f, 0.8f, 0.0f)		//���C�gNo.2�̕���
#define LIGHT_VEC_2		D3DXVECTOR3(0.2f, -0.8f, -0.4f)		//���C�gNo.3�̕���

//�O���[�o���ϐ�
D3DLIGHT9 g_light[NUM_LIGHT];	//���C�g�̏��

//���C�g�̏���������
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir[NUM_LIGHT];				//�ݒ�p�����x�N�g��

	//���C�g�̏����N���A����
	for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
	{
		switch (nCntLight)
		{
		case 0:
			vecDir[0] = LIGHT_VEC_0;					//���C�gNo.0�̕���
			break;

		case 1:
			vecDir[1] = LIGHT_VEC_1;					//���C�gNo.1�̕���
			break;

		case 2:
			vecDir[2] = LIGHT_VEC_2;					//���C�gNo.2�̕���
			break;
		}
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���C�g�̕�����ݒ�
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//�x�N�g���𐳋K������
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ�
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//���C�g�̏I������
void UninitLight(void)
{

}

//���C�g�̍X�V����
void UpdateLight(void)
{

}