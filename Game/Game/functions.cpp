#include "Header.h"

#pragma region Util
void fixSizedConsoleWindow() {
	system("MODE 300, 44");

	RECT windowRes;
	const HWND window = GetDesktopWindow();
	GetWindowRect(window, &windowRes);

	HWND consoleWindow = GetConsoleWindow();
	MoveWindow(consoleWindow, (windowRes.right - 1080) / 2, (windowRes.bottom - 720) / 2, 1080, 720, TRUE);

	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);

	SetWindowLong(consoleWindow, GWL_STYLE, style);

	CONSOLE_CURSOR_INFO     cursorInfo;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	CONSOLE_SCREEN_BUFFER_INFOEX csbiex;
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hConsole, &csbiex);
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
	SetConsoleScreenBufferInfoEx(hConsole, &csbiex);
}

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool delay(int millisec) {
    sleep_for(milliseconds(millisec));
    return true;
}
#pragma endregion