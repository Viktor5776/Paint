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

#include <iostream>
#include <string>

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
		for (size_t y = 0; y < curSize; y++)
		{
			for (size_t x = 0; x < curSize; x++)
			{
				background[(wnd.mouse.GetPosY() + y )* Graphics::ScreenWidth + wnd.mouse.GetPosX() + x] = penColors[curColor];
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
				if (++curSize > 16)
				{
					curSize = 16;
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

}

void Game::ComposeFrame()
{
	for (size_t y = 0; y < Graphics::ScreenHeight; y++)
	{
		for (size_t x = 0; x < Graphics::ScreenWidth; x++)
		{
			gfx.PutPixel(x, y, background[y * Graphics::ScreenWidth + x]);
		}
	}


	for (size_t y = 0; y < curSize; y++)
	{
		for (size_t x = 0; x < curSize; x++)
		{
			gfx.PutPixel(20 + x, 20 + y, penColors[curColor]);
		}
	}
}
