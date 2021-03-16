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
	EatImage();
	Hit();
}
/*************************************
 *�w�i�摜�X�N���[������
 *�����F�Ȃ�
 *�߂�l�F�Ȃ�
 *************************************/
void BackScrool()
{
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

	////�`��\�G���A��ݒ�
	//SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 40, Iwa, TRUE);
	//DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 40, Iwa, TRUE);

	////�G���A��߂�
	//SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetFontSize(30);
	//DrawFormatString(0, 0, 0x000000, "Level = %d", Leve);
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


	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	int motion_index = anime[act_index];
	/*DrawGraph(player.x, player.y, sakana[motion_index], TRUE);*/
	//DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	/*ScreenFlip();*/

}

int LoadImages() {
	//�v���C���[�摜
	//�����x��1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//�����x��2
	if ((LoadDivGraph("Image/sakana2.png", 9, 10, 1, 100, 100, sakana[1])) == -1)return-1;
	//�X�e�[�W�w�i
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//��O�̔w�i
	if ((Iwa = LoadGraph("Image/��O�̔w�i2.png")) == -1) return -1;
	//�a(�H�ׂ�鐶����)�摜
	//�G�r
	if ((feedImage[0] = LoadGraph("Image/ebi.png")) == -1)return 0;
	//�A�W
	if ((feedImage[1] = LoadGraph("Image/azi.png")) == -1)return 0;
	//�C�J
	if ((feedImage[2] = LoadGraph("Image/ika.png")) == -1)return 0;

	return 0;
}

void EatImage() {

	/*for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {

		}
	}*/

	DrawGraph(e_x + ((ScroolSpeed * 2) % SCREEN_WIDTH), e_y, feedImage[0], FALSE);
	//�X�N���[�����Ă���Ԃ̍��W���i�[���Ă���
	e_xs = e_x + ((ScroolSpeed * 2) % SCREEN_WIDTH);

	/*DrawFormatString(100,100,0xffffff,"x %d y %d",e_xs,e_ys);*/
}

//����
void PlayerGrowth() {

	//�v���C���[�̃T�C�Y�ύX
	player.w *= Scke;
	player.h *= Scke;

	//�T�C�Y�̕ύX�ʂ̑���
	Scke++;
	//���x�����グ��
	Leve++;
}

//�Q�[�W
void PlayerEat() {

	//�H�ׂ����̂�ʂ𑝉�������
	EatAmount++;

	//�H�ׂ����̗̂ʂ����ʂɒB�����珈�����ڂ�
	if (EatAmount <= LeveUp) {
		PlayerGrowth();
	}
}

void Hit() {

	//���ɓ���������ꍇ�̏���
	if (e_xs + 40 >= player.x && e_xs <= player.x + 30 &&
		e_y + 40 >= player.y && e_y <= player.y + 30) {
		feedImage[0] = 0;
		PlayerEat();
	}

}
