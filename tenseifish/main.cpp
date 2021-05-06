#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
#include "Boss.h"
#include "Sound.h"
#include "CollisionDetiction.h"
int	g_OldKey;				// 前回の入力キー
int	g_NowKey;				// 今回の入力キー
int	g_KeyFlg;				// 入力キー情報
char key[256];

int GameState = 0;          //初期化　

//画面領域の大きさ
const int SCREEN_WIDTH = 1410;
const int SCREEN_HEIGHT = 800;

int ScroolSpeed;


void GameTitle();
void GameInit();		//ゲーム初期化処理
void GameMain();		//ゲームメイン処理
void GameHelp();		//ゲームヘルプ処理
void BackScrool();         //背景画像スクロール処理
void GameClear();		//ゲームクリア処理
void GameOver();
void Goal();
void Pouse();
void GameGiyo();
void GameGiyo2();
void GameGiyo3();
void GameRule();
void GameRule2();
void dieam();

int LoadImages();          //画像読み込み
int LoadSound();		// 音楽読込み

int Time = 2400;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	SetMainWindowText("a");// タイトル変更

	ChangeWindowMode(TRUE);		// ウィンドウモードで起動

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//SetGraphMode(800, 600, 16);

	if (DxLib_Init() == -1) return -1;	// DXライブラリの初期化処理

	SetDrawScreen(DX_SCREEN_BACK);	// 描画先画面を裏にする

	if (LoadImages() == -1)return -1; //画像呼び出し

	if (LoadSound() == -1)return -1;	// 音楽の読み込み

	//ゲームループ
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 && GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		if (key1 < 1) {
			Time--;
		}

		//入力キー取得
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// 画面の初期化

		switch (GameState) {
		case 0:
			GameTitle();
			break;
		case 1:
			GameInit();		//ゲーム初期処理
			break;
		case 2:
			GameMain();		//ゲームメイン処理
			break;
		case 3:
			GameClear();
			break;
		case 4:
			BossInit();
			break;
		case 5:
			BossStage();
			break;
		case 6:
			GameHelp();
			break;
		case 7:
			GameOver();
			break;
		case 8:
			GameGiyo();
			break;
		case 9:
			GameGiyo2();
			break;
		case 10:
			GameGiyo3();
			break;
		case 11:
			GameRule();
			break;
		case 12:
			GameRule2();
			break;
		case 13:
			Musicmixer();
		}
		ScreenFlip();			// 裏画面の内容を表画面に反映
	}

	DxLib_End();	// DXライブラリ使用の終了処理
	return 0;	// ソフトの終了
}

void GameTitle() {
	static bool push = 0;	// 押されたかどうか確認する関数

	SHIPFlg = FALSE;
	if (CheckSoundMem(TitleSound) == 0) {
		if (BGMFlg == FALSE) {
			PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
	StopSoundMem(HelpSound);
	
	DrawGraph(0, 0, Gametitle, TRUE);
	// メニューカーソルの描画
	DrawRotaGraph(590, 385 + MenuNo * 108, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 2)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 2;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 6;
			MenuNo = 0;
			if (BGMFlg == FALSE) {
				PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
		else {
			push = 0;
			DxLib_End();
		}
	}
}

void GameHelp()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamehelp, TRUE);

	StopSoundMem(TitleSound);

	// メニューカーソルの描画
	DrawRotaGraph(530, 275 + MenuNo * 140, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 3)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 3;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 8;
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 11;
			MenuNo = 0;
		}
		else if (MenuNo == 2) {
			push = 0;
			GameState = 13;
			MenuNo = 0;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}

void GameGiyo()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(455 + MenuNo * 325, 760, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 9;
		}
		else {
			push = 0;
			GameState = 6;
			MenuNo = 0;
		}
	}
}

void GameGiyo2()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo2, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(460 + MenuNo * 360, 755, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 10;
		}
		else {
			push = 0;
			GameState = 8;
			MenuNo = 0;
		}
	}
}

void GameGiyo3()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo3, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(400 + MenuNo * 320, 775, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 6;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(HelpSound);
		}
	}
}

void GameRule()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamerule, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(500 + MenuNo * 260, 770, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 12;
		}
		else {
			push = 0;
			GameState = 6;
			MenuNo = 0;
		}
	}
}

void GameRule2()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamerule2, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(345 + MenuNo * 460, 755, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 6;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
		}
	}
}

/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit() {
	//スクロール速度を初期化
	ScroolSpeed = 0;
	//食べた量個別
	em = 0;
	am = 0;
	im = 0;
	EatAmount = 0;
	Leve = 1;
	Scke = 1;
	LeveUp = 10;
	RandEat = 3;

	//当たり判定用変数の初期化
	CollXadd = 0;
	CollYadd = 0;
	PointNom = 0;

	//プレイヤーの初期化
	player.flg = TRUE;
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGHT;
	player.speed = PLAYER_SPEED;
	LifeMax = 3;
	player.life = LifeMax;
	player.muteki = 0;
	Umispeed = 0;
	Time = 100;
	//Time = 60;
	Iwaspeed = 0;
	motion_index7 = 0;
	LeveUpflg1 = FALSE;
	LeveUpflg2 = FALSE;
	LCount = 0;
	countS = 0;
	//餌の初期化
	for (int i = 0; i < 10; i++) {
		eat[i].flg = FALSE;
	}

	
	//ゲームメインへ
	GameState = 2;

	StopSoundMem(TitleSound);
	if (BGMFlg == FALSE) {
		PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
	}
}

/***********************************************
/*ゲームメインの処理*/
/***********************************************/
void GameMain() {
	BackScrool();
	PlayerMove();
	EatMove();
	LifeImage();
	MeterImage();
	BossST(&player);
	Pouse();
}
/*************************************
 *背景画像スクロール処理
 *引数：なし
 *戻り値：なし
 *************************************/
void BackScrool()
{

	Range += player.speed;

	if (GameState == 2) {
		if (Time > 0) {
			ScroolSpeed -= player.speed;
		}
	}

	//ステージ画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//透過
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);*/
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//設定を元に戻す。
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//岩画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);

	DrawGraph(Iwaspeed % IwaHaba, 50, Iwa[0], TRUE);
	DrawGraph(IwaHaba + (Iwaspeed % IwaHaba), 50, Iwa[0], TRUE);


	//エリアを戻す
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);


	//レベル表示
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
}

void PlayerMove() {
	
	if (--act_wait <= 0)
	{
		if (key1 < 1) {
			act_index++;
			act_wait = ACT_SPEED;
			act_index %= MAX_MOTION_INDEX;
		}
	}
	if (key1 < 1) {
		//上下左右移動
		if (player.flg == TRUE) {
			if (g_NowKey & PAD_INPUT_UP)player.y -= player.speed;
			if (g_NowKey & PAD_INPUT_DOWN)player.y += player.speed;
			if (g_NowKey & PAD_INPUT_LEFT)player.x -= player.speed;
			if (g_NowKey & PAD_INPUT_RIGHT)player.x += player.speed;
		}
	}

	//画面からはみ出さないようにする
	if (player.x < 0)player.x = 0;
	if (player.x > SCREEN_WIDTH - player.w)player.x = SCREEN_WIDTH - player.w;
	if (Leve == 1) {
		if (player.y < player.h / Scke + 50)player.y = player.h / Scke + 50;
	}
	if (Leve == 2) {
		if (player.y < player.h / Scke)player.y = player.h / Scke;
	}
	if (Leve == 3) {
		if (player.y < player.h / Scke-100)player.y = player.h / Scke-100;
	}
	if (player.y > SCREEN_HEIGHT - player.h)player.y = SCREEN_HEIGHT - player.h;

	if (Damege != 1 && LeveUpflg1 != TRUE ) {
		if (Leve == 1 && key1 != 1  && key2 != 1) {
			Umispeed -= 2;
			Iwaspeed -= 5;
		}
		if (Leve == 2 && key1 != 1 && key2 != 1) {
			Umispeed -= 4;
			Iwaspeed -= 7;
			player.speed = 8;
		}
		if (Leve == 3 && key1 != 1 && key2 != 1) {
			Umispeed -= 6;
			Iwaspeed -= 9;
			player.speed = 9;
		}
	}

	if (Leve != 2) {
		CollXadd = 10;
		if (Leve < 2) CollYadd = 7;
		if (Leve > 2) CollYadd = 10;
	}
	else if (Leve == 2) {
		CollXadd = 40;
		CollYadd = 10;
	}

	int ColorCheck(int x, int y);

	if (Leve == 1) {
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 - CollYadd);	//右上
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 + CollYadd);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 + CollYadd);	//右下
	}
	else if (Leve == 2) {
		//if (Leve == 2) DrawBox(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4, player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2, GetColor(0, 255, 0), TRUE);

		ColorCheck(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2);		//右上
		ColorCheck(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2);		//右下
	}
	else if (Leve == 3) {
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2);		//右上
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2);		//右下
	}

	if (player.muteki != 0 && Damege != 0 && player.life > 0) {
		if (key1 < 1) {
			player.flg = FALSE;
			if (PointNom == 1) player.x += 3;
			if (PointNom == 2) player.x -= 1;
			Feedflg = TRUE;
			player.y -= 5;
		}
	}
	else {
		PointNom = 0;
		Damege = 0;
		player.flg = TRUE;
	}
	if (Feedflg == TRUE) {
		if (key1 < 1) {
			DEat();
		}
	}

	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	int motion_index = anime[act_index];
	if (player.muteki == 0&&key2 < 1) {
		if (key2 < 1) {
			DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
		}
	}

	if (player.muteki == 1) {
		if (key1 < 1 && key2 < 1) {

			// ダメージが入ると５回のうち２回表示する。
			static int count = 0;
			count = (count + 1) % 20;
			if (count % 2 == 0) {
				//表示
				DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
			}
			if (count == 19 || player.life == 0) {
				player.muteki = 0;

			}
		}
		
	}

	if (LeveUpflg1 == TRUE) {
		if (key1 < 1 || key2 < 1) {
			player.flg = FALSE;
			// ダメージが入ると５回のうち２回表示する。
			LCount = (LCount + 1) % 21;
			if (LCount % 5 == 0) {
				if (LeveUpflg2 != TRUE) {
					if (Leve == 1) {
						player.w *= Scke;
						player.h *= Scke;
						player.x -= PLAYER_WIDTH / 2;
						player.y -= PLAYER_HEIGHT / 2;
						Leve = 2;
					}
					else if (Leve == 2) {
						player.w = PLAYER_WIDTH;
						player.h = PLAYER_HEIGHT;
						player.x += player.w / 2;
						player.y += player.h / 2;
						Leve = 1;
					}
				}
				if (LeveUpflg2 == TRUE) {
					if (Leve == 2) {
						Leve = 3;
					}
					else if (Leve == 3) {
						Leve = 2;
					}
				}
			}

			if (LCount >= 20 || player.life == 0) {
				LeveUpflg1 = FALSE;
				LeveUpflg2 = FALSE;
				player.flg = TRUE;
				LCount = 0;

			}
		}
	}
	
	//当たり判定の範囲確認用BOX
	//if (Leve == 1) DrawBox(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd, player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 + CollYadd, GetColor(255, 0, 0), TRUE);		//右下
	//if (Leve == 2) DrawBox(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4, player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2, GetColor(0, 255, 0), TRUE);		//右下
	//if (Leve == 3) DrawBox(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd, player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2, GetColor(0, 0, 255), TRUE);		//右下
}

int LoadImages() {
	//タイトル画像
	if ((Gametitle = LoadGraph("Image/umi.png")) == -1)return -1;
	//ゲームヘルプ画像
	if ((Gamehelp = LoadGraph("Image/GameHelp2.png")) == -1)return -1;
	//ゲーム概要画像
	if ((Gamegiyo = LoadGraph("Image/ゲーム概要.png")) == -1)return -1;
	//ゲーム概要画像2
	if ((Gamegiyo2 = LoadGraph("Image/ゲーム概要2.png")) == -1)return -1;
	//ゲーム概要画像3
	if ((Gamegiyo3 = LoadGraph("Image/ゲーム概要3.png")) == -1)return -1;
	// ゲームルール画像
	if ((Gamerule = LoadGraph("Image/ゲームルール1.png")) == -1)return -1;
	// ゲームルール2画像
	if ((Gamerule2 = LoadGraph("Image/ゲームルール2.png")) == -1)return -1;
	// ゲーム音量調整画像
	if ((GameMS = LoadGraph("Image/GameMusicSelect.png")) == -1)return -1;
	//ポーズ画像
	if ((pauseImage = LoadGraph("Image/pause.png")) == -1)return-1;
	// カーソル画像
	if ((Corsol = LoadGraph("Image/coursol.png")) == -1)return -1;
	//ポーズ画面のカーソル画像
	if ((Corsol2 = LoadGraph("Image/coursol2.png")) == -1)return -1;
	//プレイヤー画像
	//魚レベル1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//魚レベル2
	if ((LoadDivGraph("Image/sakana2.png", 3, 3, 1, 150, 150, sakana[1])) == -1)return-1;
	//魚レベル3
	if ((LoadDivGraph("Image/sakana3.png", 3, 3, 1, 225, 225, sakana[2])) == -1)return-1;
	//ステージ背景
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//手前の背景
	if ((Iwa[0] = LoadGraph("Image/temae4.png")) == -1) return -1;
	/*if ((Iwa[1] = LoadGraph("Image/temae2.png")) == -1) return -1;
	if ((Iwa[2] = LoadGraph("Image/temae3.png")) == -1) return -1;*/
	//餌(食べれる生き物)画像
	//エビ
	if ((LoadDivGraph("Image/ebi.png", 3, 3, 1, 50, 50, feedImage[0])) == -1)return-1;
	//アジ
	if ((LoadDivGraph("Image/azi.png", 3, 3, 1, 60, 60, feedImage[1])) == -1)return-1;
	//イカ
	if ((LoadDivGraph("Image/ika.png", 3, 3, 1, 50, 50, feedImage[2])) == -1)return-1;

	//敵
	//クラゲ
	if ((LoadDivGraph("Image/kurage.png", 3, 3, 1, 80, 80, EnemyImage[0])) == -1)return-1;
	//ミノカサゴ
	if ((LoadDivGraph("Image/minokasago.png", 3, 3, 1, 120,120, EnemyImage[1])) == -1)return-1;

	//船
	if ((LoadDivGraph("Image/船.png", 9, 3, 1, 400, 700, ship1)) == -1)return -1;

	//船2
	if ((LoadDivGraph("Image/船2.png", 9, 3, 1, 400, 700, ship2)) == -1)return -1;

	//網
	if ((LoadDivGraph("Image/網.png", 7, 7, 1, 400, 400, net1)) == -1)return -1;

	//網2
	if ((LoadDivGraph("Image/網2.png", 7, 7, 1, 400, 400, net2)) == -1)return -1;

	//Boss
	if ((LoadDivGraph("Image/maguro.png", 12, 12, 1, 350, 350, Boss1)) == -1)return -1;
	if ((LoadDivGraph("Image/maguro2.png", 6, 6, 1, 350, 350, Boss2)) == -1)return -1;
	if ((LoadDivGraph("Image/rasubosu.png", 6, 6, 1, 350, 350, Boss3)) == -1)return -1;
	//Boss必殺技
	if ((LoadDivGraph("Image/ソニックブーム.png", 2, 2, 1, 450, 450, Sonic)) == -1)return -1;
	//Boss警告
	if ((Keikoku = LoadGraph("Image/keikoku.png")) == -1)return -1;
	//Boss捕獲
	if ((LoadDivGraph("Image/Hokaku.png", 4, 4, 1, 350, 500, Hokaku)) == -1)return -1;
	//Boss捕獲2
	if ((LoadDivGraph("Image/マグロ捕獲2.png", 4, 4, 1, 350, 500, Hokaku2)) == -1)return -1;
	//ゲームクリア画像
	if ((Gameclear[0] = LoadGraph("Image/GameClear.png")) == -1)return -1;
	if ((Gameclear[1] = LoadGraph("Image/GameClear(NoEat).png")) == -1)return -1;
	if ((Gameclear[2] = LoadGraph("Image/GameClear(MaxEat).png")) == -1)return -1;
	if ((Gameclear[3] = LoadGraph("Image/GameClear(ikaMax).png")) == -1)return -1;
	if ((Gameclear[4] = LoadGraph("Image/GameClear(aziMax).png")) == -1)return -1;
	if ((Gameclear[5] = LoadGraph("Image/GameClear(ebiMax).png")) == -1)return -1;
	if ((Gameclear[6] = LoadGraph("Image/GameClear(レベル1).png")) == -1)return -1;
	if ((Gameclear[7] = LoadGraph("Image/GameClear(レベル2).png")) == -1)return -1;
	if ((Gameclear[8] = LoadGraph("Image/GameClear(レベル3).png")) == -1)return -1;
	//ゲームオーバー時のアニメーション
	if ((LoadDivGraph("Image/FishDie.png", 7, 7, 1, 30, 30, fishdie[0])) == -1)return -1;
	if ((LoadDivGraph("Image/FishDie Level2.png", 7, 7, 1, 150, 150, fishdie[1])) == -1)return -1;
	if ((LoadDivGraph("Image/FishDie Level3.png", 7, 7, 1, 225, 225, fishdie[2])) == -1)return -1;
	//ゲームオーバー画像
	if ((Gameover = LoadGraph("Image/GameOver.png")) == -1)return -1;
	if ((Gameover2 = LoadGraph("Image/GameOver2.png")) == -1)return -1;
	if ((Gameover3 = LoadGraph("Image/GameOver3.png")) == -1)return -1;

	//ボスへの道
	if ((BOSSROAD = LoadGraph("Image/ボスへの道.png")) == -1)return -1;
	//水しぶき
	if ((LoadDivGraph("Image/hasira.png", 8, 8, 1, 167, 500, sibuki)) == -1)return -1;

	//UI画像
	//ライフ
	if ((Life = LoadGraph("Image/Life.png")) == -1)return 0;
	//メーター
	for (int i = 0; i < 3; i++) {
		if ((Meter[i][0] = LoadGraph("Image/Meter.png")) == -1)return 0;
		//エビメーター
		if (i == 0) {
			if ((Meter[i][1] = LoadGraph("Image/MeterE1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterE2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterE3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterE4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterE5.png")) == -1)return 0;
		}
		//アジメーター
		if (i == 1) {
			if ((Meter[i][1] = LoadGraph("Image/MeterA1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterA2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterA3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterA4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterA5.png")) == -1)return 0;
		}
		//イカメーター
		if (i == 2) {
			if ((Meter[i][1] = LoadGraph("Image/MeterI1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterI2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterI3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterI4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterI5.png")) == -1)return 0;
		}
	}



	return 0;
}

/******************************************************************
* BGMの読み込み
*******************************************************************/
int LoadSound() {
	// BGM
	if ((MainSound = LoadSoundMem("Sound/神秘の泉_-_8bit.mp3")) == -1)return -1;
	if ((TitleSound = LoadSoundMem("Sound/おさかなシャトル_-_8bit.mp3")) == -1)return -1;
	if ((BossSound = LoadSoundMem("Sound/よぞらのなみだ.mp3")) == -1)return -1;
	if ((HelpSound = LoadSoundMem("Sound/かえるのピアノ.mp3")) == -1)return -1;
	if ((GameOverSound = LoadSoundMem("Sound/不穏_-_8bit.mp3")) == -1)return -1;

	// SE
	if ((ClearSE = LoadSoundMem("Sound/bgm_fanfare_1.mp3")) == -1)return -1;
	if ((CarsolSE = LoadSoundMem("Sound/select09.mp3")) == -1)return -1;
	if ((DesitionSE = LoadSoundMem("Sound/button67.mp3")) == -1)return -1;
	if ((EatSE = LoadSoundMem("Sound/nc78521.mp3")) == -1)return -1;
	if ((DamegeSE = LoadSoundMem("Sound/se_dosun_2.mp3")) == -1)return -1;
	if ((WallSE = LoadSoundMem("Sound/se_wall_1.mp3")) == -1)return -1;
	if ((EvoSE = LoadSoundMem("Sound/powerup10.mp3")) == -1)return -1;
	if ((DieSE = LoadSoundMem("Sound/NES-General01-10(Pitch).mp3")) == -1)return -1;
	if ((KeikokuSE = LoadSoundMem("Sound/meka_ge_keihou03.mp3")) == -1)return -1;
	if ((SibukiSE = LoadSoundMem("Sound/水・ざぶーん05.mp3")) == -1)return -1;

	//音量調整
		// BGM
		ChangeVolumeSoundMem(125, MainSound);
		ChangeVolumeSoundMem(125, TitleSound);
		ChangeVolumeSoundMem(125, GameOverSound);
		ChangeVolumeSoundMem(150, HelpSound);
		ChangeVolumeSoundMem(200, BossSound);
		// SE
		ChangeVolumeSoundMem(125, KeikokuSE);
		ChangeVolumeSoundMem(200, EatSE);
		ChangeVolumeSoundMem(200, CarsolSE);
		ChangeVolumeSoundMem(200, DesitionSE);
		ChangeVolumeSoundMem(200, SibukiSE);
		ChangeVolumeSoundMem(200, ClearSE);
		ChangeVolumeSoundMem(225, EvoSE);
	
	return 0;
}

/*************************************************
* 音ON:OFF
**************************************************/
void Musicmixer()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, GameMS, TRUE);
	if (BGMFlg == FALSE) {
		SetFontSize(60);
		DrawString(710, 270, "ON", 0xff0000);
		DrawString(800, 270, "/", 0x000000);
		DrawString(850, 270, "OFF", 0x000000);
	}
	else if (BGMFlg == TRUE) {
		SetFontSize(60);
		DrawString(710, 270, "ON", 0x000000);
		DrawString(800, 270, "/", 0x000000);
		DrawString(850, 270, "OFF", 0xff0000);
	}

	if (SEFlg == FALSE) {
		SetFontSize(60);
		DrawString(710, 420, "ON", 0xff0000);
		DrawString(800, 420, "/", 0x000000);
		DrawString(850, 420, "OFF", 0x000000);
	}
	else if (SEFlg == TRUE) {
		SetFontSize(60);
		DrawString(710, 420, "ON", 0x000000);
		DrawString(800, 420, "/", 0x000000);
		DrawString(850, 420, "OFF", 0xff0000);
	}

	DrawRotaGraph(210, 300 + MenuNo * 147, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 2)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 2;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}

		if ((MenuNo == 0) && (BGMFlg == FALSE)){
			BGMFlg = TRUE;
			StopSoundMem(HelpSound);
			push = 0;
		}
		else if ((MenuNo == 0) && (BGMFlg == TRUE)) {
			BGMFlg = FALSE;
			PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
			push = 0;
		}
		else if ((MenuNo == 1) && (SEFlg == FALSE)){
			SEFlg = TRUE;
			StopSoundMem(DesitionSE);
			push = 0;
		}
		else if ((MenuNo == 1) && (SEFlg == TRUE)) {
			SEFlg = FALSE;
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 0;
		}
		else {
			GameState = 6;
			MenuNo = 0;
		}
	}
}


void EatMove() {
	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == TRUE) {
			//餌の表示
			if (Time > 0) {
				DrawExtendGraph(eat[i].e_x, eat[i].e_y, eat[i].e_x + eat[i].e_w, eat[i].e_y + eat[i].e_h, eat[i].image, TRUE);
			}

			if (Damege != 1 && LeveUpflg1 != TRUE && LeveUpflg2 != TRUE) {
				//真っすぐ左に移動
				if (Leve == 1 && key1 != 1 && Leve == 1 && key2 != 1) {
					eat[i].e_x -= 6;
				}
				if (Leve == 2 && key1 != 1 && key2 != 1) {
					eat[i].e_x -= 8;
				}
				if (Leve == 3 && key1 != 1 && key2 != 1) {
					eat[i].e_x -= 10;
				}
			}

			/*if (Leve == 1) {
				if (eat[i].e_x == SCREEN_WIDTH - 5)EatCheck(eat[i].e_x - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (eat[i].e_w / 2) + 3) EatCheck(eat[i].e_x + (eat[i].e_w / 2) - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - eat[i].e_w) EatCheck(eat[i].e_x + eat[i].e_w - 1, eat[i].e_y + eat[i].e_h);
			}
			if (Leve == 2) {
				if (eat[i].e_x == SCREEN_WIDTH - 7) EatCheck(eat[i].e_x - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (eat[i].e_w / 2) - 5) EatCheck(eat[i].e_x + (eat[i].e_w / 2) - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - eat[i].e_w - 2) EatCheck(eat[i].e_x + eat[i].e_w - 1, eat[i].e_y + eat[i].e_h);
			}
			if (Leve == 3) {
				if (eat[i].e_x == SCREEN_WIDTH - 9) EatCheck(eat[i].e_x - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (eat[i].e_w / 2) - 8) EatCheck(eat[i].e_x + (eat[i].e_w / 2) - 1, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - eat[i].e_w - 6) EatCheck(eat[i].e_x + eat[i].e_w - 1, eat[i].e_y + eat[i].e_h);

			}*/



			if (EatCont >= 1)
			{
				EatCont = 0;
				eat[i] = eat0;
			}

			if (eat[i].flg == FALSE)continue;

			//画面外に行ったら削除
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;

			//魚に当たったら場合の処理
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
			}
			//アニメーションを動かす
			if (key1 < 1) {
				if (--MoveEat <= 0)
				{
					CountEat++;
					MoveEat = MoveSpeed;
					CountEat %= MAX_MOTION_INDEX;
				}

				int motion_index = anime[CountEat];
				if (eat[i].type <= 2) {
					eat[i].image = feedImage[eat[i].type][motion_index];
				}
				else {
					eat[i].image = EnemyImage[eat[i].type - 3][motion_index];
				}

			}
		}
	}

	//餌の設定
	if (Range / 5 % 50 == 0 && key1 < 1	&&Damege != 1 && LeveUpflg1 != TRUE && LeveUpflg2 != TRUE) {
		EatImage();
	}
}

int EatImage() {

	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {
			eat[i] = eat0;
			eat[i].type = GetRand(7)%5;
			if (eat[i].type <= 2) {
				eat[i].image = feedImage[eat[i].type][0];
			}
			else {
				eat[i].image = EnemyImage[eat[i].type - 3][0];
			}
			switch (eat[i].type) {
			case 0:
				eat[i].e_y = (GetRand(2) + 3) * 100 + 150;
				break;
			case 1:
				eat[i].e_y = GetRand(2) * 100 + 150;
				break;
			case 2:
				eat[i].e_y = GetRand(5) * 100 + 150;
				break;
			case 3:
				eat[i].e_y = GetRand(1) * 100 + 150;
				eat[i].typeD = TRUE;
				break;
			case 4:
				eat[i].e_y = GetRand(3) * 100 + 150;
				eat[i].typeD = TRUE;
				break;
			}

			eat[i].e_w = 50 * 1.5;
			eat[i].e_h = 50 * 1.5;

			return TRUE;
		}
	}

	return FALSE;
}

//ライフ
void LifeImage() {

	for (int i = 0; i < player.life; i++)
	{
		DrawGraph(LifeX + (60 * i), LIfeY, Life, TRUE);

	}
	if (player.life < 1) {
		key2 = 1;
		player.flg = FALSE;
		boss.flg = FALSE;
		boss.speed = FALSE;
		dieam();

	}
}
void dieam() {

	if (--DEI_act_wait <= 0)
	{
		if (key1 < 1) {
			DEI_act_index++;
			DEI_act_wait = DEI_ANI_SPEED;
			DEI_act_index %= DEI_MOTION_INDEX;
		}

		motion_index7 = deianime[DEI_act_index];

	}

	DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, fishdie[Leve - 1][motion_index7], TRUE);


	StopSoundMem(MainSound);
	StopSoundMem(BossSound);
	StopSoundMem(WallSE);
	StopSoundMem(DamegeSE);
	if (CheckSoundMem(DieSE) == 0) {
		if (SEFlg == FALSE) {
			PlaySoundMem(DieSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	if (motion_index7 > 5) {
		key2 = 0;
		GameState = 7;
		StopSoundMem(KeikokuSE);
	}
}

//メーター表示
void MeterImage() {

	//エビメーター
	DrawGraph(m_x - 10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0][0], TRUE);

	//アジメーター
	DrawGraph(m_x + 240, m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1][0], TRUE);

	//イカメーター
	DrawGraph(m_x + 480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2][0], TRUE);
}

//成長
void PlayerGrowth() {

	//サイズの変更量の増加
	if (Leve == 1) {
		Scke = 2;
		player.w *= Scke;
		player.h *= Scke;
		player.x -= PLAYER_WIDTH / 2;
		player.y -= PLAYER_HEIGHT / 2;
	}
	else if (Leve == 2) {
		Scke = 1.5;
		LeveUpflg2 = TRUE;
		player.w *= Scke;
		player.h *= Scke;
	}
	LeveUpflg1 = TRUE;
	LeveUp *= 2;
	Leve++;
	LifeMax++;
}

//食べた時の処理
void PlayerEat(int* e) {

	switch (*e) {
	case 0:
		if (em < 5) em++;
		break;
	case 1:
		if (am < 5)am++;
		break;
	case 2:
		if (im < 5)im++;
		break;
	}

	if (*e <= 2) {
		//食べたものを量を増加させる
		EatAmount++;
		if (player.life < LifeMax) player.life++;
	}

	//食べたものの量が一定量に達したら処理を移す
	if ((EatAmount >= LeveUp)&&(Leve <3)) {
		PlayerGrowth();
		if (SEFlg == FALSE) {
			PlaySoundMem(EvoSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
}

void DEat() {

	static int DEflg = FALSE;
	static int Dx = 0, Dy = 0, Dw = 0, Dh = 0;

	if (DEflg == FALSE) {
		RandEat = GetRand(2);
		switch (RandEat) {
		case 0:
			if (em < 6 && em>0) em--;
			else RandEat = 3;

			break;
		case 1:
			if (am < 6 && am>0)am--;
			else RandEat = 3;
			break;
		case 2:
			if (im < 6 && im>0)im--;
			else RandEat = 3;
			break;
		}
		DEflg = TRUE;
	}

	if (Deatflg == FALSE) {
		Dx = player.x;
		Dy = player.y;
		if (Leve == 1) {
			Dw = player.w;
			Dh = player.h;
		}
		else {
			Dw = player.w / 1.5;
			Dh = player.h / 1.5;
		}
		Deatflg = TRUE;
	}

	if (RandEat < 3) {
		DrawExtendGraph(Dx + Dw / 2 + 10 + DSpeed, Dy + Dh / 2 + 10, Dx + Dw + DSpeed - 10, Dy + Dh - 10, feedImage[RandEat][0], Deatflg);
		DSpeed += 10;
		if (Dx + DSpeed >= SCREEN_WIDTH) {
			Deatflg = FALSE;
			Feedflg = FALSE;
			DEflg = FALSE;
			DSpeed = 0;
		}
	}
	else {
		DEflg = FALSE;
	}

	if (RandEat > 2) {
		Deatflg = FALSE;
		Feedflg = FALSE;
	}
}

//餌とプレイヤーのあたり判定
int Hit(Player* p, Eat* e) {

	if (Time > 0) {
		int px = player.x;
		int py = player.y;
		int ph = player.h;
		int pw = player.w;
		int phx = player.x;
		int phy = player.y;
		int phh = player.h;
		int phw = player.w;
		int ex = e->e_x;
		int ey = e->e_y;
		int ew = e->e_w;
		int eh = e->e_h;
		int etypeD = e->typeD;

		if (Leve == 1) {
			px += 10;
			py += 25;
			pw -= 10;
			ph -= 50;
			phx += 45;
			phy += 45;
			phw -= 90;
			phh -= 90;
		}
		else if (Leve == 2) {
			px += 50;
			py += 70;
			pw -= 80;
			ph -= 140;
			phx += 95;
			phy += 95;
			phw -= 190;
			phh -= 190;
		}
		else if (Leve == 3) {
			px += 40;
			py += 60;
			pw -= 50;
			ph -= 120;
			phx += 95;
			phy += 95;
			phw -= 150;
			phh -= 180;
		}

		//餌とのあたり判定
		if (etypeD == FALSE) {
			if (ex + ew >= px && ex <= px + pw &&
				ey + eh >= py && ey <= py + ph) {
				e->flg = FALSE;
				if (SEFlg == FALSE) {
					PlaySoundMem(EatSE, DX_PLAYTYPE_BACK, TRUE);
				}
				return TRUE;
			}
		}
		else {
			if (ex + ew >= phx && ex <= phx + phw &&
				ey + eh >= phy && ey <= phy + phh && player.muteki == 0) {
				player.life--;
				player.muteki = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
				}
				Feedflg = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void Goal() {
	/*if (Time <= 0) {
		DrawBox(1200, 400, 1300, 500, GetColor(255, 212, 0), FALSE);
	}*/
}
//ゲームクリア（当たったら）
void GameClearHit() {

	GameState = 3;
	StopSoundMem(BossSound);
	StopSoundMem(KeikokuSE);
	if (SEFlg == FALSE) {
		PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
	}
	SHIPFlg = FALSE;
}

void GameClear() {
	static bool push = 0; // 押されたかどうか確認する関数

	//エンディング分岐変数
	int EndBranch = 0;

	//エンディング判定
	//何も食べなかった場合
	if (EatAmount == 0) {
		EndBranch = 1;
	}
	//全てが平均的な場合
	else if (em == im && em == am && am == im) {
		EndBranch = 2;
	}
	//イカが一番多い時
	else if (im >= am!=em && im >= em!=am && im > 4) {
		EndBranch = 3;
	}
	//アジが一番多い場合
	else if (am >= em && am > im&&am>4) {
		EndBranch = 4;
	}
	//エビが一番多い場合
	else if (em > am && em > im&&em>4) {
		EndBranch = 5;
	}
	//レベルが1の状態で餌が平均、MAXじゃない場合
	else if (Leve == 1 && em < 5 && am < 5 && im < 5) {
		EndBranch = 6;
	}
	//レベルが2の状態で餌が平均、MAXじゃない場合
	else if (Leve == 2 && em < 5 && am < 5 && im < 5) {
		EndBranch = 7;
	}
	//レベルが3の状態で餌が平均、MAXじゃない場合
	else if (Leve == 3 && em < 5 && am < 5 && im < 5) {
		EndBranch = 8;
	}
	

	//エンディング表示
	DrawGraph(0, 0, Gameclear[EndBranch], TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(420 + MenuNo * 300, 750, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(ClearSE);
			if (BGMFlg == FALSE) {
				PlaySoundMem(MainSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(ClearSE);
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}

//ボスステージ移行（当たったら）
void BossST(Player* p) {

	int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;

	if (Time <= 0) {
		DrawGraph(800, 50, BOSSROAD, TRUE);
		if (1410 >= px && 1410 <= px + ph &&
			800 >= py && 0 <= py + pw) {
			GameState = 4;
			StopSoundMem(MainSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(BossSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}

void BossInit() {

	//Bossの初期化
	boss.flg = TRUE;
	boss.dir = 0;
	boss.bx = BOSS_POS_X;
	boss.by = BOSS_POS_Y;
	boss.bw = BOSS_WIDTH;
	boss.bh = BOSS_HEIGHT;
	boss.speed = BOSS_SPEED;
	count = 0;
	counth = 0;
	BOSS_PATTREN = 1;


	soni.sx = SONIC_POS_X;
	soni.sy = SONIC_POS_Y;
	soni.sw = SONIC_WIDTH;
	soni.sh = SONIC_HEIGHT;


	SHIP_X = 1300;
	SHIP_lX = -400;
	SHIP_Y = -100;
	SHIP_W = 350;
	SHIP_H = 350;
	SHIP_SPEED = 5;
	SHIP_COUNT = 0;
	NET = 0;
	motion_index4 = 0;
	motion_index8 = 0;
	sc = 2;
	cr = 0;
	cr2 = 0;

	net.nx = 1150;
	net.lnx = -250;
	net.ny = 50;
	net.nw = 350;
	net.nh = 350;
	net.speed = 15;
	netflg = 0;

	//playerの位置初期化
	player.x = 500;
	player.y = PLAYER_POS_Y;

	motion_index9 = 0;
	motion_index4 = 0;
	NET_act_index = 0;

	//ゲームメインへ
	GameState = 5;
}

void BossBackScrool() {
	if (GameState == 5 && key1 < 1 && GameState == 5 && key2 < 1) {
		ScroolSpeed -= player.speed;

	}

	//ステージ画像表示
	//描画可能エリアを設定
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//レベル表示
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
}
void BossMove() {

	if (--BOSS_act_wait <= 0)
	{
		if (key1 < 1) {
			BOSS_act_index++;
			BOSS_act_wait = BOSS_ACT_SPEED;
			BOSS_act_index %= BOSS_MOTION_INDEX;
			BOSS_act_index3++;
			BOSS_act_index3 %= SONIC_MOTION_INDEX;
		}
	}

	if (BOSS_PATTREN == 1) {
	
		BossMove1();
	}
	if (BOSS_PATTREN == 2) {
	
		BossMove2();
	}
	if (BOSS_PATTREN == 3) {
	
		BossMove3();
	}
	if (player.muteki == 0) {
		if (key1 < 1) {
			if (player.muteki == 0) {
				//当たり判定
				if (HitBoxPlayer(&player, &boss) == TRUE) {

					player.life -= 1;
					player.muteki = 1;
					if (SEFlg == FALSE) {
						PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
			//当たり判定
			if (HitBoxPlayer2(&player, &soni) == TRUE) {

				player.life -= 1;
				player.muteki = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
		}
	}
}
void BossMove1() {

	if (key1 < 1) {

		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		/*DrawFormatString(100, 180, 0x000000, "%d", boss.bx);
		DrawFormatString(100, 240, 0x000000, "%d", boss.by);*/

		if (count == 1) {
			BOSS_SPEED = 20;
		}
		if (count > 0 && count < 100) {
			if (boss.bx == 0) {
				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;

			motion_index2 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}

		if (count > 101 && count < 150) {
			motion_index2 = BOSSAttack[BOSS_act_index];
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 200, boss.by - 50, Keikoku, TRUE);
			if (Leve == 1) {
				BOSS_SPEED = 20;
			}
			else if (Leve == 2) {
				BOSS_SPEED = 35;
			}
			else if (Leve == 3) {
				BOSS_SPEED = 50;
			}
		}
		if (count > 151 && count < 300) {
			motion_index2 = BOSSAttack[BOSS_act_index];
			boss.bx += BOSS_SPEED;
		}
		if (count == 301) {
			boss.bx = 1400;
		}

		if (count > 302 && count < 499) {
			motion_index2 = BossKoutai[BOSS_act_index];
			boss.dir = 1;
			if (boss.bx < -450) {
				boss.dir = 0;
				BOSS_SPEED = 0;
			}
			boss.bx -= BOSS_SPEED;
			boss.by = 600;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;
		}

		if (count == 499) {
			count = 0;
			boss.by = 200;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 2;
			countS += 1;
		}
	}
	if (boss.dir == 0) {
		DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
	}
	if (boss.dir == 1) {
		DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss3[motion_index2], TRUE);
	}
}

void BossMove2() {
	if (key1 < 1) {
		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		/*DrawFormatString(100, 180, 0x000000, "%d", boss.bx);
		DrawFormatString(100, 240, 0x000000, "%d", boss.by);*/
		if (count > 0 && count < 100) {
			if (boss.bx == 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			motion_index2 = BOSSAnime[BOSS_act_index];
		}

		if (count > 100 && count < 200) {
			motion_index2 = BOSSDown[BOSS_act_index];
			BOSS_SPEED = 20;
			boss.bx += BOSS_SPEED;
			boss.by += 10;
		}
		if (count > 170 && count < 200) {
			DrawGraph(boss.bx + 100, 600, Keikoku, TRUE);
			boss.bx = player.x - 130;
		}
		if (count > 160 && count < 170) {
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 201 && count < 399) {
			motion_index2 = BOSSUp[BOSS_act_index];
			BOSS_SPEED = 0;
			boss.by -= 20;
			if (count == 160) {
				sibuki_act_index = 0;
			}
			if (count > 270 && count < 340) {

				//sibuki_act_index = 0;
				Sibuki();
			}

		}

		if (count == 399) {
			boss.bx = BOSS_POS_X;
			BOSS_SPEED = 20;
		}

		if (count > 399 && count < 498) {
			motion_index2 = BOSSAnime[BOSS_act_index];
			if (boss.bx == 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			boss.by = player.y - 180;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;
		}
		if (count == 499) {
			count = 0;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 3;
			countS += 1;
		}
	}

	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);


}

void BossMove3() {
	if (--BOSS_act_wait2 <= 0)
	{

		if (key1 < 1) {
			if (MS == 1) {
				BOSS_act_index2++;
				BOSS_act_wait2 = BOSS_ACT_SPEED2;
				BOSS_act_index2 %= BOSSB_MOTION_INDEX;
			}
		}
	}
	if (key1 < 1) {
		count = (count + 1) % 900;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		if (count > 0 && count < 25) {
			if (boss.bx == 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			motion_index2 = BOSSAnime[BOSS_act_index];
		}
		if (count > 0 && count < 50) {
			BOSS_act_index2 = 0;
			MS = 0;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 50 && count < 100) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);
		}
		if (count == 99) {
			soni.sx = 190;
			//soni.sy = 120;
			soni.sy = player.y - 180;
		}
		if (count > 100 && count < 300) {
			BOSS_act_index2 = 0;
			MS = 0;
			//BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*if (boss.by > 300) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 300 && count < 350) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);

		}
		if (count == 349) {
			soni.sx = 190;
			//soni.sy = 340;
			soni.sy = player.y - 100;
		}
		if (count > 350 && count < 500) {
			BOSS_act_index2 = 0;
			MS = 0;
			//BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*if (boss.by > 450) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 500 && count < 550) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);

		}
		if (count == 549) {
			soni.sx = 190;
			//soni.sy = 470;
			soni.sy = player.y - 100;
		}
		if (count > 550 && count < 800) {
			BOSS_act_index2 = 0;
			MS = 0;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by < 300) {
				BOSS_SPEED = 0;
			}
			boss.by -= BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}

		}

		if (count > 100 && count < 300 || count > 350 && count < 550 ||
			count > 550 && count < 800) {
			motion_index6 = BossSonic[BOSS_act_index3];
			if (Leve == 1) {
				soni.sx += SONIC_SPEED_Lev1;
			}
			else if (Leve == 2) {
				soni.sx += SONIC_SPEED_Lev2;
			}
			else if (Leve == 3) {
				soni.sx += SONIC_SPEED_Lev3;
			}
			DrawExtendGraph(soni.sx, soni.sy, soni.sx + soni.sw, soni.sy + soni.sh, Sonic[motion_index6], TRUE);
		}
		if (count == 799) {
			count = 0;
			BOSS_act_index2 = 0;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 1;
			countS += 1;
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);

		}
	}
	//DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);
}

void Sibuki() {//水しぶきのアニメーション


	int count_s = 10;

	if (count_s != 0) {
		if (Leve == 1) {
			boss.bx -= 1.5;
		}
		else if (Leve == 2) {
			boss.bx -= 4;
		}
		else {
			boss.bx -= 6;
		}
	}

	if (--sibuki_act_wait <= 0)
	{
		if (key1 < 1) {
			sibuki_act_index++;
			sibuki_act_wait = sibuki_ANI_SPEED;
			sibuki_act_index %= sibuki_MOTION_INDEX;
		}

		motion_index9 = sibukianime[sibuki_act_index];

	}
	DrawExtendGraph(boss.bx, SIBUKI_Y, boss.bx + boss.bw, SIBUKI_Y + 162, sibuki[motion_index9], TRUE);
	//DrawExtendGraph(500, SIBUKI_Y, 500 + 300, SIBUKI_Y + 300, sibuki[motion_index9], TRUE);
	if (CheckSoundMem(SibukiSE) == 0) {
		if (SEFlg == FALSE) {
			PlaySoundMem(SibukiSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
}


//船アニメーション
void Ship() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			SHIP_COUNT += sc;
			SHIP_COUNT2 = 400 - SHIP_COUNT;
			if (SHIP_COUNT2 == 0) {
				sc = 0;
			}
			if (SHIP_COUNT2 != 0) {
				SetFontSize(50);
				//DrawFormatString(700, 30, 0x000000, "船が来るまで%d秒", SHIP_COUNT2 / 100);
			}
			if (countS >= 3)
			{
				SHIP_X -= SHIP_SPEED;
				net.nx -= SHIP_SPEED;
				SetFontSize(50);
				DrawFormatString(700, 30, 0x000000, "マグロを捕獲しよう");
			}
			if (SHIP_X == 1300)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;

			}
			if (SHIP_X == 800)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;

			}
			if (SHIP_X == 300)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;
			}
			if (SHIP_X == -300)
			{
				SHIPFlg = TRUE;
				SHIP_X = 1500;
				net.nx = 1350;
			}

			if (counth > 1 && counth < 148) {
				NET = 1;
			}
			if (motion_index4 == 6) {
				netflg = 1;
			}

			//if (SHIP_X != 300)
			//{
			if (SHIPFlg != TRUE) {
				if (netflg == 1) {
					NET = 0;
					SHIP_SPEED = 10;
					netflg = 0;
					motion_index4 = 0;
					NET_act_index = 0;
				}
			}
			//}
			if (countS >= 3) {
				if (--SHIP_act_wait <= 0)
				{
					if (key1 < 1) {
						SHIP_act_index++;
						SHIP_act_wait = SHIP_ANI_SPEED;
						SHIP_act_index %= SHIP_MOTION_INDEX;
					}
				}

				motion_index3 = shipanime[SHIP_act_index];
			}
			DrawExtendGraph(SHIP_X, SHIP_Y, SHIP_X + SHIP_W, SHIP_Y + SHIP_H, ship1[motion_index3], TRUE);
		}
	}
}
// 船（反転）アニメーション
void ShipLs()
{
	if (SHIPFlg == TRUE)
	{
		SHIP_lX += SHIP_SPEED;
		net.lnx += SHIP_SPEED;
		SetFontSize(50);
		DrawFormatString(700, 30, 0x000000, "マグロを捕獲しよう");

		if (SHIP_lX == 100)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;

		}
		if (SHIP_lX == 500)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;

		}
		if (SHIP_lX == 900)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;
		}
		if (SHIP_lX == 1500)
		{
			SHIPFlg = FALSE;
			SHIP_lX = -400;
			net.lnx = -250;
		}
		if (counth > 1 && counth < 148) {
			NET = 1;
		}
		if (motion_index4 == 6) {
			netflg = 1;
		}

		if (SHIPFlg != FALSE)
		{
			if (netflg == 1) {
				NET = 0;
				SHIP_SPEED = 10;
				netflg = 0;
				motion_index4 = 0;
				NET_act_index = 0;
			}
			//}

			if (--SHIP_act_wait <= 0)
			{
				if (key1 < 1) {
					SHIP_act_index++;
					SHIP_act_wait = SHIP_ANI_SPEED;
					SHIP_act_index %= SHIP_MOTION_INDEX;
				}

				motion_index3 = shipanime[SHIP_act_index];
			}
			DrawExtendGraph(SHIP_lX, SHIP_Y, SHIP_lX + SHIP_W, SHIP_Y + SHIP_H, ship2[motion_index3], TRUE);
		}
	}
}

//網アニメーション
void Ami() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			if (cr == 0) {
				if (countS >= 3) {
					if (SHIP_SPEED == 0) {
						if (--NET_act_wait <= 0)
						{
							if (key1 < 1) {
								NET_act_index++;
								NET_act_wait = NET_ANI_SPEED;
								NET_act_index %= NET_MOTION_INDEX;
							}
						}
					}
					motion_index4 = netanime[NET_act_index];
				}
				if (SHIP_X == 1300) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (1300 >= boss.bx && 1400 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (1300 >= boss.bx && 1400 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (1300 >= boss.bx && 1400 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				else if (SHIP_X == 700) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				else if (SHIP_X == 300) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (300 >= boss.bx && 400 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (300 >= boss.bx && 400 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (300 >= boss.bx && 400 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				DrawExtendGraph(net.nx, net.ny, net.nx + net.nw, net.ny + net.nh, net1[motion_index4], TRUE);
			}
		}
	}
}

void AmiLs() {
	if (SHIPFlg == TRUE) {
		if (key1 < 1) {
			if (cr == 0) {
				if (SHIP_SPEED == 0) {
					if (--NET_act_wait <= 0)
					{
						if (key1 < 1) {
							NET_act_index++;
							NET_act_wait = NET_ANI_SPEED;
							NET_act_index %= NET_MOTION_INDEX;
						}
					}
					motion_index4 = netanime[NET_act_index];
				}
				if (SHIP_lX == 100) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (200 >= boss.bx && 300 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (200 >= boss.bx && 300 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (200 >= boss.bx && 300 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				else if (SHIP_lX == 500) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (600 >= boss.bx && 700 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				else if (SHIP_lX == 900) {
					if (motion_index4 == 4) {
						//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
						if (1000 >= boss.bx && 1100 <= boss.bx + boss.bh &&
							250 >= boss.by && 270 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
						if (1000 >= boss.bx && 1100 <= boss.bx + boss.bh &&
							310 >= boss.by && 330 <= boss.by + boss.bw) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
						if (1000 >= boss.bx && 1100 <= boss.bx + boss.bh &&
							360 >= boss.by && 380 <= boss.by + boss.bw) {
							cr = 1;
						}
					}
				}
				DrawExtendGraph(net.lnx, net.ny, net.lnx + net.nw, net.ny + net.nh, net2[motion_index4], TRUE);
			}
		}
	}
}

void BHA() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			if (--HOKAKU_act_wait <= 0)
			{
				if (key1 < 1) {
					HOKAKU_act_index++;
					HOKAKU_act_wait = HOKAKU_SPEED;
					HOKAKU_act_index %= HOKAKU_MOTION_INDEX;
				}
				motion_index8 = BossHokaku[HOKAKU_act_index];
			}
			if (motion_index8 == 3) {
				cr2 = 1;
			}
			DrawGraph(net.nx, net.ny, Hokaku[motion_index8], TRUE);
		}
	}
}

void BHALs() {
	if (SHIPFlg == TRUE) {
		if (key1 < 1) {
			if (--HOKAKU_act_wait <= 0)
			{
				if (key1 < 1) {
					HOKAKU_act_index++;
					HOKAKU_act_wait = HOKAKU_SPEED;
					HOKAKU_act_index %= HOKAKU_MOTION_INDEX;
				}
				motion_index8 = BossHokaku[HOKAKU_act_index];
			}
			if (motion_index8 == 3) {
				cr2 = 1;
			}
			DrawGraph(net.lnx, net.ny, Hokaku2[motion_index8], TRUE);
		}
	}
}

void BossStage() {

	BossBackScrool();
	PlayerMove();
	if (cr == 0) {
		BossMove();
		HOKAKU_act_index = 0;
	}
	LifeImage();
	MeterImage();
	if (cr == 1) {
		if (SHIPFlg == FALSE) {
			BHA();
		}
		else if (SHIPFlg == TRUE) {
			BHALs();
		}
	}
	if (cr2 == 1) {
		GameClearHit();
	}
	//Goal();
	if (NET == 1) {
		Ami();
		AmiLs();
	}
	Ship();
	ShipLs();
	Pouse();
}


/*************************************
*自機と敵機の当たり判定（四角）
* 引数：PLAYER　ポインタ
* 戻り値：TRUE：あたり、FALSE：はずれ
*************************************/
int HitBoxPlayer(Player* p, Boss* b)
{
	if (Leve == 1) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int bx1 = b->bx - (b->bw - boss.bw - 100);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 2) {
		//x,yは中心座標とする
		int px = p->x - 20 - (p->w - player.w - 90);
		int py = p->y - 10 - (p->h - player.h - 90);
		int ph = px - 20 + p->h - 90;
		int pw = py - 5 + p->w - 150;

		int bx1 = b->bx - (b->bw - boss.bw - 100);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 3) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w - 70);
		int py = p->y - (p->h - player.h - 70);
		int ph = px + p->h - 90;
		int pw = py + p->w - 130;

		int bx1 = b->bx - (b->bw - boss.bw - 100);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}

int HitBoxPlayer2(Player* p, Soni* s)
{
	if (Leve == 1) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		//DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		//DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 2) {
		//x,yは中心座標とする
		int px = p->x - 20 - (p->w - player.w - 90);
		int py = p->y - 10 - (p->h - player.h - 90);
		int ph = px - 20 + p->h - 90;
		int pw = py - 5 + p->w - 150;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		//DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		//DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 3) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w - 70);
		int py = p->y - (p->h - player.h - 70);
		int ph = px + p->h - 90;
		int pw = py + p->w - 130;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}


int ColorCheck(int x, int y) {
	
	Cr1 = GetPixel(x, y);
	GetColor2(Cr1, &r, &g, &b);
	if (r == 98 && g == 96 && b == 94) {
		if (player.muteki == 0) {
			if (PointNom == 0) {

				//Level 1 左の判定
				if ((y == player.y + player.h / 2 - CollYadd) || (y == player.y + player.h / 2 + CollYadd)) {
					if (x == player.x + player.w / 2 - CollXadd) PointNom = 1;
					if (x == player.x + player.w / 2 + CollXadd * 3) PointNom = 2;
				}

				//Level 2 左の判定
				if ((player.x + player.w / 2) && ((y == player.y + player.h / 2 - CollYadd / 4) || (y == player.y + player.h / 2 + CollYadd / 4))) {
					PointNom = 1;
				}
				//Level 2 右の判定
				if ((player.x + player.w / 2 + CollXadd) && ((y == player.y + player.h / 2 - CollYadd + 2) || (y == player.y + player.h / 2 + CollYadd + 2))) {
					PointNom = 2;
				}

				//Level 3 左の判定
				if ((x == player.x + player.w / 2 - CollXadd) && ((y == player.y + player.h / 2 - CollYadd) || (y == player.y + player.h / 2 + CollYadd))) {
					PointNom = 1;
				}
				//Level 3 右の判定
				if ((x == player.x + player.w / 2 + CollXadd * 5 + 5) && ((y == player.y + player.h / 2 - CollYadd * 2) || (y == player.y + player.h / 2 + CollYadd * 2))) {
					PointNom = 2;
				}
			}
			Damege = 1;
			player.life -= 1;
			player.muteki = 1;
			if (SEFlg == FALSE) {
				PlaySoundMem(WallSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
	
	return colorsum;
}

int EatCheck(int x, int y) {
	Cr1 = GetPixel(x, y);
	GetColor2(Cr1, &er, &eg, &eb);

	if ((er == 98) && (eg == 96) && (eb == 94) && (Eatflg < 1)) {
		Eatflg = 1;
		EatCont++;
	}

	if (Eatflg == 1) Eatflg = 0;
	return colorsum;
}

void Pouse() {
	static bool push = 0;

	if (player.life > 0) {
		if (g_KeyFlg & PAD_INPUT_8 && key1 < 1) {
			key1 = 1;
			player.flg = FALSE;
			boss.flg = FALSE;
			boss.speed = FALSE;
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		else if (key1 > 0 && g_KeyFlg & PAD_INPUT_8) {
			player.flg = TRUE;
			boss.flg = TRUE;
			boss.speed = TRUE;
			key1 = 0;
		}

		if (key1 > 0) {
			DrawGraph(355, 100, pauseImage, TRUE);
			//player.flg = FALSE;
			// メニューカーソルの描画
			DrawRotaGraph(590, 445 + MenuNo * 80, 0.3f, 0, Corsol2, TRUE);

			// メニューカーソル移動処理
			if (g_KeyFlg & PAD_INPUT_DOWN) {
				if (++MenuNo > 1)MenuNo = 0;
				if (SEFlg == FALSE) {
					PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (g_KeyFlg & PAD_INPUT_UP) {
				if (--MenuNo < 0)MenuNo = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (g_KeyFlg & PAD_INPUT_2) {
				if (push == 0) {
					push = 1;
				}
				if (MenuNo == 0) {
					push = 0;
					//MenuNo = 0;
					StopSoundMem(MainSound);
					StopSoundMem(BossSound);
					key1 = 0;
					GameState = 1;
				}
				else if (MenuNo == 1) {
					push = 0;
					MenuNo = 0;
					key1 = 0;
					StopSoundMem(MainSound);
					StopSoundMem(BossSound);
					GameState = 0;
				}
			}
		}
	}
}
void GameOver() {
	static bool push = 0; // 押されたかどうか確認する関数
	SHIPFlg = FALSE;
	if (Leve == 2) {
		DrawGraph(0, 0, Gameover2, TRUE);
	}
	else if (Leve == 3) {
		DrawGraph(0, 0, Gameover3, TRUE);
	}
	else {
		DrawGraph(0, 0, Gameover, TRUE);
	}

	if (CheckSoundMem(GameOverSound) == 0) {
		if (BGMFlg == FALSE) {
			PlaySoundMem(GameOverSound, DX_PLAYTYPE_BACK, TRUE);
		}
	}


	// メニューカーソルの描画
	if (MenuNo != 1) {
		DrawRotaGraph(100 + MenuNo * 450, 715, 0.3f, 0, Corsol2, TRUE);
	}
	else {
		DrawRotaGraph(740, 715, 0.3f, 0, Corsol2, TRUE);
	}

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(GameOverSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(GameOverSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
}