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
	ComposeFrame();
	UpdateModel();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (wnd.mouse.LeftIsPressed())
	{
		BrushLogic(curColor);
	}
	else if (wnd.mouse.RightIsPressed())
	{
		BrushLogic(Colors::Black);
	}

	showColorPallet = wnd.kbd.KeyIsPressed('C');
	

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

			if (e.GetCode() == 'Q')
			{
				curColor = gfx.GetPixel(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
			}
		}
	}

	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();

		if (e.GetType() == Mouse::Event::Type::WheelUp)
		{
			if (++curSize > 100)
			{
				curSize = 100;
			}
		}
		else if (e.GetType() == Mouse::Event::Type::WheelDown)
		{
			if (--curSize < 2)
			{
				curSize = 2;
			}
		}
	}

	lastMousePos = wnd.mouse.GetPos();
}

void Game::BrushLogic(Color c)
{
	if (wnd.kbd.KeyIsPressed(VK_SHIFT))
	{
		if (lineDir == 0)
		{
			Vec2 startPos = wnd.mouse.GetPos();
			Vec2 endPos = lastMousePos;

			float dx = (startPos.x - endPos.x) * (startPos.x - endPos.x);
			float dy = (startPos.y - endPos.y) * (startPos.y - endPos.y);
			if (dx == dy)
			{
				return;
			}

			if (dx > dy)
			{
				lineDir = 1; 
				stuckCordinate = startPos.y;
			}
			else
			{
				lineDir = -1;
				stuckCordinate = startPos.x;
			}
		}

		for (int y = -curSize / 2; y < curSize / 2; y++)
		{
			for (int x = -curSize / 2; x < curSize / 2; x++)
			{
				if (x* x + y * y < (curSize / 2) * (curSize / 2))
				{
					Vec2 startPos = wnd.mouse.GetPos();
					Vec2 endPos = lastMousePos;

					if (lineDir > 0)
					{
						startPos.y = stuckCordinate;
						endPos.y = stuckCordinate;
					}
					else
					{
						startPos.x = stuckCordinate;
						endPos.x = stuckCordinate;
					}

					startPos.x += x;
					startPos.y += y;

					endPos.x += x;
					endPos.y += y;

					DrawLine(startPos, endPos, c);
				}
			}
		}
	}
	else
	{
		for (int y = -curSize / 2; y < curSize / 2; y++)
		{
			for (int x = -curSize / 2; x < curSize / 2; x++)
			{
				if (x * x + y * y < (curSize / 2) * (curSize / 2))
				{
					Vec2 startPos = wnd.mouse.GetPos();
					Vec2 endPos = lastMousePos;

					startPos.x += x;
					startPos.y += y;

					endPos.x += x;
					endPos.y += y;

					DrawLine(startPos, endPos, c);
				}
			}
		}
		lineDir = 0;
	}
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
	//Draw Image
	for (size_t y = 0; y < Graphics::ScreenHeight; y++)
	{
		for (size_t x = 0; x < Graphics::ScreenWidth; x++)
		{
				gfx.PutPixel(x, y, GetBackground(x, y));
		}
	}
	
	//Draw Color Pallet
	if (showColorPallet)
	{
		for (int i = 0; i < penColorSize; i++)
		{
			if (wnd.kbd.KeyIsPressed(VK_CONTROL))
			{
				int nVariens = 5;

				for (int j = 0; j < nVariens; j++)
				{
					float alpha = float(1) / float(nVariens);
					alpha *= j;

					Color a = penColors[i];
					Color b = Colors::Black;

					int red = int(a.GetR() * (1 - alpha) + b.GetR() * alpha);
					int green = int(a.GetG() * (1 - alpha) + b.GetG() * alpha);
					int blue = int(a.GetB() * (1 - alpha) + b.GetB() * alpha);

					Color c = Color(red, green, blue);

					for (size_t y = 0; y < 25; y++)
					{
						for (size_t x = 0; x < 25; x++)
						{
							
							gfx.PutPixel(Graphics::ScreenWidth - x - 25 - 40 * i, y + 25 + 40 * j, c);
						}
					}
				}
			}
			else
			{
				for (size_t y = 0; y < 25; y++)
				{
					for (size_t x = 0; x < 25; x++)
					{
						gfx.PutPixel(Graphics::ScreenWidth - x - 25 - 40 * i, y + 25, penColors[i]);
					}
				}

			}
		}
	}

	//Draw Preview of cursor
	for (int y = -curSize / 2; y < curSize / 2; y++)
	{
		for (int x = -curSize / 2; x < curSize / 2; x++)
		{
			if (x * x + y * y < (curSize / 2) * (curSize / 2))
			{
				gfx.PutPixel(50 + x, 50 + y, curColor);
			}
		}
	}
}
