//********************************************************************************************************************************************//
//********************************************************************************************************************************************//
//																																			  //
//															STRUCTURED PROGRAMMING LAB PROJECT												  //
//																																			  //
//																TOTAL DESTRUCTION THE GAME													  //
//																																			  //
//																  CREATED AND DESIGNED BY													  //
//																																			  //
//														SHAKLEEN ISHFAR				-	160041029											  //
//														SHEIKH SHAKIB ISHRAK SHOUMO -	160041038											  //
//														IFTA KHAIRUL ALAM ADIL	    -	160041034											  //
//																																			  //
//********************************************************************************************************************************************//
//********************************************************************************************************************************************//

#pragma comment(lib, "legacy_stdio_definitions.lib")

# include "iGraphics.h"
#include <iostream>
#include <cstdlib>

using namespace std;

//********************************************************************************************************************************************//
//															Defining The Window Proportions													  //
//********************************************************************************************************************************************//

#define screenWidth 1280
#define screenHight 720

//********************************************************************************************************************************************//
//																Image names and locatons													  //
//********************************************************************************************************************************************//


//Button and writing Co-ordinates
struct Co_ordinates {
	int x;
	int y;
}button_position[7];

//Button Images
char button_Image[7][100] = {
	"Pictures\\Buttons\\Play.bmp",
	"Pictures\\Buttons\\High Score.bmp",
	"Pictures\\Buttons\\Exit.bmp",
	"Pictures\\Buttons\\Main Menu.bmp",
	"Pictures\\Buttons\\Restart.bmp",
	"Pictures\\Buttons\\Pause.bmp",
	"Pictures\\Buttons\\Resume.bmp"
};

//Text Position for starting
int textx = 450, texty = 100;

//********************************************************************************************************************************************//
//																 Scoring Variables															  //
//********************************************************************************************************************************************//

int score = 0, point[5] = {};
bool scoreChange = true;
char scoreDigits[10] = {};

//Score Digit Images
char scoreDigitImage[10][100] = {
	"Pictures\\Score\\0.bmp",
	"Pictures\\Score\\1.bmp",
	"Pictures\\Score\\2.bmp",
	"Pictures\\Score\\3.bmp",
	"Pictures\\Score\\4.bmp",
	"Pictures\\Score\\5.bmp",
	"Pictures\\Score\\6.bmp",
	"Pictures\\Score\\7.bmp",
	"Pictures\\Score\\8.bmp",
	"Pictures\\Score\\9.bmp"
};

//Score Digit variables
int ScoreDigit4 = 0, ScoreDigit3 = 0, ScoreDigit2 = 0, ScoreDigit1 = 0;

//Score Digit Image co-ordinates
int ScoreDigit4X = 1080, ScoreDigit3X = ScoreDigit4X + 50, ScoreDigit2X = ScoreDigit3X + 50, ScoreDigit1X = ScoreDigit2X + 50;
int ScoreHeight = 100, ScoreHeight1 = ScoreHeight+80;

//Game Over Score display
int SX = 250, SX4 = SX + 480, SX3 = SX4 + 60, SX2 = SX3 + 60, SX1 = SX2 + 60;
int SY = 420, SY1 = SY + 35;

//Structure For Link List
struct PS
{
    int score;
    struct PS *next;
} *head = NULL;

//Variables for Link List operations
struct PS *cur = NULL, *prv = NULL, *nxt = NULL, *itr = NULL, *pos = NULL;

//Variables for File operations
FILE *FP_store, *FP_load;
errno_t err;

//Variable to control saving
bool SaveScore = true;

//Variable to control viewing
bool VIEW = true;

//********************************************************************************************************************************************//
//																In-Game State Control														  //
//********************************************************************************************************************************************//

//Game State -> Different scenarios of the game
int gameState = -1;
bool initializeCondition = true;
bool resetCondition = true;

//Music control variable
bool musicOn = true;

//Count-down timer
int Timer = 31, TimerDigit1 = 1, TimerDigit2 = 3;

//time as image
char TImeDigitImage[11][100] = { 
	"Pictures\\time\\0.bmp",
	"Pictures\\time\\1.bmp",
	"Pictures\\time\\2.bmp", 
	"Pictures\\time\\3.bmp", 
	"Pictures\\time\\4.bmp", 
	"Pictures\\time\\5.bmp", 
	"Pictures\\time\\6.bmp", 
	"Pictures\\time\\7.bmp", 
	"Pictures\\time\\8.bmp", 
	"Pictures\\time\\9.bmp" 
};

//Time variables
int Timer1 = 1205, Timer2 = 1130, TimerHeight = 350, SandClockX = 1080, SandClockY = 350;

//City range
int cityX = 1080, cityY = 706;

//Preliminary destruction of city area
int destroyX = 300, destroyY = 300;

//Movement factors of cross hair
int velocity = 5;
int moveX = destroyX + 50, moveY = destroyY + 50;
int currentX = moveX, currentY = moveY;

//Cross hair picture
char CrossHair[50] = "Pictures\\Cross Hair.bmp";

//Zones of city
int Cell[1081][721];

//Zone factors
int Left = -1, Right = -1;
bool DestroyCondition = false;

//Color change
int Red = 255, Green = 0, Blue = 255, ColorPhase = 2;


//********************************************************************************************************************************************//
//															User Defined Functions															  //
//********************************************************************************************************************************************//

void numToString(int num, char *no) {
	int i = 0;

	while (num>0) {
		no[i++] = ((num % 10) + '0');
		num /= 10;
	}

	no[i] = '\0';

	reverse(no, no + i);
}

struct PS *view(struct PS *head)
{
	cur = head;

	for (int i = 0; (i < 5 && cur!=NULL); i++)
	{
		point[i] = cur->score;
		cur = cur->next;
	}

	return head;
}

struct PS *save(struct PS *head, int score)
{
	itr = prv = nxt = head;
	cur = (struct PS *) malloc(sizeof(struct PS));

	cur->score = score;

	if (head == NULL)
	{
		head = cur;
		cur->next = NULL;
	}
	else
	{
		while ((itr != NULL) && (score < itr->score))
		{
			prv = itr;
			itr = itr->next;
		}
		if (itr == head)
		{
			head = cur;
			cur->next = itr;
		}
		else if (itr == NULL)
		{
			prv->next = cur;
			cur->next = NULL;
		}
		else
		{
			prv->next = cur;
			cur->next = itr;
		}
	}

	return head;
}

struct PS *load(struct PS *head)
{
	prv = head;
	cur = (struct PS*) malloc(sizeof(struct PS));

	if ((err = fopen_s(&FP_load,"Score.txt", "rb")) != 0)
	{
		//cout << "Error in saving contact!" << endl;
		exit(1);
	}

	while (fread(cur, sizeof(struct PS), 1, FP_load) == 1)
	{

		if (head == NULL)
		{
			head = prv = cur;
		}
		else
		{
			prv->next = cur;
			prv = cur;
		}

		cur = (struct PS*) malloc(sizeof(struct PS));
	}

	fclose(FP_load);

	return head;
}

struct PS *store(struct PS *head)
{
	cur = head;

	if ((err = fopen_s(&FP_store,"Score.txt", "wb")) != 0)
	{
		//cout << "Error in saving contact!" << endl;
		exit(1);
	}

	while (cur != NULL)
	{
		if (fwrite(cur, sizeof(struct PS), 1, FP_store) != 1)
		{
			//printf("Error in writing contact info!\n");
			exit(1);
		}

		cur = cur->next;
	}

	fclose(FP_store);

	return head;
}

void Initialize()
{
	initializeCondition = false;

	head = load(head);

	int sum = 100;

	for (int i = 2; i >= 0; i--) {
		button_position[i].x = 350;
		button_position[i].y = sum;

		sum += 200;
	}

	button_position[3].x = 350;
	button_position[3].y = 100;

	button_position[4].x = 350;
	button_position[4].y = 300;

	button_position[5].x = 1105;
	button_position[5].y = 550;

	button_position[6].x = 335;
	button_position[6].y = 250;
}

void RESET()
{
	resetCondition = false;

	for (int i = 0; i <= cityX; i++)
	{
		for (int j = 0; j <= cityY; j++)
		{
			if (i >= destroyX && i <= (destroyX + 100) && j >= destroyY && j <= (destroyY + 100))
			{
				Cell[i][j] = 2;
			}
			else
			{
				Cell[i][j] = 0;
			}
		}
	}

	moveX = destroyX + 50;
	moveY = destroyY + 50;

	scoreChange = true;
}

void SCORE()
{
	if (scoreChange == true)
	{
		score = 0;

		for (int i = 0; i <= cityX; i++)
		{
			for (int j = 0; j <= cityY; j++)
			{
				if (Cell[i][j] == 2) 
				{
					score++;
				}
			}
		}

		score = score / 100;

		ScoreDigit1 = max(score % 10, 0);
		ScoreDigit2 = (max((score % 100) - ScoreDigit1, 0)) / 10;
		ScoreDigit3 = (max((score % 1000) - (ScoreDigit1 + ScoreDigit2), 0)) / 100;
		ScoreDigit4 = (max((score % 10000) - (ScoreDigit1 + ScoreDigit2 + ScoreDigit3), 0)) / 1000;
	}
}

void timeFunction() //for time counter
{
	if (gameState == 0)
	{
		Timer = 31;
		Timer--;
		int timertmp = Timer / 10;
		TimerDigit1 = Timer % 10;
		TimerDigit2 = timertmp % 10;
	}
	if (gameState == 1)
	{
		if (Timer == 0)
		{
			RESET();

			Timer = 31;
			Timer--;
			int timertmp = Timer / 10;
			TimerDigit1 = Timer % 10;
			TimerDigit2 = timertmp % 10;

			gameState = 4;
		}
		else
		{
			Timer--;
			int timertmp = Timer / 10;
			TimerDigit1 = Timer % 10;
			TimerDigit2 = timertmp % 10;
		}
	}
}

void traverseForward(int lo, int hi, bool type)
{
	//For vertical traverse
	if (type == 1)
	{
		for (int i = lo; i <= hi; i++)
		{
			if (Cell[moveX][i] != 2)
			{
				DestroyCondition = false;

				if (Cell[moveX][i] == 0)
				{
					Cell[moveX][i] = 1;
				}
			}
			else if (Cell[moveX][i] == 2)
			{
				DestroyCondition = true;
			}
		}
	}
	
	//For horizontal traverse
	else
	{
		for (int i = lo; i <= hi; i++)
		{
			if (Cell[i][moveY] != 2)
			{
				DestroyCondition = false;

				if (Cell[i][moveY] == 0)
				{
					Cell[i][moveY] = 1;
				}
			}
			else if (Cell[i][moveY] == 2)
			{
				DestroyCondition = true;
			}
		}
	}
}

void traverseBackward(int hi, int lo, bool type)
{
	//For vertical traverse
	if (type == 1)
	{
		for (int i = hi; i >= lo; i--)
		{
			if (Cell[moveX][i] != 2)
			{
				DestroyCondition = false;

				if (Cell[moveX][i] == 0)
				{
					Cell[moveX][i] = 1;
				}
			}
			else if (Cell[moveX][i] == 2)
			{
				DestroyCondition = true;
			}
		}
	}

	//For horizontal traverse
	else
	{
		for (int i = hi; i >= lo; i--)
		{
			if (Cell[i][moveY] != 2)
			{
				DestroyCondition = false;

				if (Cell[i][moveY] == 0)
				{
					Cell[i][moveY] = 1;
				}
			}
			else if (Cell[i][moveY] == 2)
			{
				DestroyCondition = true;
			}
		}
	}
}

void colorChange()
{
	if (ColorPhase == 1)
	{
		Blue = Blue + 5;
		if (Blue == 255)
		{
			ColorPhase = 2;
		}
	}
	else if (ColorPhase == 2)
	{
		Red = Red - 5;
		if (Red == 0)
		{
			ColorPhase = 3;
		}
	}
	else if (ColorPhase == 3)
	{
		Green = Green + 5;
		if (Green == 255)
		{
			ColorPhase = 4;
		}
	}
	else if (ColorPhase == 4)
	{
		Blue = Blue - 5;
		if (Blue == 0)
		{
			ColorPhase = 5;
		}
	}
	else if (ColorPhase == 5)
	{
		Red = Red + 5;
		if (Red == 255)
		{
			ColorPhase = 6;
		}
	}
	else if (ColorPhase == 6)
	{
		Green = Green - 5;
		if (Green == 0)
		{
			ColorPhase = 1;
		}
	}
}

//********************************************************************************************************************************************//
//															Graphics.h Functions														      //
//********************************************************************************************************************************************//

void iDraw()
{
	//Starting Screen
	if (gameState == -1)
	{
		iClear();
		iShowBMP(0, 0, "Pictures\\Main Menu BG.bmp");
		iShowBMP2(0, 0, "Pictures\\Start.bmp",255);
	}
	
	//Main Menu screen
	else if (gameState == 0)
	{
		iClear();

		scoreChange = true;
		resetCondition = true;
		SaveScore = true;
		
		iShowBMP(0, 0, "Pictures\\Main Menu BG.bmp");

		for (int i = 0; i < 3; i++)
		{
			iShowBMP2(button_position[i].x, button_position[i].y, button_Image[i], 255);
		}
		
	}
	
	//Play screen
	else if (gameState == 1)
	{
		iClear();

		if (resetCondition == true)
		{
			RESET();
		}
		
		//Background
		iSetColor(Red, Green, Blue);
		iFilledRectangle(0, 0, 1280, 720);

		//Map
		iShowBMP(0, 0, "Pictures\\Play BG.bmp");
		
		//Destroyed portion color
		for (int i = 0; i <= cityX; i++)
		{
			Left = Right = -1;

			for (int j = 0; j <= cityY; j++)
			{
				if (Cell[i][j] != 2)
				{
					//Mapping future destruction color
					if (Cell[i][j] == 1)
					{
						iSetColor(255, 255, 255);
						iFilledRectangle(i, j, 1, 1);
					}

					//Drawing destroyed area
					if (Left != -1)
					{
						iSetColor(Red, Green, Blue);
						iLine(i, Left, i, Right);
					}
				}
				else if (Cell[i][j] == 2)
				{
					if (Left == -1)
					{
						Left = Right = j;
					}
					else
					{
						Right = j;
					}
				}
			}
		}

		//Cross hair
		iShowBMP2(moveX-25, moveY-25, CrossHair, 0);

		//Timer
		iShowBMP2(Timer1, TimerHeight, TImeDigitImage[TimerDigit1],0);
		iShowBMP2(Timer2, TimerHeight, TImeDigitImage[TimerDigit2],0);
		iShowBMP2(SandClockX, SandClockY, "Pictures\\timer.bmp",0);

		//Pause Button
		iShowBMP2(button_position[5].x, button_position[5].y, button_Image[5], 255);

		//Score
		iShowBMP2(ScoreDigit4X, ScoreHeight1, "Pictures\\Score\\Score.bmp", 255);
		iShowBMP2(ScoreDigit4X, ScoreHeight, scoreDigitImage[ScoreDigit4], 255);
		iShowBMP2(ScoreDigit3X, ScoreHeight, scoreDigitImage[ScoreDigit3], 255);
		iShowBMP2(ScoreDigit2X, ScoreHeight, scoreDigitImage[ScoreDigit2], 255);
		iShowBMP2(ScoreDigit1X, ScoreHeight, scoreDigitImage[ScoreDigit1], 255);
	}
	
	//High Score Back Ground
	else if (gameState == 2)
	{
		iClear();
		iShowBMP(0, 0, "Pictures\\Main Menu BG.bmp");
		iShowBMP2(350, 550, "Pictures\\High Score Writing.bmp", 255);
		iShowBMP2(325, 20, button_Image[3], 255);

		char cha[10];
		int sum = 500;

		if (VIEW == true)
		{
			head = view(head);
			VIEW = false;
		}

		for (int i = 0; i < 5; i++)
		{
			ScoreDigit1 = max(point[i] % 10, 0);
			ScoreDigit2 = (max((point[i] % 100) - ScoreDigit1, 0)) / 10;
			ScoreDigit3 = (max((point[i] % 1000) - (ScoreDigit1 + ScoreDigit2), 0)) / 100;
			ScoreDigit4 = (max((point[i] % 10000) - (ScoreDigit1 + ScoreDigit2 + ScoreDigit3), 0)) / 1000;

			iShowBMP2(400, sum, scoreDigitImage[i + 1], 255);
			iShowBMP2(600, sum, scoreDigitImage[ScoreDigit4], 255);
			iShowBMP2(660, sum, scoreDigitImage[ScoreDigit3], 255);
			iShowBMP2(720, sum, scoreDigitImage[ScoreDigit2], 255);
			iShowBMP2(780, sum, scoreDigitImage[ScoreDigit1], 255);

			sum -= 80;
		}
	}
	
	//Exit Game
	else if (gameState == 3)
	{
		exit(1);
	}
	
	//Game Over
	else if (gameState == 4)
	{
		//Minimizing score to 4 digit
		if (scoreChange == true)
		{
			scoreChange = false;
		}

		//Saving Score
		if (SaveScore == true)
		{
			head = save(head, score);				//Adding score to link list
			head = store(head);						//Adding link list to file
			SaveScore = false;						//Stoping re-including same score multiple times
		}

		iClear();
		iShowBMP(0, 0, "Pictures\\Main Menu BG.bmp");
		iShowBMP2(350, 500, "Pictures\\Game Over.bmp", 255);
		iShowBMP2(button_position[gameState].x, button_position[gameState].y, button_Image[gameState], 255);
		iShowBMP2(button_position[3].x, button_position[3].y, button_Image[3], 255);

		//numToString(score, scoreDigits);
		
		//iSetColor(0, 0, 0);
		//iText(100, 100, scoreDigits, GLUT_BITMAP_TIMES_ROMAN_24);

		//Showing scored points
		iShowBMP2(SX, SY, "Pictures\\Your Score.bmp", 255);
		iShowBMP2(SX4, SY1, scoreDigitImage[ScoreDigit4], 255);
		iShowBMP2(SX3, SY1, scoreDigitImage[ScoreDigit3], 255);
		iShowBMP2(SX2, SY1, scoreDigitImage[ScoreDigit2], 255);
		iShowBMP2(SX1, SY1, scoreDigitImage[ScoreDigit1], 255);
	}
	
	//Pause Menu
	else if (gameState == 5)
	{
		iClear();
		iShowBMP(0, 0, "Pictures\\Main Menu BG.bmp");
		iShowBMP2(250, 400, "Pictures\\Game Paused.bmp", 255);
		iShowBMP2(button_position[6].x, button_position[6].y, button_Image[6], 255);
		iShowBMP2(button_position[3].x, button_position[3].y, button_Image[3], 255);
	}
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//Start Screen
		if (gameState == -1)
		{
			// Changing from start screen to main menu
			gameState = 0;
		}
		
		//Main Menu Screen
		else if (gameState == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (mx >= button_position[i].x && mx <= button_position[i].x + 500 && my >= button_position[i].y && my <= button_position[i].y + 160)
				{
					gameState = i + 1;
				}
			}
		}
		
		//Play Screen
		else if (gameState == 1)
		{
			if (mx >= button_position[5].x && mx <= button_position[5].x + 150 && my >= button_position[5].y && my <= button_position[5].y + 150)
			{
				gameState = 5;
			}
		}
		
		//HighScore Screen
		else if (gameState == 2)
		{
			if (mx >= 325 && mx <= 825 && my >= 10 && my <= 170)
			{
				gameState = 0;
				VIEW = true;
			}
		}

		//Game Over Screen
		else if (gameState == 4)
		{
			for (int i = 3; i < 5; i++)
			{
				if (mx >= button_position[i].x && mx <= button_position[i].x + 150 && my >= button_position[i].y && my <= button_position[i].y + 150)
				{
					scoreChange = true;
					resetCondition = true;
					SaveScore = true;

					if (i == 3)
					{
						gameState = 0;
					}
					else if (i == 4)
					{
						gameState = 1;
					}
				}
			}
		}
		
		//Pause Screen
		else if (gameState == 5)
		{
			if (mx >= button_position[6].x && mx <= button_position[6].x + 500 && my >= button_position[6].y && my <= button_position[6].y + 500)
			{
				gameState = 1;
			}
			else if (mx >= button_position[3].x && mx <= button_position[3].x + 500 && my >= button_position[3].y && my <= button_position[3].y + 500)
			{
				gameState = 0;
			}
		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
	}
}

/*iPassiveMouseMove is called to detect and use
the mouse point without pressing any button */
void iPassiveMouseMove(int mx,int my)
{
	//place your code here

 if(mx== 2){}        /*Something to do with mx*/
 else if(my== 2){}   /*Something to do with my*/

}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if(key == 'm')
	{
		if (musicOn == true)
		{
			PlaySound(0, 0, 0);
			musicOn = false;
		}
		else
		{
			musicOn = true;
			PlaySound("Sound\\Music.wav", NULL, SND_LOOP | SND_ASYNC);
		}
	}
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/

void iSpecialKeyboard(unsigned char key)
{
	DestroyCondition = false;

	//Upward Motion
	if(key == GLUT_KEY_UP)
	{
		currentY = moveY + velocity;
		traverseForward(moveY, currentY, 1);
		moveY = currentY;
	}
	
	//Downward Motion
	else if (key == GLUT_KEY_DOWN)
	{
		currentY = moveY - velocity;
		traverseBackward(moveY, currentY, 1);
		moveY = currentY;
	}
	
	//Left Motion
	else if (key == GLUT_KEY_LEFT)
	{
		currentX = moveX - velocity;
		traverseBackward(moveX, currentX, 0);
		moveX = currentX;
	}
	
	//Right Motion
	else if (key == GLUT_KEY_RIGHT)
	{
		currentX = moveX + velocity;
		traverseForward(moveX, currentX, 0);
		moveX = currentX;
	}

	//Marking the enclosed space
	if (DestroyCondition == true)
	{
		for (int i = 0; i <= cityX; i++)
		{
			Left = Right = -1;

			for (int j = 0; j <= cityY; j++)
			{
				if (Left == -1 && Cell[i][j])
				{
					Left = j;
				}
				else if (Cell[i][j])
				{
					Right = j;
				}
			}

			if (Left != -1 && Right != -1) 
			{
				if (Left > Right) 
				{
					swap(Left, Right);
				}
				for (int j = Left; j <= Right; j++)
				{
					Cell[i][j] = 2;
				}
			}
		}
	}

	DestroyCondition = false;
}

int main()
{
	if (musicOn == true)
	{
		PlaySound("Sound\\Music.wav", NULL, SND_LOOP | SND_ASYNC);
	}

	if (initializeCondition == true)
	{
		Initialize();
	}
	if (resetCondition == true)
	{
		RESET();
	}

	iSetTimer(1000, timeFunction);	//for time counter
	iSetTimer(1000, SCORE);			//score
	iSetTimer(10, colorChange);		//ColourChange

	iInitialize(screenWidth, screenHight, "Total Destruction The Game");

	return 0;
}
