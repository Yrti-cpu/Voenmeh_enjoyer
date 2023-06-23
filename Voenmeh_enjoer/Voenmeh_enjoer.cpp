#include "Game.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <string>

Game* game = nullptr;
std::list<Records*> records;

bool comp(Records* a, Records* b)//компаратор
{
	return a->get_points() > b->get_points();
}
void DownloadData();//загрузка рекордов
void UploadData();//выгрузка рекордов
void PrintData();
int main(int argc, char* args[])
{
	DownloadData();
	game = new Game();//инициализация игры
	setlocale(LC_ALL, "rus");
	game->init("Collector", 1920, 1080, false);//инициализация окна и рендеринга
	if (!game->running())
	{
		return 1;
	}
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < 3)) { game->render(); }
	while (game->running())
	{

		while (game->running() == 4 || game->running() == 2 || game->running() == 5 || game->running() == 6)
		{
			game->menu();

			if (game->running() == 4)
			{
				game->info();
			}
			if (game->running() == 5)
			{
				game->player_name();
			}
			if (game->running() == 6)
			{
				game->records(records);
				if (Game::Flag1)
					records.clear();
				if (Game::Flag2)
					PrintData();

				Game::Flag1 = Game::Flag2 = false;
			}
		}


		if (game->running())
		{

			game->start();
			game->end();

		}


		UploadData();
	}


	game->clean();//удаление всего что связнано с игрой
	return 0;
}

void DownloadData()
{
	remove("records.txt");//удаляем старый файл
	std::ifstream fin;
	fin.open("records", std::ifstream::binary);//открываем файл на чтение
	if (!fin.is_open())//если файла нет, то прекращаем работу метода
		return;
	std::string row;
	short i = 2;
	while (!fin.eof())//идем по файлу пока не конец
	{
		std::string* result = new std::string[i];//массив строк
		short count = 0;
		row = "";
		getline(fin, row);//считываем строку
		std::istringstream input(row);//добавляем в поток 
		while (!input.eof())//пока не конец потока
		{
			std::string str;
			input >> str;
			result[count] = str;//добавляем в массив 
			count++;
		}

		Records* r = new Records(atoi(result[1].c_str()), result[0]);
		records.push_back(r); //добавляем в список
		delete[] result;//удаляем результирующий массив 
		input.clear();//чистим поток input
	}
	fin.close();//закрываем файл
}

void UploadData()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv; // создаем конвертер
	std::string s = conv.to_bytes(Game::name); // конвертируем в std::string
	if (Game::score != 0)
	{
		Records* r = new Records(Game::score, s);
		records.push_back(r); //добавляем в массив
		records.sort(comp);
	}

	remove("records");//удаляем старый файл
	if (records.empty())
	{
		return;
	}

	if (records.size() > 10)
	{
		records.pop_back();
	}

	std::string entry = "";
	std::fstream fs;


	fs.exceptions(std::fstream::badbit | std::fstream::failbit);
	try
	{
		fs.open("records", std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);//создаем одноименный файл на чтение и запись
		for (auto& r : records)//записываем в файл наш массив рекордов (поля отделяются пробелом, а записи  - \n
		{
			if (entry.empty())
			{
				entry = r->get_player_name() + " " + std::to_string(r->get_points());
				fs << entry;
			}
			else
			{
				entry = r->get_player_name() + " " + std::to_string(r->get_points());
				fs << "\n" << entry;
			}
		}

	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;//если возникла ошибка
		std::cout << "Ошибка открытия файла!\n";
		system("pause");
	}
	fs.close();//закрываем файл

}

void PrintData()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv; // создаем конвертер
	std::string s = conv.to_bytes(Game::name); // конвертируем в std::string

	if (Game::score != 0)
	{
		Records* r = new Records(Game::score, s);
		records.push_back(r); //добавляем в массив
		records.sort(comp);
	}



	if (records.size() > 10)
	{
		records.pop_back();
	}
	remove("records.txt");//удаляем старый файл
	std::string entry = "";
	std::fstream fs;
	fs.exceptions(std::fstream::badbit | std::fstream::failbit);
	try
	{
		fs.open("records.txt", std::fstream::in | std::fstream::out | std::fstream::app);//создаем одноименный файл на чтение и запись
		for (auto& r : records)//записываем в файл наш массив рекордов (поля отделяются пробелом, а записи  - \n
		{
			if (entry.empty())
			{
				entry = r->get_player_name() + " " + std::to_string(r->get_points());
				fs << entry;
			}
			else
			{
				entry = r->get_player_name() + " " + std::to_string(r->get_points());
				fs << "\n" << entry;
			}
		}

	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;//если возникла ошибка
		std::cout << "Ошибка открытия файла!\n";
		system("pause");
	}
	fs.close();//закрываем файл
}

