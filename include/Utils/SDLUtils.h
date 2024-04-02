#pragma once

void SDL_DrawCircle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY, int32_t radius)
{
	int32_t x = radius - 1;
	int32_t y = 0;
	int32_t dx = 1;
	int32_t dy = 1;
	int32_t err = dx - (radius << 1);

	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}
