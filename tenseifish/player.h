#pragma once

void PlayerMove();
//���@�̏����l
int PLAYER_POS_X =100;
int PLAYER_POS_Y =100;
int PLAYER_WIDTH = 30;
int PLAYER_HEIGHT = 30;
int PLAYER_SPEED = 5;

int ACT_SPEED = 20;
int act_wait = ACT_SPEED;

int act_index = 0;
int MAX_MOTION_INDEX = 4;	// 0,1,2,1 ��4��� 



struct Player//�v���C���[
{
	int flg = TRUE;
	int x=0, y=0, w=0, h=0;
	int speed=0;
};
//���@
struct Player player;

//�L�[�擾�p�z��
extern char key[256];
