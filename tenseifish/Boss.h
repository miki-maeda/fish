#pragma once
void BossInit();
void BossMove();
void BossStage();
void BossBackScrool();

//Boss�̏����l
int BOSS_POS_X = 1000;
int BOSS_POS_Y = 300;
int BOSS_WIDTH = 350;
int BOSS_HEIGHT = 350;
int BOSS_SPEED = 5;

int Boss1[6] = { 0 };		//�{�X�̉摜���i�[����z��
int BOSSanime[] = { 0,1,0,2 };		//�j���Ƃ��̃A�j���[�V�����̔ԍ� 
int BOSSAttack[] = { 3,4,3,5 };		//�ːi����Ƃ��̃A�j���[�V�����̔ԍ� 

int BOSS_ACT_SPEED = 3;
int BOSS_act_wait = BOSS_ACT_SPEED;

int BOSS_act_index = 0;

int motion_index2;

int BOSS_MOTION_INDEX = 4;	// 0,1,2,1 ��4��� 
int BOSSA_MOTION_INDEX = 4;	// 3,4,3,5 ��4��� 
//�D
int shipanime[] = { 0,1,0,2 };      //�D�̃A�j���[�V����
int ship1[9]{ 0 };
int motion_index3;
int SHIP_X = 1000;
int SHIP_Y = -40;
int SHIP_W = 350;
int SHIP_H = 350;
int SHIP_ANI_SPEED = 15;
int SHIP_act_wait = SHIP_ANI_SPEED;
int SHIP_act_index = 0;
int SHIP_MOTION_INDEX = 4;

//��
int netanime[] = { 0,1,2,3,4,5 };
int net1[6] = { 0 };
int motion_index4;
int NET_MOTION_INDEX = 6;
int NET_ANI_SPEED = 20;
int NET_act_wait = NET_ANI_SPEED;
int NET_act_index = 0;

struct Net //��
{

	int flg = true;
	int nx = 0, ny = 0, nw = 0, nh = 0;
	int speed = 0;
};
static Net net;

struct Boss//�v���C���[
{
	int flg = true;
	int bx = 0, by = 0, bw = 0, bh = 0;
	int speed = 0;
};
//���@
struct Boss boss;
int HitBoxPlayer(Player* p, Boss* b);