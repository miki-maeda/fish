#pragma once

void PlayerMove();
void LifeImage();
void MeterImage();

//���@�̏����l
int PLAYER_POS_X = 100;
int PLAYER_POS_Y = 300;
int PLAYER_WIDTH = 100;
int PLAYER_HEIGHT = 100;
int PLAYER_SPEED = 5;

//int ACT_SPEED = 20;
int ACT_SPEED = 3;
int act_wait = ACT_SPEED;

int act_index = 0;
int MAX_MOTION_INDEX = 4;	// 0,1,2,1 ��4��� 

int Range;

//���C�t�ʒu
int LifeX = 150;
int LIfeY = 10;

//Lv���[�^�[�ʒu
int m_x = 50;
int m_y = 50;

struct Player//�v���C���[
{
	int flg = true;
	int x = 0, y = 0, w = 0, h = 0;
	int speed = 0;
	int life = 0;
	int muteki = 0;
};
void GameClearHit(Player* P);
void BossST(Player* P);
//���@
struct Player player;

//�L�[�擾�p�z��
extern char key[256];
