#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)
#pragma warning(disable:6385)
#pragma warning(disable:6054)
#pragma warning(disable:28182)
#pragma warning(disable:6011)
#pragma warning(disable:6308)
#pragma warning(disable:28183)
#pragma warning(disable:6387)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

int x;//�����ƶ�ͼƬλ�ò���
int y;//�����ƶ�ͼƬλ�ò���

int y1 = 0;

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
IMAGE play1, play2, title1, title2, play, pile1, pile2;

SYSTEMTIME t;//ȫ�ּ�ʱ��
struct tm t1;
time_t now1;
struct tm t2;
time_t now2;//��¼����ǰλ��ר�ã�����������
int msc1;
int msc2;
struct tm dialogue_timer[2];//̨�ʲ��ż�ʱ��
struct tm pile_timer[2];//�ڵ���ʱ��

bool mouse;

int flags;
int count;
int flagm;
int bk_flag;

#define NumOfWall 6
#define NumOfTrap 4
#define NumOfLevel 2

void move();
void HitDet();
void BackTrack();
void ChangeLevel(int);
void flash();
void dialogue_play();
void start_menu();

DWORD WINAPI musicplay(LPVOID lpparamer);
DWORD WINAPI Star(LPVOID lpparame);
DWORD WINAPI RecordPosition(LPVOID postion);
DWORD WINAPI dialogue(LPVOID dialogue);
DWORD WINAPI mouse_hit(LPVOID mouse);

//���ֲ��Ų���
int music_move_flag;
int music_back_flag;
int music_jump_flag;
int music_load_flag;
int dialogue_flag;//̨�ʲ��ſ���
int dialogue_no;
int step;

int pile_flag;

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

int NumOfPile;
struct pile
{
	int x_left;
	int x_right;
	int y_up;
	int y_down;
};
struct pile* pile;

STAR star[MAXSTAR];
