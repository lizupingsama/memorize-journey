#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)
#pragma warning(disable:6385)
#pragma warning(disable:6054)
#pragma warning(disable:28182)
#pragma warning(disable:6011)
#pragma warning(disable:6308)
#pragma warning(disable:28183)
#pragma warning(disable:6387)
#pragma warning(disable:4244)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

FILE* fp1, * fp2, * fp3, * fp4;


int NumOfPlay;
int first_game_flag;
int endflag;
int game_again_flag = 0;

int storage_no;

int flag_draw;
int flag_dialogue;
int flag_A;
int flag_D;
int flag_space;
int flag_R;
int flag_B;

int x;//�����ƶ�ͼƬλ�ò���
int y;//�����ƶ�ͼƬλ�ò���


int old_bk_x;
int old_bk_y;
int bk_x;
int bk_y;

int NumOfOldPosition;
int* old_x;
int* old_y;
int* old_speed_y;
int* TIME;
int level_num;//��ǰ�ؿ���

IMAGE people_right, people_left, people_right_2, people_left_2, bk, bk2;
IMAGE play1, play2, title1, title2, play, pile_1, pile_2, pile3, pile4;
IMAGE back_a_1, back_a_2, back_b_1, back_b_2, back_c_1, back_c_2, back_d_1, back_d_2, back_e_1, back_e_2, back_f_1, back_f_2;
IMAGE back_menu1, back_menu2, gameover1, gameover2,������;
IMAGE back_a2_1, back_a2_2, back_b2_1, back_b2_2, back_c2_1, back_c2_2, back_d2_1, back_d2_2, back_e2_1, back_e2_2, back_f2_1, back_f2_2;
IMAGE FLASH, back_track;

ExMessage m2;

SYSTEMTIME t;//����ȫ�ּ�ʱ������¼λ��ר��
SYSTEMTIME t3,t4;//����
struct tm t1;
time_t now1;
struct tm t2;
time_t now2;//��¼����ǰλ��ר�ã�����������
int msc1;
int msc2;
struct tm dialogue_timer[2];//̨�ʲ��ż�ʱ��
struct tm pile_x_timer[2];//�ڵ���ʱ��
struct tm pile_y_timer[2];
struct tm flash_timer[2];//����CD��ʱ��
int flash_cd;
struct tm back_track_timer[2];//����CD��ʱ��
int back_track_cd;

bool mouse;

int flags;
int count;
int flagm;
int bk_flag;

//����CD����
int flash_flag;
int back_track_flag;

#define NumOfWall 14
#define NumOfTrap 4
#define NumOfLevel 2
#define NumOfBack 6

void move();
void HitDet();
void BackTrack();
void ChangeLevel(int);
void flash();
void dialogue_play();
void start_menu();
void take_back();
void back_menu();
void GameDraw();
void first_game();
void storage();
void back_storage_position();
void EndGame();
void end_menu();
int main();

DWORD WINAPI musicplay(LPVOID lpparamer);
DWORD WINAPI Star(LPVOID lpparame);
DWORD WINAPI RecordPosition(LPVOID postion);
DWORD WINAPI dialogue(LPVOID dialogue);
DWORD WINAPI mouse_hit(LPVOID mouse);
DWORD WINAPI cd_timer(LPVOID Cd);
DWORD WINAPI draw(LPVOID d);
DWORD WINAPI sleep(LPVOID s);
DWORD WINAPI mouse_position(LPVOID ms);

//���ֲ��Ų���
int music_move_flag;
int music_back_flag;
int music_jump_flag;
int music_load_flag;
int dialogue_flag;//̨�ʲ��ſ���
int dialogue_no;
int step;

struct people
{
	int life;
	int direction;
	int x_speed;
	int y_speed;
	int past_y_speed;
	int body_left_x;
	int body_right_x;
	int foot_x;
	int foot_y;
	int head_x;
	int head_y;
	int past_foot_x;
	int past_foot_y;
	bool flying;
	bool flying_past;
}people;

enum direction
{
	left, right
};

#define MAXSTAR 2000	// ��������

struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};

struct Wall
{
	int x1;//ǽ���������
	int x2;//ǽ���Ҳ�����
	int y1;//ǽ�Ķ�������
	int y2;//ǽ�ĵ�������
} wall[NumOfWall];

struct trap
{
	int x1;//������������
	int x2;//������Ҳ�����
	int y1;//����Ķ�������
	int y2;//����ĵײ�����
} trap[NumOfTrap];

struct Windows
{
	int x1;
	int x2;
	int y1;
	int y2;
}windows;

struct Level
{
	int start_x;
	int start_y;
}level[NumOfLevel];

struct pile
{
	int x_left;
	int x_right;
	int y_up;
	int y_down;
};
struct pile* pile_x;
struct pile* pile_y;
int pile_flag1;
int pile_flag2;
int pile_shooter_no = 1;
int NumOfPile_X;
int NumOfPile_Y;
int pile_x_y1 = 0;
int pile_y_x1 = 0;

struct BACK
{
	int x1;
	int x2;
	int y1;
	int y2;
	int back_flag;//�жϱ����Ƿ�ʰȡ
}back[NumOfBack];
int back_menu_flag;
int back_move_flag_a = 1;
int back_move_flag_b = 1;
int back_move_flag_c = 1;
int back_move_flag_d = 1;
int back_move_flag_e = 1;
int back_move_flag_f = 1;

struct storage_point
{
	int x1;
	int x2;
	int y;
	int flag;
}storage_point[3];

STAR star[MAXSTAR];
