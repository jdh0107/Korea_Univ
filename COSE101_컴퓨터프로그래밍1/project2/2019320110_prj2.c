#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>

/*����ó�� ���
	1. ������ �ٽ� �ȵſ� �Ф�
	2. �� �ϳ��� ������ �ٷ� ��������
*/
// COSE101, Spring Semester, 2019
// Computer Science and Engineering, College of Informatics, Korea University, Seoul.

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define SPACE 32 
#define p 112 
#define P 80 
#define ESC 27

#define MAP_ADJ_X 10
#define MAP_ADJ_Y 5
#define X 8
#define Y 5

typedef struct _block {
	int pos_x; //position of block
	int pos_y;
	int num;
	int nextnum;
	int isactive;
}Block;

Block block, block2;

int time_interval_moveBlockDown = 100;
int goal = 64;
int currentStage = 0;
int score = 0;
int best_score = 0;
int gameScreen[X][Y] = { 0 };

/* 'DO NOT MODIFY THESE FUNCTIONS.'		'�� �Լ����� ��ġ�� ������.'		 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd���� Ŀ���� x, y ��ġ�� ����

								  /* display fuctions */
void goMainMenuScreen(); // ���θ޴��� �̵� Moving to main menu.
void printGameScreen();  // ����ȭ�� ���	 Printing game screen.
void printPauseScreen(); // �Ͻ� ���� ȭ��	 Prining pause screen.
void printEndScreen(int endType); //  // �������� ���� ȭ�� ���. ��������Ŭ����, ��罺������Ŭ����, �й�.
									//Printing result of end of stages including 'cleared the stage', 'cleared whole stages', and 'lost'.
void clearScreen();
/* 'DO NOT MODIFY THESE FUNCTIONS.' ends	'�� �Լ����� ��ġ�� ������' ��.	*/

		/* @ --- You can modify following functions ----  �� �Ʒ����ִ� �Լ����� ��ġ����.  */
//Implement these functions below to complete your project.
//�� �Լ����� �����ϼż� ������Ʈ�� �ϼ��ϼ���.

		/* Block generating functions */
int getRandomNumber();
void newBlock();

/* block control and merging functions  */
void flipBlock();					// Fliping function.
void moveBlock(int direction);		// �Է� ���� �������� ��� �̵�		Moving blocks to corresponding direction of input key.
int takeBlockControl(int*);				//��� ���� ���� �Է� ����			Taking input for moving blocks
int checkAdjacentBlock(int, int, int**);	//Checking adjacent blocks for merging conditions
void checkNumber(int, int, int*);			// ��� ���� ��ϼ��� Ȯ��			Checking number of adjacent block
int hor_able(int, int);				// ���� ��갡�ɼ� Ȯ��
int ver_able(int, int);				// ���� ��갡�ɼ� Ȯ��
void hor_movedown(int, int, int);	// ��� ��� ������
void ver_movedown(int, int);		// ��� ��� ������
void score64(int, int, int***);		// 64 ����� ����üũ

		/* game flow control functions */
int isStageEnd(int*);  // ���������� �������� üũ. (�������� Ŭ����, ��� �������� Ŭ����, �й�)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // �� ���������� �´� ���� ȯ�� ����.   Setting up status values for each stages.

void main() {
	system("mode con cols=120 lines=38");
	srand(time(NULL));
	removeCursorFromCMD();
	goMainMenuScreen();

	int count = 0;
	while (1) {
		printGameScreen();

		for (int j = 0; j < 5; j++) {
				if (takeBlockControl(&count) == SPACE) break;
				printGameScreen();
				Sleep(time_interval_moveBlockDown);
		}
		moveBlock(DOWN);
		printGameScreen();
		if (!block.isactive && !block2.isactive) { // �� �κ��� �������� ����� �ٴ��̳� �ٸ���Ͽ� ��Ҵ��� üũ�մϴ�. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
			checkNumber(block2.pos_x, block2.pos_y, &count);
			if (isStageEnd(&count) != 1)
      				newBlock();
		}
	}
}

int getRandomNumber() {
	/*
	���� Ȯ���� 2, 4, 8 �� �ϳ��� �����Ͽ� �� ���� ������.
	Generate 2, 4, or 8 with certain percentage and return the number.
	*/

	int number = 0;
	int ratioVar = (rand() % 10 + 1);

	if (1 <= ratioVar && ratioVar <= 5)
		number = 2;
	else if (6 <= ratioVar && ratioVar <= 8)
		number = 4;
	else if (9 <= ratioVar && ratioVar <= 10)
		number = 8;
	// You should modify example value for complete this function.
	return number; // example retrun value
}

char getRandomOper() {
	/*
	���� Ȯ���� +, - �� �ϳ��� �����Ͽ� �� ���� ������.
	Generate +, or - with certain percentage and return the number.
	*/
	char oper;

	if (currentStage <= 3) {
		oper = '+';
	}
	else {
		if (rand() % 2)
			oper = '+';
		else
			oper = '-';
	}
	// You should modify example value for complete this function. ���ð��� ��ġ�ż� �Լ��� �ϼ��ϼ���.
	return oper; // example retrun value 	
}

void newBlock() {
	/*
	�� �Լ��� ������ ���� �������� �����ϴ� ��ġ�� �Է��ؼ� �� ����� ����ϴ�.
	�׸��� ����, ���� ��Ͽ� ���� �� ���� �����մϴ�.
	This fuction makes new blocks with setting generated value and the initial position where starts falling down.
	After that, generate new number for value of the next block.
	*/

	// You should modify these example values for complete this function.
		
		if (!block.num && !block2.num) {	//�� ó�� ���°Ÿ�
			block.num = getRandomNumber();
			block2.num = getRandomOper();
			block.nextnum = getRandomNumber();
			block2.nextnum = getRandomOper();
		}
	
		else {
			block.num = block.nextnum;
			block2.num = block2.nextnum;
			block.nextnum = getRandomNumber();
			block2.nextnum = getRandomOper();
		}

		block.pos_x = 0; // example value of position ���ð�
		block.pos_y = (rand() % 4); // example value of position ���ð�
		block.isactive = 1;
		gameScreen[block.pos_x][block.pos_y] = block.num;

		block2.pos_x = 0; // example value of position ���ð�
		block2.pos_y = block.pos_y + 1; // example value of position ���ð�
		block2.isactive = 1;
		gameScreen[block2.pos_x][block2.pos_y] = block2.num;
}

int takeBlockControl(int *count) {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //����Ű�ΰ��  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//����Ű���ð��� ����  dropping the value of direction
			switch (input_blockControl) {
			case UP:
				flipBlock();
				printGameScreen();
				break;
				/*
				Implement cases of each dilection key
				�� ����Ű�� ���� ������ �����ϼ���
				*/
			case DOWN:
				moveBlock(DOWN);
				printGameScreen();
				break;

			case LEFT:
				moveBlock(LEFT);
				printGameScreen();
				break;

			case RIGHT:
				moveBlock(RIGHT);
				printGameScreen();
				break;

			}
		}
		else { //����Ű�� �ƴѰ��  Cases for other keys
			int original, original2;

			switch (input_blockControl) {
				/*
				Implement hard drop
				�ϵ����� �����ϼ���
				*/

			case SPACE:

				original = block.pos_x;
				original2 = block2.pos_x;

				while (!(gameScreen[block.pos_x + 1][block.pos_y])) {
					if (block.pos_x == X - 1)
						break;
					block.pos_x++;
				}

				while (!(gameScreen[block2.pos_x + 1][block2.pos_y])) {
					if (block2.pos_x == X - 1)
						break;
					block2.pos_x++;
				}

				gameScreen[original][block.pos_y] = 0;
				gameScreen[original2][block2.pos_y] = 0;

				gameScreen[block.pos_x][block.pos_y] = block.num;
				gameScreen[block2.pos_x][block2.pos_y] = block2.num;

				printGameScreen();
				score += 5;
				while (_kbhit()) _getch(); //Ű���۸� ���. Emptying key buffer.
				return SPACE;

			case P:
			case p:
				printPauseScreen();
				break;
			case ESC:
				*count = 0;
				printEndScreen(0);
				break;
			}
		}
	}
	while (_kbhit()) _getch(); //Ű���۸� ���. Emptying key buffer.
	return 0;
}

void flipBlock(void) {//�¿� �����Լ�		Block flipping function
		/*
		Fliping blocks when the 'up' butten is pressed.
		���� ��ư�� ��������� ����� �ٲߴϴ�.
		*/
	int temp;
	temp = block.num;
	block.num = block2.num;
	block2.num = temp;
	gameScreen[block.pos_x][block.pos_y] = block.num;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;
}

void moveBlock(int direction) { // ��,��,�Ʒ� �Է½� ������ �Լ�	Moving blocks for left, reight, and down input
	switch (direction) {
	case LEFT:
		/*
		implement left action
		*/
		if (block.pos_y > 0 && gameScreen[block.pos_x][block.pos_y - 1] == 0) {
			gameScreen[block.pos_x][block.pos_y - 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y--;
		}
		if (block2.pos_y > 1 && gameScreen[block2.pos_x][block2.pos_y - 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y - 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y--;
		}
		break;

	case RIGHT:
		/*
		implement right action
		*/
		if (block2.pos_y < Y - 1 && gameScreen[block2.pos_x][block2.pos_y + 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y + 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y++;
		}
		if (block.pos_y < Y - 1 && gameScreen[block.pos_x][block.pos_y + 1] == 0) {
			gameScreen[block.pos_x][block.pos_y + 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y++;
		}
		break;

	case DOWN:
		if (block.pos_x < X - 1 && gameScreen[block.pos_x + 1][block.pos_y] == 0) {
			gameScreen[block.pos_x + 1][block.pos_y] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_x++;

			if (block.pos_x == X - 1 || gameScreen[block.pos_x + 1][block.pos_y] != 0) {
				block.isactive = 0;
			}
		}
		else {
			block.isactive = 0;
		}

		if (block2.pos_x < X - 1 && gameScreen[block2.pos_x + 1][block2.pos_y] == 0) {
			gameScreen[block2.pos_x + 1][block2.pos_y] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_x++;

			if (block2.pos_x == X - 1 || gameScreen[block2.pos_x + 1][block2.pos_y] != 0) {
				block2.isactive = 0;
			}
		}
		else {
			block2.isactive = 0;
		}

		if (block.isactive == 0 || block2.isactive == 0) {
			Sleep(time_interval_moveBlockDown);
			int original = block.pos_x;
			int original2 = block2.pos_x;

			while (!(gameScreen[block.pos_x + 1][block.pos_y])) {
				if (block.pos_x == X - 1)
					break;
				block.pos_x++;
			}

			while (!(gameScreen[block2.pos_x + 1][block2.pos_y])) {
				if (block2.pos_x == X - 1)
					break;
				block2.pos_x++;
			}

			gameScreen[original][block.pos_y] = 0;
			gameScreen[original2][block2.pos_y] = 0;

			gameScreen[block.pos_x][block.pos_y] = block.num;
			gameScreen[block2.pos_x][block2.pos_y] = block2.num;
		}
	}
}

int checkAdjacentBlock(int x, int y, int **count) { //Merging ���� Ȯ�� �Լ�		Checking merging condition
	 /*
	 ����Ȯ���Ͽ� �����ϰ� Merging
	  64 ������ ���� ���

	 Implement checking condition of Merging.
	 If 64 is found, player earns points.
	 */
	int merged = 0;
	int t = 0;
	int t2 = 0;

	/*���� ����*/
	if( hor_able(x, y) )
		if (y == 2) {								// a �� b
			switch (t = hor_able(x, y)) {
			case 1:									// ����
				gameScreen[x][y - 1] = gameScreen[x][y - 1] + gameScreen[x][y + 1];
				gameScreen[x][y] = 0;
				gameScreen[x][y + 1] = 0;
				printGameScreen();
				Sleep(200);

				score64(x, y - 1, &count);
				hor_movedown(x, y, 2);
				merged++;
				break;

			case 2:									//�E��
				gameScreen[x][y - 1] = abs(gameScreen[x][y - 1] - gameScreen[x][y + 1]);
				if (gameScreen[x][y - 1] == 0) {
					gameScreen[x][y - 1] = 999;		//* ��� ����
				}
				gameScreen[x][y] = 0;
				gameScreen[x][y + 1] = 0;
				printGameScreen();
				Sleep(200);

				score64(x, y - 1, &count);
				hor_movedown(x, y, 2);
				merged++;
				break;
			default:
				break;
			}
		}

		else if (y == 1 || y == 3) {
			if ( hor_able(x, 1) && hor_able(x, 3)) {	// a �� b �� c
				switch (hor_able(x, 1)) {
				case 1:								// a + b �� c
					switch (hor_able(x, 3)) {
					case 1:							// a + b + c
						gameScreen[x][0] = gameScreen[x][0] + gameScreen[x][2] + gameScreen[x][4];
						merged++;
						break;
					case 2:							// a + b - c
						gameScreen[x][0] = abs((gameScreen[x][0] + gameScreen[x][2]) - gameScreen[x][4]);
						if (gameScreen[x][0] == 0) {
							gameScreen[x][0] = 999;		//* ��� ����
						}
						merged++;
						break;
					default:
						break;
					}
					break;

				case 2:								// a - b �� c
					switch (hor_able(x, 3)) {
					case 1:							// a - b + c
						gameScreen[x][0] = abs((gameScreen[x][2] + gameScreen[x][4]) - gameScreen[x][0]);
						if (gameScreen[x][0] == 0) {
							gameScreen[x][0] = 999;		//* ��� ����
						}
						merged++;
						break;
					case 2:							// a - b - c
						gameScreen[x][0] = abs(abs(gameScreen[x][0] - gameScreen[x][2]) - gameScreen[x][4]);
						if (gameScreen[x][0] == 0) {
							gameScreen[x][0] = 999;		//* ��� ����
						}
						merged++;
						break;
					default:
						break;
					}
					break;

				default:
					break;
				}

				score64(x, 0, &count);				//64�� ���ְ� ���� ����
				for (int i = 1; i < Y; i++)
					gameScreen[x][i] = 0;			// [x][1] ~ [x][4] �� ĭ ����
				hor_movedown(x, 1, 4);				// ��� ����

				printGameScreen();
				Sleep(200);
			}

			else if (hor_able(x, y)) {				// a �� b
				switch (hor_able(x, y)) {
				case 1:									// ����
					gameScreen[x][y - 1] = gameScreen[x][y - 1] + gameScreen[x][y + 1];
					gameScreen[x][y] = 0;
					gameScreen[x][y + 1] = 0;

					printGameScreen();
					Sleep(200);

					score64(x, y - 1, &count);
					hor_movedown(x, y, 2);
					merged++;
					break;

				case 2:									//�E��
					gameScreen[x][y - 1] = abs(gameScreen[x][y - 1] - gameScreen[x][y + 1]);
					if (gameScreen[x][y - 1] == 0) {
						gameScreen[x][y - 1] = 999;		//* ��� ����
					}
					gameScreen[x][y] = 0;
					gameScreen[x][y + 1] = 0;

					printGameScreen();
					Sleep(200);

					score64(x, y - 1, &count);
					hor_movedown(x, y, 2);
					merged++;
					break;
				default:
					break;
				}
			}
		}

	/* ���� ���� */
	if ( t = ver_able(x, y)) {
		switch ( t ) {
		case 1:
			gameScreen[x + 1][y] = gameScreen[x + 1][y] + gameScreen[x - 1][y];
			merged++;
			break;
		case 2:
			gameScreen[x + 1][y] = abs(gameScreen[x + 1][y] - gameScreen[x - 1][y]);
			if (gameScreen[x + 1][y] == 0) {
				gameScreen[x + 1][y] = 999;		//* ��� ����
			}
			merged++;
			break;
		default:
			break;
		}

		gameScreen[x][y] = 0;
		gameScreen[x - 1][y] = 0;

		printGameScreen();
		Sleep(200);

		score64(x + 1, y, &count);
		ver_movedown(x, y);
	}
	
	return merged;
}

void checkNumber(int x, int y, int *count) {
	checkAdjacentBlock(x, y, &count);						//�ϴ� ���� ������ ����
	printGameScreen();
	Sleep(200);
	
	for (int i = 1; i < X; i++) {							//�»�ܺ��� ���鼭 ���� �� �� �ִ� �� �� ã��
		for (int j = Y - 1; j >= 0; j--) {
			if (checkAdjacentBlock(i, j, &count)) {			//���� ���� ��쿡
				printGameScreen();
				Sleep(200);
				i = 1;
				break;
			}
		}
	}
}

void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  ���������� ���ǵ��� �����ϱ����� �� �Լ��� �ϼ��ϼ���.
	switch (setGameFlowType) {
	case 0:  /* initialize game */
		if (best_score < score)
			best_score = score;

		currentStage = 0;
		score = 0;

	case 1: /* To next stage */
		currentStage++;

		switch (currentStage) {
		case 1:
			time_interval_moveBlockDown = 100;
			goal = 100;
			break;

		case 2:
			time_interval_moveBlockDown = (int) 100 / 1.3;
			goal = 300;
			break;

		case 3:
			time_interval_moveBlockDown = (int) 100 / 1.5;
			goal = 500;
			break;

		case 4:
			time_interval_moveBlockDown = (int) 100 / 1.7;
			goal = 600;
			break;

		case 5:
			time_interval_moveBlockDown = (int) 100 / 2.0;
			goal = 750;
			break;

		case 6:
			time_interval_moveBlockDown = (int) 100 / 2.5;
			goal = 850;
			break;
		}
		clearScreen();
		newBlock();
		break;

	default:
		clearScreen();
		setCoordinate(MAP_ADJ_X + 2, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}

int isStageEnd(int *count) { // ���������� �������� üũ      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {

				if (currentStage == 6) {
					*count = 0;
				}

				currentStage != 6 ? printEndScreen(1) : printEndScreen(2);
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //�й�			Game over

			*count = 0;
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}

int hor_able(int x, int y)
{
	if (1 <= y && y <= 3) {
		if (((gameScreen[x][y]) && (gameScreen[x][y] != 999) && (gameScreen[x][y - 1]) && gameScreen[x][y - 1] != '+' && gameScreen[x][y - 1] != '-' && gameScreen[x][y - 1] != 999)
			&& ((gameScreen[x][y + 1]) && gameScreen[x][y + 1] != '+' && gameScreen[x][y + 1] != '-' && gameScreen[x][y + 1] != 999)) {
			if (gameScreen[x][y] == '+') return 1;
			else if (gameScreen[x][y] == '-') return 2;
			else return 0;
		}
		else return 0;
	}
	else return 0;
}

int ver_able(int x, int y) 
{
	if (1 <= x && x <= 6) {
		if (((gameScreen[x][y]) && (gameScreen[x - 1][y]) && gameScreen[x - 1][y] != '+' && gameScreen[x - 1][y] != '-' && gameScreen[x - 1][y] != 999)
			&& ((gameScreen[x + 1][y]) && gameScreen[x + 1][y] != '+' && gameScreen[x + 1][y] != '-' && gameScreen[x + 1][y] != 999)) {
			if (gameScreen[x][y] == '+') {
				return 1;
			}
			else if (gameScreen[x][y] == '-') {
				return 2;
			}
			else {
				return 0;
			}
		}
		else return 0;
	}
	else return 0;
}

void hor_movedown(int x, int y, int i)
{
	for (int j = 1; j <= i; j++) {
		int k = 1;

		while (x - k >= 0) {
			if (gameScreen[x - k][y - 1 + j]) { //���� ������
				gameScreen[x - k + 1][y - 1 + j] = gameScreen[x - k][y - 1 + j];		//�� ĭ �Ʒ��� ����
				gameScreen[x - k][y - 1 + j] = 0;										//���� �ڸ��� ���
				k++;
			}
			else
				break;							//���� ������ ���� ����
		}
	}
	printGameScreen();
} 

void ver_movedown(int x, int y)
{
	/*�Է� ���� �ڸ��� �� ���� ����*/
	int k = 2;
	while (gameScreen[x - k][y]) {								//���� ������
		gameScreen[x - k + 2][y] = gameScreen[x - k][y];		//�� ĭ ����
		gameScreen[x - k][y] = 0;								//���� �� ����
		k++;
	}
	printGameScreen();
}

void score64(int x, int y, int ***count)
{
	if (gameScreen[x][y] == 64) {
		***count += 1;
		gameScreen[x][y] = 0;					//��� ����
		score += 10 * (***count - 1);
		printGameScreen();

		hor_movedown(x, y, 1);
	}
}

/* @ ����'DO NOT MODIFY FROM HERE.'���� starts		 �� �Ʒ��δ� �������� ������.	*/
void removeCursorFromCMD() { // cmd���� �����̴� Ŀ������ Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void 
setCoordinate(int x, int y) { //cmd���� Ŀ�� ��ġ�� x, y�� ����   Setting curser to (x,y).
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printGameScreen() {
	int i, j, k;

	for (i = 0; i < X; i++) {
		for (j = 0; j < Y; j++) {
			setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
			if (gameScreen[i][j] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("     ��      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     ��      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("             ");
				}
			}

			else {
				for (k = 0; k < 12;) if (1 << (++k) == gameScreen[i][j]) break;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
				if (gameScreen[i][j] == 45 || gameScreen[i][j] == 43)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("���������������������� ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("���������������������� ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("����������������������  ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("����������������������  ");
				}

			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y); printf(" Stage : %4d", currentStage);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 1); printf(" GOAL  : %4d", goal);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 2); printf(" SPEED  : %.1f", 100.0 / (float)time_interval_moveBlockDown);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 3); printf("+-  N E X T  -+ ");
	for (k = 0; k < 12;) if (1 << (++k) == block.nextnum) break;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  ����������������������    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  ����������������������    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  ����������������������    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  ����������������������    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  ��   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("��  �� : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ��   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //����ȭ�� ��� �Լ�. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("��������������������������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�� ��������������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o �� ��������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo �� ������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo �� ������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo �� ����������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�� 64 PuyoPuyo �٦���������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������ 64 PuyoPuyo �� ������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�������� 64 PuyoPuyo �� ����������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������  64 PuyoPuyo �� ������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������  64 PuyoPuyo �� ��������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������  64 PuyoPuyo �� ������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������  64 PuyoPuyo �� ����������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������  64 PuyoPuyo �� ��������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������������  64 PuyoPuyo �� ������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������������  64 PuyoPuyo �� ����������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������������  64 PuyoPuyo �� ��������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������������������  64 PuyoPuyo �� ������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������������������  64 PuyoPuyo �� ����");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������������������  64 PuyoPuyo �� ��");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" �ǢǢǢ� HOW TO CONTROL �ǢǢǢ�");
	setCoordinate(x, y + 7);  printf(" ��                            ��");
	setCoordinate(x, y + 8);  printf(" ��      ��   : Flip           ��");
	setCoordinate(x, y + 9); printf(" ��    ��  �� : Left / Right   ��");
	setCoordinate(x, y + 10); printf(" ��      ��   : Soft Drop      ��");
	setCoordinate(x, y + 11); printf(" ��    SPACE  : Hard Drop      ��");
	setCoordinate(x, y + 12); printf(" ��      P    : Pause          ��");
	setCoordinate(x, y + 13); printf(" ��     ESC   : Quit           ��");
	setCoordinate(x, y + 14); printf(" ��                            ��");
	setCoordinate(x, y + 15); printf(" �ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

	setCoordinate(x, y + 16); printf("          ESC: Exit game.     ");



	while (_kbhit())
		_getch();
	input_command = _getch();


	if (input_command == ESC) {
		clearScreen();
		exit(0);
	}
	else
		setGameFlow(0); /* Reset the game */

}

void printPauseScreen() { // ���� �Ͻ�����		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
	setCoordinate(x, y + 1); printf("��                              ��");
	setCoordinate(x, y + 2); printf("��  +-----------------------+   ��");
	setCoordinate(x, y + 3); printf("��  |       P A U S E       |   ��");
	setCoordinate(x, y + 4); printf("��  +-----------------------+   ��");
	setCoordinate(x, y + 5); printf("��   Press any key to resume.   ��");
	setCoordinate(x, y + 6); printf("��                              ��");
	setCoordinate(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

	_getch();

	system("cls");
	printGameScreen();
}



void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  |  G A M E  O V E R..   |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��   YOUR SCORE: %6d         ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("��   Press any key to restart.  ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 16); printf("      ESC: Back to main menu.     ");

		while (_kbhit()) _getch();
		input_command = _getch();

		if (input_command == ESC) {
			while (_kbhit()) _getch();
			clearScreen();
			goMainMenuScreen();
			return;
		}

		setGameFlow(0); /* Reset the game */
		break;

	case 1:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  |      STAGE CLEAR      |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��   YOUR SCORE: %6d         ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("�� Press any key to next stage. ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-------------------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  | WINNER WINNER CHICKEN DINNER! |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-------------------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��         YOUR SCORE: %6d           ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("��         Cleared final stage.         ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

		while (_kbhit()) _getch();
		input_command = _getch();
		setGameFlow(0); /* Reset the game */
		break;

	default:
		clearScreen();
		setCoordinate(x, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}
void clearScreen() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			gameScreen[i][j] = 0;
		}
	}
	block.pos_x = 0;
	block.pos_x = 0;
	block.num = 0;
	block.nextnum = 0;
	block.isactive = 0;

	block2.pos_x = 0;
	block2.pos_x = 0;
	block2.num = 0;
	block2.nextnum = 0;
	block2.isactive = 0;

	system("cls");
}
/* @ '����DO NOT MODIFY.����'ends. */