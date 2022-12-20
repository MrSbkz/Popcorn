#include "Engine.h"

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Size = 7;

int Inner_Width = 21;

HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

COLORREF Blue_Color = RGB(85, 255, 255);
COLORREF Red_Color = RGB(255, 85, 85);
COLORREF Dark_Red_Color = RGB(151, 0, 0);
COLORREF Dark_Blue_Color = RGB(0, 128, 192);
COLORREF White_Color = RGB(255, 255, 255);

char Level_01[14][12] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

enum EBrick_Type
{
	EBT_None,
	EBT_Purple,
	EBT_Blue
};

void Create_Pen_Brush(COLORREF color, HPEN &pen, HBRUSH &brush)
{
	pen = CreatePen(PS_SOLID, 0, color);
	brush = CreateSolidBrush(color);
}

void Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	Create_Pen_Brush(Red_Color, Brick_Red_Pen, Brick_Red_Brush);
	Create_Pen_Brush(Blue_Color, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(Dark_Red_Color, Platform_Circle_Pen, Platform_Circle_Brush);
	Create_Pen_Brush(Dark_Blue_Color, Platform_Inner_Pen, Platform_Inner_Brush);
}

void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	HPEN pen;
	HBRUSH brush;

	switch (brick_type)
	{
	case EBT_Purple:
		pen = Brick_Red_Pen;
		brush = Brick_Red_Brush;
		break;
	case EBT_Blue:
		pen = Brick_Blue_Pen;
		brush = Brick_Blue_Brush;
		break;
	default:
		return;
	}

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}

void Draw_Level(HDC hdc)
{
	for (int i = 0; i < 14; i++)
		for (int j = 0; j < 12; j++)
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}

void Draw_Platform(HDC hdc, int x, int y)
{
	// Draw circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + 21) * Global_Scale, y * Global_Scale, (x + 21 + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);

	// Draw highlight
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc, 
		(x + 1) * Global_Scale, (y + 1) * Global_Scale,
		(x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
		(x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale,
		(x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);

	// Draw inner
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}

void Draw_Frame(HDC hdc)
{
	Draw_Level(hdc);
	Draw_Platform(hdc, 50, 100);
}