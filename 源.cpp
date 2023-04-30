#include<��ͷ.h>

// ��ʼ������
void InitStar(int i)
{
	star[i].x = 0;
	star[i].y = rand() % 700;
	star[i].step = (rand() % 5000) / 1000.0 + 1;
	star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5);	// �ٶ�Խ�죬��ɫԽ��
	star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}

// �ƶ�����
void MoveStar(int i)
{
	// ����ԭ��������
	putpixel((int)star[i].x, star[i].y, 0);

	// ������λ��
	star[i].x += star[i].step;
	if (star[i].x > 1300)
		InitStar(i);

	// ��������
	putpixel((int)star[i].x, star[i].y, star[i].color);
}

// ����������
DWORD WINAPI Star(LPVOID lpparame)
{
	srand((unsigned)time(NULL));	// �������			

	// ��ʼ����������
	for (int i = 0; i < MAXSTAR; i++)
	{
		InitStar(i);
		star[i].x = rand() % 1300;
	}
	// �����ǿ�
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
	while (_kbhit())
	{
		switch (_getch())
		{
		case 'd':
		case 'D':
		case 39:
			//printf("��");
			people.direction = right;
			people.x_speed = 10;
			move();
			flag = 1;
			break;

		case 'a':
		case 'A':
		case 37:
			//printf("��");
			people.direction = left;
			people.x_speed = 10;
			move();
			flag = 1;
			break;
			//case 'w':
			//case 'W':
				//old_bk_y = people.foot_y - 422;
				//people.foot_y -= people.y_speed;
				//bk_y = people.foot_y - 422;
				//break;
			//case 's':
			//case 'S':

				//old_bk_y = people.foot_y - 422;
				//people.foot_y += people.y_speed;
				//bk_y = people.foot_y - 422;
			//	break;

		case 32://�ո���Ծ
			if (people.flying == false)//��ֹ����ʱʹ����Ծ
			{
				//printf("��");
				people.y_speed = -20;
				people.x_speed = 10;
				flag = 1;
				move();
				music_jump_flag = 1;
				CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
			}
			break;

		case 'b':
			BackTrack();
			break;

		case 13://�س�����
			if (3350 <= people.foot_x && people.body_left_x <= 3455 && people.foot_y == 825)//1�Ŵ�����
			{
				ChangeLevel(2);
			}
			break;

		case 'r':
		case 'R':
			flash();
			break;
		}
		//while(_kbhit())
		//_getch();

		time(&now1);
		localtime_s(&t1, &now1);
		//�����ƶ���Ч����
		if (flagm == 0)
		{
			msc2 = (t1.tm_min * 60 + t1.tm_sec) * 1000 + t.wMilliseconds;//�ú����¼��ǰʱ��
			if (msc2 - msc1 >= 200 || msc1 == 1)//���ýŲ������ʱ��
				flagm = 1;
		}

		if (flagm == 1)
		{
			msc1 = (t1.tm_min * 60 + t1.tm_sec) * 1000 + t.wMilliseconds;//�ú����¼��ǰʱ��
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
	if (people.flying == false)//�ڿ���ʱ��x�ٶȲ�˥�����ڵ���ʱ���ɿ������x�ٶȹ���
		people.x_speed = 0;
	if (flag == 0)//δ�û�����
		x = 0;
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
		if (x == 360)//���������ͼƬ��ص���һ��
			x = 0;
		if (people.flying == false)//��ֹ����ʱʹ����Ծ
			while (_kbhit())//�����ƶ�ʱ�����Ծ
			{
				switch (_getch())
				{
				case 32:
					people.y_speed = -20;
					music_jump_flag = 1;
					CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
					break;
				}
			}
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
		if (x == 360)//���������ͼƬ��ص���һ��
			x = 0;

		if (people.flying == false)//��ֹ����ʱʹ����Ծ
			while (_kbhit())//�����ƶ�ʱ�����Ծ
			{
				switch (_getch())
				{
				case 32:
					people.y_speed = -20;
					music_jump_flag = 1;
					CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
					break;
				}
			}
	}

	if (people.y_speed != 0 || people.flying == true)//��ֱ�����ϵ��ƶ�
	{
		old_bk_y = people.foot_y - 425;
		people.foot_y += people.y_speed;
		bk_y = people.foot_y - 425;
		windows.y1 += people.y_speed;
		windows.y2 += people.y_speed;

		people.y_speed += 2;//ģ������Ϊ2

		HitDet();
		if (people.flying == false)//����Ƿ����ʵ����
		{
			people.y_speed = 0;
			music_load_flag = 1;
			CreateThread(NULL, NULL, musicplay, NULL, NULL, NULL);
		}
	}

}

void HitDet()//��ײ�����ռ��
{
	int flag = 0;
	//ǽ����ײ
	for (int i = 0; i < NumOfWall; i++)//�ж��Ƿ�����
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

	for (int i = 0; i < NumOfTrap; i++)//���崥�����
	{
		if (((trap[i].x1 <= people.body_left_x && people.body_left_x <= trap[i].x2) ||
			(trap[i].x1 <= people.body_right_x && people.body_right_x <= trap[i].x2)) &&
			(trap[i].y1 <= people.foot_y && people.foot_y <= trap[i].y2))
		{
			if (people.life > 0)
			{
				people.life -= 10;
				if (people.direction == right)//����Ч��
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
				if (people.direction == left)//����Ч��
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

	if (NumOfPile > 0)
	{
		for (int i = 0; i < NumOfPile; i++)
		{
			if ((pile[i].x_left + 5 <= people.body_left_x && people.body_left_x <= pile[i].x_right) || (pile[i].x_left + 5 <= people.body_right_x && people.body_right_x <= pile[i].x_right))
				if ((people.head_y <= pile[i].y_up && pile[i].y_up <= people.foot_y) || (people.head_y <= pile[i].y_down && pile[i].y_down <= people.foot_y))
				{
					people.life -= 10;
					for (int n = i; n < NumOfPile - 1; n++)
						pile[n] = pile[n + 1];
					NumOfPile--;
					pile = (struct pile*)realloc(pile, sizeof(struct pile) * NumOfPile);
				}
		}

	}
}

void GameDraw()
{
	cleardevice();
	BeginBatchDraw();//˫�����ֹ����
	char c1[50];
	char c2[50], c3[50], c4[50], c5[50], c6[50], c7[50];
	sprintf(c1, "%d", people.foot_x);
	sprintf(c2, "%d", people.foot_y);
	sprintf(c3, "%d", people.body_left_x);
	sprintf(c4, "%d", people.body_right_x);
	sprintf(c5, "%d", people.past_foot_x);
	sprintf(c6, "%d", people.past_foot_y);
	sprintf(c7, "%d", people.life);

	if (level_num == 1)//��һ��
	{
		putimage(0, 0, 1600, 700, &bk, bk_x, bk_y);//����ͼƬ

		setfillcolor(RGB(255, 63, 63));//ǽ����ɫ
		if (wall[2].x1 <= windows.x2 || wall[2].x2 >= windows.x1)
			fillrectangle(wall[2].x1 - windows.x1, wall[2].y1 - windows.y1, wall[2].x2 - windows.x1, wall[2].y1 - windows.y1 + 5);
		if (flags == 1)//ʵ��ǽ���Ҷ�
		{
			wall[2].x1 += 10;
			wall[2].x2 += 10;
			count++;
			if (count == 8)
			{
				flags = 0;
				count = 0;
			}
		}
		if (flags == 0)//ʵ��ǽ���Ҷ�
		{
			wall[2].x1 -= 10;
			wall[2].x2 -= 10;
			count++;
			if (count == 8)
			{
				flags = 1;
				count = 0;
			}
		}
	}

	if (level_num == 2)//�ڶ���
	{
		putimage(0, 0, 1600, 700, &bk2, bk_x, bk_y);//����ͼƬ

		//�����ڵ�
		time(&now1);
		localtime_s(&pile_timer[1], &now1);
		if (pile_flag == 1)
		{
			NumOfPile++;
			if (NumOfPile == 1)
				pile = (struct pile*)malloc(sizeof(struct pile) * NumOfPile);
			if (NumOfPile > 1)
				pile = (struct pile*)realloc(pile, sizeof(struct pile) * NumOfPile);
			pile[NumOfPile - 1].x_left = 2230;
			pile[NumOfPile - 1].x_right = pile[NumOfPile - 1].x_left + 65;
			pile[NumOfPile - 1].y_up = 950;
			pile[NumOfPile - 1].y_down = pile[NumOfPile - 1].y_up + 20;
			pile_flag = 0;

			time(&now1);
			localtime_s(&pile_timer[0], &now1);
		}
		if ((pile_timer[1].tm_min * 60 + pile_timer[1].tm_sec) - (pile_timer[0].tm_min * 60 + pile_timer[0].tm_sec) >= 5)
			pile_flag = 1;

		if (NumOfPile > 0)
		{
			for (int i = 0; i < NumOfPile; i++)
			{
				putimage(pile[i].x_left - windows.x1, pile[i].y_up - windows.y1, 68, 25, &pile1, 0, y1, SRCAND);
				putimage(pile[i].x_left - windows.x1, pile[i].y_up - windows.y1, 68, 25, &pile2, 0, y1, SRCPAINT);
			}
			y1 += 25;
			if (y1 == 300)
				y1 = 0;
			for (int i = 0; i < NumOfPile; i++)
			{
				pile[i].x_left -= 5;
				pile[i].x_right -= 5;
			}
		}
	}

	//Ѫ�����ơ�
	setfillcolor(RGB(255, 63, 63));//Ѫ������ɫ
	fillrectangle(70, 50, 470 - 400 * (100 - people.life) * 0.01, 60);
	if (people.life != 100)
		setfillcolor(RGB(46, 46, 46));//��Ѫ������ɫ
	fillrectangle(470 - 400 * (100 - people.life) * 0.01, 50, 470, 60);

	//�������
	if (people.direction == right)
	{
		putimage(400, 343, 90, 90, &people_right_2, x, y, SRCAND);//����ͼƬ
		putimage(400, 343, 90, 90, &people_right, x, y, SRCPAINT);
	}
	if (people.direction == left)
	{
		putimage(400, 343, 90, 90, &people_left_2, x, y, SRCAND);//����ͼƬ
		putimage(400, 343, 90, 90, &people_left, x, y, SRCPAINT);
	}


	//fillrectangle(0, 422, 1300, 427);
	//line(455, 0, 455, 700);
	//line(433, 0, 433, 700);
	//line(0, 425, 1300, 425);
	//line(0, 350, 1300, 350);

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

	EndBatchDraw();
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

	level_num = 1;//�ؿ���

	bk_x = people.foot_x - 445;
	bk_y = people.foot_y - 425;
	old_bk_x = people.foot_x - 445;
	old_bk_y = people.foot_y - 425;

	NumOfOldPosition = 1;//���ݹ��ܲ���

	windows.x1 = 0;//��������������
	windows.x2 = 1300;
	windows.y1 = 0;
	windows.y2 = 700;

	//ǽ��������岿��
	//��һ��
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
	//�ڶ���
	wall[4].x1 = 300;
	wall[4].x2 = 1300;
	wall[4].y1 = 1000;
	wall[5].x1 = 1500;
	wall[5].x2 = 2500;
	wall[5].y1 = 1000;

	//�������
	//�ڶ���
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
	trap[3].y2 = 1000;

	//�ؿ����������꣬�ԽŲ�����Ϊ��׼
	level[0].start_x = 445;
	level[0].start_y = 425;
	level[1].start_x = 330;
	level[1].start_y = 1000;

	NumOfPile = 0;//�ڵ���

	count = 0;
	flags = 0;
	flagm = 1;
	step = 1;//�Ų���ѭ������
	dialogue_no = 1;//����̨��ѭ������
	dialogue_flag = 1;
	pile_flag = 1;//�����ڵ�����

	mouse = false;
	msc1 = 0;

	time(&now1);//��ʼ����ʱ����һ
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
	loadimage(&pile1, "�ڵ�1.png");
	loadimage(&pile2, "�ڵ�2.png");

	mciSendString("close bgm1", NULL, 0, NULL);
	mciSendString("open bgm1.mp3 alias bgm1", 0, NULL, 0);
	mciSendString("play bgm1 repeat", NULL, 0, NULL);
}

DWORD WINAPI RecordPosition(LPVOID postion)//����һ���߳�����¼��ɫ����ǰλ��
{
	while (1)
	{
		GetSystemTime(&t);
		time(&now2);
		localtime_s(&t2, &now2);
		int msc = (t2.tm_min * 60 + t2.tm_sec) * 1000 + t.wMilliseconds;//�ú����¼��ǰʱ��
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
		if (msc - TIME[0] >= 3000)//��TIME[0]��ʱ��δ������������������д����ʱ����Ҳһ��δ����3��
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
}

DWORD WINAPI musicplay(LPVOID lpparamer)//ʹ�ö��߳̽�����ֲ��ŻῨ������
{
	if (music_move_flag == 1)//�Ų���
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

	if (music_back_flag == 1)//������
	{
		mciSendString("close back", NULL, 0, NULL);
		mciSendString("open ����.MP3 alias back", NULL, 0, NULL);
		mciSendString("play back", NULL, 0, NULL);
		music_back_flag = 0;
	}

	if (music_jump_flag == 1)//��Խ��
	{
		mciSendString("close jump", NULL, 0, NULL);
		mciSendString("open jump.MP3 alias jump", NULL, 0, NULL);
		mciSendString("play jump", NULL, 0, NULL);
		music_jump_flag = 0;
	}

	if (music_load_flag == 1)//�����
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
	old_x[NumOfOldPosition - 1] = level[a - 1].start_x;
	old_y[NumOfOldPosition - 1] = level[a - 1].start_y;
	old_speed_y[NumOfOldPosition - 1] = 0;
}

void flash()//����
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
		dialogue_flag = 1;//����̨�ʲ��ż�ʱ��

	//printf("\n%d", dialogue_no);
}

DWORD WINAPI dialogue(LPVOID dialogue)
{
	switch (dialogue_no)
	{
	case 1:
		mciSendString("close dialogue1", NULL, 0, NULL);
		mciSendString("open ����̨��1.mp3 alias dialogue1", NULL, 0, 0);
		mciSendString("play dialogue1", NULL, 0, NULL);
		break;
	case 2:
		mciSendString("close dialogue2", NULL, 0, NULL);
		mciSendString("open ����̨��2.mp3 alias dialogue2", NULL, 0, 0);
		mciSendString("play dialogue2", NULL, 0, NULL);
		break;
	case 3:
		mciSendString("close dialogue3", NULL, 0, NULL);
		mciSendString("open ����̨��3.mp3 alias dialogue3", NULL, 0, 0);
		mciSendString("play dialogue3", NULL, 0, NULL);
		break;
	case 4:
		mciSendString("close dialogue4", NULL, 0, NULL);
		mciSendString("open ����̨��4.mp3 alias dialogue4", NULL, 0, 0);
		mciSendString("play dialogue4", NULL, 0, NULL);
		break;
	case 5:
		mciSendString("close dialogue5", NULL, 0, NULL);
		mciSendString("open ����̨��5.mp3 alias dialogue5", NULL, 0, 0);
		mciSendString("play dialogue5", NULL, 0, NULL);
		break;
	case 6:
		mciSendString("close dialogue6", NULL, 0, NULL);
		mciSendString("open ����̨��6.mp3 alias dialogue6", NULL, 0, 0);
		mciSendString("play dialogue6", NULL, 0, NULL);
		break;
	case 7:
		mciSendString("close dialogue7", NULL, 0, NULL);
		mciSendString("open ����̨��7.mp3 alias dialogue7", NULL, 0, 0);
		mciSendString("play dialogue7", NULL, 0, NULL);
		break;
	case 8:
		mciSendString("close dialogue8", NULL, 0, NULL);
		mciSendString("open ����̨��8.mp3 alias dialogue8", NULL, 0, 0);
		mciSendString("play dialogue8", NULL, 0, NULL);
		break;
	case 9:
		mciSendString("close dialogue9", NULL, 0, NULL);
		mciSendString("open ����̨��9.mp3 alias dialogue9", NULL, 0, 0);
		mciSendString("play dialogue9", NULL, 0, NULL);
		break;
	case 10:
		mciSendString("close dialogue10", NULL, 0, NULL);
		mciSendString("open ����̨��10.mp3 alias dialogue10", NULL, 0, 0);
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

		//������
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
		m = getmessage(EX_MOUSE);//��ȡ���״̬��m
		system("cls");
		printf("x=%d y=%d\n", m.x, m.y);
		if (580 < m.x && m.x < 710 && 500 < m.y && m.y < 535)//�ж�����Ƿ��ڰ�ť��
		{
			m = getmessage(EX_MOUSE);
			if (m.lbutton)//�ж�����Ƿ���
				break;
		}
	}
	mouse = true;
	return 0;
}

DWORD WINAPI draw_pile(LPVOID PILE)
{
	int x1, y1 = 0;
	while (1)
	{
		//BeginBatchDraw();
		pile[0].x_left = 2230;
		pile[0].x_right = pile[0].x_left + 65;
		pile[0].y_up = 950;
		pile[0].y_down = pile[0].y_up + 20;
		for (int i = 0; i < NumOfPile; i++)
		{
			//if (pile[0].x_left <= windows.x2 || pile[0].x_right >= windows.x1)
			putimage(pile[i].x_left - windows.x1, pile[1].y_up - windows.y1, 68, 25, &pile1, 0, y1, SRCAND);
			putimage(pile[i].x_left - windows.x1, pile[1].y_up - windows.y1, 68, 25, &pile2, 0, y1, SRCPAINT);
		}
		y1 += 25;
		if (y1 == 300)
			y1 = 0;
		//Sleep(58);
		//EndBatchDraw();
	}
}

int main()
{
	GameInit();

	CreateThread(NULL, NULL, Star, NULL, NULL, NULL);
	start_menu();
	CreateThread(NULL, NULL, RecordPosition, NULL, NULL, NULL);
	//CreateThread(NULL, NULL, draw_pile, NULL, NULL, NULL);
	while (1)
	{
		GameDraw();
		KeyControl();
		move();
		HitDet();
		dialogue_play();
	}
}
