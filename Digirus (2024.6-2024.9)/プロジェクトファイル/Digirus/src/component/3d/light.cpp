//=============================================================
//
// ���C�g�R���|�[�l���g [light.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "light.h"
#include "manager.h"

//=============================================================
// [CD3DLight] ������
//=============================================================
void CD3DLight::Init()
{

}

//=============================================================
// [CD3DLight] �I��
//=============================================================
void CD3DLight::Uninit()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �L���ȃ��C�g���擾
	std::vector<CD3DLight*> lightList = GetComponents<CD3DLight>(false, true);

	// �Ō���̃��C�g�𖳌��ɂ���
	pDevice->LightEnable(lightList.size() - 1, FALSE);
}

//=============================================================
// [CD3DLight] �X�V
//=============================================================
void CD3DLight::Update()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �L���ȃ��C�g���擾
	std::vector<CD3DLight*> lightList = GetComponents<CD3DLight>(false, true);

	for (int i = 0; i < (int)lightList.size(); i++)
	{
		// ���C�g��ݒ肷��
		pDevice->SetLight(i, &lightList[i]->m_Light);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(i, TRUE);
	}
}



//=============================================================
// [CLight] �R���X�g���N�^
//=============================================================
CLight::CLight(float fIntensity, D3DXCOLOR color, D3DXCOLOR volumeColor)
{
	m_col = color;
	m_volumeCol = volumeColor;
	m_fIntensity = fIntensity;
}