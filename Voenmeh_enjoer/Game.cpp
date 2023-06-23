#include "Game.h"
#include "SoundManager.h"
#include "Manager.h"
#include <random>
#include <locale>
#include <codecvt>

SDL_Renderer* Game::renderer = nullptr;
int Game::isRunning = OFF;
int Game::score = 0;
TTF_Font* Game::gFont = NULL;
int Game::SelectedItem = 0;
std::wstring Game::name;

bool Game::Flag1 = false;
bool Game::Flag2 = false;


SDL_Color color = { 155, 0, 0 };


Manager manager;
Player* player;

std::chrono::steady_clock::time_point game_time;
std::chrono::steady_clock::time_point isActive_time;
std::chrono::steady_clock::time_point tick_time;
int time_isActive = 0;
int count = 0;
int x = 1;
int ypos = 0;
int tick = 90;
Game::Game()
{
	isRunning = OFF;
	window = NULL;
	renderer = NULL;

	gMusic = NULL;
	gMenu = NULL;

	gWin = NULL;
	gLose = NULL;
	gSelect = NULL;
	gStart = NULL;
	gItem = NULL;
	gScroll = NULL;
}
Game::~Game()
{
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{

	isRunning = PAUSE;
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;//окно игры на весь экран
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)//инициализация всех подсистем SDL(timer, audio, video, cdrom и joystick)
	{
		std::cout << "Can't init everything: " << SDL_GetError() << std::endl;//возникла ошибка инициализации
		isRunning = OFF;
	}

	std::cout << "Sybsystems Initialised!...\n";
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);//Создать окно с указанным положением, размерами и флагами.

	if (window == NULL)
	{
		std::cout << "Can't create window: " << SDL_GetError() << std::endl;//возникла ошибка создания окна
		isRunning = OFF;
	}

	std::cout << "Window created!\n";
	renderer = SDL_CreateRenderer(window, -1, 0);//Создать контекст 2D-рендеринга для окна

	if (renderer == nullptr)
	{
		std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;//возникла ошибка создания рендеринга
		isRunning = OFF;

	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		isRunning = OFF;
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		isRunning = OFF;
	}

	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		isRunning = OFF;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//Установить цвет, используемый для операций рисования
	std::cout << "Renderer created!\n";

	manager.addMap(0);

	gMenu = SoundManager::LoadMusic("sounds/main_menu.mp3");
	gMusic = SoundManager::LoadMusic("sounds/Dr_Livesey.mp3");

	/*gMenu = SoundManager::LoadMusic("sounds/Black_eyes.mp3");
	gMusic = SoundManager::LoadMusic("sounds/Bomzh.mp3");*/

	gWin = SoundManager::LoadMusic("sounds/winner.mp3");
	gLose = SoundManager::LoadMusic("sounds/loser.mp3");

	gStart = SoundManager::LoadSound("sounds/here_we_go_again.wav");
	gItem = SoundManager::LoadSound("sounds/get_item.wav");
	gSelect = SoundManager::LoadSound("sounds/select_menu.wav");
	gScroll = SoundManager::LoadSound("sounds/gtasa.wav");
	Mix_PlayChannel(-1, gStart, 0);
	if (isRunning)
	{
		isRunning = MENU;
	}
}


bool flag = true;
void Game::menu()
{
	score = 0;
	SelectedItem = 0;

	if (flag && isRunning == MENU)
	{
		Mix_PlayMusic(gMenu, -1);
		flag = false;
	}
	isRunning = MENU;
	Text::cnt = 0;
	GameObject* o;
	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}
	manager.refresh();

	color = { 55, 0, 0 };
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 100);

	manager.addText(50, 50, L"Запустить", color);
	manager.addText(50, 250, L"Справка", color);
	manager.addText(50, 450, L"Имя игрока", color);
	manager.addText(50, 650, L"Рекорды", color);
	manager.addText(50, 850, L"Выход", color);
	while (isRunning == MENU)
	{
		update();
		render();
		handleEvents();
		SDL_Delay(10);
	}
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);

}

void Game::start()
{
	tick = 90;
	SelectedItem = 10;
	count = 0;
	time_isActive = 0;
	x = 1;
	GameObject* o;

	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}

	manager.refresh();

	isRunning = ON;
	manager.addMap(1);
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 35);
	color = { 0, 0, 0 };
	manager.addText(0, 0, L"Время: ", color);
	manager.addText(0, 50, L"Очки: ", color);
	player = new Player();
	manager.addPlayer(player);





	manager.addText(100, 50, L"0", color);
	manager.addText(120, 0, L"90", color);
	tick_time = std::chrono::steady_clock::now();
	game_time = std::chrono::steady_clock::now();
	isActive_time = std::chrono::steady_clock::now();
	Mix_PlayMusic(gMusic, -1);

	std::chrono::steady_clock::time_point generate__time = std::chrono::steady_clock::now();
	generateGameObject();
	const short FPS = 60;//количество кадров в секунду
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime = 0;
	while (isRunning == ON)
	{
		frameStart = SDL_GetTicks();//сколько прошло времени с начала инициализации окна
		handleEvents();//опрос действий предпринятых пользователем
		if ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - generate__time).count() >= 3))
		{
			generateGameObject();
			generate__time = std::chrono::steady_clock::now();
		}
		update();//обновление состояния игры
		render();//отрисовка на экране игры

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)//ограничение по кадрам в секунду
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
	Mix_HaltMusic();
}

void Game::end()
{
	GameObject* o;

	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}
	color = { 0, 0, 0 };
	manager.refresh();
	isRunning = END;
	if (score > 0)
	{
		wchar_t buffer[10];
		swprintf(buffer, 10, L"%d", score);
		const wchar_t* myWcharPtr = buffer;

		Mix_PlayMusic(gWin, -1);
		manager.addText(1920 / 2 - 300, 300, L"Игра окончена!", color);
		manager.addText(1920 / 2 - 300, 350, L"Ваши очки: ", color);
		manager.addText(1920 / 2, 350, myWcharPtr, color);

	}
	else
	{
		manager.addText(1920 / 2 - 300, 300, L"Увы, Вы проиграли :-(", color);
		Mix_PlayMusic(gLose, -1);
	}

	manager.addText(1920 / 2 - 300, 400, L"Для продолжения нажмите enter", color);
	while (isRunning == END) {
		render(); handleEvents(); SDL_Delay(10);
	}
	Mix_HaltMusic();
	flag = true;
}


void Game::info()
{
	isRunning = INFO;
	GameObject* o;

	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}
	color = { 0, 0, 0 };
	manager.refresh();

	manager.addText(1920 / 2 - 400, 100, L"Добро пожаловать в игру Военмех enjoer!", color);
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 40);
	manager.addText(100, 200, L"Краткие правила и действия:", color);
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 35);
	manager.addText(100, 300, L"Вы играете за студента БГТУ Военмех, игра длится 1.5 минуты. Ваш персонаж передвигается по карте и собирает", color);
	manager.addText(100, 370, L"различные   предметы,   которые   дают   или   отнимают   очки.  Цель  игры  -   набрать   как  можно больше очков.", color);
	manager.addText(100, 440, L"Каждые 30 секунд время жизни объектов уменьшается,  но при этом их  ценность возрастает (дают больше очков).", color);
	manager.addText(100, 510, L"Если  игрок собирает 3 предмета, отбирающие очки (пачка сигарет или оценка 3), то он  проигрывает, и игра завер-", color);
	manager.addText(100, 580, L"шается  с 0 очков. Для перемещения Вы используете клавиши w, s, d, a. Существует возможность  выйти из игры в", color);
	manager.addText(100, 650, L"главное меню нажатием клавиши ESC.", color);

	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
	manager.addText(100, 750, L"Военмех - лучше всех!", color);
	manager.addText(100, 850, L"Для возврата в главное меню нажмите enter.", color);

	while (isRunning == INFO) { render(); handleEvents(); SDL_Delay(10); }
}

void Game::player_name()
{
	isRunning = PLAYER_NAME;
	GameObject* o;
	name.clear();
	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}
	color = { 0, 0, 0 };
	manager.refresh();

	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
	manager.addText(100, 100, L"Введите имя игрока (не больше 10 символов и без пробелов):", color);

	SDL_StartTextInput();
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 35);
	while (isRunning == PLAYER_NAME || name.empty())
	{
		handleEvents();
		update();
		render();
	}
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
}

void Game::records(std::list<Records*>& r)
{
	Flag1 = false;
	Flag2 = false;
	SelectedItem = 0;
	Text::cnt = 0;
	isRunning = RECORDS;
	GameObject* o;
	for (int i = 0; i < manager.get_size(); i++)
	{
		o = manager.get_GameObject(i);
		o->destroy();
	}
	color = { 55, 0, 0 };
	manager.refresh();
	manager.addText(1200, 100, L"Очистить список", color);
	manager.addText(1200, 200, L"Подготовить файл для печати", color);
	manager.addText(1200, 300, L"Вернуться в главное меню", color);

	color = { 0, 0, 0 };
	ypos = 250;
	manager.addText(100, 100, L"Рекорды:", color);
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 35);
	manager.addText(100, 200, L"         Игрок             Очки      ", color);
	int count = 235;
	for (auto element : r)
	{
		ypos += 35;
		count += 35;
		std::string s = element->get_player_name();
		std::string s1 = std::to_string(element->get_points());
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring myWstring = converter.from_bytes(s);
		const wchar_t* myWcharPtr = myWstring.c_str();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter1;
		std::wstring myWstring1 = converter.from_bytes(s1);
		const wchar_t* myWcharPtr1 = myWstring1.c_str();

		manager.addText(120, count, myWcharPtr, color);
		manager.addText(400, count, myWcharPtr1, color);
		count += 35;
		ypos += 35;
	}



	while (isRunning == RECORDS)
	{
		update();
		render();
		handleEvents();
		SDL_Delay(10);
	}
	gFont = TTF_OpenFont("text/Times_New_Roman.ttf", 50);
}
void Game::handleEvents()
{
	SDL_PollEvent(&event);//опрос текущих ожидающих событий
	switch (event.type)
	{
	case SDL_QUIT: //нажали на крестик
		isRunning = OFF;
		break;
	default:
		break;
	}


	if (isRunning == MENU)
	{

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				SelectedItem = (SelectedItem - 1 + 5) % 5;
				Mix_PlayChannel(-1, gScroll, 0);
				break;
			case SDLK_DOWN:
				SelectedItem = (SelectedItem + 1) % 5;
				Mix_PlayChannel(-1, gScroll, 0);

				break;
			case SDLK_RETURN:
				Mix_PlayChannel(-1, gSelect, 0);
				switch (SelectedItem)
				{
				case 0:
					if (!name.empty())
					{
						std::cout << "Play selected" << std::endl;
						isRunning = ON;
					}
					else
					{
						std::cout << "Change Player selected" << std::endl;
						isRunning = PLAYER_NAME;
					}
					break;

				case 1:
					std::cout << "Info selected" << std::endl;
					isRunning = INFO;
					break;

				case 2:
					std::cout << "Change Player selected" << std::endl;
					isRunning = PLAYER_NAME;
					break;

				case 3:
					std::cout << "Leaderboard selected" << std::endl;
					isRunning = RECORDS;
					break;

				case 4:
					isRunning = OFF;
					break;
				}
				break;
			}
		}
	}
	else if (isRunning == ON)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if (player->get_pos().y <= 0)
				{
					player->velocity.y = 0;
					player->set_ypos(0);
				}

				else
				{
					player->velocity.y = -1;
				}

				player->Play("WalkUp");
				break;
			case SDLK_a:
				if (player->get_pos().x <= 0)
				{
					player->velocity.x = 0;
					player->set_xpos(0);
				}

				else
				{
					player->velocity.x = -1;
				}

				player->Play("WalkLeft");
				break;
			case SDLK_s:
				if (player->get_pos().y > 980)
				{
					player->velocity.y = 0;
					player->set_ypos(980);
				}
				else
				{
					player->velocity.y = 1;
				}

				player->Play("WalkDown");
				break;
			case SDLK_d:
				if (player->get_pos().x > 1780)
				{
					player->set_xpos(1780);
				}

				else
				{
					player->velocity.x = 1;
				}

				player->Play("WalkRight");
				break;
			case SDLK_ESCAPE:
				isRunning = END;
				break;
			case SDLK_9:
				//If there is no music playing
				if (Mix_PlayingMusic() == 0)
				{
					//Play the music
					Mix_PlayMusic(gMusic, -1);
				}
				//If music is being played
				else
				{
					//If the music is paused
					if (Mix_PausedMusic() == 1)
					{
						//Resume the music
						Mix_ResumeMusic();
					}
					//If the music is playing
					else
					{
						//Pause the music
						Mix_PauseMusic();
					}
				}
				break;
			case SDLK_0:
				//Stop the music
				Mix_HaltMusic();
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				player->velocity.y = 0;
				player->Play("Idle");
				break;
			case SDLK_s:
				player->velocity.y = 0;
				player->Play("Idle");
				break;
			case SDLK_a:
				player->velocity.x = 0;
				player->Play("Idle");
				break;
			case SDLK_d:
				player->velocity.x = 0;
				player->Play("Idle");
				break;
			}
		}
	}
	else if (isRunning == END || isRunning == INFO)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				isRunning = MENU;
				break;
			}
		}
	}
	else if (isRunning == PLAYER_NAME)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				isRunning = MENU;
				break;
			case SDLK_BACKSPACE:

				if (!name.empty())
				{
					name.pop_back();
					const wchar_t* myWcharPtr = name.c_str();
					manager.refresh();
					if (!name.empty())
						manager.addText(100, 200, myWcharPtr, color);
					else
					{
						GameObject* o = manager.get_GameObject(1);
						o->destroy();
					}
					std::wcout << name << std::endl;

				}
				break;

			}
		}
		if (event.type == SDL_TEXTINPUT && *event.text.text != ' ' && name.size() < 10)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv; // создаем конвертер
			std::wstring ws = conv.from_bytes(event.text.text); // конвертируем в std::wstring
			name = name + ws;
			const wchar_t* myWcharPtr = name.c_str();
			manager.addText(100, 200, myWcharPtr, color);
			std::wcout << name << std::endl;

		}

	}
	else if (isRunning == RECORDS)
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				SelectedItem = (SelectedItem - 1 + 3) % 3;
				Mix_PlayChannel(-1, gScroll, 0);
				break;
			case SDLK_DOWN:
				SelectedItem = (SelectedItem + 1) % 3;
				Mix_PlayChannel(-1, gScroll, 0);
				break;
			case SDLK_RETURN:

				Mix_PlayChannel(-1, gSelect, 0);
				switch (SelectedItem)
				{
				case 0:
					std::cout << "Clear selected" << std::endl;
					Mix_PlayChannel(-1, gSelect, 0);
					Flag1 = true;
					break;

				case 1:
					std::cout << "Print selected" << std::endl;
					Mix_PlayChannel(-1, gSelect, 0);
					Flag2 = true;
					break;

				case 2:
					std::cout << "Back menu selected" << std::endl;
					isRunning = MENU;;
					break;
				}
				break;
			}
		}
	}

}

void Game::generateGameObject()
{
	std::random_device rdx, rdy, rdn;
	std::mt19937 gen1(rdx());
	std::uniform_int_distribution<> dist1(0, 1840);
	int xpos = dist1(gen1);

	std::mt19937 gen2(rdy());
	std::uniform_int_distribution<> dist2(0, 1000);

	int ypos = dist2(gen2);

	std::mt19937 gen3(rdn());
	std::uniform_int_distribution<> dist3(0, 7);

	char n = dist3(gen3);
	manager.addGameObject(xpos, ypos, 8 - time_isActive, n);
}



void Game::update()
{

	if (isRunning == PLAYER_NAME && manager.get_size() > 2)
	{
		GameObject* o = manager.get_GameObject(1);
		o->destroy();
	}

	manager.refresh();
	manager.update();


	if (isRunning == ON)
	{
		if ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - game_time).count() >= 92))
			isRunning = END;
		if (count >= 3)
		{
			score = 0;
			isRunning = END;
		}
		if ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - isActive_time).count() >= 30))
		{
			++x;
			time_isActive += 2;
			isActive_time = std::chrono::steady_clock::now();
		}

		if (player->get_ypos() > 980)
		{
			player->set_ypos(980);
		}
		if (player->get_xpos() > 1780)
		{
			player->set_xpos(1780);
		}
		if (player->get_ypos() <= 0)
		{
			player->set_ypos(0);
		}
		if (player->get_xpos() <= 0)
		{
			player->set_xpos(0);
		}

		GameObject* o;
		GameObject* s = NULL;

		for (int i = 4, points = 0; i < manager.get_size(); i++)
		{
			if (manager.isText(i) && manager.get_GameObject(i)->get_ypos() == 50)
			{
				s = manager.get_GameObject(i);
				break;
			}
		}


		for (int i = 4, points = 0; i < manager.get_size(); i++)
		{
			o = manager.get_GameObject(i);
			if (manager.isText(i) && manager.get_GameObject(i)->get_ypos() == 50)
			{
				continue;
			}
			if (manager.isText(i) && manager.get_GameObject(i)->get_ypos() == 0 && (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - tick_time).count() >= 1))
			{
				o->destroy();
				manager.refresh();
				tick--;
				wchar_t buffer[10];
				swprintf(buffer, 10, L"%d", tick);
				const wchar_t* myWcharPtr = buffer;
				manager.addText(120, 0, myWcharPtr, color);
				tick_time = std::chrono::steady_clock::now();
				continue;
			}
			else if (manager.isText(i) && manager.get_GameObject(i)->get_ypos() == 0)
			{
				continue;
			}


			if (Collision::AABB(player->get_Rect(), o->get_Rect()))
			{
				Mix_PlayChannel(-1, gItem, 0);
				points = x * o->get_point();
				o->destroy();
				manager.refresh();
				--i;
				score += points;
				s->destroy();
				manager.refresh();
				wchar_t buffer[10];
				swprintf(buffer, 10, L"%d", score);
				const wchar_t* myWcharPtr = buffer;
				manager.addText(100, 50, myWcharPtr, color);
				s = manager.get_GameObject(manager.get_size()-1);
				if (points < 0)
				{
					count++;
				}
			}
		}
	}


}

void Game::render()
{
	SDL_RenderClear(renderer);//Очистить текущую цель рендеринга цветом рисунка
	manager.draw();
	if (isRunning == RECORDS)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//Установить цвет, используемый для операций рисования
		SDL_Rect rect2 = { 100, 190, 400, 70 };
		SDL_RenderDrawRect(renderer, &rect2);
		SDL_RenderDrawLine(renderer, 360, 190, 360, ypos);
		SDL_RenderDrawLine(renderer, 100, 190, 100, ypos);
		SDL_RenderDrawLine(renderer, 499, 190, 499, ypos);
		SDL_RenderDrawLine(renderer, 100, ypos, 499, ypos);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//Установить цвет, используемый для операций рисования

	}
	SDL_RenderPresent(renderer);//Обновить экран рендерингом
}

void Game::clean()
{
	SDL_DestroyWindow(window);//удалить окно
	SDL_DestroyRenderer(renderer);//удалить рендеринг

	TTF_CloseFont(gFont);

	Mix_FreeMusic(gMusic);
	Mix_FreeMusic(gMenu);
	Mix_FreeMusic(gWin);
	Mix_FreeMusic(gLose);

	Mix_FreeChunk(gSelect);
	Mix_FreeChunk(gItem);
	Mix_FreeChunk(gStart);
	Mix_FreeChunk(gScroll);

	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();//Очистить все инициализированные подсистемы
	std::cout << "Game cleaned!\n";
}

