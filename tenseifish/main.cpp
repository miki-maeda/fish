#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
#include "Boss.h"
int	g_OldKey;				// �O��̓��̓L�[
int	g_NowKey;				// ����̓��̓L�[
int	g_KeyFlg;				// ���̓L�[���
char key[256];

int GameState = 0;          //�������@

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 1410;
const int SCREEN_HEIGHT = 800;

int ScroolSpeed;

void GameInit();		//�Q�[������������
void GameMain();		//�Q�[�����C������
void BackScrool();         //�w�i�摜�X�N���[������
void GameClear();		//�Q�[���N���A����
void Goal();

int LoadImages();          //�摜�ǂݍ���

int Time = 2400;
//int hk = LoadGraph("Image/temae4.png");

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

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 && GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		Time--;

		//���̓L�[�擾
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// ��ʂ̏�����

		switch (GameState) {
		case 0:
			GameInit();		//�Q�[����������
			break;
		case 1:
			GameMain();		//�Q�[�����C������
			break;
		case 2:
			GameClear();
			break;
		case 3:
			BossInit();
			break;
		case 4:
			BossStage();
			break;
		}
		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f
	}

	DxLib_End();	// DX���C�u�����g�p�̏I������
	return 0;	// �\�t�g�̏I��
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
	//�v���C���[�̏�����
	player.flg = TRUE;
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGHT;
	player.speed = PLAYER_SPEED;
	player.life = LifeMax;
	player.muteki = 0;

	//�a�̏�����
	for (int i = 0; i < 10; i++) {
		eat[i].flg = FALSE;
	}
	//�Q�[�����C����
	GameState = 1;
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

}
/*************************************
 *�w�i�摜�X�N���[������
 *�����F�Ȃ�
 *�߂�l�F�Ȃ�
 *************************************/
void BackScrool()
{

	Range += player.speed;

	if (GameState == 1) {
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
	/*DrawFormatString(100, 160, 0x000000, "Scke = %f", Scke);*/
}

void PlayerMove() {

	if (--act_wait <= 0)
	{
		act_index++;
		act_wait = ACT_SPEED;
		act_index %= MAX_MOTION_INDEX;
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
	if (player.y < player.h/Scke)player.y = player.h/Scke;
	if (player.y > SCREEN_HEIGHT - player.h)player.y = SCREEN_HEIGHT - player.h;

	if (Leve == 1) {
		Umispeed -= 2;
		Iwaspeed -= 5;
	}
	if (Leve == 2) {
		Umispeed -= 4;
		Iwaspeed -= 7;
		player.speed = 8;
	}

	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	int motion_index = anime[act_index];
	if (player.muteki == 0) {
		DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	}
	if (player.muteki == 1) {

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
}

int LoadImages() {
	//�v���C���[�摜
	//�����x��1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//�����x��2
	/*if ((LoadDivGraph("Image/sakana2.png", 9, 10, 1, 100, 100, sakana[1])) == -1)return-1;*/
	//�����x��2
	if ((LoadDivGraph("Image/sakana2.png", 3, 3, 1, 150, 150, sakana[1])) == -1)return-1;
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
	//�n���Z���{��
	if ((LoadDivGraph("Image/harisennbon.png", 3, 3, 1, 80, 80, EnemyImage[1])) == -1)return-1;
	
	//Boss
	if ((LoadDivGraph("Image/rasubosu.png", 6, 6, 1, 350, 350, Boss1)) == -1)return -1;
	//�Q�[���N���A�摜
	if ((Gameclear = LoadGraph("Image/GameClear.png")) == -1)return -1;

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

void EatMove() {
	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == TRUE) {
			//�a�̕\��
			if (Time > 0) {
				DrawExtendGraph(eat[i].e_x, eat[i].e_y, eat[i].e_x + eat[i].e_w, eat[i].e_y + eat[i].e_h, eat[i].image, TRUE);
			}
			//�^���������Ɉړ�
			if(Leve==1){
			eat[i].e_x -= 5;
            }
			if (Leve == 2) {
			eat[i].e_x -= 7;
			}

			if (eat[i].flg == FALSE)continue;

			//��ʊO�ɍs������폜
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;

			//���ɓ���������ꍇ�̏���
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
			}
			//�A�j���[�V�����𓮂���
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

	//�a�̐ݒ�
	if (Range / 5 % 50 == 0) {
		EatImage();
	}
}

int EatImage() {

	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {
			eat[i] = eat0;
			eat[i].type = GetRand(4);
			if (eat[i].type <= 2) {
				eat[i].image = feedImage[eat[i].type][0];
			}
			else {
				eat[i].image = EnemyImage[eat[i].type - 3][0];
			}
			switch (eat[i].type) {
			case 0:
				eat[i].e_y = (GetRand(1) + 4) * 100 + 150;
				eat[i].e_w = 50 * 1.5;
				eat[i].e_h = 50 * 1.5;
				break;
			case 1:
				eat[i].e_y = GetRand(2) * 100 + 150;
				eat[i].e_w = 60 * 1.5;
				eat[i].e_h = 60 * 1.5;
				break;
			case 2:
				eat[i].e_y = GetRand(5) * 100 + 150;
				eat[i].e_w = 50 * 1.5;
				eat[i].e_h = 50 * 1.5;
				break;
			case 3:
				eat[i].e_y = GetRand(1) * 100 + 150;
				eat[i].e_w = 50 * 1.5;
				eat[i].e_h = 50 * 1.5;
				eat[i].typeD = TRUE;
				break;
			case 4:
				eat[i].e_y = GetRand(3) * 100 + 150;
				eat[i].e_w = 50 * 1.5;
				eat[i].e_h = 50 * 1.5;
				eat[i].typeD = TRUE;
				break;
			}
			return TRUE;
		}
	}

	return FALSE;
}

//���C�t
void LifeImage() {

	for (int i = 0; i < player.life; i++)
	{
		DrawGraph(LifeX+(60*i), LIfeY, Life, TRUE);

	}
}

//���[�^�[�\��
void MeterImage() {

	//�G�r���[�^�[
	DrawGraph(m_x-10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0][0], TRUE);

	//�A�W���[�^�[
	DrawGraph(m_x+240 , m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1][0], TRUE);

	//�C�J���[�^�[
	DrawGraph(m_x+480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2][0], TRUE);

}

//����
void PlayerGrowth() {

	//�T�C�Y�̕ύX�ʂ̑���
	Scke++;

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
	if (EatAmount == LeveUp) {
		PlayerGrowth();
	}
}

//�a�ƃv���C���[�̂����蔻��
int Hit(Player* p, Eat* e) {

	if (Time > 0) {
		int px = p->x + 30 * Scke;
		int py = p->y + 45 * Scke;
		int ph = p->h - 40 * Scke;
		int pw = p->w - 80 * Scke;
		int ex = e->e_x;
		int ey = e->e_y;
		int ew = e->e_w;
		int eh = e->e_h;
		int etypeD = e->typeD;

		//�a�Ƃ̂����蔻��
		if (etypeD == FALSE) {
			if (ex + ew >= px && ex <= px + pw &&
				ey + eh >= py && ey <= py + ph) {
				e->flg = FALSE;
				return TRUE;
			}
		}
		else {
			if (ex + ew >= px && ex <= px + pw &&
				ey + eh >= py && ey <= py + ph) {
				player.life--;
				e->flg = FALSE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void Goal() {
	if (Time <= 0) {
		DeleteGraph(Iwa[0]);
		DrawBox(1200, 400, 1300, 500, GetColor(255, 212, 0), FALSE);
	}
}
//�Q�[���N���A�i����������j
void GameClearHit(Player* p) {


	int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;


	if(Time <= 0){
		if (1300 >= px && 1200 <= px + ph &&
			500 >= py && 400 <= py + pw) {
			GameState = 2;
		}
	}

}

void GameClear() {

	DrawGraph(0,0, Gameclear, TRUE);

}

//�{�X�X�e�[�W�ڍs�i����������j
void BossST(Player* p) {


	int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;

	if (Time <= 0) {
		DeleteGraph(Iwa[0]);
		DrawBox(1200, 400, 1300, 500, GetColor(255, 255, 255), FALSE);

		if (1300 >= px && 1200 <= px + ph &&
			500 >= py && 400 <= py + pw) {
			GameState = 3;

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

	//player�̈ʒu������
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;

	//�Q�[�����C����
	GameState = 4;
}

void BossBackScrool() {
	if (GameState == 4) {
		ScroolSpeed -= player.speed;

	}

	//�X�e�[�W�摜�\��
	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed% SCREEN_WIDTH), 0, StageImage, TRUE);
	//�G���A��߂�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//���x���\��
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
}

void BossMove() {
	if (--BOSS_act_wait <= 0)
	{
		BOSS_act_index++;
		BOSS_act_wait = BOSS_ACT_SPEED;
		BOSS_act_index %= BOSS_MOTION_INDEX;
	}


	// �_���[�W������ƂT��̂����Q��\������B
	static int count = 0;
	count = (count + 1) % 500;
	//DrawFormatString(100, 160, 0x000000, "%d", count);

	if (count > 0) {
		motion_index2 = BOSSanime[BOSS_act_index];
	}
	if (count > 100) {
		motion_index2 = BOSSAttack[BOSS_act_index];
		boss.bx -= 5;

	}

	if (count == 399) {
		motion_index2 = BOSSanime[BOSS_act_index];
		boss.bx = BOSS_POS_X;
		boss.by = BOSS_POS_Y;
		boss.bw = BOSS_WIDTH;
		boss.bh = BOSS_HEIGHT;
		count = 0;
	}

	if (player.muteki == 0) {
		//�����蔻��
		if (HitBoxPlayer(&player, &boss) == TRUE) {

			player.life -= 1;
			player.muteki = 1;
		}
	}
	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
}

void BossStage() {
	
	BossBackScrool();
	BossMove();
	PlayerMove();
	LifeImage();
	MeterImage(); 
	GameClearHit(&player);
	Goal();

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
		int px = p->x - (p->w - player.w + 10);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + p->h;
		int pw = py + p->w - 50;

		int bx1 = b->bx - (b->bw - boss.bw - 50);
		int by2 = b->by - (b->bh - boss.bh - 95);
		int bh1 = bx1 + b->bh - 55;
		int bw2 = by2 + b->bw - 185;

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

		int bx1 = b->bx - (b->bw - boss.bw - 50);
		int by2 = b->by - (b->bh - boss.bh - 95);
		int bh1 = bx1 + b->bh - 55;
		int bw2 = by2 + b->bw - 185;

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