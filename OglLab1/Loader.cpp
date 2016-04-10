#include "stdafx.h"
#include "Loader.h"
#include <iostream>

vector<vector<string>> mysql(const char query[], bool getInfo, unsigned rowNumb)
{
	//string *result = new string[100];
	vector <vector<string>> result;
	MYSQL mysql; // Дескриптор соединения
	MYSQL_ROW row; // Массив полей текущей строки
	MYSQL_RES *res; // Дескриптор результирующей таблицы

	char host[] = "localhost"; // хост
	char user[] = "root"; // пользователь
	char passwd[] = "cupcakes"; // пароль
	char db[] = "ogllab1"; // название базы данных
	int port = 3306; //  порт. Если порт у сервера MySQL не по умолчанию (3306), то нужно указывать конкретный номер порта

	mysql_init(&mysql);  // Инициализация
	mysql_real_connect(&mysql, host, user, passwd, db, port, NULL, 0); // соединение
	std::cout << query << std::endl;
	if (mysql_query(&mysql, query) > 0) // запорс. Если ошибок нет, то продолжаем работу
	{

		// Если была ошибка, ...
		return vector<vector<string>>(); // и завершим работу
	}

	if (getInfo)
	{
		res = mysql_store_result(&mysql); // Берем результат,
		int num_fields = mysql_num_fields(res); // количество полей
		int num_rows = mysql_num_rows(res); // и количество строк.

		result.resize(num_rows);
		for (int i = 0; i < num_rows; i++) // Вывод таблицы
		{
			result[i].resize(num_fields);
			row = mysql_fetch_row(res); // получаем строку
			for (int j = 0; j < num_fields; j++)
			result[i][j] = row[j];
		}
	}
	mysql_close(&mysql); // Закрываем соединение
	return result;
}