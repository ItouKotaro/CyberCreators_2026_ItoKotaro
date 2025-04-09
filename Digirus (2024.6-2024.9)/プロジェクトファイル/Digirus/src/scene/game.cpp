//=============================================================
//
// ゲームシーン [game.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "game.h"
#include "component.h"
#include "component/3d/camera.h"
#include "component/3d/light.h"
#include "component/3d/mesh.h"
#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "component/3d/collision.h"
#include "component/other/render_objects_texture.h"
#include "component/other/sound.h"
#include "component/3d/skybox.h"
#include "scripts/bar.h"
#include "scripts/player.h"
#include "scripts/camera_move.h"
#include "scripts/enemy.h"
#include "scripts/next_portal.h"
#include "scripts/jumppad.h"
#include "scripts/skillpt_ui.h"
#include "scripts/shard.h"
#include "scripts/laser_block.h"
#include "scripts/move_tile.h"
#include "scripts/cannon.h"
#include "scripts/damage_ball.h"
#include "manager.h"
#include "benlib.h"
#include "operation_system.h"
#include <fstream>
#include "component/3d/billboard.h"

const int CGameScene::RESULT_BAR_SPACE = 800;
const D3DXVECTOR2 CGameScene::TIME_POS = D3DXVECTOR2(CRenderer::SCREEN_WIDTH - 180.0f, 60.0f);

//=============================================================
// [CGameScene] 読み込むワールドを設定する
//=============================================================
void CGameScene::SetWorld(const std::string& path)
{
	// パスを記録しておく
	strcpy(&m_sWorldPath[0], path.c_str());

	// 名前を記録する
	std::string sWorldName = path;
	sWorldName.erase(sWorldName.length() - strlen(COperationSystem::WORLD_FILENAME) - 1, strlen(COperationSystem::WORLD_FILENAME) + 1);
	for (int i = sWorldName.length() - 1; 0 <= i; i--)
	{
		if (sWorldName[i] == '\\')
		{ // 区切りが来たとき
			sWorldName.erase(0, i+1);
			break;
		}
	}
	strcpy(&m_sWorldName[0], sWorldName.c_str());
 }

//=============================================================
// [CGameScene] 制限時間を設定する
//=============================================================
void CGameScene::SetLimitTime(const int& nSec)
{
	m_nLimitTime = nSec * 1000;
}

//=============================================================
// [CGameScene] 明るさを設定する
//=============================================================
void CGameScene::SetAmbient(const float& fAmbient)
{
	m_fAmbient = fAmbient;
}

//=============================================================
// [CGameScene] スカイボックスを設定する
//=============================================================
void CGameScene::SetSkybox(const std::string& sPath)
{
	strcpy(&m_sSkyboxPath[0], sPath.c_str());
}

//=============================================================
// [CGameScene] BGMを設定する
//=============================================================
void CGameScene::SetBGM(const std::string& sPath, const float& fVolume)
{
	strcpy(&m_sBGMPath[0], sPath.c_str());
	m_fBGMVolume = fVolume;
}

//=============================================================
// [CGameScene] ゲームオーバー時
//=============================================================
void CGameScene::OnGameOver()
{
	m_bGameOver = true;

	// BGMを停止する
	if (m_pBGM != nullptr)
	{
		m_pBGM->GetComponent<CSound>()->FadeOut(0.01f);
	}

	// 現在のオブジェクトを残した状態でシーンを切り替える
	CSceneManager::GetInstance()->SetScene("dead", false);
}

//=============================================================
// [CGameScene] 初期化
//=============================================================
void CGameScene::Init()
{
	// 変数の初期化
	m_nScore = 0;
	m_fScoreFade = 0.0f;
	m_bGameOver = false;

	// フェードの作成
	m_pFade = new GameObject();
	m_pFade->SetPriority(10);
	m_pFade->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_pFade->AddComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// ポーズの初期化
	m_pPause = new GameObject;
	m_pPause->SetName("Pause");
	m_pPause->AddComponent<CPause>();

	// スキルガイド
	m_pSkillGuide = new GameObject;
	m_pSkillGuide->transform->SetSize(170.0f, 100.0f);
	m_pSkillGuide->transform->SetPos(1700.0f, 950.0f);
	m_pSkillGuide->AddComponent<CPolygon>();
	m_pSkillGuide->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SKILL\\skill_control.png");

	// クロック音
	m_pClockSound = new GameObject;
	m_pClockSound->AddComponent<CSound>();
	m_pClockSound->GetComponent<CSound>()->LoadWav("data\\SOUND\\SE\\clock.wav");
	m_pClockSound->GetComponent<CSound>()->SetLoop(true);

	// BGM
	if (strlen(m_sBGMPath) > 0)
	{
		m_pBGM = new GameObject;
		m_pBGM->SetTag("result");
		m_pBGM->AddComponent<CSound>();
		m_pBGM->GetComponent<CSound>()->LoadWav(m_sBGMPath);
		m_pBGM->GetComponent<CSound>()->SetVolume(m_fBGMVolume);
		m_pBGM->GetComponent<CSound>()->SetLoop(true);
		m_pBGM->GetComponent<CSound>()->Play();
	}

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// リザルトの初期化
	InitResult();

	// タイトルテキスト
	m_nTitleCounter = 100;
	m_nTitleTimeCounter = 130;
	m_fTitleAlpha = 1.0f;
	m_pTitleName = new GameObject;
	m_pTitleName->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 300.0f);
	m_pTitleName->AddComponent<CText>();
	m_pTitleName->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	m_pTitleName->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_pTitleName->GetComponent<CText>()->SetFontSize(160);
	m_pTitleName->GetComponent<CText>()->SetText(m_sWorldName);
	m_pTitleName->GetComponent<CText>()->SetOutlineSize(4);
	m_pTitleName->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pTitleName->GetComponent<CText>()->SetEdgeColor(D3DCOLOR_RGBA(0, 0, 0, 255));

	// ステージローダーの生成
	m_pStageLoader = new StageLoader();
	m_pStageLoader->Load(m_sWorldPath);

	// 制限時間を設定する
	m_nCurrentTime = 0;
	m_bIsGoal = false;
	m_pTimeUI = new GameObject;
	m_pTimeUI->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 600);
	m_pTimeUI->AddComponent<CText>();
	m_pTimeUI->GetComponent<CText>()->SetFont("ベストテン-CRT");
	m_pTimeUI->GetComponent<CText>()->SetFontSize(120);
	m_pTimeUI->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
}


//=============================================================
// [CGameScene] 終了
//=============================================================
void CGameScene::Uninit()
{
	// ステージローダーの破棄
	if (m_pStageLoader != nullptr)
	{
		delete m_pStageLoader;
		m_pStageLoader = nullptr;
	}
}

//=============================================================
// [CGameScene] 更新
//=============================================================
void CGameScene::Update()
{
	// ゲームオーバー
	if (m_bGameOver)
		return;

	// ポーズ中は以降の処理を無視する
	if (m_pPause->GetComponent<CPause>()->GetIsPause())
		return;

	// クロックサウンド
	if (m_nLimitTime - 10000 < m_nCurrentTime && !m_bGameOver)
	{ // 制限時間10秒前のとき
		if (m_pClockSound->GetComponent<CSound>()->GetState() != CSound::PLAYING)
		{
			m_pClockSound->GetComponent<CSound>()->Play();
		}
	}

	// 時間の更新処理
	UpdateTime();

	// スタートアニメーションの更新処理
	UpdateStartAnim();

	// リザルトの更新
	UpdateResult();
}

//=============================================================
// [CGameScene] 時間の更新
//=============================================================
void CGameScene::UpdateTime()
{
	// 現在時間に加算する
	m_nCurrentTime += static_cast<int>(CManager::GetDeltaTime() * 1000);

	// 制限時間
	int nRemainingTime = (m_nLimitTime - m_nCurrentTime) / 1000 + 1;
	if (!m_bIsGoal && m_nCurrentTime >= m_nLimitTime)
	{ // 制限時間になったとき
		GameObject* pPlayer = GameObject::Find("Player");
		if (pPlayer != nullptr && pPlayer->GetComponent<CPlayer>() != nullptr)
		{
			m_pClockSound->GetComponent<CSound>()->Stop();
			pPlayer->GetComponent<CPlayer>()->OnDeath();
		}
	}
	else if (!m_bIsGoal)
	{
		// 制限時間UIの更新
		char sSec[3];
		sprintf(&sSec[0], "%02d", nRemainingTime % 60);

		std::string sTimeText;
		sTimeText = std::to_string(nRemainingTime / 60) + "<color=180,180,180>:<color=reset>" + sSec;
		m_pTimeUI->GetComponent<CText>()->SetText(sTimeText);
	}
}

//=============================================================
// [CGameScene] 描画
//=============================================================
void CGameScene::Draw()
{

}

//=============================================================
// [CGameScene] カメラの初期化
//=============================================================
void CGameScene::InitCamera()
{
	// ゲームオブジェクトの生成
	m_pCamera = new GameObject();
	m_pCamera->SetName("Camera");

	// コンポーネントの追加
	m_pCamera->AddComponent<CCamera>();
	m_pCamera->AddComponent<CCameraMove>();
	m_pCamera->GetComponent<CCamera>()->GetSkybox()->LoadSkybox(m_sSkyboxPath);
	m_pCamera->GetComponent<CCamera>()->SetAmbient(m_fAmbient);

	// 色の設定
	CCamera* pCameraComp = m_pCamera->GetComponent<CCamera>();
	pCameraComp->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
}

//=============================================================
// [CGameScene] ライトの初期化
//=============================================================
void CGameScene::InitLight()
{
	// ライトを作成
	GameObject* pLight = new GameObject();

	CD3DLight* pLightComp = pLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.22f, -0.87f, 0.44f));

	pLightComp = pLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(-0.18f, 0.88f, -0.44f));

	pLightComp = pLight->AddComponent<CD3DLight>();
	pLightComp->SetType(D3DLIGHT_DIRECTIONAL);
	pLightComp->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pLightComp->SetDirection(D3DXVECTOR3(0.89f, 0.11f, 0.44f));
}

//=============================================================
// [CGameScene] カーソルを中央に移動する処理
//=============================================================
void CGameScene::CenterCosor()
{
	// 画面中心の位置を取得する
	POINT centerPoint;
	centerPoint.x = CRenderer::SCREEN_WIDTH / 2;
	centerPoint.y = CRenderer::SCREEN_HEIGHT / 2;
	ClientToScreen(CManager::GetHWND(), &centerPoint);

	// 現在のカーソル位置を取得する
	POINT currentPos;
	GetCursorPos(&currentPos);

	// 前回とカーソル位置が異なるとき
	if (currentPos.x != centerPoint.x ||
		currentPos.y != centerPoint.y)
	{
		m_pSkillGuide->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SKILL\\skill_key.png");
	}

	// コントローラースティックの状態を取得する
	short controlStickLx = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLX;
	short controlStickLy = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>()->GetState().Gamepad.sThumbLY;

	// コントローラーで動かしているとき
	if (controlStickLx != 0 || controlStickLy != 0)
	{
		m_pSkillGuide->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SKILL\\skill_control.png");
	}

	// 画面の中心にカーソルを移動する処理
	if (!m_pPause->GetComponent<CPause>()->GetIsPause())
	{
		SetCursorPos(centerPoint.x, centerPoint.y);
	}
}

//=============================================================
// [CGameScene] スタートアニメーションの更新
//=============================================================
void CGameScene::UpdateStartAnim()
{
	// フェードの処理
	if (m_pFade != nullptr)
	{
		if (m_pFade->GetComponent<CPolygon>()->GetColor().a > 0.001f)
		{
			m_pFade->GetComponent<CPolygon>()->SetColor(m_pFade->GetComponent<CPolygon>()->GetColor() - D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.008f));
		}
		else
		{
			m_pFade->Destroy();
			m_pFade = nullptr;
		}
	}

	// タイトルテキスト
	if (m_nTitleCounter <= 0)
	{
		// タイトル表示
		if (m_pTitleName != nullptr)
		{
			m_fTitleAlpha -= 0.02f;
			m_pTitleName->GetComponent<CText>()->SetAlpha(m_fTitleAlpha);

			if (m_fTitleAlpha <= 0.0f)
			{
				m_pTitleName->Destroy();
				m_pTitleName = nullptr;
			}
		}
	}
	else if (m_fTitleAlpha > 0.0f)
	{
		m_nTitleCounter--;
	}

	// 時間強調表示
	if (m_nTitleTimeCounter <= 0)
	{
		// 時間表示を右上に移動する
		D3DXVECTOR2 timePos = { m_pTimeUI->transform->GetPos().x, m_pTimeUI->transform->GetPos().y };
		timePos += (TIME_POS - timePos) * 0.03f;
		m_pTimeUI->transform->SetPos(timePos);
	}
	else
	{
		m_nTitleTimeCounter--;
	}
}

//=============================================================
// [StageLoader] カスタムオブジェクト
//=============================================================
void StageLoader::CustomObject(GameObject* gameObject)
{
	if (gameObject->GetTag() == "Player")
	{ // プレイヤー
		gameObject->AddComponent<CPlayer>();

		gameObject->GetComponent<CRigidBody>()->SetAlwayActive(true);
		gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -70.0f, 0.0f));
	}

	if (gameObject->GetName() == "DebugEnemy")
	{ // デバッグエネミー
		gameObject->AddComponent<CDebugEnemy>();
		gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		gameObject->GetComponent<CRigidBody>()->SetAlwayActive(true);
	}

	if (gameObject->GetName() == "Slime")
	{ // スライム
		gameObject->AddComponent<CSlimeEnemy>();
		gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		gameObject->GetComponent<CRigidBody>()->SetAlwayActive(true);
	}

	if (gameObject->GetTag() == "Portal")
	{ // ポータル
		CNextPortal* pPortal = gameObject->AddComponent<CNextPortal>();
		pPortal->SetDisplayOffset({ 0.0f, 0.0f, -5.0f }, { -D3DX_PI * 0.5f, 0.0f, 0.0f });
		pPortal->SetNextPos(CDataField::GetDataField(gameObject)->GetVector3("MovePos"));
		pPortal->SetDisplaySize(32, 18);
		pPortal->SetFrontStop(21.7f);
		pPortal->SetIsGoal(CDataField::GetDataField(gameObject)->GetBool("IsGoal"));

		// スカイボックスを設定する
		CGameScene* pGameScene = (CGameScene*)CSceneManager::GetInstance()->GetScene("game")->pScene;
		pPortal->GetNextCamera()->GetComponent<CCamera>()->GetSkybox()->LoadSkybox(pGameScene->GetSkybox());
		pPortal->GetNextCamera()->GetComponent<CCamera>()->SetAmbient(pGameScene->GetAmbient());
	}

	// レーザーブロック
	if (gameObject->GetName() == "LaserBlock" &&
		gameObject->GetTag() == "SP")
	{
		gameObject->AddComponent<CLaserBlock>();
		gameObject->GetComponent<CLaserBlock>()->SetLength(CDataField::GetDataField(gameObject)->GetFloat("Length"));
		gameObject->GetComponent<CLaserBlock>()->SetDamage(CDataField::GetDataField(gameObject)->GetInt("Damage"));
		gameObject->GetComponent<CLaserBlock>()->SetMove(CDataField::GetDataField(gameObject)->GetVector3("Move"));
		gameObject->GetComponent<CLaserBlock>()->SetMoveCounter(CDataField::GetDataField(gameObject)->GetInt("MoveCounter"));
	}

	if (gameObject->GetName() == "MoveTile")
	{ // 移動床
		gameObject->AddComponent<CMoveTile>();
		gameObject->GetComponent<CMoveTile>()->SetMove(CDataField::GetDataField(gameObject)->GetVector3("Move"));
		gameObject->GetComponent<CMoveTile>()->SetMoveCount(CDataField::GetDataField(gameObject)->GetInt("MoveCounter"));
	}

	if (gameObject->GetName() == "Shard" &&
		gameObject->GetTag() == "Item")
	{ // シャード
		gameObject->AddComponent<CShard>();
	}

	if (gameObject->GetName() == "Billboard")
	{ // ビルボード
		gameObject->AddComponent<CBillboard>();
		gameObject->GetComponent<CBillboard>()->Set(50, 30);
		gameObject->GetComponent<CBillboard>()->SetTexture(CDataField::GetDataField(gameObject)->GetString("TexturePath"));
	}

	if (gameObject->GetName() == "CannonBlock")
	{ // キャノン砲
		gameObject->AddComponent<CCannon>();
	}

	if (gameObject->GetName() == "DamageBallEmitter")
	{ // ダメージボールのエミッター
		gameObject->AddComponent<CDamageBallEmitter>();
	}

	// 特殊: カメラの視点位置を変更する
	if (gameObject->GetName() == "ChangeViewpoint" &&
		gameObject->GetTag() == "Special")
	{
		CChangePlayerFocus* pChange = gameObject->AddComponent<CChangePlayerFocus>();
		pChange->SetOffset(CDataField::GetDataField(gameObject)->GetVector3("ChangedPos"));
	}

	GameObject* jumppadCollision = gameObject->FindNameChildren("JumpCollision");
	if (jumppadCollision != nullptr)
	{ // ジャンプパッド
		CJumppad* pJumppad = jumppadCollision->AddComponent<CJumppad>();
		pJumppad->SetPower(CDataField::GetDataField(gameObject)->GetFloat("Power"));
	}
}

//=============================================================
// [CGameScene] リザルトの初期化
//=============================================================
void CGameScene::InitResult()
{
	// レンダーを作成する
	m_pResultRender = new GameObject;
	CRenderObjectsTexture* pRenderObject = m_pResultRender->AddComponent<CRenderObjectsTexture>();
	pRenderObject->Create(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// リザルト
	GameObject* pResultText = new GameObject;
	pResultText->transform->SetPos(700.0f, 80.0f);
	pResultText->SetVisible(false);
	pResultText->AddComponent<CText>();
	pResultText->GetComponent<CText>()->SetFont("ベストテン-CRT");
	pResultText->GetComponent<CText>()->SetFontColor(D3DCOLOR_RGBA(0, 255, 0, 255));
	pResultText->GetComponent<CText>()->SetEdgeColor(D3DCOLOR_RGBA(0, 255, 0, 255));
	pResultText->GetComponent<CText>()->SetText("<size=180>リザルト");
	pResultText->SetTag("result");
	pRenderObject->AddObject(pResultText);

	// スコア
	m_pScoreText = new GameObject;
	m_pScoreText->SetName("ScoreText");
	m_pScoreText->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 350.0f);
	m_pScoreText->SetVisible(false);
	m_pScoreText->AddComponent<CText>();
	m_pScoreText->GetComponent<CText>()->SetFont("ベストテン-CRT");
	m_pScoreText->GetComponent<CText>()->SetText("<color=reset>???");
	m_pScoreText->GetComponent<CText>()->SetFontSize(200);
	m_pScoreText->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	m_pScoreText->SetTag("result");
	pRenderObject->AddObject(m_pScoreText);

	// バー
	GameObject* pBarObj = new GameObject;
	pBarObj->SetVisible(false);
	pBarObj->transform->SetPos(RESULT_BAR_SPACE / 2, 550);
	pBarObj->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH - RESULT_BAR_SPACE), 2);
	pBarObj->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(100, 100, 100, 100));
	pBarObj->SetTag("result");
	pRenderObject->AddObject(pBarObj);

	// クリアタイム
	m_pClearTimeText = new GameObject;
	m_pClearTimeText->SetName("ClearTime");
	m_pClearTimeText->transform->SetPos(440.0f, 600.0f);
	m_pClearTimeText->SetVisible(false);
	m_pClearTimeText->AddComponent<CText>();
	m_pClearTimeText->GetComponent<CText>()->SetFont("ベストテン-CRT");
	m_pClearTimeText->GetComponent<CText>()->SetText("<color=130,221,255>クリアタイム<color=110,110,110>: <color=reset>???");
	m_pClearTimeText->SetTag("result");
	pRenderObject->AddObject(m_pClearTimeText);

	// ベストタイム
	m_pBestTimeText = new GameObject;
	m_pBestTimeText->SetName("BestTime");
	m_pBestTimeText->transform->SetPos(640.0f, 740.0f);
	m_pBestTimeText->SetVisible(false);
	m_pBestTimeText->AddComponent<CText>();
	m_pBestTimeText->GetComponent<CText>()->SetFont("ベストテン-CRT");
	m_pBestTimeText->GetComponent<CText>()->SetText("<size=60><color=82,110,255>ベストタイム<color=110,110,110>: <color=180,180,180>???");
	m_pBestTimeText->SetTag("result");
	pRenderObject->AddObject(m_pBestTimeText);
}

//=============================================================
// [CGameScene] リザルトの更新
//=============================================================
void CGameScene::UpdateResult()
{
	if (!m_bIsGoal)
	{ // ゴールしていないとき
		m_nClearTime = m_nCurrentTime;
	}
}

//=============================================================
// [CGameScene] カメラが初期位置移動のときのゴール処理
//=============================================================
void CGameScene::onInitGoal()
{
	if (!m_bIsGoal)
	{
		// 時間スコアを加算する
		m_nScore += static_cast<int>((m_nLimitTime - m_nCurrentTime) * 0.01f);
		m_bIsGoal = true;

		m_fAlphaUI = 1.0f;

		// 音を鳴らす
		GameObject* pResultBGM = new GameObject;
		pResultBGM->SetTag("result");
		pResultBGM->AddComponent<CSound>()->LoadWav("data\\SOUND\\BGM\\result.wav");
		pResultBGM->GetComponent<CSound>()->SetLoop(true);
		pResultBGM->GetComponent<CSound>()->Play();
		pResultBGM->GetComponent<CSound>()->SetVolume(0.0f);
		pResultBGM->GetComponent<CSound>()->FadeIn(0.002f, 0.5f);
	}

	// 最終結果（ミリ秒）
	m_nClearTime = m_nCurrentTime;
	int clearTime = m_nClearTime / 1000;
	char sSec[10];
	sprintf(&sSec[0], "%02d.%03d", clearTime % 60, m_nClearTime % 1000);
	std::string remainingText = std::to_string(clearTime / 60) + ":" + sSec;
	m_pClearTimeText->GetComponent<CText>()->SetText("<color=130,221,255>クリアタイム<color=110,110,110>: <color=reset><size=120>" + remainingText);
}

//=============================================================
// [CGameScene] ゴールの更新処理
//=============================================================
void CGameScene::UpdateGoal()
{
	// スコアのアニメーション
	m_fScoreFade += ((m_nScore + 1) - m_fScoreFade) * 0.03f;
	m_pScoreText->GetComponent<CText>()->SetText(std::to_string((int)m_fScoreFade));

	// バーUIの透明化
	m_fAlphaUI += (0.0f - m_fAlphaUI) * 0.03f;
	GameObject* pPlayer = GameObject::Find("Player");
	if (pPlayer != nullptr)
	{
		pPlayer->GetComponent<CPlayer>()->GetHPBar()->GetComponent<CProgressBar>()->SetAlpha(m_fAlphaUI);
		pPlayer->GetComponent<CPlayer>()->GetStaminaBar()->GetComponent<CProgressBar>()->SetAlpha(m_fAlphaUI);
		m_pSkillGuide->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlphaUI));
	}

	// スキルゲージの透明化
	GameObject* pSkillUI = GameObject::Find("SkillPointUI", "UI");
	if (pSkillUI != nullptr)
	{
		pSkillUI->GetComponent<CSkillPointUI>()->SetAlpha(m_fAlphaUI);
	}

	// 時間の透明化
	m_pTimeUI->GetComponent<CText>()->SetAlpha(m_fAlphaUI);

	// 残機の透明化
	GameObject::Find("RevivalUI")->GetComponent<CText>()->SetAlpha(m_fAlphaUI);
}

//=============================================================
// [CGameScene] ゴールの最終処理
//=============================================================
void CGameScene::FinishGoal()
{
	// スコア
	m_pScoreText->GetComponent<CText>()->SetText(std::to_string(m_nScore));

	// ベストタイムを文字型に変換する
	int bestTime = GetBestTime();
	int secBestTime = bestTime / 1000;
	char sSec[10];
	sprintf(&sSec[0], "%02d.%03d", secBestTime % 60, bestTime % 1000);
	std::string bestText = std::to_string(secBestTime / 60) + ":" + sSec;

	// ベストタイム表示
	m_pBestTimeText->GetComponent<CText>()->SetText("<size=60><color=82,110,255>ベストタイム<color=110,110,110>:  <color=180,180,180>"+ bestText);

	// BGMを停止する
	if (m_pBGM != nullptr)
	{
		m_pBGM->GetComponent<CSound>()->FadeOut(0.01f);
	}

	// リザルトに移行する（オブジェクトを消さない）
	CSceneManager::GetInstance()->SetScene("result", false);
}

//=============================================================
// [CGameScene] リザルトのテクスチャを取得する
//=============================================================
LPDIRECT3DTEXTURE9 CGameScene::GetResultTexture()
{
	return m_pResultRender->GetComponent<CRenderObjectsTexture>()->GetTexture();
}

//=============================================================
// [CGameScene] プレイデータをセーブする
//=============================================================
void CGameScene::SavePlayData()
{
	// フォルダのパス
	std::string sSavePath = m_sWorldPath;
	RemovePathSplit(sSavePath, 1);
	sSavePath += "\\playdata.bin";

	// 変数
	PlayData* pPlayDatas = nullptr;
	int nNum = 0;

	// ファイルを読み込む
	FILE* pFile = nullptr;
	pFile = fopen(sSavePath.c_str(), "rb");
	if (pFile != nullptr)
	{
		// 人数を取得する
		fread(&nNum, sizeof(int), 1, pFile);

		if (nNum > 0)
		{ // データが有るとき
			// データを人数分作成する
			pPlayDatas = new PlayData[nNum+1];

			// 読み込む
			fread(&pPlayDatas[0], sizeof(PlayData), nNum, pFile);
		}
		else
		{
			pPlayDatas = new PlayData[1];
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		pPlayDatas = new PlayData[1];
	}


	// 今回のデータを格納する
	pPlayDatas[nNum].nClearTime = m_nClearTime;
	pPlayDatas[nNum].nBestScore = m_nScore;

	// ファイルに書き込み
	pFile = fopen(sSavePath.c_str(), "wb");
	if (pFile != nullptr)
	{
		// プレイ人数を書き込む
		nNum++;
		fwrite(&nNum, sizeof(int), 1, pFile);

		// データを書き込む
		fwrite(&pPlayDatas[0], sizeof(PlayData), nNum, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}

	// 格納変数を破棄する
	if (pPlayDatas != nullptr)
	{
		delete[] pPlayDatas;
	}
}

//=============================================================
// [CGameScene] ベストタイムを取得する
//=============================================================
int CGameScene::GetBestTime()
{
	// フォルダのパス
	std::string sSavePath = m_sWorldPath;
	RemovePathSplit(sSavePath, 1);
	sSavePath += "\\playdata.bin";

	// 変数
	PlayData* pPlayDatas = nullptr;
	int nNum = 0;

	// ファイルを読み込む
	FILE* pFile = nullptr;
	pFile = fopen(sSavePath.c_str(), "rb");
	if (pFile != nullptr)
	{
		// 人数を取得する
		fread(&nNum, sizeof(int), 1, pFile);

		if (nNum > 0)
		{ // データが有るとき
			// データを人数分作成する
			pPlayDatas = new PlayData[nNum + 1];

			// 読み込む
			fread(&pPlayDatas[0], sizeof(PlayData), nNum, pFile);
		}

		// ファイルを閉じる
		fclose(pFile);
	}

	// ベストタイムを取得する
	int nBestTime = pPlayDatas[0].nClearTime;
	for (int i = 0; i < nNum; i++)
	{
		if (nBestTime > pPlayDatas[i].nClearTime)
		{
			nBestTime = pPlayDatas[i].nClearTime;
		}
	}

	// 格納変数を破棄する
	if (pPlayDatas != nullptr)
	{
		delete[] pPlayDatas;
	}

	return nBestTime;
}