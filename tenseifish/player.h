#pragma once

void PlayerMove();//�v���C���[�̓��쏈��
void LifeImage();
void MeterImage();

//�v���C���[�̂̏����l
int PLAYER_POS_X = 100;
int PLAYER_POS_Y = 300;
int PLAYER_WIDTH = 100;
int PLAYER_HEIGHT = 100;
int PLAYER_SPEED = 6;

//�A�j���[�V�����̑���
int ACT_SPEED = 5;
int act_wait = ACT_SPEED;

//���[�V�����̏����l
int act_index = 0;
//���[�V�����̐�
int MAX_MOTION_INDEX = 4;	// 0,1,2,1 ��4��� 

int Range;
int key1;
int key2;

//���C�t�ʒu
int LifeX = 150;
int LIfeY = 10;
int LifeMax = 3;

//Lv���[�^�[�ʒu
int m_x = 50;
int m_y = 50;

struct Player//�v���C���[�̃N���X
{
	int flg = true;
	int x = 0, y = 0, w = 0, h = 0;
	int speed = 0;
	int life = 0;
	int muteki = 0;
};
struct Player player;

void GameClearHit();
void BossST(Player* P);//�{�X�X�e�[�W�ڍs


//�L�[�擾�p�z��
extern char key[256];

int deianime[] = { 0,1,2,3,4,5,6 };
int dei1[7] = { 0 };
int dei2[7] = { 0 };
int dei3[7] = { 0 };
int DEI_ANI_SPEED = 5;
int DEI_act_wait = DEI_ANI_SPEED;
int DEI_MOTION_INDEX = 7;
int DEI_act_index = 0;
int motion_index7;


