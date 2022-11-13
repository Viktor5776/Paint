/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	for (size_t i = 0; i < Graphics::ScreenHeight * Graphics::ScreenWidth; i++)
	{
		background[i] = Colors::Black;
	}
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (wnd.mouse.LeftIsPressed())
	{
		for (int y = -curSize / 2; y < curSize / 2; y++)
		{
			for (int x = -curSize / 2; x < curSize / 2; x++)
			{
				if (x * x + y * y < curSize / 2)
				{
					Vec2 startPos = wnd.mouse.GetPos();
					Vec2 endPos = lastMousePos;

					startPos.x += x;
					startPos.y += y;

					endPos.x += x;
					endPos.y += y;

					DrawLine(startPos, endPos, penColors[curColor]);
				}
			}
		}
	}
	else if (wnd.mouse.RightIsPressed())
	{
		for (int y = -curSize / 2; y < curSize / 2; y++)
		{
			for (int x = -curSize / 2; x < curSize / 2; x++)
			{
				if (x * x + y * y < curSize / 2)
				{
					Vec2 startPos = wnd.mouse.GetPos();
					Vec2 endPos = lastMousePos;

					startPos.x += x;
					startPos.y += y;

					endPos.x += x;
					endPos.y += y;

					DrawLine(startPos, endPos, Colors::Black);
				}
			}
		}
	}

	while (!wnd.kbd.KeyIsEmpty())
	{
		const Keyboard::Event e = wnd.kbd.ReadKey();

		if (e.IsRelease())
		{
			if (e.GetCode() == 'R')
			{
				for (size_t i = 0; i < Graphics::ScreenHeight * Graphics::ScreenWidth; i++)
				{
					background[i] = Colors::Black;
				}
			}
			
			else if (e.GetCode() == 'C')
			{
				if (++curColor >= penColorSize)
				{
					curColor = 0;
				}
			}

			else if (e.GetCode() == VK_UP)
			{
				if (++curSize > 2500)
				{
					curSize = 2500;
				}
			}

			else if (e.GetCode() == VK_DOWN)
			{
				if (--curSize < 1)
				{
					curSize = 1;
				}
			}
		}
	}
	if (wnd.kbd.KeyIsPressed(VK_UP) && curSize >= 10)
	{
		if (curSize > 2500)
		{
			curSize = 2500;
		}
		else
		{
			curSize = curSize * 1.2;
			if (curSize > 2500)
			{
				curSize = 2500;
			}
		}
	}
	else if (wnd.kbd.KeyIsPressed(VK_DOWN) && curSize >= 10)
	{
		curSize = curSize * 0.8;
		if (curSize < 10)
		{
			curSize = 11;
		}
	}

	lastMousePos = wnd.mouse.GetPos();
}

void Game::DrawLine(Vec2 v1, Vec2 v2, Color c)
{
	float x1 = v1.x;
	float x2 = v2.x;
	float y1 = v1.y;
	float y2 = v2.y;


	const float dx = x2 - x1;
	const float dy = y2 - y1;

	if (dy == 0.0f && dx == 0.0f)
	{
		GetBackground(int(x1), int(y1)) = c;
	}
	else if (abs(dy) > abs(dx))
	{
		if (dy < 0.0f)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float m = dx / dy;
		float y = y1;
		int lastIntY;
		for (float x = x1; y < y2; y += 1.0f, x += m)
		{
			lastIntY = int(y);
			GetBackground(int(x), lastIntY) = c;
		}
		if (int(y2) > lastIntY)
		{
			GetBackground(int(x2), int(y2));
		}
	}
	else
	{
		if (dx < 0.0f)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float m = dy / dx;
		float x = x1;
		int lastIntX;
		for (float y = y1; x < x2; x += 1.0f, y += m)
		{
			lastIntX = int(x);
			GetBackground(lastIntX, int(y)) = c;
		}
		if (int(x2) > lastIntX)
		{
			GetBackground(int(x2), int(y2)) = c;
		}
	}
}

Color& Game::GetBackground(int x, int y)
{
	x >= Graphics::ScreenWidth ? x = Graphics::ScreenWidth - 1: 0;

	x < 0 ? x = 0 : 0;

	y >= Graphics::ScreenHeight ? y = Graphics::ScreenHeight - 1 : 0;

	y < 0 ? y = 0 : 0;

	return background[y * Graphics::ScreenWidth + x];
}

void Game::ComposeFrame()
{
	for (size_t y = 0; y < Graphics::ScreenHeight; y++)
	{
		for (size_t x = 0; x < Graphics::ScreenWidth; x++)
		{
			gfx.PutPixel(x, y, GetBackground(x,y));
		}
	}


	for (int y = -curSize / 2; y < curSize / 2; y++)
	{
		for (int x = -curSize / 2; x < curSize / 2; x++)
		{
			if (x * x + y * y < curSize / 2)
			{
				gfx.PutPixel(50 + x, 50 + y, penColors[curColor]);
			}
		}
	}
}
