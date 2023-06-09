#include<标头.h>

// 初始化星星
void InitStar(int i)
{
	star[i].x = 0;
	star[i].y = rand() % 700;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);	// 速度越快，颜色越亮
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}

// 移动星星
void MoveStar(int i)
{
	// 擦掉原来的星星
	putpixel((int)star[i].x, star[i].y, 0);

	// 计算新位置
	star[i].x += star[i].step;
	if (star[i].x > 1300)
		InitStar(i);

	// 画新星星
	putpixel((int)star[i].x, star[i].y, star[i].color);
}

// 星星主函数
DWORD WINAPI Star(LPVOID lpparame)
{
	srand((unsigned)time(NULL));	// 随机种子			

	// 初始化所有星星
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 1300;
	}
	// 绘制星空
	while (1)
	{
		for (int i = 0; i < MAXSTAR; i++)
			MoveStar(i);
		Sleep(20);
	}
	return 1;
}

void KeyControl()
{
	int flag = 0;
	if (GetAsyncKeyState(68))//d键右走
	{
		people.direction = right;
		people.x_speed = 10;
		flag = 1;
		move();
	}
	if (GetAsyncKeyState(65))//a键左走
	{
		people.direction = left;
		people.x_speed = 10;
		flag = 1;
		move();
	}
	if (GetAsyncKeyState(32))//空格跳跃
	{
		if (people.flying == false)//禁止悬空时使用跳跃
		{
			people.y_speed = -20;
			flag = 1;
			if (people.x_speed != 0)
				people.x_speed = 10;

			move();
			music_jump_flag = 1;
			CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		}
	}
	if (GetAsyncKeyState(66))//b键回溯3秒前位置
	{
		BackTrack();
	}
	if (GetAsyncKeyState(13))//回车传送
	{
		if (3350 <= people.foot_x && people.body_left_x <= 3455 && people.foot_y == 825)//1号传送门
		{
			ChangeLevel(2);
		}
	}
	if (GetAsyncKeyState(82))//r键闪现
	{
		flash();
	}
	if (GetAsyncKeyState(70))//f键拾取
	{
		take_back();
	}
	if (GetAsyncKeyState(9))//tap打开背包
	{
		back_menu_flag = 1;
		back_menu();
	}
	if (GetAsyncKeyState(84))//T存档
	{
		storage();
	}
	if (GetAsyncKeyState(27))//esc退出
	{
		people.life = 0;
	}
	if (flag == 1)
	{
		time(&now1);
		localtime_s(&t1, &now1);
		//进入移动音效函数
		if (flagm == 0)
		{
			msc2 = (t1.tm_min * 60 + t1.tm_sec) * 1000 + t.wMilliseconds;//用毫秒记录当前时间
			if (msc2 - msc1 >= 200 || msc1 == 1)//设置脚步声间隔时间
				flagm = 1;
		}

		if (flagm == 1)
		{
			msc1 = (t1.tm_min * 60 + t1.tm_sec) * 1000 + t.wMilliseconds;//用毫秒记录当前时间
			HitDet();
			if (people.flying == false)
			{
				music_move_flag = 1;
				CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
				flagm = 0;
			}
		}
	}
	
	HitDet();
	if (people.flying == false)//在空中时，x速度不衰减，在地面时，松开方向键x速度归零
		people.x_speed = 0;

	if (flag == 0)//未敲击键盘
		x = 0;//人物图片复位到第一张
}

void move()
{
	if (people.direction == right && people.x_speed != 0)
	{
		x += 90;
		old_bk_x = people.foot_x - 445;
		people.foot_x += people.x_speed;
		people.body_left_x += people.x_speed;
		people.body_right_x += people.x_speed;
		windows.x1 += people.x_speed;
		windows.x2 += people.x_speed;
		bk_x = people.foot_x - 445;
		if (x == 360)//放完第四张图片后回到第一张
			x = 0;
	}

	if (people.direction == left && people.x_speed != 0)
	{
		x += 90;
		old_bk_x = people.foot_x - 445;
		people.foot_x -= people.x_speed;
		people.body_left_x -= people.x_speed;
		people.body_right_x -= people.x_speed;
		windows.x1 -= people.x_speed;
		windows.x2 -= people.x_speed;
		bk_x = people.foot_x - 445;
		if (x == 360)//放完第四张图片后回到第一张
			x = 0;

	}

	if (people.y_speed != 0 || people.flying == true)//竖直方向上的移动
	{
		old_bk_y = people.foot_y - 425;
		people.foot_y += people.y_speed;
		people.head_y += people.y_speed;
		bk_y = people.foot_y - 425;
		windows.y1 += people.y_speed;
		windows.y2 += people.y_speed;

		people.y_speed += 2;//模拟重力为2

		HitDet();
		if (people.flying == false)//检测是否踩在实体上
		{
			people.y_speed = 0;
			music_load_flag = 1;
			CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		}
	}

	
}


void HitDet()//碰撞，悬空检测
{
	int flag = 0;
	//墙体碰撞
	for (int i = 0; i < NumOfWall; i++)//判断是否悬空
	{
		if (wall[i].x1 <= people.foot_x && people.foot_x <= wall[i].x2 && people.foot_y == wall[i].y1)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 1)
		people.flying = false;
	else
		people.flying = true;

	//掉落回到存档点
	if (people.y_speed > 100 && storage_no!=0 &&endflag==0)
		back_storage_position();
	if (people.y_speed > 100 && storage_no == 0 && endflag == 0)
		ChangeLevel(1);
	for (int i = 0; i < NumOfTrap; i++)//陷阱触碰检测
	{
		if (((trap[i].x1 <= people.body_left_x && people.body_left_x <= trap[i].x2) ||
			(trap[i].x1 <= people.body_right_x && people.body_right_x <= trap[i].x2)) &&
			(trap[i].y1 <= people.foot_y && people.foot_y <= trap[i].y2))
		{
			if (people.life > 0)
			{
				people.life -= 10;
				if (people.direction == right)//击退效果
				{
					int X = 100;
					people.foot_x -= X;
					people.body_left_x -= X;
					people.body_right_x -= X;
					people.head_x -= X;
					windows.x1 -= X;
					windows.x2 -= X;
					bk_x -= X;
				}
				if (people.direction == left)//击退效果
				{
					int X = -100;
					people.foot_x -= X;
					people.body_left_x -= X;
					people.body_right_x -= X;
					people.head_x -= X;
					windows.x1 -= X;
					windows.x2 -= X;
					bk_x -= X;
				}
			}
		}
	}

	if (NumOfPile_X > 0)//炮弹击中判定
	{
		for (int i = 0; i < NumOfPile_X; i++)
		{
			if ((pile_x[i].x_left + 5 <= people.body_left_x && people.body_left_x <= pile_x[i].x_right) ||
				(pile_x[i].x_left + 5 <= people.body_right_x && people.body_right_x <= pile_x[i].x_right))
				if ((people.head_y <= pile_x[i].y_up && pile_x[i].y_up <= people.foot_y) ||
					(people.head_y <= pile_x[i].y_down && pile_x[i].y_down <= people.foot_y))
				{
					if (people.life > 0)
						people.life -= 10;

					for (int n = i; n < NumOfPile_X - 1; n++)//擦除原有的炮弹
						pile_x[n] = pile_x[n + 1];

					NumOfPile_X--;
					pile_x = (struct pile*)realloc(pile_x, sizeof(struct pile) * NumOfPile_X);
				}
		}

		for (int i = 0; i < NumOfPile_Y; i++)
		{
			if ((pile_y[i].x_left + 5 <= people.body_left_x && people.body_left_x <= pile_y[i].x_right) || 
				(pile_y[i].x_left + 5 <= people.body_right_x && people.body_right_x <= pile_y[i].x_right))
				if ((people.head_y <= pile_y[i].y_up && pile_y[i].y_up <= people.foot_y) ||
					(people.head_y <= pile_y[i].y_down && pile_y[i].y_down <= people.foot_y))
				{
					if (people.life > 0)
					people.life -= 10;

					for (int n = i; n < NumOfPile_Y - 1; n++)//擦除原有的炮弹
						pile_y[n] = pile_y[n + 1];

					NumOfPile_Y--;
					pile_y = (struct pile*)realloc(pile_y, sizeof(struct pile) * NumOfPile_Y);
				}
		}
	}
}

void take_back()
{
	//背包拾取判定
	for (int i = 0; i < NumOfBack; i++)
	{
		if ((back[i].x1 <= people.body_left_x && people.body_left_x <= back[i].x2 ||
			back[i].x1 <= people.body_right_x && people.body_right_x <= back[i].x2)
			&& people.foot_y == back[i].y2)

			back[i].back_flag = 0;

		if (back[4].back_flag == 0 && endflag == 0)
		{
			wall[13].x1 = 0;
			wall[13].x2 = 0;
			wall[13].y1 = 0;
			wall[13].y2 = 0;
			endflag = 1;
			mciSendString("close bgm1", NULL, 0, NULL);
			mciSendString("open bgm2.mp3 alias bgm2", NULL, 0, 0);
			mciSendString("play bgm2", NULL, 0, NULL);
			CreateThread(NULL, NULL, sleep, NULL, NULL, NULL);
		}
		if (back[5].back_flag == 0 && endflag == 1)
		{
			remove("A\\位置存档.txt");
			remove("A\\背包存档.txt");
			remove("A\\关卡数.txt");

			int ye = 700;
			cleardevice();
			while (ye != -700)
			{
				putimage(0, ye, &结束语);
				ye--;
				Sleep(10);
			}
			exit(0);
		}
	}
}

DWORD WINAPI sleep(LPVOID s)
{
	Sleep(750);
	ChangeLevel(1);
	mciSendString("open p5.mp3 alias p5", NULL, 0, 0);	
	mciSendString("play p5", NULL, 0, NULL);
	return 1;
}

void back_menu()
{
	//背包菜单界面绘制
	cleardevice();
	while (back_menu_flag == 1)
	{
		putimage(350, 175, &back_menu1, SRCAND);
		putimage(350, 175, &back_menu2, SRCPAINT);
		for (int i = 0; i < NumOfBack; i++)
		{
			if (back[i].back_flag == 0)
			{
				switch (i+1)
				{
				case 1:
					putimage(385, 205, &back_a2_1, SRCAND);
					putimage(385, 205, &back_a2_2, SRCPAINT);
					break;
				case 2:
					putimage(585, 205, &back_b2_2, SRCAND);
					putimage(585, 205, &back_b2_1, SRCPAINT);
					break;
				case 3:
					putimage(785, 205, &back_c2_1, SRCAND);
					putimage(785, 205, &back_c2_2, SRCPAINT);
					break;
				case 4:
					putimage(385, 380, &back_d2_1, SRCAND);
					putimage(385, 380, &back_d2_2, SRCPAINT);
					break;
				case 5:
					putimage(585, 380, &back_e2_1, SRCAND);
					putimage(585, 380, &back_e2_2, SRCPAINT);
					break;
				case 6:
					if (endflag == 1)
					{
						putimage(785, 380, &back_f2_1, SRCAND);
						putimage(785, 380, &back_f2_2, SRCPAINT);
					}
					break;

				}
			}
		}
		Sleep(200);
		
		
		if (GetAsyncKeyState(9))
		{
			back_menu_flag = 0;
			Sleep(100);
		}
					
	}
}

void GameDraw()	
{
	cleardevice();
	BeginBatchDraw();//双缓冲防止闪屏

	if (level_num == 1)//第一关
	{
		putimage(0, 0, 1600, 700, &bk, bk_x, bk_y);//背景图片

		//新手教程
		{
			if (flag_D == 1)
			{
				outtextxy(people.foot_x - 100, people.foot_y - 130, "按D右移");
			}
			if (flag_A == 1)
			{
				outtextxy(people.foot_x - 100, people.foot_y - 130, "按A左移");
			}
			if (flag_space == 1)
			{
				outtextxy(people.foot_x - 100, people.foot_y - 130, "按空格键跳跃");
			}
			if (flag_R == 1)
			{
				outtextxy(people.foot_x - 100, people.foot_y - 130, "按R闪现");
			}
			if (flag_B == 1)
			{
				outtextxy(people.foot_x - 100, people.foot_y - 130, "按B回到3秒前位置");
			}
		}

		if (back[0].back_flag == 1)
		{
			putimage(back[0].x1 - windows.x1, back[0].y1 - windows.y1, &back_a_1, SRCAND);//背包图片
			putimage(back[0].x1 - windows.x1, back[0].y1 - windows.y1, &back_a_2, SRCPAINT);

			if (back_move_flag_a == 1)
			{
				back[0].y1 += 2;
				if (back[0].y1 == 765)
					back_move_flag_a = 0;
			}
			if (back_move_flag_a == 0)
			{
				back[0].y1 -= 2;
				if (back[0].y1 == 745)
					back_move_flag_a = 1;
			}
		}
		if (back[5].back_flag == 1 && endflag == 1)
		{
			putimage(back[5].x1 - windows.x1, back[5].y1 - windows.y1, &back_f_1, SRCAND);//背包图片
			putimage(back[5].x1 - windows.x1, back[5].y1 - windows.y1, &back_f_2, SRCPAINT);

			if (back_move_flag_f == 1)
			{
				back[5].y1 += 2;
				if (back[5].y1 == 365)
					back_move_flag_f = 0;
			}
			if (back_move_flag_f == 0)
			{
				back[5].y1 -= 2;
				if (back[5].y1 == 345)
					back_move_flag_f = 1;
			}
		}
	}

	if (level_num == 2)//第二关
	{
		putimage(0, 0, 1600, 700, &bk2, bk_x, bk_y);//背景图片

		//绘制横向炮弹
		{
		time(&now1);
		localtime_s(&pile_x_timer[1], &now1);
		if (pile_flag1 == 1)
		{
			NumOfPile_X++;
			if (NumOfPile_X == 1)
				pile_x = (struct pile*)malloc(sizeof(struct pile) * NumOfPile_X);
			if (NumOfPile_X > 1)
				pile_x = (struct pile*)realloc(pile_x, sizeof(struct pile) * NumOfPile_X);

			pile_x[NumOfPile_X - 1].x_left = 2230;
			pile_x[NumOfPile_X - 1].x_right = pile_x[NumOfPile_X - 1].x_left + 65;
			pile_x[NumOfPile_X - 1].y_up = 950;
			pile_x[NumOfPile_X - 1].y_down = pile_x[NumOfPile_X - 1].y_up + 20;
			pile_flag1 = 0;

			time(&now1);
			localtime_s(&pile_x_timer[0], &now1);
		}
		if ((pile_x_timer[1].tm_min * 60 + pile_x_timer[1].tm_sec) - (pile_x_timer[0].tm_min * 60 + pile_x_timer[0].tm_sec) >= 5)//设置炮弹间时间间隔
			pile_flag1 = 1;

		if (NumOfPile_X > 0)
		{
			for (int i = 0; i < NumOfPile_X; i++)
			{
				putimage(pile_x[i].x_left - windows.x1, pile_x[i].y_up - windows.y1, 68, 25, &pile_1, 0, pile_x_y1, SRCAND);
				putimage(pile_x[i].x_left - windows.x1, pile_x[i].y_up - windows.y1, 68, 25, &pile_2, 0, pile_x_y1, SRCPAINT);
			}
			pile_x_y1 += 25;
			if (pile_x_y1 == 300)
				pile_x_y1 = 0;
			for (int i = 0; i < NumOfPile_X; i++)
			{
				pile_x[i].x_left -= 5;
				pile_x[i].x_right -= 5;
			}
		}
	}
			//绘制竖向炮弹
		{
			time(&now1);
			localtime_s(&pile_y_timer[1], &now1);
			GetSystemTime(&t3);

			if (pile_flag2 == 1)
			{
				NumOfPile_Y++;
				if (NumOfPile_Y == 1)
					pile_y = (struct pile*)malloc(sizeof(struct pile) * NumOfPile_Y);
				if (NumOfPile_Y > 1)
					pile_y = (struct pile*)realloc(pile_y, sizeof(struct pile) * NumOfPile_Y);
				switch (pile_shooter_no)
				{
				case 1:
					pile_y[NumOfPile_Y - 1].x_left = 2940;
					pile_y[NumOfPile_Y - 1].x_right = pile_y[NumOfPile_Y - 1].x_left + 25;
					pile_y[NumOfPile_Y - 1].y_up = 940;
					pile_y[NumOfPile_Y - 1].y_down = pile_y[NumOfPile_Y - 1].y_up + 70;
					pile_flag2 = 0;
					pile_shooter_no = 3;
					break;
				case 2:
					pile_y[NumOfPile_Y - 1].x_left = 3340;
					pile_y[NumOfPile_Y - 1].x_right = pile_y[NumOfPile_Y - 1].x_left + 25;
					pile_y[NumOfPile_Y - 1].y_up = 940;
					pile_y[NumOfPile_Y - 1].y_down = pile_y[NumOfPile_Y - 1].y_up + 70;
					pile_flag2 = 0;
					pile_shooter_no = 4;
					break;
				case 3:
					pile_y[NumOfPile_Y - 1].x_left = 3735;
					pile_y[NumOfPile_Y - 1].x_right = pile_y[NumOfPile_Y - 1].x_left + 25;
					pile_y[NumOfPile_Y - 1].y_up = 940;
					pile_y[NumOfPile_Y - 1].y_down = pile_y[NumOfPile_Y - 1].y_up + 70;
					pile_flag2 = 0;
					pile_shooter_no = 2;
					break;
				case 4:
					pile_y[NumOfPile_Y - 1].x_left = 4110;
					pile_y[NumOfPile_Y - 1].x_right = pile_y[NumOfPile_Y - 1].x_left + 25;
					pile_y[NumOfPile_Y - 1].y_up = 940;
					pile_y[NumOfPile_Y - 1].y_down = pile_y[NumOfPile_Y - 1].y_up + 70;
					pile_flag2 = 0;
					pile_shooter_no = 1;
					break;
				}
				time(&now1);
				localtime_s(&pile_y_timer[0], &now1);
				GetSystemTime(&t4);
			}
			if (((pile_y_timer[1].tm_min * 60 + pile_y_timer[1].tm_sec) * 1000 + t3.wMilliseconds) -
				((pile_y_timer[0].tm_min * 60 + pile_y_timer[0].tm_sec) * 1000 + t4.wMilliseconds) >= 350)//设置炮弹间时间间隔
				pile_flag2 = 1;

			if (NumOfPile_Y > 0)
			{
				for (int i = 0; i < NumOfPile_Y; i++)
				{
					putimage(pile_y[i].x_left - windows.x1, pile_y[i].y_up - windows.y1, 25, 68, &pile3, pile_y_x1, 0, SRCAND);
					putimage(pile_y[i].x_left - windows.x1, pile_y[i].y_up - windows.y1, 25, 68, &pile4, pile_y_x1, 0, SRCPAINT);
				}
				pile_y_x1 += 25;
				if (pile_y_x1 == 300)
					pile_y_x1 = 0;
				for (int i = 0; i < NumOfPile_Y; i++)
				{
					pile_y[i].y_down += 10;
					pile_y[i].y_up += 10;
				}
			}
		}

		

		if (back[1].back_flag == 1)
		{
			putimage(back[1].x1 - windows.x1, back[1].y1 - windows.y1, &back_b_2, SRCAND);//背包图片
			putimage(back[1].x1 - windows.x1, back[1].y1 - windows.y1, &back_b_1, SRCPAINT);

			if (back_move_flag_b == 1)
			{
				back[1].y1 += 2;
				if (back[1].y1 == 940)
					back_move_flag_b = 0;
			}
			if (back_move_flag_b == 0)
			{
				back[1].y1 -= 2;
				if (back[1].y1 == 920)
					back_move_flag_b = 1;
			}
		}
		if (back[2].back_flag == 1)
		{
			putimage(back[2].x1 - windows.x1, back[2].y1 - windows.y1, &back_c_1, SRCAND);//背包图片
			putimage(back[2].x1 - windows.x1, back[2].y1 - windows.y1, &back_c_2, SRCPAINT);

			if (back_move_flag_c == 1)
			{
				back[2].y1 += 2;
				if (back[2].y1 == 1210)
					back_move_flag_c = 0;
			}
			if (back_move_flag_c == 0)
			{
				back[2].y1 -= 2;
				if (back[2].y1 == 1190)
					back_move_flag_c = 1;
			}
		}
		if (back[3].back_flag == 1)
		{
			putimage(back[3].x1 - windows.x1, back[3].y1 - windows.y1, &back_d_1, SRCAND);//背包图片
			putimage(back[3].x1 - windows.x1, back[3].y1 - windows.y1, &back_d_2, SRCPAINT);

			if (back_move_flag_d == 1)
			{
				back[3].y1 += 2;
				if (back[3].y1 == 1780)
					back_move_flag_d = 0;
			}
			if (back_move_flag_d == 0)
			{
				back[3].y1 -= 2;
				if (back[3].y1 == 1760)
					back_move_flag_d = 1;
			}
		}
		if (back[4].back_flag == 1)
		{
			putimage(back[4].x1 - windows.x1, back[4].y1 - windows.y1, &back_e_1, SRCAND);//背包图片
			putimage(back[4].x1 - windows.x1, back[4].y1 - windows.y1, &back_e_2, SRCPAINT);

			if (back_move_flag_e == 1)
			{
				back[4].y1 += 2;
				if (back[4].y1 == 1780)
					back_move_flag_e = 0;
			}
			if (back_move_flag_e == 0)
			{
				back[4].y1 -= 2;
				if (back[4].y1 == 1760)
					back_move_flag_e = 1;
			}
		}
	}
	settextcolor(RGB(118, 216, 254));
	settextstyle(19, 10, "System");
	for (int i = 0; i < NumOfBack; i++)
	{
		if ((back[i].x1 <= people.body_left_x && people.body_left_x <= back[i].x2 ||
			back[i].x1 <= people.body_right_x && people.body_right_x <= back[i].x2)
			&& people.foot_y == back[i].y2)
			if (back[i].back_flag == 1)
				if (i != 5 && endflag == 0)
					outtextxy(back[i].x1 + 5 - windows.x1, back[i].y2 - 120 - windows.y1, "F拾取");
		if (i == 5 && endflag == 1)
			outtextxy(back[i].x1 + 5 - windows.x1, back[i].y2 - 120 - windows.y1, "F拾取");
	}

	for (int i = 0; i < 3; i++)
	{
		if (storage_point[i].x1 <= people.foot_x && people.foot_x <= storage_point[i].x2 && people.foot_y == storage_point[i].y)
			if (storage_point[i].flag == 0 && i + 1 != storage_no)
			{
				outtextxy(storage_point[i].x1 +10 - windows.x1, storage_point[i].y - 150 - windows.y1, "T存档");
			}
	}

	//技能图标绘制
	settextcolor(RGB(255, 63, 63));
	settextstyle(30, 30, "新宋体");
	if (flash_flag == 1)//闪现图标
		putimage(0, 230, &FLASH);
	if (flash_flag == 0)
	{
		putimage(0, 230, &FLASH, SRCAND);
		char cd[40];
		sprintf(cd, "%d", flash_cd);
		outtextxy(10, 240, cd);
	} 

	if (back_track_flag == 1)//回溯图标
		putimage(0, 290, &back_track);
	if (back_track_flag == 0)
	{
		putimage(0, 290, &back_track, SRCAND);
		char cd[40];
		sprintf(cd, "%d", back_track_cd);
		outtextxy(10, 300, cd);
	}


	//血条绘制、
	setfillcolor(RGB(255, 63, 63));//血条的颜色
	fillrectangle(70, 50, 470 - 400 * (100 - people.life) * 0.01, 60);
	if (people.life != 100)
		setfillcolor(RGB(46, 46, 46));//空血条的颜色
	fillrectangle(470 - 400 * (100 - people.life) * 0.01, 50, 470, 60);

	//人物绘制
	if (people.direction == right)
	{
		putimage(400, 343, 90, 90, &people_right_2, x, y, SRCAND);//右走图片
		putimage(400, 343, 90, 90, &people_right, x, y, SRCPAINT);
	}
	if (people.direction == left)
	{
		putimage(400, 343, 90, 90, &people_left_2, x, y, SRCAND);//左走图片
		putimage(400, 343, 90, 90, &people_left, x, y, SRCPAINT);
	}

	

	//fillrectangle(0, 422, 1300, 427);
	//line(455, 0, 455, 700);
	//line(433, 0, 433, 700);
	//line(0, 425, 1300, 425);
	//line(0, 350, 1300, 350);
	//line(5200 - windows.x1, 1760 - windows.y1, 5500 - windows.x1, 1760 - windows.y1);
	//调试参数
	{
		settextstyle(15, 8, "新宋体");
		char c1[50];
		char c2[50], c3[50], c4[50], c5[50], c6[50], c7[50];
		sprintf(c1, "%d", people.foot_x);
		sprintf(c2, "%d", people.foot_y);
		sprintf(c3, "%d", people.body_left_x);
		sprintf(c4, "%d", people.body_right_x);
		sprintf(c5, "%d", people.past_foot_x);
		sprintf(c6, "%d", people.past_foot_y);
		sprintf(c7, "%d", people.life);

		setbkmode(TRANSPARENT);

		strcat(c1, "foot_x");
		strcat(c2, "foot_y");
		strcat(c3, "body_left_x");
		strcat(c4, "body_right_y");
		strcat(c5, "past_foot_x");
		strcat(c6, "past_foot_y");
		strcat(c7, "people_life");
		char c8[] = "HP";

		settextcolor(WHITE);
		outtextxy(1000, 200, c1);
		outtextxy(1000, 220, c2);
		outtextxy(1000, 240, c3);
		outtextxy(1000, 260, c4);
		outtextxy(1000, 280, c5);
		outtextxy(1000, 300, c6);
		outtextxy(1000, 320, c7);
		settextcolor(RED);
		outtextxy(45, 48, c8);
	}

	EndBatchDraw();

	
		
	if (people.flying == true && people.x_speed != 0)
		Sleep(70);
	else
		Sleep(58);
}

void GameInit()
{
	people.life = 100;
	people.direction = right;
	people.x_speed = 0;
	people.y_speed = 0;
	people.past_y_speed = 0;
	people.body_left_x = 430;
	people.body_right_x = 455;
	people.foot_x = 445;
	people.foot_y = 425;
	people.head_x = 445;
	people.head_y = 350;
	people.flying = false;
	people.past_foot_x = 445;
	people.past_foot_y = 425;

	level_num = 1;//关卡数

	bk_x = people.foot_x - 445;
	bk_y = people.foot_y - 425;
	old_bk_x = people.foot_x - 445;
	old_bk_y = people.foot_y - 425;

	NumOfOldPosition = 1;//回溯功能参数

	windows.x1 = 0;//窗口相对坐标参数
	windows.x2 = 1300;
	windows.y1 = 0;
	windows.y2 = 700;

	fp2 = fopen("A\\位置存档.txt", "r");
	if (fp2 == NULL);
	else
	{
		fscanf(fp2, "%d", &storage_no);
		fclose(fp2);
	}
	fp3 = fopen("A\\背包存档.txt", "rb");
	if (fp3 == NULL)
	{
		back[0].back_flag = 1;
		back[1].back_flag = 1;
		back[2].back_flag = 1;
		back[3].back_flag = 1;
	}
	else
	{
		fread(back, sizeof(struct BACK), NumOfBack, fp3);
		fclose(fp3);
	}
	
	


	//墙体参数定义部分
	//第一关
	wall[0].x1 = 415;
	wall[0].x2 = 1210;
	wall[0].y1 = 425;
	wall[1].x1 = 1400;
	wall[1].x2 = 2200;
	wall[1].y1 = 555;
	wall[2].x1 = 1400;
	wall[2].x2 = 2200;
	wall[2].y1 = 565;
	wall[3].x1 = 2450;
	wall[3].x2 = 3600;
	wall[3].y1 = 825;
	//第二关
	wall[4].x1 = 300;
	wall[4].x2 = 1300;
	wall[4].y1 = 1000;
	wall[5].x1 = 1500;
	wall[5].x2 = 2500;
	wall[5].y1 = 1000;
	wall[6].x1 = 2730;
	wall[6].x2 = 4530;
	wall[6].y1 = 1270;
	wall[6].y2 = 1400;
	wall[7].x1 = 4660;
	wall[7].x2 = 4760;
	wall[7].y1 = 1400;
	wall[7].y2 = 1480;
	wall[8].x1 = 4950;
	wall[8].x2 = 5110;
	wall[8].y1 = 1530;
	wall[8].y2 = 1660;
	wall[9].x1 = 5210;
	wall[9].x2 = 5310;
	wall[9].y1 = 1840;
	wall[9].y2 = 1920;
	wall[10].x1 = 5490;
	wall[10].x2 = 5550;
	wall[10].y1 = 1840;
	wall[10].y2 = 1860;
	wall[11].x1 = 5670;
	wall[11].x2 = 5730;
	wall[11].y1 = 1840;
	wall[11].y2 = 1860;
	wall[12].x1 = 5850;
	wall[12].x2 = 5910;
	wall[12].y1 = 1840;
	wall[12].y2 = 1860;
	wall[13].x1 = 6030;
	wall[13].x2 = 6090;
	wall[13].y1 = 1840;
	wall[13].y2 = 1860;
	

	//陷阱参数
	//第二关
	trap[0].x1 = 580;
	trap[0].x2 = 625;
	trap[0].y1 = 930;
	trap[0].y2 = 1000;
	trap[1].x1 = 860;
	trap[1].x2 = 885;
	trap[1].y1 = 930;
	trap[1].y2 = 1000;
	trap[2].x1 = 1930;
	trap[2].x2 = 2075;
	trap[2].y1 = 1000;
	trap[2].y2 = 1000;
	trap[3].x1 = 1670;
	trap[3].x2 = 1815;
	trap[3].y1 = 1000;
	trap[3].y2 = 1130;



	//背包参数定义
	back_menu_flag = 0;
	back[0].x1 = 3230;
	back[0].x2 = 3260;
	back[0].y1 = 745;
	back[0].y2 = 825;
	
	back[1].x1 = 2380;
	back[1].x2 = 2410;
	back[1].y1 = 920;
	back[1].y2 = 1000;
	
	back[2].x1 = 3570;
	back[2].x2 = 3600;
	back[2].y1 = 1190;
	back[2].y2 = 1270;

	back[3].x1 = 5240;
	back[3].x2 = 5270;
	back[3].y1 = 1760;
	back[3].y2 = 1840;
	
	back[4].x1 = 6045;
	back[4].x2 = 6075;
	back[4].y1 = 1760;
	back[4].y2 = 1840;
	back[4].back_flag = 1;

	back[5].x1 = 445;
	back[5].x2 = 475;
	back[5].y1 = 345;
	back[5].y2 = 425;
	back[5].back_flag = 1;
	

	//关卡出生点坐标，以脚步坐标为基准
	level[0].start_x = 445;
	level[0].start_y = 425;
	level[1].start_x = 330;
	level[1].start_y = 1000;

	//存档点
	storage_point[0].x1 = 715;
	storage_point[0].x2 = 805;
	storage_point[0].y = 425;
	storage_point[0].flag = 0;
	storage_point[1].x1 = 390;
	storage_point[1].x2 = 480;
	storage_point[1].y = 1000;
	storage_point[1].flag = 0;
	storage_point[2].x1 = 4400;
	storage_point[2].x2 = 4470;
	storage_point[2].y = 1270;
	storage_point[2].flag = 0;
	if (storage_no > 1)
	{
		ChangeLevel(2);
		back_storage_position();
	}

	NumOfPile_X = 0;//炮弹数
	NumOfPile_Y = 0;

	count = 0;
	flags = 0;
	flagm = 1;
	step = 1;//脚步声循环参数
	dialogue_no = 1;//休闲台词循环播放
	dialogue_flag = 1;
	pile_flag1 = 1;//控制炮弹发射
	pile_flag2 = 1;

	flash_flag = 1;
	back_track_flag = 1;

	mouse = false;
	msc1 = 0;

	time(&now1);//初始化计时参数一
	localtime_s(&t1, &now1);


	initgraph(1300, 700);
	setbkcolor(BLACK);
	cleardevice();
	x = 0;
	y = 0;
	bk_flag = 1;

	loadimage(&people_right, "people-right.png", 360, 90);
	loadimage(&people_left, "people-left.png", 360, 90);
	loadimage(&people_right_2, "people-right-2.png", 360, 90);
	loadimage(&people_left_2, "people-left-2.png", 360, 90);
	loadimage(&bk, "bk1.png", 6500, 3500);
	loadimage(&bk2, "bk2.png", 6500, 3500);
	loadimage(&play1, "play1.png", 250, 150);
	loadimage(&play2, "play2.png", 250, 150);
	loadimage(&title1, "title1.png");
	loadimage(&title2, "title2.png");
	loadimage(&play, "play.png", 130, 40);
	loadimage(&pile_1, "炮弹1.png");
	loadimage(&pile_2, "炮弹2.png");
	loadimage(&pile3, "炮弹竖向1.png");
	loadimage(&pile4, "炮弹竖向2.png");
	loadimage(&back_a_1, "back_a_1.png");
	loadimage(&back_a_2, "back_a_2.png");
	loadimage(&back_a2_1, "back_a_1.png", 135, 115);
	loadimage(&back_a2_2, "back_a_2.png", 135, 115);
	loadimage(&back_b_1, "back_b_1.png");
	loadimage(&back_b_2, "back_b_2.png");
	loadimage(&back_b2_1, "back_b_1.png", 135, 115);
	loadimage(&back_b2_2, "back_b_2.png", 135, 115);
	loadimage(&back_c_1, "back_c_1.png");
	loadimage(&back_c_2, "back_c_2.png");
	loadimage(&back_c2_1, "back_c_1.png", 135, 115);
	loadimage(&back_c2_2, "back_c_2.png", 135, 115);
	loadimage(&back_d_1, "back_d_1.png");
	loadimage(&back_d_2, "back_d_2.png");
	loadimage(&back_d2_1, "back_d_1.png", 135, 115);
	loadimage(&back_d2_2, "back_d_2.png", 135, 115);
	loadimage(&back_e_1, "back_e_1.png");
	loadimage(&back_e_2, "back_e_2.png");
	loadimage(&back_e2_1, "back_e_1.png", 135, 115);
	loadimage(&back_e2_2, "back_e_2.png", 135, 115);
	loadimage(&back_f_1, "back_f_1.png");
	loadimage(&back_f_2, "back_f_2.png");
	loadimage(&back_f2_1, "back_f_1.png", 135, 115);
	loadimage(&back_f2_2, "back_f_2.png", 135, 115);
	loadimage(&back_menu1, "背包菜单1.png");
	loadimage(&back_menu2, "背包菜单2.png");
	loadimage(&FLASH, "flash.png",50,50);
	loadimage(&back_track, "back_track.png",50,50);
	loadimage(&gameover1, "gameover1.png");
	loadimage(&gameover2, "gameover2.png");
	loadimage(&结束语, "结束语.png");

	mciSendString("close bgm1", NULL, 0, NULL);
	mciSendString("open bgm1.mp3 alias bgm1", 0, NULL, 0);
	mciSendString("play bgm1 repeat", NULL, 0, NULL);

	fp1 = fopen("A\\NumOfPlay.txt", "r");
	if (fp1 == NULL)
	{
		fp1 = fopen("A\\NumOfPlay.txt", "w");
		fclose(fp1);
		NumOfPlay = 0;
	}
	else
		fscanf(fp1, "%d", &NumOfPlay);

	if (NumOfPlay == 0)//判断是否第一次运行游戏
		first_game_flag = 1;

	NumOfPlay++;
	fp1 = fopen("A\\NumOfPlay.txt", "w");
	fprintf(fp1, "%d", NumOfPlay);
	fclose(fp1);

	if (first_game_flag == 1)
	{
		flag_draw = 1;
		flag_dialogue = 1;
		flag_A = 0;
		flag_D = 0;
		flag_space = 0;
		flag_R = 0;
		flag_B = 0;
	}
}

DWORD WINAPI RecordPosition(LPVOID postion)//开辟一条线程来记录角色三秒前位置
{
	while (1)
	{
		GetSystemTime(&t);
		time(&now2);
		localtime_s(&t2, &now2);
		int msc = (t2.tm_min * 60 + t2.tm_sec) * 1000 + t.wMilliseconds;//用毫秒记录当前时间
		//system("cls");
		//printf("%d", msc);
		if (NumOfOldPosition == 1)
		{
			old_x = (int*)malloc(sizeof(int) * NumOfOldPosition);
			old_y = (int*)malloc(sizeof(int) * NumOfOldPosition);
			old_speed_y = (int*)malloc(sizeof(int) * NumOfOldPosition);
			TIME = (int*)malloc(sizeof(int) * NumOfOldPosition);

			old_x[NumOfOldPosition - 1] = people.foot_x;
			old_y[NumOfOldPosition - 1] = people.foot_y;
			old_speed_y[NumOfOldPosition - 1] = people.y_speed;
			TIME[NumOfOldPosition - 1] = msc;
			NumOfOldPosition++;
		}
		else
		{
			old_x = (int*)realloc(old_x, sizeof(int) * NumOfOldPosition);
			old_y = (int*)realloc(old_y, sizeof(int) * NumOfOldPosition);
			old_speed_y = (int*)realloc(old_speed_y, sizeof(int) * NumOfOldPosition);
			TIME = (int*)realloc(TIME, sizeof(int) * NumOfOldPosition);

			old_x[NumOfOldPosition - 1] = people.foot_x;
			old_y[NumOfOldPosition - 1] = people.foot_y;
			old_speed_y[NumOfOldPosition - 1] = people.y_speed;
			TIME[NumOfOldPosition - 1] = msc;
			NumOfOldPosition++;
		}

		int flag = 0;
		if (msc - TIME[0] >= 3000)//若TIME[0]中时间未超过三秒则后续数组中储存的时间间隔也一定未超过3秒
		{
			for (int i = 0; i < NumOfOldPosition - 1; i++)
			{
				if (msc - TIME[i] <= 3000)
				{
					flag = i;
					break;
				}
			}

			for (int n = flag; n < NumOfOldPosition - 1; n++)
			{
				TIME[n - flag] = TIME[n];
				old_speed_y[n - flag] = old_speed_y[n];
				old_x[n - flag] = old_x[n];
				old_y[n - flag] = old_y[n];
			}

			NumOfOldPosition -= (flag + 1);
			old_x = (int*)realloc(old_x, sizeof(int) * NumOfOldPosition);
			old_y = (int*)realloc(old_y, sizeof(int) * NumOfOldPosition);
			old_speed_y = (int*)realloc(old_speed_y, sizeof(int) * NumOfOldPosition);
			TIME = (int*)realloc(TIME, sizeof(int) * NumOfOldPosition);
		}
		people.past_foot_x = old_x[0];
		people.past_foot_y = old_y[0];
		people.past_y_speed = old_speed_y[0];

		system("cls");
		printf("%d\n", NumOfOldPosition);
		printf("%d %d", TIME[0], msc);
	}
	return 1;
}

void BackTrack()
{
	if (back_track_flag == 1)
	{
		int X = people.foot_x - people.past_foot_x;
		int Y = people.foot_y - people.past_foot_y;
		people.foot_x = people.past_foot_x;
		people.foot_y = people.past_foot_y;
		people.y_speed = people.past_y_speed;
		people.body_left_x -= X;
		people.body_right_x -= X;
		people.head_x -= X;
		people.head_y -= Y;
		windows.x1 -= X;
		windows.x2 -= X;
		windows.y1 -= Y;
		windows.y2 -= Y;
		bk_x -= X;
		bk_y -= Y;
		music_back_flag = 1;
		CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		time(&now1);
		localtime_s(&back_track_timer[1], &now1);
		back_track_flag = 0;
	}
}

DWORD WINAPI musicplay(LPVOID lpparamer)//使用多线程解决音乐播放会卡顿问题
{
	if (music_move_flag == 1)//脚步声
	{
		if (step == 4)
		{
			mciSendString("close m4", NULL, 0, NULL);
			mciSendString("open move4.mp3 alias m4", NULL, 0, 0);
			mciSendString("play m4", NULL, 0, NULL);
			music_move_flag = 0;
			step = 1;
		}
		if (step == 3)
		{
			mciSendString("close m3", NULL, 0, NULL);
			mciSendString("open move3.mp3 alias m3", NULL, 0, NULL);
			mciSendString("play m3", NULL, 0, NULL);
			music_move_flag = 0;
			step = 4;
		}
		if (step == 2)
		{
			mciSendString("close m2", NULL, 0, NULL);
			mciSendString("open move2.mp3 alias m2", NULL, 0, NULL);
			mciSendString("play m2", NULL, 0, NULL);
			music_move_flag = 0;
			step = 3;
		}
		if (step == 1)
		{
			mciSendString("close m1", NULL, 0, NULL);
			mciSendString("open move1.mp3 alias m1", NULL, 0, NULL);
			mciSendString("play m1", NULL, 0, NULL);
			music_move_flag = 0;
			step = 2;
		}
	}

	if (music_back_flag == 1)//传送声
	{
		mciSendString("close back", NULL, 0, NULL);
		mciSendString("open 传送.MP3 alias back", NULL, 0, NULL);
		mciSendString("play back", NULL, 0, NULL);
		music_back_flag = 0;
	}

	if (music_jump_flag == 1)//跳越声
	{
		mciSendString("close jump", NULL, 0, NULL);
		mciSendString("open jump.MP3 alias jump", NULL, 0, NULL);
		mciSendString("play jump", NULL, 0, NULL);
		music_jump_flag = 0;
	}

	if (music_load_flag == 1)//落地声
	{
		mciSendString("close load", NULL, 0, NULL);
		mciSendString("open load.MP3 alias load", NULL, 0, NULL);
		mciSendString("play load", NULL, 0, NULL);
		music_load_flag = 0;
	}

	return 1;
}

void ChangeLevel(int a)
{
	bk_flag = a;
	level_num = a;
	int X = people.foot_x - level[a - 1].start_x;
	int Y = people.foot_y - level[a - 1].start_y;
	people.foot_x = level[a - 1].start_x;
	people.foot_y = level[a - 1].start_y;
	people.y_speed = 0;
	people.body_left_x -= X;
	people.body_right_x -= X;
	people.head_x -= X;
	people.head_y -= Y;
	windows.x1 -= X;
	windows.x2 -= X;
	windows.y1 -= Y;
	windows.y2 -= Y;

	bk_x -= X;
	bk_y -= Y;
	music_back_flag = 1;
	CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
	NumOfOldPosition = 1;
}

void flash()//闪现
{
	if (flash_flag == 1)
	{
		if (people.direction == right)
		{
			int X = 150;
			people.foot_x += X;
			people.body_left_x += X;
			people.body_right_x += X;
			people.head_x += X;
			windows.x1 += X;
			windows.x2 += X;
			bk_x += X;
			music_back_flag = 1;
			CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		}

		if (people.direction == left)
		{
			int X = 150;
			people.foot_x -= X;
			people.body_left_x -= X;
			people.body_right_x -= X;
			people.head_x -= X;
			windows.x1 -= X;
			windows.x2 -= X;
			bk_x -= X;
			music_back_flag = 1;
			CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		}

		time(&now1);
		localtime_s(&flash_timer[1], &now1);
		flash_flag = 0;
	}
}

DWORD WINAPI cd_timer(LPVOID Cd)//技能CD计时线程
{
	while (1)
	{
		time(&now1);
		localtime_s(&flash_timer[0], &now1);
		flash_cd = 5 - ((flash_timer[0].tm_min * 60 + flash_timer[0].tm_sec) - (flash_timer[1].tm_min * 60 + flash_timer[1].tm_sec));
		if ((flash_timer[0].tm_min * 60 + flash_timer[0].tm_sec) - (flash_timer[1].tm_min * 60 + flash_timer[1].tm_sec) >= 5)
			flash_flag = 1;

		time(&now1);
		localtime_s(&back_track_timer[0], &now1);
		back_track_cd = 5 - ((back_track_timer[0].tm_min * 60 + back_track_timer[0].tm_sec) - (back_track_timer[1].tm_min * 60 + back_track_timer[1].tm_sec));
		if ((back_track_timer[0].tm_min * 60 + back_track_timer[0].tm_sec) - (back_track_timer[1].tm_min * 60 + back_track_timer[1].tm_sec) >= 5)
			back_track_flag = 1;
	}

	return 0;
}

void dialogue_play()
{
	time(&now1);
	localtime_s(&dialogue_timer[0], &now1);
	if (dialogue_flag == 1)
	{
		CreateThread(NULL, NULL, dialogue, NULL, NULL, NULL);
		time(&now1);
		localtime_s(&dialogue_timer[1], &now1);
		dialogue_flag = 0;
	}

	if ((dialogue_timer[0].tm_min * 60 + dialogue_timer[0].tm_sec) - (dialogue_timer[1].tm_min * 60 + dialogue_timer[1].tm_sec) >= 30)
		dialogue_flag = 1;//休闲台词播放计时器

	//printf("\n%d", dialogue_no);
}

DWORD WINAPI dialogue(LPVOID dialogue)
{
	switch (dialogue_no)
	{
	case 1:
		mciSendString("close dialogue1", NULL, 0, NULL);
		mciSendString("open 休闲台词1.mp3 alias dialogue1", NULL, 0, 0);
		mciSendString("play dialogue1", NULL, 0, NULL);
		break;
	case 2:
		mciSendString("close dialogue2", NULL, 0, NULL);
		mciSendString("open 休闲台词2.mp3 alias dialogue2", NULL, 0, 0);
		mciSendString("play dialogue2", NULL, 0, NULL);
		break;
	case 3:
		mciSendString("close dialogue3", NULL, 0, NULL);
		mciSendString("open 休闲台词3.mp3 alias dialogue3", NULL, 0, 0);
		mciSendString("play dialogue3", NULL, 0, NULL);
		break;
	case 4:
		mciSendString("close dialogue4", NULL, 0, NULL);
		mciSendString("open 休闲台词4.mp3 alias dialogue4", NULL, 0, 0);
		mciSendString("play dialogue4", NULL, 0, NULL);
		break;
	case 5:
		mciSendString("close dialogue5", NULL, 0, NULL);
		mciSendString("open 休闲台词5.mp3 alias dialogue5", NULL, 0, 0);
		mciSendString("play dialogue5", NULL, 0, NULL);
		break;
	case 6:
		mciSendString("close dialogue6", NULL, 0, NULL);
		mciSendString("open 休闲台词6.mp3 alias dialogue6", NULL, 0, 0);
		mciSendString("play dialogue6", NULL, 0, NULL);
		break;
	case 7:
		mciSendString("close dialogue7", NULL, 0, NULL);
		mciSendString("open 休闲台词7.mp3 alias dialogue7", NULL, 0, 0);
		mciSendString("play dialogue7", NULL, 0, NULL);
		break;
	case 8:
		mciSendString("close dialogue8", NULL, 0, NULL);
		mciSendString("open 休闲台词8.mp3 alias dialogue8", NULL, 0, 0);
		mciSendString("play dialogue8", NULL, 0, NULL);
		break;
	case 9:
		mciSendString("close dialogue9", NULL, 0, NULL);
		mciSendString("open 休闲台词9.mp3 alias dialogue9", NULL, 0, 0);
		mciSendString("play dialogue9", NULL, 0, NULL);
		break;
	case 10:
		mciSendString("close dialogue10", NULL, 0, NULL);
		mciSendString("open 休闲台词10.mp3 alias dialogue10", NULL, 0, 0);
		mciSendString("play dialogue10", NULL, 0, NULL);
		break;
	}
	dialogue_no++;
	if (dialogue_no == 11)
		dialogue_no = 1;

	return 0;
}

void start_menu()
{
	int x = 180;
	int y = 140;
	int flagx = 1;
	int flagy = 1;
	int countx = 0;
	int county = 0;
	CreateThread(NULL, NULL, mouse_hit, NULL, NULL, NULL);
	while (!mouse)
	{
		BeginBatchDraw();

		//主标题
		clearrectangle(x, y, x + 885, y + 152);
		putimage(x, y, &title1, SRCAND);
		putimage(x, y, &title2, SRCPAINT);
		if (flagx == 1)
		{
			x++;
			countx++;
			if (countx == 60)
			{
				flagx = 0;
				countx = 0;
			}
		}
		if (flagx == 0)
		{
			x--;
			countx++;
			if (countx == 60)
			{
				flagx = 1;
				countx = 0;
			}
		}
		if (flagy == 1)
		{
			y++;
			county++;
			if (county == 20)
			{
				flagy = 0;
				county = 0;
			}
		}
		if (flagy == 0)
		{
			y--;
			county++;
			if (county == 20)
			{
				flagy = 1;
				county = 0;
			}
		}
		//putimage(550, 500, &play2, SRCAND);
	   // putimage(550, 500, &play2, SRCAND);
		putimage(580, 500, &play);

		EndBatchDraw();
		Sleep(20);
	}

}

DWORD WINAPI mouse_hit(LPVOID Mouse)
{
	ExMessage m;
	while (1)
	{
		m = getmessage(EX_MOUSE);//获取鼠标状态给m
		system("cls");
		printf("x=%d y=%d\n", m.x, m.y);
		if (580 < m.x && m.x < 710 && 500 < m.y && m.y < 535)//判断鼠标是否在按钮内
		{
			m = getmessage(EX_MOUSE);
			if (m.lbutton)//判断左键是否点击
				break;
		}
	}
	mouse = true;
	return 0;
}

void storage()//存档
{
	for (int i = 0; i < 3; i++)
	{
		if (storage_point[i].x1 <= people.foot_x && people.foot_x <= storage_point[i].x2 && people.foot_y == storage_point[i].y)
		{
			storage_point[i].flag = 1;
			storage_no = i + 1;
		}
	}
}

void EndGame()
{
	fp2 = fopen("A\\位置存档.txt", "w");
	fprintf(fp2, "%d", storage_no);
	fp3 = fopen("A\\背包存档.txt", "wb");
	fwrite(back, sizeof(struct BACK), NumOfBack, fp3);
	fp4 = fopen("A\\关卡数.txt", "w");
	fprintf(fp4, "%d", level_num);
}

void back_storage_position()
{
	if (storage_no == 0)
	{
		ChangeLevel(1);
	}
	else
	{
		int X = people.foot_x - storage_point[storage_no - 1].x1;
		int Y = people.foot_y - storage_point[storage_no - 1].y;
		people.foot_x = storage_point[storage_no - 1].x1;
		people.foot_y = storage_point[storage_no - 1].y;
		people.y_speed = 0;
		people.body_left_x -= X;
		people.body_right_x -= X;
		people.head_x -= X;
		people.head_y -= Y;
		windows.x1 -= X;
		windows.x2 -= X;
		windows.y1 -= Y;
		windows.y2 -= Y;
		bk_x -= X;
		bk_y -= Y;
		music_back_flag = 1;
		CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		NumOfOldPosition = 1;
	}
}

void first_game()
{
	if (flag_dialogue == 1)
	{
		CreateThread(NULL, NULL, draw, NULL, NULL, NULL);
		mciSendString("open p1.mp3 alias p1", NULL, 0, 0);
		mciSendString("play p1", NULL, 0, NULL);
		Sleep(10000);
		mciSendString("open p2.mp3 alias p2", NULL, 0, 0);
		mciSendString("play p2", NULL, 0, NULL);
		Sleep(12000);
		mciSendString("open p3.mp3 alias p3", NULL, 0, 0);
		mciSendString("play p3", NULL, 0, NULL);
		Sleep(7000);
		mciSendString("open p4.mp3 alias p4", NULL, 0, 0);
		mciSendString("play p4", NULL, 0, NULL);
		Sleep(9000);
		flag_dialogue = 0;
		flag_D = 1;
		flag_draw = 0;
	}

	settextcolor(RGB(255, 63, 63));
	settextstyle(30, 30, "新宋体");
	
	if (flag_D == 1)
	{
		if (GetAsyncKeyState(68))//d键右走
		{
			people.x_speed = 10;
			people.direction = right;
			move();
			flag_D = 0;
			flag_A = 1;
		}
	}
	if (flag_A == 1)
	{
		HitDet();
		if (people.flying == false)//在空中时，x速度不衰减，在地面时，松开方向键x速度归零
			people.x_speed = 0;
		if (GetAsyncKeyState(65))//
		{
			people.x_speed = 10;
			people.direction = left;
			move();
			flag_A = 0;
			flag_space = 1;
		}
	}
	if (flag_space == 1)
	{
		HitDet();
		if (people.flying == false)//在空中时，x速度不衰减，在地面时，松开方向键x速度归零
			people.x_speed = 0;
		if (GetAsyncKeyState(32))//
		{
			people.y_speed = -20;
			move();
			flag_space = 0;
			flag_R = 1;
		}
	}
	if (flag_R == 1 && people.y_speed == 0)
	{
		if (GetAsyncKeyState(82))//
		{
			flash();
			move();
			flag_R = 0;
			flag_B = 1;
		}
	}
	if (flag_B == 1)
	{
		if (GetAsyncKeyState(66))//d键右走
		{
			BackTrack();
			move();
			flag_B = 0;
			first_game_flag = 0;
		}
	}
}

void end_menu()
{
		int count = 0;
		int sb = 0;
		int hit = 0;
		while (!hit)
		{
			if (sb == 0)
			{
				putimage(0, 0, &gameover1);
				count++;
				if (count == 90)
				{
					count = 0;
					sb = 1;
				}
			}
			if (sb == 1)
			{
				putimage(0, 0, &gameover2);
				count++;
				if (count == 50)
				{
					count = 0;
					sb = 0;
				}
			}
			Sleep(10);
			if (595 <= m2.x && m2.x <= 707 && 477 <= m2.y && m2.y <= 529)//重新开始
			{
				if (m2.lbutton == true)
				{
					hit = 1;
					game_again_flag = 1;
					people.life = 100;
					back_storage_position();
					main();
				}
			}

			if (597 <= m2.x && m2.x <= 713 && 556 <= m2.y && m2.y <= 588)//退出游戏
			{
				if (m2.lbutton == true)
				{
					hit = 1;
					exit(0);
				}
			}
		}
	
}

DWORD WINAPI mouse_position(LPVOID ms)
{
	while (1)
	{
		m2 = getmessage(EX_MOUSE);//获取鼠标状态给m2
	}

	return 0;
}

DWORD WINAPI draw(LPVOID d)
{
	while (flag_draw == 1)
	{
		GameDraw();
	}
	return 0;
}

int main()
{
	if (game_again_flag == 0)
	{
		GameInit();
		CreateThread(NULL, NULL, Star, NULL, NULL, NULL);
		start_menu();
		CreateThread(NULL, NULL, RecordPosition, NULL, NULL, NULL);
		CreateThread(NULL, NULL, cd_timer, NULL, NULL, NULL);
		CreateThread(NULL, NULL, mouse_position, NULL, NULL, NULL);
	}

	while (people.life > 0)
	{
		GameDraw();

		if (first_game_flag == 1)
		first_game();

		if (first_game_flag == 0)
		KeyControl();

		move();
		HitDet();

		if (first_game_flag == 0 && endflag == 0)
		dialogue_play();
	}
	EndGame();
	end_menu();
}
