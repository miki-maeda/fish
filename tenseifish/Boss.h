#pragma once
void BossInit();//BOSS�̏�����
void BossMove();//BOSS�̓����̃p�^�[��
void BossMove1();//BOSS�̓����p�^�[���P
void BossMove2();//�p�^�[���Q
void BossMove3();//�p�^�[���R
void BossStage();//BOSS�̃X�e�[�W
void BossBackScrool();//BOSS�X�e�[�W�̃X�N���[��
void BossMeter();
void BossMovie();//�{�X��ɍs���O�̃��[�r�[����

void Ami();//�Ԃ̏���
void AmiLs();	// ���]��
void Ship();//�D�̏���
void ShipLs();	// ���]��
void BHA();//BOSS�̕ߊl�A�j���[�V��������
void BHALs();	// ���]��

void Sibuki();


//Boss�̏����l
int BOSS_POS_X = -400;
int BOSS_POS_Y = 200;
int BOSS_WIDTH = 450;
int BOSS_HEIGHT = 450;
int BOSS_SPEED = 20;

int Keikoku;        //�{�X�̌x��
int Messege;

//�Ռ��g�̏����l
int SONIC_POS_X = 1400;
int SONIC_POS_Y = 130;
int SONIC_WIDTH = 350;
int SONIC_HEIGHT = 350;
int SONIC_SPEED_Lev1 = 20;
int SONIC_SPEED_Lev2 = 25;
int SONIC_SPEED_Lev3 = 30;


int Boss1[13] = { 0 };		//�{�X�̉摜���i�[����z��
int Boss2[13] = { 0 };		//�{�X�̉摜���i�[����z��
int Boss3[5] = { 0 };		//�{�X�̉摜���i�[����z��
int Sonic[3] = { 0 };      //�{�X�̕K�E�Z���i�[����z��
int Hokaku[5] = { 0 };    //�{�X�̕ߊl���i�[����z��
int Hokaku2[5] = { 0 };	  //�{�X�̕ߊl���i�[����z��i���]�j

//�{�X��O�̃��[�r�[�̏�����
bool BRflg;
int BRMove;
int MovieCount;
int MY;
int MY2;


//�A�j���[�V�����̔ԍ�
int BOSSBoom[] = { 2,2,1,1,0,0,5,6,5,4,5,6 };		//�K�E�Z����Ƃ��̃A�j���[�V�����̔ԍ� 
int BOSSAnime[] = { 11,10,11,9 };		//�j���Ƃ��̃A�j���[�V�����̔ԍ� 
int BOSSAttack[] = { 8,7,8,6 };		//�ːi����Ƃ��̃A�j���[�V�����̔ԍ� 
int BOSSDown[] = { 5,4,5,3 };         //���~����Ƃ��̃A�j���[�V�����̔ԍ� 
int BOSSUp[] = { 2,1,2,0 };         //�㏸����Ƃ��̃A�j���[�V�����̔ԍ� 
int BossSonic[] = { 0,1 };         //�K�E�Z�̃A�j���[�V�����ԍ�
int BossHokaku[] = { 0,1,2,3 };         //�ߊl�̃A�j���[�V�����ԍ�
int BossKoutai[] = { 0,1,0,2 };         //��ނ̃A�j���[�V�����ԍ�

//�ߊl�A�j���[�V�����̃X�s�[�h�̏�����
int HOKAKU_act_index = 0;
int HOKAKU_SPEED = 20;
int HOKAKU_act_wait = HOKAKU_SPEED;

//BOSS�A�j���[�V�����̑����̏�����
int BOSS_ACT_SPEED = 3;
int BOSS_act_wait = BOSS_ACT_SPEED;
int BOSS_ACT_SPEED2 = 10;
int BOSS_act_wait2 = BOSS_ACT_SPEED2;
int BOSS_act_index = 0;
int BOSS_act_index2 = 0;
int BOSS_act_index3 = 0;

//���[�V�����ԍ����i�[����ϐ�
int motion_index2;
int motion_index5;
int motion_index6;
int motion_index8;

int BOSS_PATTREN;   //BOSS�p�^�[�����i�[����ϐ�
int count;         //BOSS�̎��̓����܂ł𐔂���ϐ�
int MS = 0;        //�{�X��̃p�^�[��3�ɐ�����݂���ϐ�
int sc;
int cr;
int cr2;

//���[�V�����̐�
int BOSS_MOTION_INDEX = 4;
int BOSSB_MOTION_INDEX = 12;	
int SONIC_MOTION_INDEX = 2;	
int HOKAKU_MOTION_INDEX = 4;	// 4�R�}


int sibukianime[] = { 0,1,2,3,4,5,6,7 };
int sibuki[8] = { 0 };
int sibuki_ANI_SPEED = 10;
int sibuki_act_wait = sibuki_ANI_SPEED;
int sibuki_act_index = 0;
int sibuki_MOTION_INDEX = 8;
int motion_index9;
int SIBUKI_Y = 5;
int SIBUKI_X;

int shipanime[] = { 0,1,0,2 };      //�D�̃A�j���[�V�����̔ԍ�
int ship1[9]{ 0 };                 //�D�̉摜���i�[����z��
int ship2[9]{ 0 };                //�D�̉摜���i�[����z��
int motion_index3;

//�D�̏�����
int SHIP_X = 1400;
int SHIP_Y = -40;
int SHIP_W = 350;
int SHIP_H = 350;
int SHIP_lX = 100;
int SHIP_SPEED = 5;
int SHIPFlg = FALSE;

//�D�̎��̓����܂ł𐔂���ϐ�
int SHIP_COUNT = 0;
int SHIP_COUNT2 = 0;

//�D�̃A�j���[�V�����̑����̏�����
int SHIP_ANI_SPEED = 15;
int SHIP_act_wait = SHIP_ANI_SPEED;
int SHIP_act_index = 0;
//���[�V�����̐�
int SHIP_MOTION_INDEX = 4;



//��
int netanime[] = { 0,1,2,3,4,5,6 };
int net1[7] = { 0 };
int net2[7] = { 0 };
int motion_index4;

//�Ԃ̃A�j���[�V�����̑����̏�����
int NET_MOTION_INDEX = 7;
int NET_ANI_SPEED = 17;
int NET_act_wait = NET_ANI_SPEED;
int NET_act_index = 0;
int NET = 0;
int netflg = 0;
int counth = 0;
int countS = 0;

//boss��̃��[�^�[
float boss_Y = 923;
float bm = 0;
struct Net //�Ԃ̃N���X
{

	int flg = true;
	int nx = 0, lnx = 0, ny = 0, nw = 0, nh = 0;
	int speed = 0;
};
static Net net;

struct Boss//BOSS�̃N���X
{
	int flg = true;
	int bx = 0, by = 0, bw = 0, bh = 0;
	int speed = 0;
	int dir = 0;
};
struct Boss boss;

struct Soni//BOSS�K�E�Z�̃N���X
{
	int flg = true;
	int sx = 0, sy = 0, sw = 0, sh = 0;
	int speed = 0;
};
struct Soni soni;

//�v���C���[�ƃ{�X�̓����蔻��
int HitBoxPlayer(Player* p, Boss* b);
//�v���C���[�ƃ{�X�̕K�E�Z�̓����蔻��
int HitBoxPlayer2(Player* p, Soni* s);