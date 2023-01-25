#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES 
#include <math.h>

enum EKey_Type
{
	EKT_Right,
	EKT_Left,
	EKT_Space,
};

enum EBrick_Type
{
	EBT_None,
	EBT_Red,
	EBT_Blue
};

enum ELetter_Type
{
	ELT_None,
	ELT_O,
	ELT_E,
};

const int Timer_ID = WM_USER + 1;

class AsEngine;
class ALevel;
class ABall
{
public:
	ABall();

	void Init();
	void Draw(HDC hdc, RECT& paint_area, AsEngine* engine);
	void Move(AsEngine* engine, ALevel* level);

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;

	double Ball_Direction;

	static const int Ball_Size = 4;
private:
	int Ball_X_Pos, Ball_Y_Pos;
	double Ball_Speed;

	RECT Ball_Rect, Prev_Ball_Rect;
};

class ALevel
{
public:
	void Init();
	void Draw_Level(HDC hdc, RECT& paint_area);
	void Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction);


	static const int Level_Width = 12; // Level width by cells
	static const int Level_Height = 14; // Level height by cells
	static const int Level_X_Offset = 8;
	static const int Level_Y_Offset = 6;
	static const int Cell_Width = 16;
	static const int Cell_Height = 8;

private:

	void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
	void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
	void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

	HPEN Brick_Red_Pen, Brick_Blue_Pen, Letter_Pen;
	HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
	RECT Level_Rect;
	static const int Brick_Width = 15;
	static const int Brick_Height = 7;
};

class AsEngine
{
public:
	AsEngine();
	void Init_Engine(HWND hWnd);
	void Draw_Frame(HDC hdc, RECT& paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	static int Scale_Value(int value);
	static void Create_Pen_Brush(COLORREF color, HPEN& pen, HBRUSH& brush);

	int Platform_X_Pos;
	int Platform_X_Step;
	int Platform_Width;
	HWND HWnd;

	HPEN BG_Pen;
	HBRUSH BG_Brush;


	static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width;
	static const int Max_Y_Pos = 199 - ABall::Ball_Size;
	static const int Platform_Y_Pos = 185;
	static const int Border_X_Offset = 6;
	static const int Border_Y_Offset = 4;
	static const int Global_Scale = 3;
	static const COLORREF Blue_Color = RGB(85, 255, 255);
	static const COLORREF Red_Color = RGB(255, 85, 85);
	static const COLORREF Dark_Red_Color = RGB(151, 0, 0);
	static const COLORREF Dark_Blue_Color = RGB(0, 128, 192);
	static const COLORREF White_Color = RGB(255, 255, 255);
	static const COLORREF BG_Color = RGB(15, 15, 31);

private:
	void Redraw_Platform();
	void Draw_Platform(HDC hdc, int x, int y);

	void Draw_Border(HDC hdc, int x, int y, bool top_border);
	void Draw_Bounds(HDC hdc, RECT& paint_area);

	int Inner_Width;

	HPEN Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Border_Blue_Pen, Border_White_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush, Border_Blue_Brush, Border_White_Brush;


	RECT Platform_Rect, Prev_Platform_Rect;

	ABall Ball;
	ALevel Level;

	static const int Circle_Size = 7;
	static const int Platform_Height = 7;
};