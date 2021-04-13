#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
#include "Boss.h"
#include "Sound.h"
#include "CollisionDetiction.h"
int	g_OldKey;				// �O��̓��̓L�[
int	g_NowKey;				// ����̓��̓L�[
int	g_KeyFlg;				// ���̓L�[���
char key[256];

int GameState = 0;          //�������@

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 1410;
const int SCREEN_HEIGHT = 800;

int ScroolSpeed;


void GameTitle();
void GameInit();		//�Q�[������������
void GameMain();		//�Q�[�����C������
void GameHelp();		//�Q�[���w���v����
void BackScrool();         //�w�i�摜�X�N���[������
void GameClear();		//�Q�[���N���A����
void GameOver();
void Goal();
void Pouse();

int LoadImages();          //�摜�ǂݍ���
int LoadSound();		// ���y�Ǎ���

int Time = 2400;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	SetMainWindowText("a");// �^�C�g���ύX

	ChangeWindowMode(TRUE);		// �E�B���h�E���[�h�ŋN��

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//SetGraphMode(800, 600, 16);

	if (DxLib_Init() == -1) return -1;	// DX���C�u�����̏���������

	SetDrawScreen(DX_SCREEN_BACK);	// �`����ʂ𗠂ɂ���

	if (LoadImages() == -1)return -1; //�摜�Ăяo��

	if (LoadSound() == -1)return -1;	// ���y�̓ǂݍ���

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 && GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		if (key1 < 1) {
			Time--;
		}

		//���̓L�[�擾
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// ��ʂ̏�����

		switch (GameState) {
		case 0:
			GameTitle();
			break;
		case 1:
			GameInit();		//�Q�[����������
			break;
		case 2:
			GameMain();		//�Q�[�����C������
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
		}
		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f
	}

	DxLib_End();	// DX���C�u�����g�p�̏I������
	return 0;	// �\�t�g�̏I��
}

void GameTitle() {
	static bool push = 0;	// �����ꂽ���ǂ����m�F����֐�

	if (CheckSoundMem(TitleSound) == 0) {
		PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
	}
	StopSoundMem(HelpSound);
	
	DrawGraph(0, 0, Gametitle, TRUE);
	// ���j���[�J�[�\���̕`��
	DrawRotaGraph(590, 385 + MenuNo * 108, 0.3f, 0, Corsol, TRUE);

	// ���j���[�J�[�\���ړ�����
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 2)MenuNo = 0;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 2;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}



	// Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 1;
		}
		// sceneStage�ɍs������
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 6;
			MenuNo = 0;
			PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
		}
		else {
			push = 0;
			DxLib_End();
		}
	}
}

void GameHelp()
{
	static bool push = 0;	// �����ꂽ���ǂ����m�F����֐�
	DrawGraph(0, 0, Gamehelp, TRUE);

	StopSoundMem(TitleSound);

	// ���j���[�J�[�\���̕`��
	DrawRotaGraph(480, 300 + MenuNo * 185, 0.3f, 0, Corsol, TRUE);

	// ���j���[�J�[�\���ړ�����
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 2)MenuNo = 0;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 2;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}



	// Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 1;
		}
		// sceneStage�ɍs������
		if (MenuNo == 0) {
			push = 0;
		}
		else if (MenuNo == 1) {
			push = 0;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
}

/***********************************************
 * �Q�[����������
 ***********************************************/
void GameInit() {
	//�X�N���[�����x��������
	ScroolSpeed = 0;
	//�H�ׂ��ʌ�
	em = 0;
	am = 0;
	im = 0;
	EatAmount = 0;
	Leve = 1;
	Scke = 1;
	LeveUp = 1;

	//�v���C���[�̏�����
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
	Time = 2400;
	Iwaspeed = 0;

	//�a�̏�����
	for (int i = 0; i < 10; i++) {
		eat[i].flg = FALSE;
	}
	//�Q�[�����C����
	GameState = 2;

	StopSoundMem(TitleSound);
	PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
}

/***********************************************
/*�Q�[�����C���̏���*/
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
 *�w�i�摜�X�N���[������
 *�����F�Ȃ�
 *�߂�l�F�Ȃ�
 *************************************/
void BackScrool()
{

	Range += player.speed;

	if (GameState == 2) {
		if (Time > 0) {
			ScroolSpeed -= player.speed;
		}
	}

	//�X�e�[�W�摜�\��

	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//����
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);*/
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//�ݒ�����ɖ߂��B
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�G���A��߂�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//��摜�\��

	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);

	DrawGraph(Iwaspeed % IwaHaba, 50, Iwa[0], TRUE);
	DrawGraph(IwaHaba + (Iwaspeed % IwaHaba), 50, Iwa[0], TRUE);


	//�G���A��߂�
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);


	//���x���\��
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
	DrawFormatString(0, 100, 0x000000, "er.%d, eg.%d, eb.%d", er, eg, eb);
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

	//�㉺���E�ړ�
	if (player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_UP)player.y -= player.speed;
		if (g_NowKey & PAD_INPUT_DOWN)player.y += player.speed;
		if (g_NowKey & PAD_INPUT_LEFT)player.x -= player.speed;
		if (g_NowKey & PAD_INPUT_RIGHT)player.x += player.speed;
	}

	//��ʂ���͂ݏo���Ȃ��悤�ɂ���
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

	if (Leve == 1 && key1 != 1) {
		Umispeed -= 2;
		Iwaspeed -= 5;
	}
	if (Leve == 2 && key1 != 1) {
		Umispeed -= 4;
		Iwaspeed -= 7;
		player.speed = 8;
	}
	if (Leve == 3 && key1 != 1) {
		Umispeed -= 6;
		Iwaspeed -= 9;
		player.speed = 9;
	}



	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	int motion_index = anime[act_index];
	if (player.muteki == 0) {
		DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	}
	if (player.muteki == 1) {
		if (key1 < 1) {

			// �_���[�W������ƂT��̂����Q��\������B
			static int count = 0;
			count = (count + 1) % 60;
			if (count % 2 == 0) {
				//�\��
				DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
			}
			if (count == 59 || player.life == 0) {
				player.muteki = 0;

			}
		}
		else {
			DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
		}
	}
	int ColorCheck(int x, int y);
	ColorCheck(player.x + player.w, player.y + (Scke * Scke * 30));						//�E��
	ColorCheck(player.x + (player.w / 2), player.y + (Scke * Scke * 15));				//�㕔����
	ColorCheck(player.x, player.y + player.h - (Scke * Scke * 30));						//����
	ColorCheck(player.x + player.w, player.y + player.h - (Scke * Scke * 30));			//�E��
	ColorCheck(player.x + (player.w / 2), player.y + player.h - (Scke * 15));	//��������
	
	//�����蔻��m�F�pBOX
	DrawBox(player.x - 2, player.y + (Scke * Scke * 30) - 2, player.x + 2, player.y + (Scke * Scke * 30) + 2, GetColor(255, 255, 255), TRUE);
	DrawBox(player.x + player.w - 2, player.y + (Scke * Scke * 30) - 2, player.x + player.w + 2, player.y + (Scke * Scke * 30) + 2, GetColor(255, 255, 255), TRUE);
	DrawBox(player.x + (player.w / 2) - 2, player.y + (Scke * Scke * 15) - 2, player.x + (player.w / 2) + 2, player.y + (Scke * Scke * 15) + 2, GetColor(255, 255, 255), TRUE);
	DrawBox(player.x - 2, player.y + player.h - (Scke * Scke * 30) - 2, player.x + 2, player.y + player.h - (Scke * Scke * 30) + 2, GetColor(255, 255, 255), TRUE);
	DrawBox(player.x + player.w - 2, player.y + player.h - (Scke * Scke * 30) - 2, player.x + player.w + 2, player.y + player.h - (Scke * Scke * 30) + 2, GetColor(255, 255, 255), TRUE);
	DrawBox(player.x + (player.w / 2) - 2, player.y + player.h - (Scke * Scke * 15) - 2, player.x + (player.w / 2) + 2, player.y + player.h - (Scke * Scke * 15) + 2, GetColor(255, 255, 255), TRUE);

}

int LoadImages() {
	//�^�C�g���摜
	if ((Gametitle = LoadGraph("Image/umi.png")) == -1)return -1;
	//�Q�[���w���v�摜
	if ((Gamehelp = LoadGraph("Image/GameHelp.png")) == -1)return -1;
	//�|�[�Y�摜
	if ((pauseImage = LoadGraph("Image/pause.png")) == -1)return-1;
	// �J�[�\���摜
	if ((Corsol = LoadGraph("Image/coursol.png")) == -1)return -1;
	//�|�[�Y��ʂ̃J�[�\���摜
	if ((Corsol2 = LoadGraph("Image/coursol2.png")) == -1)return -1;
	//�v���C���[�摜
	//�����x��1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//�����x��2
	if ((LoadDivGraph("Image/sakana2.png", 3, 3, 1, 150, 150, sakana[1])) == -1)return-1;
	//�����x��3
	if ((LoadDivGraph("Image/sakana3.png", 3, 3, 1, 225, 225, sakana[2])) == -1)return-1;
	//�X�e�[�W�w�i
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//��O�̔w�i
	if ((Iwa[0] = LoadGraph("Image/temae4.png")) == -1) return -1;
	/*if ((Iwa[1] = LoadGraph("Image/temae2.png")) == -1) return -1;
	if ((Iwa[2] = LoadGraph("Image/temae3.png")) == -1) return -1;*/
	//�a(�H�ׂ�鐶����)�摜
	//�G�r
	if ((LoadDivGraph("Image/ebi.png", 3, 3, 1, 50, 50, feedImage[0])) == -1)return-1;
	//�A�W
	if ((LoadDivGraph("Image/azi.png", 3, 3, 1, 60, 60, feedImage[1])) == -1)return-1;
	//�C�J
	if ((LoadDivGraph("Image/ika.png", 3, 3, 1, 50, 50, feedImage[2])) == -1)return-1;

	//�G
	//�N���Q
	if ((LoadDivGraph("Image/kurage.png", 3, 3, 1, 80, 80, EnemyImage[0])) == -1)return-1;
	//�~�m�J�T�S
	if ((LoadDivGraph("Image/minokasago.png", 3, 3, 1, 120,120, EnemyImage[1])) == -1)return-1;

	//�D
	if ((LoadDivGraph("Image/�D.png", 9, 3, 1, 400, 700, ship1)) == -1)return -1;

	//��
	if ((LoadDivGraph("Image/��.png", 7, 7, 1, 400, 400, net1)) == -1)return -1;
	//Boss
	if ((LoadDivGraph("Image/maguro.png", 12, 12, 1, 350, 350, Boss1)) == -1)return -1;
	if ((LoadDivGraph("Image/maguro2.png", 6, 6, 1, 350, 350, Boss2)) == -1)return -1;
	//Boss�K�E�Z
	if ((LoadDivGraph("Image/Sonic.png", 2, 2, 1, 450, 450, Sonic)) == -1)return -1;
	//�Q�[���N���A�摜
	if ((Gameclear[0] = LoadGraph("Image/GameClear.png")) == -1)return -1;
	if ((Gameclear[1] = LoadGraph("Image/GameClear(NoEat).png")) == -1)return -1;
	if ((Gameclear[2] = LoadGraph("Image/GameClear(MaxEat).png")) == -1)return -1;
	//�Q�[���I�[�o�[�摜
	if ((Gameover = LoadGraph("Image/GameOver.png")) == -1)return -1;

	//UI�摜
	//���C�t
	if ((Life = LoadGraph("Image/Life.png")) == -1)return 0;
	//���[�^�[
	for (int i = 0; i < 3; i++) {
		if ((Meter[i][0] = LoadGraph("Image/Meter.png")) == -1)return 0;
		//�G�r���[�^�[
		if (i == 0) {
			if ((Meter[i][1] = LoadGraph("Image/MeterE1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterE2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterE3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterE4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterE5.png")) == -1)return 0;
		}
		//�A�W���[�^�[
		if (i == 1) {
			if ((Meter[i][1] = LoadGraph("Image/MeterA1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterA2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterA3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterA4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterA5.png")) == -1)return 0;
		}
		//�C�J���[�^�[
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
* BGM�̓ǂݍ���
*******************************************************************/
int LoadSound() {
	// BGM
	if ((MainSound = LoadSoundMem("Sound/�_��̐�_-_8bit.mp3")) == -1)return -1;
	if ((TitleSound = LoadSoundMem("Sound/�������ȃV���g��_-_8bit.mp3")) == -1)return -1;
	if ((BossSound = LoadSoundMem("Sound/�悼��̂Ȃ݂�.mp3")) == -1)return -1;
	if ((HelpSound = LoadSoundMem("Sound/������̃s�A�m.mp3")) == -1)return -1;
	if ((GameOverSound = LoadSoundMem("Sound/�s��_-_8bit.mp3")) == -1)return -1;

	// SE
	if ((ClearSE = LoadSoundMem("Sound/bgm_fanfare_1.mp3")) == -1)return -1;
	if ((CarsolSE = LoadSoundMem("Sound/select09.mp3")) == -1)return -1;
	if ((DesitionSE = LoadSoundMem("Sound/button67.mp3")) == -1)return -1;
	if ((EatSE = LoadSoundMem("Sound/nc78521.mp3")) == -1)return -1;
	if ((DamegeSE = LoadSoundMem("Sound/se_dosun_2.mp3")) == -1)return -1;
	if ((WallSE = LoadSoundMem("Sound/se_wall_1.mp3")) == -1)return -1;
	if ((EvoSE = LoadSoundMem("Sound/powerup10.mp3")) == -1)return -1;

	//���ʒ���
	ChangeVolumeSoundMem(125, MainSound);
	ChangeVolumeSoundMem(125, TitleSound);
	ChangeVolumeSoundMem(150, GameOverSound);
	ChangeVolumeSoundMem(200, EatSE);
	ChangeVolumeSoundMem(200, CarsolSE);
	ChangeVolumeSoundMem(200, DesitionSE);
	ChangeVolumeSoundMem(200, HelpSound);
	ChangeVolumeSoundMem(225, DamegeSE);
	ChangeVolumeSoundMem(225, BossSound);
	return 0;
}


void EatMove() {
	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == TRUE) {
			//�a�̕\��
			if (Time > 0) {
				DrawBox(eat[i].e_x, eat[i].e_y, eat[i].e_x + eat[i].e_w, eat[i].e_y + eat[i].e_h, 0xFFFFFF, FALSE);
				DrawExtendGraph(eat[i].e_x, eat[i].e_y, eat[i].e_x + eat[i].e_w, eat[i].e_y + eat[i].e_h, eat[i].image, TRUE);
			}
			//�^���������Ɉړ�
			if (Leve == 1 && key1 != 1) {
				eat[i].e_x -= 5;
			}
			if (Leve == 2 && key1 != 1) {
				eat[i].e_x -= 7;
			}
			if (Leve == 3 && key1 != 1) {
				eat[i].e_x -= 9;
			}

			if (Leve == 1) {
				if (eat[i].e_x == SCREEN_WIDTH - (eat[i].e_w / 2)+2) EatCheck(eat[i].e_x + (eat[i].e_w / 2), eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - eat[i].e_w) EatCheck(eat[i].e_x + eat[i].e_w - 1, eat[i].e_y + eat[i].e_h);
			}
			if (Leve >= 2) {
				if (eat[i].e_x == SCREEN_WIDTH - (eat[i].e_w / 2)+2) EatCheck(eat[i].e_x + (eat[i].e_w / 2) - 7, eat[i].e_y + eat[i].e_h);
				if (eat[i].e_x == SCREEN_WIDTH - eat[i].e_w+5) EatCheck(eat[i].e_x + eat[i].e_w - 7, eat[i].e_y + eat[i].e_h);
			}

			if (EatCont >= 1)
			{
				EatCont = 0;
				eat[i] = eat0;
			}

			/*DrawBox(eat[i].e_x - eat[i].e_w, eat[i].e_y + eat[i].e_h, eat[i].e_x - eat[i].e_w + 2, eat[i].e_y + eat[i].e_h + 2, 0x000000, FALSE);
			DrawBox(eat[i].e_x - (eat[i].e_w / 2), eat[i].e_y + eat[i].e_h, eat[i].e_x - (eat[i].e_w / 2) + 2, eat[i].e_y + eat[i].e_h + 2, 0x000000, FALSE);
			DrawBox(eat[i].e_x - 1, eat[i].e_y + eat[i].e_h, eat[i].e_x - 1 + 2, eat[i].e_y + eat[i].e_h + 2, 0x000000, FALSE);
			DrawBox(eat[i].e_x + (eat[i].e_w / 2), eat[i].e_y + eat[i].e_h + 1, eat[i].e_x + (eat[i].e_w / 2) + 2, eat[i].e_y + eat[i].e_h + 1, 0x000000, FALSE);
			DrawBox(eat[i].e_x, eat[i].e_y + eat[i].e_h + 1, eat[i].e_x + 2, eat[i].e_y + eat[i].e_h + 1, 0x000000, FALSE);*/

			if (eat[i].flg == FALSE)continue;

			//��ʊO�ɍs������폜
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;

			//���ɓ���������ꍇ�̏���
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
			}
			//�A�j���[�V�����𓮂���
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

	//�a�̐ݒ�
	if (Range / 5 % 50 == 0 && key1 < 1) {
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
				eat[i].e_y = (GetRand(1) + 4) * 100 + 150;
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

//���C�t
void LifeImage() {

	for (int i = 0; i < player.life; i++)
	{
		DrawGraph(LifeX + (60 * i), LIfeY, Life, TRUE);

	}
	if (player.life < 1) {
		StopSoundMem(MainSound);
		StopSoundMem(BossSound);
		GameState = 7;
		PlaySoundMem(GameOverSound, DX_PLAYTYPE_LOOP, TRUE);
	}
}

//���[�^�[�\��
void MeterImage() {

	//�G�r���[�^�[
	DrawGraph(m_x - 10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0][0], TRUE);

	//�A�W���[�^�[
	DrawGraph(m_x + 240, m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1][0], TRUE);

	//�C�J���[�^�[
	DrawGraph(m_x + 480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2][0], TRUE);

}

//����
void PlayerGrowth() {

	//�T�C�Y�̕ύX�ʂ̑���
	if (Leve == 1)Scke = 2;
	else if (Leve == 2)Scke = 1.5;

	LeveUp *= 2;

	//�v���C���[�̃T�C�Y�ύX
	player.w *= Scke;
	player.h *= Scke;
	//���x�����グ��
	Leve++;
	LifeMax++;
}

//�H�ׂ����̏���
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
		//�H�ׂ����̂�ʂ𑝉�������
		EatAmount++;
		if (player.life < LifeMax) player.life++;
	}

	//�H�ׂ����̗̂ʂ����ʂɒB�����珈�����ڂ�
	if ((EatAmount >= LeveUp)&&(Leve <3)) {
		PlayerGrowth();
		PlaySoundMem(EvoSE, DX_PLAYTYPE_BACK, TRUE);
	}
}

//�a�ƃv���C���[�̂����蔻��
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

		//�a�Ƃ̂����蔻��
		if (etypeD == FALSE) {
			if (ex + ew >= px && ex <= px + pw &&
				ey + eh >= py && ey <= py + ph) {
				e->flg = FALSE;
				PlaySoundMem(EatSE, DX_PLAYTYPE_BACK, TRUE);
				return TRUE;
			}
		}
		else {
			if (ex + ew >= phx && ex <= phx + phw &&
				ey + eh >= phy && ey <= phy + phh && player.muteki == 0) {
				player.life--;
				player.muteki = 1;
				PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
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
//�Q�[���N���A�i����������j
void GameClearHit(Player* p) {

	/*int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;


	if (Time <= 0) {
		if (1300 >= px && 1200 <= px + ph &&
			500 >= py && 400 <= py + pw) {
			GameState = 3;
			StopSoundMem(BossSound);
			PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}*/

}

void GameClear() {
	static bool push = 0; // �����ꂽ���ǂ����m�F����֐�
	//�G���f�B���O����
	if (EatAmount == 0) {
		DrawGraph(0, 0, Gameclear[1], TRUE);
	}
	else if ((em >= 5) && (im >= 5) && (am >= 5)) {
		DrawGraph(0, 0, Gameclear[2], TRUE);
	}
	else {
		DrawGraph(0, 0, Gameclear[0], TRUE);
	}

	// ���j���[�J�[�\���̕`��
	DrawRotaGraph(420 + MenuNo * 300, 750, 0.3f, 0, Corsol, TRUE);

	// ���j���[�J�[�\���ړ�����
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (++MenuNo > 1)MenuNo = 0;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (--MenuNo < 0)MenuNo = 1;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}

	// Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 1;
		}
		// sceneStage�ɍs������
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(ClearSE);
			PlaySoundMem(MainSound, DX_PLAYTYPE_LOOP, TRUE);
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(ClearSE);
			PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
}

//�{�X�X�e�[�W�ڍs�i����������j
void BossST(Player* p) {

	int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;

	if (Time <= 0) {
		DrawLine(1410, 0, 1410, 800, GetColor(255, 255, 255), FALSE);
		SetFontSize(150);
		DrawFormatString(1200, 150, GetColor(255, 0, 0), "��");
		SetFontSize(50);
		DrawFormatString(1200, 100, GetColor(255, 0, 0), "�{�X���");
		if (1410 >= px && 1410 <= px + ph &&
			800 >= py && 0 <= py + pw) {
			GameState = 4;
			StopSoundMem(MainSound);
			PlaySoundMem(BossSound, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
}

void BossInit() {

	//Boss�̏�����
	boss.flg = TRUE;
	boss.bx = BOSS_POS_X;
	boss.by = BOSS_POS_Y;
	boss.bw = BOSS_WIDTH;
	boss.bh = BOSS_HEIGHT;
	boss.speed = BOSS_SPEED;
	BOSS_PATTREN = 1;
	count = 0;

	soni.sx = SONIC_POS_X;
	soni.sy = SONIC_POS_Y;
	soni.sw = SONIC_WIDTH;
	soni.sh = SONIC_HEIGHT;


	SHIP_X = 1500;
	SHIP_Y = -100;
	SHIP_W = 350;
	SHIP_H = 350;
	SHIP_SPEED = 5;
	SHIP_COUNT = 0;
	NET = 0;
	motion_index4 = 0;

	net.nx = 1100;
	net.ny = 50;
	net.nw = 350;
	net.nh = 350;
	net.speed = 15;

	//player�̈ʒu������
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;

	//�Q�[�����C����
	GameState = 5;
}

void BossBackScrool() {
	if (GameState == 5 && key1 < 1) {
		ScroolSpeed -= player.speed;

	}

	//�X�e�[�W�摜�\��
	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//�G���A��߂�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//���x���\��
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

	if (key1 < 1) {
		if (player.muteki == 0) {
			//�����蔻��
			if (HitBoxPlayer(&player, &boss) == TRUE) {

				player.life -= 1;
				player.muteki = 1;
				PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		//�����蔻��
		if (HitBoxPlayer2(&player, &soni) == TRUE) {

			player.life -= 1;
			player.muteki = 1;
			PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
		}

	}
}
void BossMove1() {

	if (key1 < 1) {

		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);

		if (count > 0 && count < 100) {
			motion_index2 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 100;
		}

		if (count > 100 && count < 150) {
			motion_index2 = BOSSAttack[BOSS_act_index];
			boss.by = player.y - 100;
		}
		if (count > 150) {
			boss.bx -= BOSS_SPEED;
			motion_index2 = BOSSAttack[BOSS_act_index];
		}
		if (count == 398) {
			boss.bx = 1400;

		}

		if (count > 399) {
			motion_index2 = BOSSAnime[BOSS_act_index];
			if (boss.bx < 1000) {

				BOSS_SPEED = 0;
			}
			boss.by = BOSS_POS_Y;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;

		}

		if (count == 499) {
			count = 0;
			BOSS_SPEED = 8;
			BOSS_PATTREN = 2;
		}
	}
	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
}

void BossMove2() {
	if (key1 < 1) {
		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);

		if (count > 0 && count < 100) {
			motion_index2 = BOSSDown[BOSS_act_index];
			boss.bx -= BOSS_SPEED;
			boss.by += 10;
		}
		if (count == 100) {
			boss.bx = player.x - 100;
		}
		if (count > 101 && count < 399) {
			motion_index2 = BOSSUp[BOSS_act_index];
			BOSS_SPEED = 0;
			boss.by -= 10;
		}

		if (count == 399) {
			boss.bx = 1400;
			BOSS_SPEED = 10;
		}

		if (count > 399 && count < 499) {
			motion_index2 = BOSSAnime[BOSS_act_index];
			if (boss.bx < 1000) {
				BOSS_SPEED = 0;
			}
			boss.bx -= BOSS_SPEED;
			boss.by = BOSS_POS_Y;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;
		}
		if (count == 499) {
			count = 0;
			BOSS_SPEED = 10;
			BOSS_PATTREN = 3;

		}
	}

	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);

}

void BossMove3() {
	if (--BOSS_act_wait <= 0)
	{

		if (key1 < 1) {
			if (MS == 1) {
				BOSS_act_index2++;
				BOSS_act_index2 %= BOSSB_MOTION_INDEX;
			}
		}
	}
	if (key1 < 1) {
		count = (count + 1) % 900;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		if (count > 0 && count < 50) {
			BOSS_act_index2 = 0;
			MS = 0;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by < 90) {
				BOSS_SPEED = 0;
			}
			boss.by -= BOSS_SPEED;
		}
		if (count > 50 && count < 100) {
			MS = 1;
			motion_index5 = BOSSBoom[BOSS_act_index2];
		}
		if (count == 99) {
			soni.sx = 890;
			soni.sy = 120;
		}
		if (count > 100 && count < 300) {
			BOSS_act_index2 = 0;
			MS = 0;
			BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by > 300) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;
		}
		if (count > 300 && count < 350) {
			MS = 1;
			motion_index5 = BOSSBoom[BOSS_act_index2];
		}
		if (count == 349) {
			soni.sx = 890;
			soni.sy = 340;
		}
		if (count > 350 && count < 500) {
			BOSS_act_index2 = 0;
			MS = 0;
			BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by > 450) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;
		}
		if (count > 500 && count < 600) {
			MS = 1;
			motion_index5 = BOSSBoom[BOSS_act_index2];
		}
		if (count == 599) {
			soni.sx = 890;
			soni.sy = 470;
		}
		if (count > 600 && count < 800) {
			BOSS_act_index2 = 0;
			MS = 0;
			BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by < 300) {
				BOSS_SPEED = 0;
			}
			boss.by -= BOSS_SPEED;

		}

		if (count > 100 && count < 300 || count > 350 && count < 550 ||
			count > 600 && count < 800) {
			motion_index6 = BossSonic[BOSS_act_index3];
			soni.sx -= SONIC_SPEED;
			DrawExtendGraph(soni.sx, soni.sy, soni.sx + soni.sw, soni.sy + soni.sh, Sonic[motion_index6], TRUE);
		}
		if (count == 799) {
			count = 0;
			BOSS_act_index2 = 0;
			BOSS_SPEED = 10;
			BOSS_PATTREN = 1;

		}
	}
	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);
}


//�D�A�j���[�V����
void Ship() {
	SHIP_COUNT = (SHIP_COUNT + 1) % 4100;
	if (SHIP_COUNT > 4000 && SHIP_COUNT < 4100)
	{
		SHIP_X -= SHIP_SPEED;
	}
	if (SHIP_X == 1200)
	{
		SHIP_SPEED = 0;
		NET = 1;
	}
	if (--SHIP_act_wait <= 0)
	{
		if (key1 < 1) {
			SHIP_act_index++;
			SHIP_act_wait = SHIP_ANI_SPEED;
			SHIP_act_index %= SHIP_MOTION_INDEX;
		}

		motion_index3 = shipanime[SHIP_act_index];
	}
	DrawExtendGraph(SHIP_X, SHIP_Y, SHIP_X + SHIP_W, SHIP_Y + SHIP_H, ship1[motion_index3], TRUE);
}

//�ԃA�j���[�V����
void Ami() {

	if (--NET_act_wait <= 0)
	{
		if (key1 < 1) {
			NET_act_index++;
			NET_act_wait = NET_ANI_SPEED;
			NET_act_index %= NET_MOTION_INDEX;
		}

		motion_index4 = netanime[NET_act_index];
	}
	if (motion_index4 == 4) {
		//DrawBox(1200, 250, 1300, 270, GetColor(255, 255, 255), FALSE);
		if (1200 >= boss.bx && 1300 <= boss.bx + boss.bh &&
			250 >= boss.by && 270 <= boss.by + boss.bw) {
			GameState = 3;
			StopSoundMem(BossSound);
			PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (motion_index4 == 5) {
		//DrawBox(1200, 310, 1300, 330, GetColor(255, 255, 255), FALSE);
		if (1200 >= boss.bx && 1300 <= boss.bx + boss.bh &&
			310 >= boss.by && 330 <= boss.by + boss.bw) {
			GameState = 3;
			StopSoundMem(BossSound);
			PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (motion_index4 == 6) {
		//DrawBox(1200, 360, 1300, 380, GetColor(255, 255, 255), FALSE);
		if (1200 >= boss.bx && 1300 <= boss.bx + boss.bh &&
			360 >= boss.by && 380 <= boss.by + boss.bw) {
			GameState = 3;
			StopSoundMem(BossSound);
			PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	DrawExtendGraph(net.nx, net.ny, net.nx + net.nw, net.ny + net.nh, net1[motion_index4], TRUE);
}

void BossStage() {

	BossBackScrool();
	BossMove();
	PlayerMove();
	LifeImage();
	MeterImage();
	GameClearHit(&player);
	Goal();
	if (NET == 1) {
		Ami();
	}
	Ship();
	Pouse();
}
/*************************************
*���@�ƓG�@�̓����蔻��i�l�p�j
* �����FPLAYER�@�|�C���^
* �߂�l�FTRUE�F������AFALSE�F�͂���
*************************************/
int HitBoxPlayer(Player* p, Boss* b)
{
	if (Leve == 1) {
		//x,y�͒��S���W�Ƃ���
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int bx1 = b->bx - (b->bw - boss.bw - 100);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 290;
		int bw2 = by2 + b->bw - 280;

		//����m�F�p
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//�Z�a���d�Ȃ��Ă����瓖����
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 2) {
		//x,y�͒��S���W�Ƃ���
		int px = p->x - (p->w - player.w - 30);
		int py = p->y - (p->h - player.h - 70);
		int ph = px + p->h - 70;
		int pw = py + p->w - 145;

		int bx1 = b->bx - (b->bw - boss.bw - 100);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 290;
		int bw2 = by2 + b->bw - 280;

		//����m�F�p
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//�Z�a���d�Ȃ��Ă����瓖����
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}

int HitBoxPlayer2(Player* p, Soni* s)
{
	if (Leve == 1) {
		//x,y�͒��S���W�Ƃ���
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////����m�F�p
		//DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		//DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);

		//�Z�a���d�Ȃ��Ă����瓖����
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}
int ColorCheck(int x, int y) {
	/*x = player.x;
	y = player.y;*/
	Cr1 = GetPixel(x, y);
	GetColor2(Cr1, &r, &g, &b);
	//colorsum = r + g + b;

	/*if ((colorsum > 287) && (colorsum < 289) && (ColorFlg < 1) && player.muteki == 0) {
		ColorFlg = 1;
		player.life -= 1;
		player.muteki = 1;

	}*/
	if ((r == 98) && (g == 96) && (b == 94) && (ColorFlg < 1) && player.muteki == 0) {
		ColorFlg = 1;
		player.life -= 1;
		player.muteki = 1;
		PlaySoundMem(WallSE, DX_PLAYTYPE_BACK, TRUE);
	}

	if ((player.muteki == 1) && (ColorFlg == 1)) ColorFlg = 0;
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
	int SavePointX = player.x;
	int SaveSpeed = player.speed;
	int SaveIwa = Iwaspeed;
	int SaveUmi = Umispeed;
	static bool push = 0;

	if (g_KeyFlg & PAD_INPUT_8 && key1 < 1) {
		key1 = 1;
		Umispeed = SaveUmi;
		Iwaspeed = SaveIwa;
		//player.speed = 0;
		player.flg = FALSE;
		boss.flg = FALSE;
		boss.speed = FALSE;
		PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
	}
	else if (key1 > 0 && g_KeyFlg & PAD_INPUT_2) {
		//PLAYER_SPEED = 5;
		player.speed = SaveSpeed;
		player.flg = TRUE;
		boss.flg = TRUE;
		boss.speed = TRUE;
		key1 = 0;
	}

	if (key1 > 0) {
		DrawGraph(355, 100, pauseImage, TRUE);
		//player.flg = FALSE;
		// ���j���[�J�[�\���̕`��
		DrawRotaGraph(570, 390 + MenuNo * 80, 0.3f, 0, Corsol2, TRUE);

		// ���j���[�J�[�\���ړ�����
		if (g_KeyFlg & PAD_INPUT_DOWN) {
			if (++MenuNo > 2)MenuNo = 0;
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
		if (g_KeyFlg & PAD_INPUT_UP) {
			if (--MenuNo < 0)MenuNo = 2;
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}

	}if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			push = 1;
		}
		if (MenuNo == 1) {
			push = 0;
			MenuNo = 0;
			StopSoundMem(MainSound);
			StopSoundMem(BossSound);
			//key1 = 0;
			GameState = 1;
		}
		else if (MenuNo == 2) {
			push = 0;
			MenuNo = 0;
			StopSoundMem(MainSound);
			StopSoundMem(BossSound);
			GameState = 0;
		}
	}
}
void GameOver() {
	static bool push = 0; // �����ꂽ���ǂ����m�F����֐�
	DrawGraph(0, 0, Gameover, TRUE);


	// ���j���[�J�[�\���̕`��
	if (MenuNo != 1) {
		DrawRotaGraph(80 + MenuNo * 450, 743, 0.3f, 0, Corsol, TRUE);
	}
	else {
		DrawRotaGraph(560, 743, 0.3f, 0, Corsol, TRUE);
	}

	// ���j���[�J�[�\���ړ�����
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 2)MenuNo = 0;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 2;
		PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
	}

	// Z�L�[�Ń��j���[�I��
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 1;
		}
		// sceneStage�ɍs������
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(GameOverSound);
			PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(GameOverSound);
			PlaySoundMem(TitleSound, DX_PLAYTYPE_BACK, TRUE);
		}
		else {
			push = 0;
			DxLib_End();
		}
	}
}