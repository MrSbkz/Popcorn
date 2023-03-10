#include "Engine.h"


char Level_01[ALevel::Level_Height][ALevel::Level_Width] =
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


// ABall
ABall::ABall()
	:Ball_X_Pos(20), Ball_Y_Pos(170),
	Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}

void ABall::Init()
{
	AsEngine::Create_Pen_Brush(AsEngine::White_Color, Ball_Pen, Ball_Brush);
}

void ABall::Move(AsEngine* engine, ALevel* level, AsPlatform* platform)
{
	int next_x_pos, next_y_pos;
	int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
	int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
	next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

	if (next_x_pos < AsBorder::X_Offset)
	{
		next_x_pos = ALevel::Level_X_Offset - (next_x_pos - ALevel::Level_X_Offset);;
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos < AsBorder::Y_Offset)
	{
		next_y_pos = AsBorder::Y_Offset - (next_y_pos - AsBorder::Y_Offset);
		Ball_Direction = -Ball_Direction;
	}

	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos > AsEngine::Max_Y_Pos)
	{
		next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
		Ball_Direction = M_PI + (M_PI - Ball_Direction);
	}

	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);


	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Ball_Rect.left = AsEngine::Scale_Value(Ball_X_Pos);
	Ball_Rect.top = AsEngine::Scale_Value(Ball_Y_Pos);
	Ball_Rect.right = Ball_Rect.left + AsEngine::Scale_Value(Ball_Size);
	Ball_Rect.bottom = Ball_Rect.top + AsEngine::Scale_Value(Ball_Size);

	InvalidateRect(engine->HWnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Ball_Rect, FALSE);
}

void ABall::Draw(HDC hdc, RECT& paint_area, AsEngine* engine)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
		return;

	// Clear bacground
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

	// Draw new ball
	SelectObject(hdc, Ball_Pen);
	SelectObject(hdc, Ball_Brush);

	Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

}





// ALevel
void ALevel::Init()
{
	Letter_Pen = CreatePen(PS_SOLID, AsEngine::Global_Scale, AsEngine::White_Color);
	Level_Rect.left = AsEngine::Scale_Value(ALevel::Level_X_Offset);
	Level_Rect.top = AsEngine::Scale_Value(ALevel::Level_Y_Offset);
	Level_Rect.right = Level_Rect.left + AsEngine::Scale_Value(ALevel::Cell_Width * ALevel::Level_Width);
	Level_Rect.bottom = Level_Rect.top + AsEngine::Scale_Value(ALevel::Cell_Height * ALevel::Level_Height);
	AsEngine::Create_Pen_Brush(AsEngine::Red_Color, Brick_Red_Pen, Brick_Red_Brush);
	AsEngine::Create_Pen_Brush(AsEngine::Blue_Color, Brick_Blue_Pen, Brick_Blue_Brush);
}

void ALevel::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		return;

	for (int i = 0; i < Level_Height; i++)
		for (int j = 0; j < Level_Width; j++)
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}

void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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

	RoundRect(hdc, AsEngine::Scale_Value(x), AsEngine::Scale_Value(y), AsEngine::Scale_Value(x + Brick_Width), AsEngine::Scale_Value(y + Brick_Height), AsEngine::Scale_Value(2), AsEngine::Scale_Value(2));
}

void ALevel::Set_Brick_Letter_Colors(
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

void ALevel::Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction)
{
	int brick_y_pos = Level_Y_Offset + Level_Height * Cell_Height;
	for (int i = Level_Height - 1; i >= 0; i--)
	{
		for (int j = 0; j < Level_Width; j++)
		{
			if (Level_01[i][j] == 0)
				continue;

			if (next_y_pos < brick_y_pos)
			{
				next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
				ball_direction = -ball_direction;
			}

		}

		brick_y_pos -= Cell_Height;
	}
}

void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
		return;

	double offset;
	double rotation_angle;
	int brick_half_height = AsEngine::Scale_Value(Brick_Height) / 2;
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

		Rectangle(hdc, x, y + brick_half_height - AsEngine::Global_Scale, x + AsEngine::Scale_Value(Brick_Width), y + brick_half_height);

		// Display foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + AsEngine::Scale_Value(Brick_Width), y + brick_half_height + AsEngine::Global_Scale - 1);
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

		offset = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)AsEngine::Global_Scale;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsEngine::Scale_Value(Brick_Width), brick_half_height - back_part_offset);

		// Display foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height, AsEngine::Scale_Value(Brick_Width), brick_half_height);
		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_O)
			{
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + AsEngine::Scale_Value(5), AsEngine::Scale_Value(-5) / 2, 0 + AsEngine::Scale_Value(10), AsEngine::Scale_Value(5) / 2);
			}
		}

		SetWorldTransform(hdc, &old_xForm);
	}
}




// AsPlatform
AsPlatform::AsPlatform()
	:Inner_Width(21),
	X_Pos(AsBorder::X_Offset),
	X_Step(AsEngine::Global_Scale * 2),
	Width(28)
{
}

void AsPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, AsEngine::White_Color);
	AsEngine::Create_Pen_Brush(AsEngine::Dark_Red_Color, Platform_Circle_Pen, Platform_Circle_Brush);
	AsEngine::Create_Pen_Brush(AsEngine::Dark_Blue_Color, Platform_Inner_Pen, Platform_Inner_Brush);
}

void AsPlatform::Redraw(AsEngine* engine)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = AsEngine::Scale_Value(X_Pos);
	Platform_Rect.top = AsEngine::Scale_Value(Y_Pos);
	Platform_Rect.right = Platform_Rect.left + AsEngine::Scale_Value(Width);
	Platform_Rect.bottom = Platform_Rect.top + AsEngine::Scale_Value(Height);

	InvalidateRect(engine->HWnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(engine->HWnd, &Platform_Rect, FALSE);
}

void AsPlatform::Draw(HDC hdc, AsEngine* engine, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	SelectObject(hdc, engine->BG_Brush);
	SelectObject(hdc, engine->BG_Pen);
	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// Draw circles
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, AsEngine::Scale_Value(X_Pos), AsEngine::Scale_Value(Y_Pos), AsEngine::Scale_Value(X_Pos + Circle_Size), AsEngine::Scale_Value(Y_Pos + Circle_Size));
	Ellipse(hdc, AsEngine::Scale_Value(X_Pos + 21), AsEngine::Scale_Value(Y_Pos), AsEngine::Scale_Value(X_Pos + 21 + Circle_Size), AsEngine::Scale_Value(Y_Pos + Circle_Size));

	// Draw highlight
	SelectObject(hdc, Highlight_Pen);
	Arc(hdc,
		AsEngine::Scale_Value(X_Pos + 1), AsEngine::Scale_Value(Y_Pos + 1),
		AsEngine::Scale_Value(X_Pos + Circle_Size - 1), AsEngine::Scale_Value(Y_Pos + Circle_Size - 1),
		AsEngine::Scale_Value(X_Pos + 1 + 1), AsEngine::Scale_Value(Y_Pos + 1),
		AsEngine::Scale_Value(X_Pos + 1), AsEngine::Scale_Value(Y_Pos + 1 + 2));

	// Draw inner
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, AsEngine::Scale_Value(X_Pos + 4), AsEngine::Scale_Value(Y_Pos + 1), AsEngine::Scale_Value(X_Pos + 4 + Inner_Width - 1), AsEngine::Scale_Value(Y_Pos + 1 + 5), AsEngine::Scale_Value(3), AsEngine::Scale_Value(3));
}




// AsBorder
void AsBorder::Init()
{
	AsEngine::Create_Pen_Brush(AsEngine::Blue_Color, Border_Blue_Pen, Border_Blue_Brush);
	AsEngine::Create_Pen_Brush(AsEngine::White_Color, Border_White_Pen, Border_White_Brush);
}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, AsEngine* engine)
{
	//draw base line
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (top_border)
		Rectangle(hdc, AsEngine::Scale_Value(x), AsEngine::Scale_Value(y + 1), AsEngine::Scale_Value(x + 4), AsEngine::Scale_Value(y + 4));
	else
		Rectangle(hdc, AsEngine::Scale_Value(x + 1), AsEngine::Scale_Value(y), AsEngine::Scale_Value(x + 4), AsEngine::Scale_Value(y + 4));

	// draw edgin
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (top_border)
		Rectangle(hdc, AsEngine::Scale_Value(x), AsEngine::Scale_Value(y), AsEngine::Scale_Value(x + 4), AsEngine::Scale_Value(y + 1));
	else
		Rectangle(hdc, AsEngine::Scale_Value(x), AsEngine::Scale_Value(y), AsEngine::Scale_Value(x + 1), AsEngine::Scale_Value(y + 4));

	// draw perforation
	SelectObject(hdc, engine->BG_Pen);
	SelectObject(hdc, engine->BG_Brush);

	if (top_border)
		Rectangle(hdc, AsEngine::Scale_Value(x + 2), AsEngine::Scale_Value(y + 2), AsEngine::Scale_Value(x + 3), AsEngine::Scale_Value(y + 3));
	else
		Rectangle(hdc, AsEngine::Scale_Value(x + 2), AsEngine::Scale_Value(y + 1), AsEngine::Scale_Value(x + 3), AsEngine::Scale_Value(y + 2));
}

void AsBorder::Draw(HDC hdc, RECT& paint_area, AsEngine* engine)
{
	int i;

	// Left line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 2, 1 + i * 4, false, engine);

	// Right line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 201, 1 + i * 4, false, engine);

	// Top line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 3 + i * 4, 0, true, engine);
}





// AsEngine
AsEngine::AsEngine()
{
}

void AsEngine::Init_Engine(HWND hWnd)
{
	HWnd = hWnd;
	Create_Pen_Brush(BG_Color, BG_Pen, BG_Brush);

	Level.Init();
	Ball.Init();
	Platform.Init();
	Border.Init();

	Platform.Redraw(this);

	SetTimer(HWnd, Timer_ID, 50, 0);
}


void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
{
	Level.Draw(hdc, paint_area);

	Platform.Draw(hdc, this, paint_area);


	/*for (int i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, Scale_Value(20 + i * Cell_Width), 100, EBT_Blue, ELT_O, i);
		Draw_Brick_Letter(hdc, Scale_Value(20 + i * Cell_Width), 130, EBT_Red, ELT_O, i);
	}*/

	Ball.Draw(hdc, paint_area, this);

	Border.Draw(hdc, paint_area, this);
}


int AsEngine::On_Key_Down(EKey_Type key_type)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.X_Pos -= Platform.X_Step;

		if (Platform.X_Pos <= Border.X_Offset)
			Platform.X_Pos = Border.X_Offset;

		Platform.Redraw(this);
		break;

	case EKT_Right:
		Platform.X_Pos += Platform.X_Step;

		if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
			Platform.X_Pos = Max_X_Pos - Platform.Width + 1;

		Platform.Redraw(this);
		break;

	case EKT_Space:
		break;
	}

	return 0;
}

int AsEngine::On_Timer()
{
	Ball.Move(this, &Level, &Platform);

	return 0;
}

int AsEngine::Scale_Value(int value)
{
	return value * Global_Scale;
}

void AsEngine::Create_Pen_Brush(COLORREF color, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, color);
	brush = CreateSolidBrush(color);
}
