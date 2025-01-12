//=============================================================
//
// �X�L���|�C���gUI [skillpt_ui.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "skillpt_ui.h"
#include "manager.h"
#include "skillpt.h"

// �ÓI�����o�ϐ��̏�����
const float CSkillPointUI::UI_SIZE = 250.0f;
const float CSkillPointUI::UI_OUTLINE = 0.06f;
const float CSkillPointUI::UI_EVERYPERCENT = CSkillPointUI::UI_SIZE / CSkillPoint::MAX_POINT;
const int CSkillPointUI::MAX_PARTICLE;
const float CSkillPointUI::MAX_UPSPEED_PARTICLE = 0.8f;
const float CSkillPointUI::MIN_UPSPEED_PARTICLE = 0.4f;
const float CSkillPointUI::MAX_SIZE_PARTICLE = 22.0f;
const float CSkillPointUI::MIN_SIZE_PARTICLE = 15.0f;

//=============================================================
// [CSkillPointUI] ������
//=============================================================
void CSkillPointUI::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pMaskVtxBuff, nullptr);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pPointVtxBuff, nullptr);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pOutlineVtxBuff, nullptr);
	VERTEX_2D* pVtx; // ���_���ւ̃|�C���^

	for (int i = 0; i < 3; i++)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		if (i == 0) m_pMaskVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else if (i == 1) m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else m_pOutlineVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);

		if (i == 2)
		{ // �A�E�g���C��
			pVtx[0].pos *= 1.0f + UI_OUTLINE;
			pVtx[1].pos *= 1.0f + UI_OUTLINE;
			pVtx[2].pos *= 1.0f + UI_OUTLINE;
			pVtx[3].pos *= 1.0f + UI_OUTLINE;
		}

		// �ʒu
		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		if (i == 0) m_pMaskVtxBuff->Unlock();
		else if (i == 1) m_pPointVtxBuff->Unlock();
		else m_pOutlineVtxBuff->Unlock();
	}

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �e�N�X�`���̓ǂݍ���
	m_pMaskTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\mask.png")->GetTexture();
	m_pPointTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\skillpoint.png")->GetTexture();
	m_pBubbleTexture = CManager::GetDataManager()->RefTexture("data\\TEXTURE\\SKILL\\bubble.png")->GetTexture();

	// �ϐ��̏�����
	m_fPosGageY = UI_SIZE;
	m_fAlpha = 1.0f;
}

//=============================================================
// [CSkillPointUI] �I��
//=============================================================
void CSkillPointUI::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pMaskVtxBuff != nullptr)
	{
		m_pMaskVtxBuff->Release();
		m_pMaskVtxBuff = nullptr;
	}
	if (m_pPointVtxBuff != nullptr)
	{
		m_pPointVtxBuff->Release();
		m_pPointVtxBuff = nullptr;
	}
	if (m_pOutlineVtxBuff != nullptr)
	{
		m_pOutlineVtxBuff->Release();
		m_pOutlineVtxBuff = nullptr;
	}
	if (m_pParticleVtxBuff != nullptr)
	{
		m_pParticleVtxBuff->Release();
		m_pParticleVtxBuff = nullptr;
	}
}

//=============================================================
// [CSkillPointUI] �X�V
//=============================================================
void CSkillPointUI::Update()
{
	// �p�[�e�B�N���̏o��
	m_nNextParticleCounter--;
	if (m_nNextParticleCounter <= 0)
	{
		AddParticle();

		if (rand() % 5 <= 1)
		{
			m_nNextParticleCounter = rand() % 30 + 20;
		}
		else
		{
			m_nNextParticleCounter = rand() % 20 + 70;
		}
	}

	// �Q�[�W�̍X�V
	UpdateMoveGage();

	// �p�[�e�B�N���̍X�V
	UpdateParticle();
}

//=============================================================
// [CSkillPointUI] �`��
//=============================================================
void CSkillPointUI::DrawUI()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	DWORD dwCurZTest, dwCurZFunc;

	// �X�e���V���̃N���A
	pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 170, 170, 170), 1.0f, 0);

	// �}�X�N -------------------------------------------------------------------------------------------------------------

	// Z�o�b�t�@�̐ݒ��ύX����̂Ō��݂̏�Ԃ�ۑ����Ă���
	pDevice->GetRenderState(D3DRS_ZENABLE, &dwCurZTest);
	pDevice->GetRenderState(D3DRS_ZFUNC, &dwCurZFunc);

	// Z�o�b�t�@�ɏ������܂Ȃ��悤�ɂ��܂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// �X�e���V���o�b�t�@�̐ݒ�ł�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);  // �X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // �X�e���V���e�X�g�͏�ɍs��
	
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pMaskVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pMaskTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��


	// �Q�[�W�`�� -------------------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x03);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pPointVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pPointTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��


	// �p�[�e�B�N�� -------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x03);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// �p�[�e�B�N���̕`��
	DrawParticle();



	// �A�E�g���C�� -------------------------------------------------------------------------------------------------

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xfe);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pOutlineVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pMaskTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��

	// ���ɖ߂� --------------------------------------------------------------------------------------------------
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);
}

//=============================================================
// [CSkillPointUI] �����x�̐ݒ�
//=============================================================
void CSkillPointUI::SetAlpha(const float& fAlpha)
{
	m_fAlpha = fAlpha;

	VERTEX_2D* pVtx; // ���_���ւ̃|�C���^

	for (int i = 0; i < 3; i++)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		if (i == 0) m_pMaskVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else if (i == 1) m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		else m_pOutlineVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[
		D3DXCOLOR color = pVtx[0].col;
		color.a = fAlpha;
		pVtx[0].col = color;
		pVtx[1].col = color;
		pVtx[2].col = color;
		pVtx[3].col = color;

		// ���_�o�b�t�@���A�����b�N����
		if (i == 0) m_pMaskVtxBuff->Unlock();
		else if (i == 1) m_pPointVtxBuff->Unlock();
		else m_pOutlineVtxBuff->Unlock();
	}
}

//=============================================================
// [CSkillPointUI] �i�s�x�̐ݒ�
//=============================================================
void CSkillPointUI::SetPoint(const int& nPercent)
{
	if (0 <= nPercent && nPercent <= CSkillPoint::MAX_POINT)
	{
		m_nSkillPt = nPercent;
	}
}

//=============================================================
// [CSkillPointUI] �p�[�e�B�N���̏�����
//=============================================================
void CSkillPointUI::InitParticle()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pParticleVtxBuff, nullptr);

	VERTEX_2D* pVtx; // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pParticleVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pParticleVtxBuff->Unlock();


	// �ϐ��̏�����
	m_nNextParticleCounter = 0;
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		m_bubbleParticle[i].pos = { 0.0f, 0.0f, 0.0f };
		m_bubbleParticle[i].move = { 0.0f, 0.0f, 0.0f };
		m_bubbleParticle[i].fDeepColor = 0.0f;
		m_bubbleParticle[i].fScale = 0.0f;
		m_bubbleParticle[i].bUse = false;
	}
}

//=============================================================
// [CSkillPointUI] �p�[�e�B�N���̍X�V
//=============================================================
void CSkillPointUI::UpdateParticle()
{
	VERTEX_2D* pVtx; // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pParticleVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (m_bubbleParticle[i].bUse)
		{ // �p�[�e�B�N�����g���Ă���Ƃ�

			// �ړ��ʂ����Z����
			m_bubbleParticle[i].pos += m_bubbleParticle[i].move;

			// �Q�[�W�ȏ�̍����܂œ��B�����Ƃ��j��
			if (m_bubbleParticle[i].pos.y < transform->GetWPos().y - UI_SIZE)
			{
				m_bubbleParticle[i].bUse = false;
			}

			// �����x������ɂ���
			if (m_fAlpha < m_bubbleParticle[i].fDeepColor)
			{
				m_bubbleParticle[i].fDeepColor = m_fAlpha;
			}

			// �����X�V����
			pVtx[0].pos = D3DXVECTOR3(-m_bubbleParticle[i].fScale * 0.5f, -m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_bubbleParticle[i].fScale * 0.5f, -m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-m_bubbleParticle[i].fScale * 0.5f, m_bubbleParticle[i].fScale * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_bubbleParticle[i].fScale * 0.5f, m_bubbleParticle[i].fScale * 0.5f, 0.0f);

			pVtx[0].pos +=m_bubbleParticle[i].pos;
			pVtx[1].pos +=m_bubbleParticle[i].pos;
			pVtx[2].pos +=m_bubbleParticle[i].pos;
			pVtx[3].pos +=m_bubbleParticle[i].pos;

			D3DXCOLOR col = pVtx[0].col;
			col.a = m_bubbleParticle[i].fDeepColor;
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pParticleVtxBuff->Unlock();
}

//=============================================================
// [CSkillPointUI] �p�[�e�B�N���̕`��
//=============================================================
void CSkillPointUI::DrawParticle()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pParticleVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (m_bubbleParticle[i].bUse)
		{ // �p�[�e�B�N�����g���Ă���Ƃ�
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pBubbleTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}
}

//=============================================================
// [CSkillPointUI] �p�[�e�B�N���̒ǉ�
//=============================================================
void CSkillPointUI::AddParticle()
{
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		if (!m_bubbleParticle[i].bUse)
		{ // �p�[�e�B�N�����g���Ă��Ȃ��Ƃ�
			srand((unsigned int)clock());

			// �o���ʒu�����߂�
			m_bubbleParticle[i].pos = transform->GetWPos();
			m_bubbleParticle[i].pos.x += fmod(rand(), UI_SIZE) - UI_SIZE * 0.5f;
			m_bubbleParticle[i].pos.y += UI_SIZE * 0.5f;

			// �ړ��ʂ����߂�
			m_bubbleParticle[i].move.x = 0.0f;
			m_bubbleParticle[i].move.y = fmod(rand(), MAX_UPSPEED_PARTICLE - MIN_UPSPEED_PARTICLE) + MIN_UPSPEED_PARTICLE;
			m_bubbleParticle[i].move.y *= -1;

			// �T�C�Y�����߂�
			m_bubbleParticle[i].fScale = fmod(rand(), MAX_SIZE_PARTICLE - MIN_SIZE_PARTICLE) + MIN_SIZE_PARTICLE;

			// �F�̐[�݂����߂�
			m_bubbleParticle[i].fDeepColor = rand() % 50 * 0.01f + 0.3f;

			m_bubbleParticle[i].bUse = true;		// �g�p��ԂɕύX
			break;
		}
	}
}

//=============================================================
// [CSkillPointUI] �Q�[�W�̈ړ��X�V
//=============================================================
void CSkillPointUI::UpdateMoveGage()
{
	VERTEX_2D* pVtx; // ���_���ւ̃|�C���^

	// ���B�ڕW�ʒu���v�Z����
	float objectivePosY = UI_EVERYPERCENT * (CSkillPoint::MAX_POINT - m_nSkillPt);

	// �ړ�����
	m_fPosGageY += (objectivePosY - m_fPosGageY) * 0.08f;


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pPointVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_SIZE * 0.5f, -UI_SIZE * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_SIZE * 0.5f, UI_SIZE * 0.5f, 0.0f);

	// �ʒu�̉��Z
	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

	// �p�[�Z���g�ɑΉ�������
	
	pVtx[0].pos.y += m_fPosGageY;
	pVtx[1].pos.y += m_fPosGageY;
	pVtx[2].pos.y += m_fPosGageY;
	pVtx[3].pos.y += m_fPosGageY;

	// ���_�o�b�t�@���A�����b�N����
	m_pPointVtxBuff->Unlock();
}