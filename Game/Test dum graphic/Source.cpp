﻿#include <Windows.h>
#include <string>
#include<iostream>
#include<vector>
#include<cstdlib>
#include <windows.h>
using namespace std;
const wstring detail = L" ▄▀█▓░╚╝╔╗║═";
#define nScreenWidth 160
#define nScreenHeight 44
#define fps 20
enum { darkblue, lightblue, orange, yellow, green, purple, red, darkwhite, white, black };
const vector<char> key = { 'W', 'A', 'S', 'D' };

const vector<wstring> e1 = { // 10 x 4
	L". '.  .' .",
	L"l.l'll'l.'",
	L" ll.ll.ll",
	L".l '  ' l."
};
const vector<wstring> e2 = { // 7 x 4
	L" '...'",
	L" l'l'l",
	L".'l'l'.",
	L"' ' ' '"
};
const vector<wstring> e3 = {  // 8 x 4
	L"  '..'",
	L"l.'ll'.l",
	L"l'llll'l",
	L" .l  l."
};
const vector<wstring> e4 = { // 9 x 4
	L" .l...l.",
	L"ll..l..ll",
	L"l'l'l'l'l",
	L" ' ' ' '"
};
const vector<vector<wstring>> e = {
	e1, e2, e3, e4
};
const vector<wstring> Title = { // 52 x 8
	L".'''.  l'''.  .'''.  .'''   .'''   'l'  l.  l  .''''",
	L"l      l'''.  l   l   '''.   '''.   l   l '.l  l ''l",
	L"'...'  l   l  '...'  '...'  '...'  .l.  l   l  '...l",
	L" ",
	L" ",
	L"             l'''.  .'''.  .'''.  l'''.",
	L"             l'''.  l   l  l...l  l   l",
	L"             l   l  '...'  l   l  l...'",
};
const vector<wstring> GameOver = { // 54 x 3
	L".'''.  .'''.  l. .l  l'''    .'''.  l   l  l'''  l'''.",
	L"l  ..  l'''l  l ' l  l''     l   l  '. .'  l''   l'''.",
	L"'...'  l   l  l   l  l...    '...'   '.'   l...  l   l"
};
const vector<wstring> Skull = { // 22 x 12
	L"      ..........",
	L"   .llllllllllllll.",
	L" .llllllllllllllllll.",
	L"llllllllllllllllllllll",
	L"llllllllllllllllllllll",
	L"llll      ll      llll",
	L"ll        ll        ll",
	L"llllllllll''llllllllll",
	L" 'llllll      llllll'",
	L"   'llllllllllllll'",
	L"    ll  llllll  ll",
	L"        ''  '' "
};
LPCWSTR intro{ L"play intro.wav" };
LPCWSTR menu_click{ L"play menu_click.wav" };
LPCWSTR game_over{ L"play game_over.wav" };

class cPlayer {
private:
	int X = 0, Y = 0;
	const vector<wstring> Sketch = {
		L"ll"
	};
public:
	int getX() { return X; }
	int getY() { return Y; }
	void setX(int x) { X = x; }
	void setY(int y) { Y = y; }
	void setXY(int x, int y) { X = x, Y = y; }
	void moveLeft() { X -= 2; } // Move 2 blocks
	void moveRight() { X += 2; } // Move 2 blocks
	void moveUp() { Y--; }
	void moveDown() { Y++; }
	vector<wstring> getSketch() { return Sketch; }
};
class cEnemy {
private:
	int direction; // -1 left, 1 right
	int color;
	int X = 0, Y = 0;
	vector<wstring> Sketch;
public:
	cEnemy(int x, int y, vector<wstring> sketch) {
		X = x;
		Y = y;
		Sketch = sketch;
	}
	cEnemy(int x, int y, vector<wstring> sketch, int d, int c) {
		X = x;
		Y = y;
		Sketch = sketch;
		direction = d;
		color = c;
	}
	int getX() { return X; }
	int getY() { return Y; }
	void setX(int x) { X = x; }
	void setY(int y) { Y = y; }
	void setXY(int x, int y) { X = x, Y = y; }
	void moveLeft() { X -= 2; } // Move 2 blocks
	void moveRight() { X += 2; } // Move 2 blocks
	void moveUp() { Y--; }
	void moveDown() { Y++; }
	int width() {
		int max = 0;
		for (int i = 0; i < Sketch.size(); i++)
			if (Sketch[i].length() > max)
				max = Sketch[i].length();
		return max;
	}
	int height() { return Sketch.size(); }
	vector<wstring> getSketch() { return Sketch; }
	void setSketch(vector<wstring> sketch) { Sketch = sketch; }
	void updatePos() {
		if (direction == -1) {
			if (X < 0)
				X = nScreenWidth;
			else moveLeft();
		}
		else if (direction == 1) {
			if (X + width() >= nScreenWidth)
				X = 0;
			else moveRight();
		}
	}
	int getColor() { return color; }
};
bool checkCollision(cPlayer player, cEnemy* enemy) {
	if (player.getX() >= enemy->getX() && player.getX() < enemy->getX() + enemy->width() &&
		player.getY() > enemy->getY() && player.getY() < enemy->getY() + enemy->height())
		return true;
	return false;
}
class cScreen {
private:
	WORD* pColor = new WORD[nScreenWidth * nScreenHeight]; // Character array
	wchar_t* pBuffer = new wchar_t[nScreenWidth * nScreenHeight]; // Color array
	HANDLE hConsole;
	DWORD dwBytesWritten = 0;
public:
	void FixConsoleWindow() {
		HWND consoleWindow = GetConsoleWindow();
		LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
		style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
		SetWindowLong(consoleWindow, GWL_STYLE, style);
	}
	void configure() {
		system("MODE 160, 46"); // Set screen size (width, height + 1)
		FixConsoleWindow(); //Fix window size
		// Make custom color palette - up to 16 colors, will update later
		HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); // A hanle to console screen buffer.
		CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
		csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(hConsoleOutput, &csbiex);
		csbiex.ColorTable[0] = RGB(16, 16, 60); // Default background color - dark blue
		csbiex.ColorTable[1] = RGB(63, 81, 181); // Light blue
		csbiex.ColorTable[2] = RGB(255, 87, 34); // Orange
		csbiex.ColorTable[3] = RGB(255, 235, 59); // Yellow
		csbiex.ColorTable[4] = RGB(76, 175, 80); // Green
		csbiex.ColorTable[5] = RGB(156, 39, 176); // Purple
		csbiex.ColorTable[6] = RGB(237, 28, 36); // Red
		csbiex.ColorTable[7] = RGB(242, 242, 242); // Dark white
		csbiex.ColorTable[8] = RGB(248, 248, 248); // White
		csbiex.ColorTable[9] = RGB(20, 20, 20); // Black
		SetConsoleScreenBufferInfoEx(hConsoleOutput, &csbiex);
	}
	void clearScreen(int colorBackground, int colorCharacter) {
		for (int i = 0; i < nScreenWidth; i++) {
			for (int j = 0; j < nScreenHeight; j++) {
				pBuffer[j * nScreenWidth + i] = L' '; // Fill screen with blank space
				pColor[j * nScreenWidth + i] = colorBackground * 16 + colorCharacter; // Set color
			}
		}
	}
	void drawScreen() {
		Sleep(1000 / fps); //Rate
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		WriteConsoleOutputCharacter(hConsole, pBuffer, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	void drawScreenColor() {
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	void drawScreenBuffer() {
		WriteConsoleOutputCharacter(hConsole, pBuffer, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	void gameOverEffect() {
		for (int i = 0; i < nScreenWidth; i++)
			for (int j = 0; j < nScreenHeight; j++)
				pColor[j * nScreenWidth + i] = 7 * 16 + 0;
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		Sleep(100);
		for (int i = 0; i < nScreenWidth; i++)
			for (int j = 0; j < nScreenHeight; j++)
				pColor[j * nScreenWidth + i] = 0 * 16 + 7;
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		Sleep(100);
		for (int i = 0; i < nScreenWidth; i++)
			for (int j = 0; j < nScreenHeight; j++)
				pColor[j * nScreenWidth + i] = 7 * 16 + 0;
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		Sleep(100);
		for (int i = 0; i < nScreenWidth; i++)
			for (int j = 0; j < nScreenHeight; j++)
				pColor[j * nScreenWidth + i] = 0 * 16 + 7;
		WriteConsoleOutputAttribute(hConsole, pColor, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		Sleep(500);
	}
	void gotoXY(int x, int y) {  // Place the cursor at (x, y)
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	void drawText(wstring content, int nPosX, int nPosY, int colorBackground, int colorCharacter) {
		for (int i = 0; i < content.length(); i++) {
			pBuffer[nPosY * nScreenWidth + nPosX + i] = content.at(i);
			pColor[nPosY * nScreenWidth + nPosX + i] = colorBackground * 16 + colorCharacter;
		}
	}
	void drawBlock(vector<wstring> Sketch, int X, int Y, int colorBackground, int colorChar) {
		if (X >= 0 && X < nScreenWidth && Y >= 0 && Y < nScreenHeight) {
			for (int i = 0; i < Sketch.size(); i++) {
				for (int j = 0; j < Sketch[i].length(); j++) {
					if (Sketch[i][j] == '.') {
						pBuffer[(Y + i) * nScreenWidth + X + j] = L'▄';
						pColor[(Y + i) * nScreenWidth + X + j] = colorBackground * 16 + colorChar;
					}
					if (Sketch[i][j] == '\'') {
						pBuffer[(Y + i) * nScreenWidth + X + j] = L'▀';
						pColor[(Y + i) * nScreenWidth + X + j] = colorBackground * 16 + colorChar;
					}
					if (Sketch[i][j] == 'l') {
						pBuffer[(Y + i) * nScreenWidth + X + j] = L'█';
						pColor[(Y + i) * nScreenWidth + X + j] = colorBackground * 16 + colorChar;
					}
				}
			}
		}
	}
	void drawBlock(wstring Sketch, int X, int Y, int colorBackground, int colorChar) {
		if (X>=0 && X <nScreenWidth&& Y>=0 && Y <nScreenHeight) {
			for (int j = 0; j < Sketch.length(); j++) {
				if (Sketch[j] == '.') {
					pBuffer[(Y)*nScreenWidth + X + j] = L'▄';
					pColor[(Y)*nScreenWidth + X + j] = colorBackground * 16 + colorChar;
				}
				if (Sketch[j] == '\'') {
					pBuffer[(Y)*nScreenWidth + X + j] = L'▀';
					pColor[(Y)*nScreenWidth + X + j] = colorBackground * 16 + colorChar;
				}
				if (Sketch[j] == 'l') {
					pBuffer[(Y)*nScreenWidth + X + j] = L'█';
					pColor[(Y)*nScreenWidth + X + j] = colorBackground * 16 + colorChar;
				}
			}
		}
	}
	void drawFrame(int x, int y, int width, int height, int colorBackground, int colorChar) {
		for (int i = 0; i < height; i++) { // y iterator
			for (int j = 0; j < width; j++) { // x iterator
				if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
					// Corners
					if (i == 0 && j == 0)
						pBuffer[(i + y) * nScreenWidth + x + j] = L'╔';
					else if (i == 0 && j == width - 1)
						pBuffer[(i + y) * nScreenWidth + x + j] = L'╗';
					else if (i == height - 1 && j == 0)
						pBuffer[(i + y) * nScreenWidth + x + j] = L'╚';
					else if (i == height - 1 && j == width - 1)
						pBuffer[(i + y) * nScreenWidth + x + j] = L'╝';
					// Sides
					if ((i == 0 && (j >= 1 && j < width - 1)) || (i == height - 1 && (j >= 1 && j < width - 1)))
						pBuffer[(i + y) * nScreenWidth + x + j] = L'═';
					if ((j == 0 && (i >= 1 && i < height - 1)) || (j == width - 1 && (i >= 1 && i < height - 1)))
						pBuffer[(i + y) * nScreenWidth + x + j] = L'║';
					pColor[(i + y) * nScreenWidth + x + j] = colorBackground * 16 + colorChar;
				}
			}
		}
	}
	void animationWiggleUpDown(vector<wstring> content, int frame, int x, int y, int colorBackground, int colorCharacter) {
		int rate = 30;
		if (frame % rate >= 0 && frame % rate < rate/4)
			drawBlock(content, x, y, colorBackground, colorCharacter);
		if (frame % rate >= rate/4 && frame % rate < rate/2)
			drawBlock(content, x, y - 1, colorBackground, colorCharacter);
		if (frame % rate >= rate/2 && frame % rate < rate*3/4)
			drawBlock(content, x, y, colorBackground, colorCharacter);
		if (frame % rate >= rate*3/4 && frame % rate < rate)
			drawBlock(content, x, y + 1, colorBackground, colorCharacter);
	}

	void makeNewStarMap(int* starmap) {
		for (int i = 0; i < nScreenHeight; i++)
			for (int j = 0; j < nScreenWidth; j++)
				starmap[i * nScreenWidth + j] = 0;
		for (int i = 0; i < 50; i++) {
			int x = rand() % nScreenWidth , y = rand() % nScreenHeight;
			pBuffer[y * nScreenWidth + x] = L'.';
			starmap[y * nScreenWidth + x] = 1;
		}


	
	}
	void drawRectangle(int x, int y, int width, int height, int colorBackground, int colorChar) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pColor[i * nScreenHeight + j] = colorBackground * 16 + colorChar;
			}
		}
	}
	void drawStars(int* starmap) {
		for (int i = 0; i < nScreenHeight; i++) {
			for (int j = 0; j < nScreenWidth; j++) {
				if (starmap[i * nScreenWidth + j] == 1)
					pBuffer[i * nScreenWidth + j] = L'.';
			}
		}
	}
	void drawHorizontalLine1(int x, int y, int length, int colorBackground, int colorChar) {
		for (int i = 0; i < length; i++) {
			pBuffer[y * nScreenWidth + x + i] = L'═';
			pColor[y * nScreenWidth + x + i] = colorBackground * 16 + colorChar;

		}
	}
	void drawHorizontalLine2(int x, int y, int length, int colorBackground, int colorChar) {
		for (int i = 0; i < length; i++) {
			if (i%2==0)pBuffer[y * nScreenWidth + x + i] = L'o';
			else pBuffer[y * nScreenWidth + x + i] = L' ';
			pColor[y * nScreenWidth + x + i] = colorBackground * 16 + colorChar;

		}
	}
	void drawHorizontalLine3(int x, int y, int length, int colorBackground, int colorChar) {
		if (x >= 0 && x < nScreenWidth && y >= 0 && y < nScreenHeight) {

			for (int i = 0; i < length; i++) {
				if (i % 2 == 0)pBuffer[y * nScreenWidth + x + i] = L'-';
				else pBuffer[y * nScreenWidth + x + i] = L' ';
				pColor[y * nScreenWidth + x + i] = colorBackground * 16 + colorChar;

			}
		}
	}
	void glitchEffectText(wstring content, int X, int Y, int colorBackground1, int colorBackground2, int colorCharacter1, int colorCharacter2) {
		drawText(content, X, Y, colorBackground2, colorCharacter2); drawScreen();
		drawText(content, X, Y, colorBackground1, colorCharacter1); drawScreen();
		drawText(content, X, Y, colorBackground2, colorCharacter2); drawScreen();
	}
	void drawMap() {
		for (int i = 0; i < 5; i+=2) {
			for (int j = 0; j < 5; j++)
				for (int k = 0; k < nScreenWidth; k++)
					pColor[(5 * i + j) * nScreenWidth + k] = 1 * 16;
		}
	
	}
	void startMenuScreen() {
		//LPCWSTR intro{ L"play intro.wav" };
		mciSendString(intro, NULL, 0, NULL);


		//PlaySound(TEXT("intro.wav"), NULL, SND_ASYNC);
		// CONFIGURE SCREEN
		configure();
		HANDLE hConsole1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole1);
		hConsole = hConsole1;
		int frame = 0;
		int* starmap = new int[nScreenWidth * nScreenHeight];

		int choiceMenu = 0; // 0 - Start game, 1 - Load game, 2 - Settings, 3 - Exit
		// MENU SCREEN
		while (true) {
			// CLEAR SCREEN
			clearScreen(9, 7);

			// READING INPUT
			bool* bKey = new bool[key.size()];
			for (int i = 0; i < key.size(); i++) { 	// Read input
				bKey[i] = (GetAsyncKeyState(key.at(i))) != 0;
			}
			// W - Move up
			if (bKey[0] == 1 ) {
				choiceMenu = (choiceMenu - 1) % 4;
				mciSendString(menu_click, NULL, 0, NULL);

			}
			// S - Move down
			if (bKey[2] == 1) {
				choiceMenu = (choiceMenu + 1) % 4;
				mciSendString(menu_click, NULL, 0, NULL);

			}
			int bg = 9, ch = 7;
			// DISPLAY CURRENT MENU
			drawBlock(Title, 54, 15, bg, ch);
			int xMenu = 74, yMenu = 27;
			//drawText(L"Press W to move up, S to move down", xMenu, yMenu+4, 0, 7);
			drawText(L" START GAME ", xMenu, yMenu, bg, ch);
			drawText(L"  LOAD GAME ", xMenu, yMenu + 1, bg, ch);
			drawText(L"  SETTINGS  ", xMenu, yMenu + 2, bg, ch);
			drawText(L"    EXIT    ", xMenu, yMenu + 3, bg, ch);
			if (choiceMenu == 0) {
				drawText(L" START GAME ", xMenu, yMenu, ch, bg);

			}
			else if (choiceMenu == 1 || choiceMenu == -3) {
				drawText(L"  LOAD GAME ", xMenu, yMenu + 1, ch, bg);

			}
			else if (choiceMenu == 2 || choiceMenu==-2) {
				drawText(L"  SETTINGS  ", xMenu, yMenu + 2, ch, bg);

			}
			else if (choiceMenu == 3 || choiceMenu ==-1) {
				drawText(L"    EXIT    ", xMenu, yMenu + 3, ch, bg);

			}
			//drawText(L"choice: " + to_wstring(choiceMenu), xMenu, yMenu + 4, 0, 7);
			drawText(L"Frame: " + to_wstring(frame), 2, nScreenHeight - 1, bg, ch);
			if (frame % 15 == 0) makeNewStarMap(starmap);
			drawStars(starmap);
			animationWiggleUpDown(e1, frame, 10, 20, black, red);
			animationWiggleUpDown(e2, frame, 22, 20, black, lightblue);
			animationWiggleUpDown(e3, frame, 30, 20, black, yellow);
			animationWiggleUpDown(e4, frame, 40, 20, black, green);
			drawScreen();
			// ENTER - Select
			if (GetAsyncKeyState(VK_RETURN)) {
				if (choiceMenu == 0) {
					// START GAME
					//PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
					glitchEffectText(L" START GAME ", xMenu, yMenu, 7, 6, 0, 7);
					//Sleep(1800);
					startLoadScreen();

					startGameScreen(1);

				}
				else if (choiceMenu == 1) {
					// LOAD GAME
					glitchEffectText(L"  LOAD GAME ", xMenu, yMenu + 1, 7, 6, 0, 7);
				}
				else if (choiceMenu == 2) {
					// SETTINGS
					glitchEffectText(L"  SETTINGS  ", xMenu, yMenu + 2, 7, 6, 0, 7);
				}
				else if (choiceMenu == 3) {
					// EXIT
					glitchEffectText(L"    EXIT    ", xMenu, yMenu + 3, 7, 6, 0, 7);
				}
			}
			frame++;
		}
	}
	void startLoadScreen() {
		clearScreen(darkblue, white);
		drawBlock(L"ll", 75, 22, darkblue, white);
		drawScreen();
		mciSendString(menu_click, NULL, 0, NULL);
		Sleep(800);
		//clearScreen(darkblue, white);
		drawBlock(L"ll", 79, 22, darkblue, white);
		drawScreen();
		mciSendString(menu_click, NULL, 0, NULL);
		Sleep(800);
		//clearScreen(darkblue, white);
		drawBlock(L"ll", 83, 22, darkblue, white);
		drawScreen();
		mciSendString(menu_click, NULL, 0, NULL);
		Sleep(800);
		
	}

	void startGameScreen(int level) {
		PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
		int nLane = 5 + level * 2;
		int lane1 = 10; // Y of lane1
		int laneSize = 5;
		//INITIALISE PLAYER
		cPlayer Player;
		Player.setXY(80, 1);
		//INITIALISE RANDOM ENEMIES
		cEnemy** Enemy = new cEnemy*[nLane];
		for (int i = 0; i < nLane; i++) {
			int x = rand() % nScreenWidth;
			int y = lane1 + i * laneSize;
			int en = rand() % 4;
			int c;
			switch (en){
			case 0:
				c = red;
				break;
			case 1:
				c = yellow;
				break;
			case 2:
				c = lightblue;
				break;
			case 3:
				c = green;
				break;
			default:
				break;
			}
			int dir = (rand() % 2) * 2 - 1;
			Enemy[i] = new cEnemy(x, y,e[en],dir, c);
		}
		int frame = 0;
		int offset = 0; // For display scrolling
		bool offsetChanged = false;
		bool gameOver = false;
		//Score
		bool* passed = new bool[nLane];
		for (int i = 0;i < nLane ; i++)
			passed[i] = false;
		

		//INITIALISE STAR MAP
		int* starmap = new int[nScreenWidth * nScreenHeight];
		int score = 0;
		//int level = 1;
		while (gameOver == false) {
			// CLEAR SCREEN
			int bg = 0, ch = 7;
			clearScreen(bg, ch);
			drawBlock(Player.getSketch(), Player.getX(), Player.getY() + offset, darkblue, lightblue); // For shadow effect

			// READ INPUT
			bool* bKeyGame = new bool[key.size()]; // Check ingame input
			for (int i = 0; i < key.size(); i++) { 	// Read input
				bKeyGame[i] = (GetAsyncKeyState(key.at(i))) != 0;
			}

			// UPDATE 
			// Player position
			if (bKeyGame[0] == 1){ // W - Move up
				if (Player.getY() - 1 >= 0) {
					Player.moveUp();
					if (Player.getY() + offset <= 5 && Player.getY() > 5) {
						offset++;
					}
				}
			}
			if (bKeyGame[1] == 1){ // A - Move left
				if (Player.getX() - 2 >= 0)
					Player.moveLeft();
			}
			if (bKeyGame[2] == 1){ 	// S - Move down
				//if (Player.getY() + 1 <= )
				if (Player.getY() + 1 <= 18 + nLane * laneSize) {
					Player.moveDown();
					if (Player.getY() +offset +5 >= nScreenHeight && Player.getY() + 5 <= 18 + nLane * laneSize ) {
						offset--;
						//offsetChanged = true;
					}
				}
			}
			if (bKeyGame[3] == 1){ // D - move right
				if (Player.getX() + 2 <= nScreenWidth)
				Player.moveRight();
			}
			// Enemies position
			for (int i = 0; i < nLane; i++) {
				Enemy[i]->updatePos();
			}

			// GAME LOGIC
			// Check lane pass
			bool newscore = false;
			for (int i = 0; i < 10; i++) {
				if (Player.getY() >= lane1 + i * 5 && passed[i] == false) {
					score += 10;
					newscore = true;
					passed[i] = true;
					PlaySound(TEXT("lane_pass.wav"), NULL, SND_ASYNC);
				}
			}
			// Check collision
			for (int i = 0; i < nLane; i++) {
				if (checkCollision(Player, Enemy[i]) == true) {
					PlaySound(TEXT("game_over.wav"), NULL, SND_ASYNC);
					gameOverEffect();
					clearScreen(9, 7);
					drawBlock(GameOver, 53, 12, 9, 6);
					drawBlock(Skull, 69, 17, 9, 7);
					drawText(L"YOUR SCORE: " + to_wstring(score), 73, 31, 9, 7);
					drawScreen();
					Sleep(10000);
					exit(0);
					//startMenuScreen();
				}
			}
			// Check level pass



			// DISPLAY GAME SCREEN
			//drawMap();
			//drawFrame(0, 0, nScreenWidth, nScreenHeight, 0, 7);
			if (frame % 15 == 0) makeNewStarMap(starmap);
			drawStars(starmap);
			for (int i = 0; i <= nLane; i++)
				drawHorizontalLine3(0, lane1 - 1 + i * laneSize + offset, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 9, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 14, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 19, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 24, nScreenWidth, bg, lightblue);			
			//drawHorizontalLine3(0, 29, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 34, nScreenWidth, bg, lightblue);
			//drawHorizontalLine3(0, 39, nScreenWidth, bg, lightblue);
			drawBlock(Player.getSketch(), Player.getX(), Player.getY() + offset, bg, 7);
			for (int i = 0; i < nLane; i++) {
				drawBlock(Enemy[i]->getSketch(), Enemy[i]->getX(), Enemy[i]->getY() + offset, bg, Enemy[i]->getColor());
			}
			drawText(L"LEVEL " + to_wstring(level), 4, 3, bg, white);
			drawText(L"SCORE: " + to_wstring(score), 4, 2, 0, 7);
			drawText(L"Frame: " + to_wstring(frame), 2, nScreenHeight - 1, bg, ch);
			drawScreen();


			frame++;

		}
	}

};

int main() {
	cScreen Screen;
	Screen.startMenuScreen();
	return 0;
}




