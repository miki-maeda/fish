#pragma once

//�a�ȂǂɊւ���֐�
int EatImage();		//�H�ׂ鐶���̕\���Ə���
void EatMove(void);			//��(�H�ו�)�̈ړ�����
void PlayerGrowth(void);	//�v���C���[����������(���x�����オ����)���̏���
void PlayerEat(int* e);		//�v���C���[���a��H�ׂ��Ƃ�

//�ϐ�
int Leve = 1;			//���x��
int Scke = 1;			//�傫���Ȃ��
int LeveUp = 10;			//���x�����オ��̂ɕK�v�ȗ�
int EatAmount;		//�H�ׂ����̗̂�(���v)
int em, am, im;
int MoveSpeed = 50;		//�A�j���[�V�������ς��X�s�[�h
int MoveEat = MoveSpeed;	//�A�j���[�V�������ς��܂ł̎���
int CountEat = 0;		//�J�E���g����
int Eatflg = 0;
int EatCont = 0;
int er = 0, eg = 0, eb = 0;


struct Eat {

	int flg;			//�g�p�t���O
	int	type;			//�^�C�v
	int image;			//�\������a
	int e_x, e_y;		//�a�̍��W
	float e_w, e_h;		//�����A�c��
	int typeD;			//�o����������(FALSE:�a,TRUE:�G)
};

struct Eat eat[10];				//�a�̕\�����鐔
struct Eat eat0 = { TRUE,0,0,1410,0,0,0,FALSE };

int Hit(Player* p, Eat* e);				//�G�Ƃ̂����蔻��
int EatCheck(int x, int y);