#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* path)
{
	SDL_Surface* tmpSurface = IMG_Load(path);//загрузка png картинки на поверхность
	if (tmpSurface == NULL)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;//возникла ошибка создания поверхности
		Game::isRunning = Game::OFF;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);//создание тектуры
	if (texture == NULL)
	{
		std::cout << "Can't create texture from surface: " << SDL_GetError() << std::endl;//возникла ошибка создания текстуры
		Game::isRunning = Game::OFF;
	}

	if (Game::isRunning)
	{
		std::cout << "texture " << path << " loaded!\n";
	}

	SDL_FreeSurface(tmpSurface);//очистить поверхность
	return texture;

}

SDL_Texture* TextureManager::LoadText(const wchar_t* message, SDL_Color color, SDL_Rect* rect)
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(Game::gFont, reinterpret_cast<Uint16 const*>(message), color);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		Game::isRunning = Game::OFF;
	}
	rect->w = textSurface->w;
	rect->h = textSurface->h;
	//Create texture from surface pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
	if (texture == NULL)
	{
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		Game::isRunning = Game::OFF;
	}

	if (Game::isRunning)
	{
		std::cout << "texture with text loaded!\n";
	}
	//Get rid of old surface
	SDL_FreeSurface(textSurface);
	return texture;
}


void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}
