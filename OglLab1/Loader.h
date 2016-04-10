#pragma once
//#include "stdafx.h"
#define __LCC__ // Объявляем директиву без которой программа не может работать
#pragma comment(lib, "libmysql.lib") // подключаем библиотеку
#pragma comment(lib, "mysqlclient.lib") // подключаем библиотеку
#include <mysql.h> // Заголовочный файл с описание функций
//#include <atlstr.h>
#include <string>
#include <vector>
using namespace std;
vector<vector<string>> mysql(const char query[], bool getInfo = false, unsigned rowNumb = 1);

//CListBox lb_var;
///////////////////
// CONTROLS
///////////////////

//
//
//////// DATABASE ///////
//// Save
//void OnBnClickedsendsql()
//{
//	CString strTmp;
//	for (unsigned i = 0; i < lb_var.GetCount(); i++)
//	{
//		CString sql = _T("INSERT INTO `ogllib1`.`models` (`path`) SELECT * FROM (SELECT '");
//		lb_var.GetText(i, strTmp);
//		sql += strTmp + _T("') AS tmp WHERE NOT EXISTS (SELECT `path` FROM `test`.`files` WHERE `path` = '") + strTmp + _T("') LIMIT 1;");
//
//		mysql((char*)_bstr_t((LPCTSTR)sql));
//	}
//
//	if (lb_var.GetCount() > 0)
//		MessageBox(hwnd, L"Data saved!", L"Success", MB_OK | MB_ICONINFORMATION);
//	else
//		MessageBox(hwnd, L"No data to save!", L"Error", MB_OK | MB_ICONSTOP);
//}
//
//// Clear
//void OnBnClickedClear()
//{
//	mysql("DELETE FROM `test`.`files`;");
//	MessageBox(hwnd, L"Data cleared!", L"Success", MB_OK | MB_ICONINFORMATION);
//}
//
//// Load
//void OnBnClickedLoad()
//{
//	CString *tmp = mysql("SELECT * FROM `test`.`files`;", true);
//	if (lb_var.GetCount() > 0)
//		lb_var.ResetContent();
//
//	unsigned i = 0;
//	while (tmp[i].GetLength() > 0)
//	{
//		lb_var.AddString(tmp[i]);
//		i++;
//	}
//	MessageBox(hwnd, L"Data loaded!", L"Success", MB_OK | MB_ICONINFORMATION);
//}