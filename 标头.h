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

int x;//人物移动图片位置参数
int y;//人物移动图片位置参数


int old_bk_x;
int old_bk_y;
int bk_x;
int bk_y;

int NumOfOldPosition;
int* old_x;
int* old_y;
int* old_speed_y;
int* TIME;
int level_num;//当前关卡数

IMAGE people_right, people_left, people_right_2, people_left_2, bk, bk2;
IMAGE play1, play2, title1, title2, play, pile_1, pile_2, pile3, pile4;
IMAGE back_a_1, back_a_2;
IMAGE back_menu1, back_menu2;
IMAGE back_a2_1, back_a2_2;
IMAGE FLASH, back_track;



SYSTEMTIME t;//毫秒全局计时器，记录位置专用
SYSTEMTIME t3,t4;//公用
struct tm t1;
time_t now1;
struct tm t2;
time_t now2;//记录三秒前位置专用，勿随意引用
int msc1;
int msc2;
struct tm dialogue_timer[2];//台词播放计时器
struct tm pile_x_timer[2];//炮弹计时器
struct tm pile_y_timer[2];
struct tm flash_timer[2];//闪现CD计时器
int flash_cd;
struct tm back_track_timer[2];//回溯CD计时器
int back_track_cd;

bool mouse;

int flags;
int count;
int flagm;
int bk_flag;

//技能CD参数
int flash_flag;
int back_track_flag;

#define NumOfWall 7
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

DWORD WINAPI musicplay(LPVOID lpparamer);
DWORD WINAPI Star(LPVOID lpparame);
DWORD WINAPI RecordPosition(LPVOID postion);
DWORD WINAPI dialogue(LPVOID dialogue);
DWORD WINAPI mouse_hit(LPVOID mouse);
DWORD WINAPI cd_timer(LPVOID Cd);

//音乐播放参数
int music_move_flag;
int music_back_flag;
int music_jump_flag;
int music_load_flag;
int dialogue_flag;//台词播放控制
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

#define MAXSTAR 2000	// 星星总数

struct STAR
{
	double	x;
	int		y;
	double	step;
	int		color;
};

struct Wall
{
	int x1;//墙的左侧坐标
	int x2;//墙的右侧坐标
	int y1;//墙的顶面坐标
	int y2;//墙的底面坐标
} wall[NumOfWall];

struct trap
{
	int x1;//陷阱的左侧坐标
	int x2;//陷阱的右侧坐标
	int y1;//陷阱的顶部坐标
	int y2;//陷阱的底部坐标
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
	int back_flag;//判断背包是否被拾取
}back[NumOfBack];
int back_menu_flag;
int back_move_flag_a = 1;


STAR star[MAXSTAR];
