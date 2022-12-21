#include "Engine.h"

#define _USE_MATH_DEFINES 
#include <math.h>

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Size = 7;

int Inner_Width = 21;

HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen;
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
	EBT_Red,
	EBT_Blue
};

enum ELetter_Type
{
	ELT_None,
	ELT_O,
	ELT_E,
};

int Scale_Value(int value)
{
	return value * Global_Scale;
}

void Create_Pen_Brush(COLORREF color, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, color);
	brush = CreateSolidBrush(color);
}

void Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));
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
	case EBT_Red:
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

	RoundRect(hdc, Scale_Value(x), Scale_Value(y), Scale_Value(x + Brick_Width), Scale_Value(y + Brick_Height), Scale_Value(2), Scale_Value(2));
}

void Set_Brick_Letter_Colors(
	bool is_switch_color,
	HPEN& front_pen,
	HBRUSH& front_brush,
	HPEN& back_pen,
	HBRUSH& back_brush)
{
	if (is_switch_color)
	{
		front_pen = Brick_Red_Pen;
		front_brush = Brick_Red_Brush;

		back_pen = Brick_Blue_Pen;
		back_brush = Brick_Blue_Brush;
	}
	else
	{
		front_pen = Brick_Blue_Pen;
		front_brush = Brick_Blue_Brush;

		back_pen = Brick_Red_Pen;
		back_brush = Brick_Red_Brush;
	}
}

void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
		return;

	double offset;
	double rotation_angle;
	int brick_half_height = Scale_Value(Brick_Height) / 2;
	int back_part_offset;
	bool switch_color;

	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;

	XFORM xForm, old_xForm;

	rotation_step = rotation_step % 16;

	if (rotation_step < 8)
	{
		rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
	}
	else
	{
		rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
	}

	if (rotation_step > 4 && rotation_step <= 12)
	{
		switch_color = brick_type == EBT_Blue;
	}
	else
	{
		switch_color = brick_type == EBT_Red;
	}

	Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

	if (rotation_step == 4 || rotation_step == 12)
	{
		// Display background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Scale_Value(Brick_Width), y + brick_half_height);

		// Display foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + Scale_Value(Brick_Width), y + brick_half_height + Global_Scale - 1);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		// set "rotate" matrix of letter
		xForm.eM11 = 1.0f;
		xForm.eM12 = 0.0f;
		xForm.eM21 = 0.0f;
		xForm.eM22 = (float)cos(rotation_angle);
		xForm.eDx = (float)x;
		xForm.eDy = (float)y + (float)brick_half_height;
		GetWorldTransform(hdc, &old_xForm);
		SetWorldTransform(hdc, &xForm);

		// Display background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		offset = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)Global_Scale;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Scale_Value(Brick_Width), brick_half_height - back_part_offset);

		// Display foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, Scale_Value(Brick_Width), brick_half_height);
		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + Scale_Value(5), Scale_Value(-5) / 2, 0 + Scale_Value(10), Scale_Value(5) / 2);
			}
		}

		SetWorldTransform(hdc, &old_xForm);
	}
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

	Ellipse(hdc, Scale_Value(x), Scale_Value(y), Scale_Value(x + Circle_Size), Scale_Value(y + Circle_Size));
	Ellipse(hdc, Scale_Value(x + 21), Scale_Value(y), Scale_Value(x + 21 + Circle_Size), Scale_Value(y + Circle_Size));

	// Draw highlight
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc,
		Scale_Value(x + 1), Scale_Value(y + 1),
		Scale_Value(x + Circle_Size - 1), Scale_Value(y + Circle_Size - 1),
		Scale_Value(x + 1 + 1), Scale_Value(y + 1),
		Scale_Value(x + 1), Scale_Value(y + 1 + 2));

	// Draw inner
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, Scale_Value(x + 4), Scale_Value(y + 1), Scale_Value(x + 4 + Inner_Width - 1), Scale_Value(y + 1 + 5), Scale_Value(3), Scale_Value(3));
}

void Draw_Frame(HDC hdc)
{
	/*Draw_Level(hdc);

	Draw_Platform(hdc, 50, 100);*/
	for (int i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, Scale_Value(20 + i * Cell_Width), 100, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, Scale_Value(20 + i * Cell_Width), 130, EBT_Red, ELT_O, i);
	}

}