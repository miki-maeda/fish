#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
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

int LoadImages();          //�摜�ǂݍ���

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
		ScroolSpeed -= player.speed;

	}

	//�X�e�[�W�摜�\��

	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//����
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);*/
	DrawGraph(ScroolSpeed % 640, 0, StageImage, TRUE);
	DrawGraph(640 + (ScroolSpeed % 640), 0, StageImage, TRUE);
	//�ݒ�����ɖ߂��B
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�G���A��߂�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//��摜�\��

	//�`��\�G���A��ݒ�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	static int count = 0;
	count = (count + 1) % 2541;
	

	if (count > 0 && count < 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[0], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[0], TRUE);
	}
	if (count > 139 && count < 694 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[0], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[0], TRUE);
	}
	if (count > 847 && count < 986) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[1], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[1], TRUE);
	}
	if (count > 986 && count < 1541 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[1], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[1], TRUE);
	}
	if (count > 1694 && count < 1833) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[2], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[2], TRUE);
	}
	if (count > 1833 && count < 2388 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[2], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[2], TRUE);
	}

	//�G���A��߂�
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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

	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	int motion_index = anime[act_index];
	/*DrawGraph(player.x, player.y, sakana[motion_index], TRUE);*/
	DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	/*ScreenFlip();*/

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
	if ((Iwa[0] = LoadGraph("Image/temae.png")) == -1) return -1;
	if ((Iwa[1] = LoadGraph("Image/temae2.png")) == -1) return -1;
	if ((Iwa[2] = LoadGraph("Image/temae3.png")) == -1) return -1;
	//�a(�H�ׂ�鐶����)�摜
	//�G�r
	if ((feedImage[0] = LoadGraph("Image/ebi.png")) == -1)return 0;
	//�A�W
	if ((feedImage[1] = LoadGraph("Image/azi.png")) == -1)return 0;
	//�C�J
	if ((feedImage[2] = LoadGraph("Image/ika.png")) == -1)return 0;

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
			DrawGraph(eat[i].e_x, eat[i].e_y, eat[i].image, TRUE);

			//�^���������Ɉړ�
			eat[i].e_x -= 10;

			if (eat[i].flg == FALSE)continue;

			//��ʊO�ɍs������폜
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;

			//���ɓ���������ꍇ�̏���
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
				eat[i].flg = FALSE;
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
			eat[i].type = GetRand(2);
			eat[i].image = feedImage[eat[i].type];
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
			}
			return TRUE;
		}
	}

	return FALSE;
}

//���C�t
void LifeImage() {

	DrawGraph(LifeX, LIfeY, Life, TRUE);
	DrawGraph(LifeX + 60, LIfeY, Life, TRUE);
	DrawGraph(LifeX + 120, LIfeY, Life, TRUE);
}

//���[�^�[�\��
void MeterImage() {

	//�G�r���[�^�[
	DrawGraph(m_x-10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0], TRUE);

	//�A�W���[�^�[
	DrawGraph(m_x+240 , m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1], TRUE);

	//�C�J���[�^�[
	DrawGraph(m_x+480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2], TRUE);

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

	//�H�ׂ����̂�ʂ𑝉�������
	EatAmount++;

	//�H�ׂ����̗̂ʂ����ʂɒB�����珈�����ڂ�
	if (EatAmount == LeveUp) {
		PlayerGrowth();
	}
}

//�a�ƃv���C���[�̂����蔻��
int Hit(Player* p, Eat* e) {

	int px = p->x + 30 * Scke;
	int py = p->y + 45 * Scke;
	int ph = p->h - 40 * Scke;
	int pw = p->w - 80 * Scke;
	int ex = e->e_x;
	int ey = e->e_y;
	int et = e->type;

	//�a�Ƃ̂����蔻�蔻��
	if (et == 0) {
		if (ex + 40 >= px && ex <= px + ph &&
			ey + 40 >= py && ey <= py + pw) {
			e->flg = FALSE;
			return TRUE;
		}
	}
	else {
		if (ex + 50 >= px && ex <= px + ph &&
			ey + 50 >= py && ey <= py + pw) {
			e->flg = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}
