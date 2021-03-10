#include "DxLib.h"
#include "player.h"
#include "image.h"

int	g_OldKey;				// �O��̓��̓L�[
int	g_NowKey;				// ����̓��̓L�[
int	g_KeyFlg;				// ���̓L�[���
char key[256];

int GameState = 0;          //�������@

//��ʗ̈�̑傫��
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

	if (DxLib_Init() == -1) return -1;	// DX���C�u�����̏���������

	SetDrawScreen(DX_SCREEN_BACK);	// �`����ʂ𗠂ɂ���

	if (LoadImages() == -1)return -1; //�摜�Ăяo��

	//�Q�[�����[�v
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 &&GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

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
	//�Q�[�����C����
	GameState = 1;

}

/***********************************************
/*�Q�[�����C���̏���*/
/***********************************************/
void GameMain() {
	BackScrool();
    PlayerMove();
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
	SetDrawArea(0, 0,640, 480);
	DrawGraph(ScroolSpeed % 640, 0, StageImage, FALSE);
	DrawGraph(640 + (ScroolSpeed % 640), 0, StageImage, FALSE);
	
	//�G���A��߂�
	SetDrawArea(0,0,640,480);

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

	
	act_index++;
	act_index %= MAX_MOTION_INDEX;
	int motion_index = anime[act_index];
	DrawGraph(player.x, player.y,sakana[motion_index],TRUE);
	ScreenFlip();
	
}
int LoadImages() {
	//�v���C���[�摜
	if((LoadDivGraph("Image/sakana.png", 10,10,1,30,30,sakana))==-1)return -1;
	//�X�e�[�W�w�i
	if ((StageImage = LoadGraph("Image/bg_natural_ocean.jpg")) == -1) return -1;
	return 0;
}