#include "stdafx.h"
#include "Loader.h"
#include <iostream>

vector<vector<string>> mysql(const char query[], bool getInfo, unsigned rowNumb)
{
	//string *result = new string[100];
	vector <vector<string>> result;
	MYSQL mysql; // ���������� ����������
	MYSQL_ROW row; // ������ ����� ������� ������
	MYSQL_RES *res; // ���������� �������������� �������

	char host[] = "localhost"; // ����
	char user[] = "root"; // ������������
	char passwd[] = "cupcakes"; // ������
	char db[] = "ogllab1"; // �������� ���� ������
	int port = 3306; //  ����. ���� ���� � ������� MySQL �� �� ��������� (3306), �� ����� ��������� ���������� ����� �����

	mysql_init(&mysql);  // �������������
	mysql_real_connect(&mysql, host, user, passwd, db, port, NULL, 0); // ����������
	std::cout << query << std::endl;
	if (mysql_query(&mysql, query) > 0) // ������. ���� ������ ���, �� ���������� ������
	{

		// ���� ���� ������, ...
		return vector<vector<string>>(); // � �������� ������
	}

	if (getInfo)
	{
		res = mysql_store_result(&mysql); // ����� ���������,
		int num_fields = mysql_num_fields(res); // ���������� �����
		int num_rows = mysql_num_rows(res); // � ���������� �����.

		result.resize(num_rows);
		for (int i = 0; i < num_rows; i++) // ����� �������
		{
			result[i].resize(num_fields);
			row = mysql_fetch_row(res); // �������� ������
			for (int j = 0; j < num_fields; j++)
			result[i][j] = row[j];
		}
	}
	mysql_close(&mysql); // ��������� ����������
	return result;
}