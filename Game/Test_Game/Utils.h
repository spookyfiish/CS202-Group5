#pragma once
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
#define FRAMERATE 60
#define screenWidth 145
#define screenHeight 41
#define LANE_HEIGHT 7
#define LEAST_SPACE 10
#define NUM_ENEMY 8
#define MAX_ENEMY_WIDTH 10
#define BG black
#define YELLOW_LIGHT_SECS 1
enum { blueDark = 0, blueLight, orange, yellow, green, purple, red, whiteDark, white, black, brown, pink };

#pragma region structs 
struct coord {
	int x, y;
};
struct BOUNDINGBOX {
	int x = 0, y = 0;
	int w = 0, h = 0;
	coord topLeft() {
		return { x,y };
	}
	coord topRight() {
		return { x + w, y };
	}
	coord bottomLeft() {
		return { x,y + h };
	}
	coord bottomRight() {
		return { x + w, y + h };
	}
};
#pragma endregion

#pragma region Utils
void fixSizedConsoleWindow() {

	//if u use this, change GameCore constructor to new Player(70,37), dont ask me why

	system("MODE 145, 41"); // Set screen size (width, height + 1)
	//Fix window size
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
	// Make custom color palette - up to 16 colors, will update later
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE); // A hanle to console screen buffer.
	
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

	CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hConsoleOutput, &csbiex);
	// up to 16 colors
	csbiex.ColorTable[0] = RGB(16, 16, 60); // Default background color - dark blue
	csbiex.ColorTable[1] = RGB(63, 81, 181); // Light blue
	csbiex.ColorTable[2] = RGB(252, 186, 3); // Orange
	csbiex.ColorTable[3] = RGB(255, 235, 59); // Yellow
	csbiex.ColorTable[4] = RGB(76, 175, 80); // Green
	csbiex.ColorTable[5] = RGB(156, 39, 176); // Purple
	csbiex.ColorTable[6] = RGB(237, 28, 36); // Red
	csbiex.ColorTable[7] = RGB(242, 242, 242); // Dark white
	csbiex.ColorTable[8] = RGB(248, 248, 248); // White
	csbiex.ColorTable[9] = RGB(20, 20, 20); // Black
	csbiex.ColorTable[10] = RGB(156, 139, 93); // Brown
	csbiex.ColorTable[11] = RGB(16, 201, 158); // Pink
	SetConsoleScreenBufferInfoEx(hConsoleOutput, &csbiex);

	//RECT windowRes;
	//const HWND window = GetDesktopWindow();
	//GetWindowRect(window, &windowRes);

	//HWND consoleWindow = GetConsoleWindow();
	//MoveWindow(consoleWindow, (windowRes.right - 1080) / 2, (windowRes.bottom - 720) / 2, 1080, 720, TRUE);

	//LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	//style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);

	//SetWindowLong(consoleWindow, GWL_STYLE, style);

	//CONSOLE_CURSOR_INFO     cursorInfo;
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//GetConsoleCursorInfo(hConsole, &cursorInfo);
	//cursorInfo.bVisible = false; // set the cursor visibility
	//SetConsoleCursorInfo(hConsole, &cursorInfo);

	//CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
	//csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	//GetConsoleScreenBufferInfoEx(hConsole, &csbiex);
	//csbiex.ColorTable[0] = RGB(16, 16, 60); // Default background color - dark blue
	//csbiex.ColorTable[1] = RGB(63, 81, 181); // Light blue
	//csbiex.ColorTable[2] = RGB(255, 87, 34); // Orange
	//csbiex.ColorTable[3] = RGB(255, 235, 59); // Yellow
	//csbiex.ColorTable[4] = RGB(76, 175, 80); // Green
	//csbiex.ColorTable[5] = RGB(156, 39, 176); // Purple
	//csbiex.ColorTable[6] = RGB(237, 28, 36); // Red
	//csbiex.ColorTable[7] = RGB(242, 242, 242); // Dark white
	//csbiex.ColorTable[8] = RGB(248, 248, 248); // White
	//csbiex.ColorTable[9] = RGB(20, 20, 20); // Black
	//SetConsoleScreenBufferInfoEx(hConsole, &csbiex);
}
int random(int a, int b) {
	long long res = (long long)rand() + (long long)rand() * (long long)rand();
	if (res < 0) res = -res;
	return a + ((int)res % (b - a + 1));
}
void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
bool delay(int millisec)
{
	sleep_for(milliseconds(millisec));
	return true;
}
wstring toWstring(int a) {
	wstring res;
	if (!a)
		res = L"0";
	while (a) {
		wstring tmp;
		switch (a % 10) {
		case 0: tmp = L'0'; break;
		case 1: tmp = L'1'; break;
		case 2: tmp = L'2'; break;
		case 3: tmp = L'3'; break;
		case 4: tmp = L'4'; break;
		case 5: tmp = L'5'; break;
		case 6: tmp = L'6'; break;
		case 7: tmp = L'7'; break;
		case 8: tmp = L'8'; break;
		case 9: tmp = L'9'; break;
		}
		res = tmp + res;
		a /= 10;
	}
	return res;
}
void toVwstring(int a, vector<wstring>& result) {
	if (!result.empty()) result.pop_back();
	wstring res;
	if (!a)
		res = {L"0" };
	while (a) {
		wstring tmp;
		switch (a % 10) {
		case 0: tmp = L'0'; break;
		case 1: tmp = L'1'; break;
		case 2: tmp = L'2'; break;
		case 3: tmp = L'3'; break;
		case 4: tmp = L'4'; break;
		case 5: tmp = L'5'; break;
		case 6: tmp = L'6'; break;
		case 7: tmp = L'7'; break;
		case 8: tmp = L'8'; break;
		case 9: tmp = L'9'; break;
		}
		res = tmp + res;
		a /= 10;
	}
	result.emplace_back(res);
	return;
}
#pragma endregion