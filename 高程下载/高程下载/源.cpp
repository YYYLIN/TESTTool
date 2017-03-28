
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "PostHTTPConnect.h"
#include <conio.h>

using namespace std;

PostHTTPConnect* m_PostHTTPConnect;

#define DownloadNumber          15

void gotoxy(int x, int y)
{
	HANDLE hConsle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordSceen = { x, y };
	SetConsoleCursorPosition(hConsle, coordSceen);
}

bool DownloadSave(char* _data, unsigned int _size, void* _pvoid, PostHTTPConnect* _post)
{
	if (_data)
	{
		if (_data[0] == 'P' && _data[1] == 'K')
		{
			FILE* pFile;

			pFile = fopen((const char*)_pvoid, "ab");

			fwrite(_data, _size, 1, pFile);

			fclose(pFile);

			_post->m_RequestD = 0;
			_post->m_ExternalState = DOWNLOAD_Completion;
			return false;
		}
	}
	if (_post->m_RequestD < 10)
	{
		char _path[256] = "http://opendemdata.info/data/srtm_contour/N61W101.zip";
		int IndexPos = strlen(_path) - 11;
		sprintf_s(&_path[IndexPos], 100, "%s", (const char*)_pvoid);
		_post->m_RequestD++;
		_post->ClearData();
		_post->DownloadFileRequest(_path, DownloadSave, 0, (const char*)_pvoid);
		_post->m_ExternalState = DOWNLOAD_Failure;
		return true;
	}
	else
	{

		_post->m_RequestD = 0;
		_post->m_ExternalState = DOWNLOAD_Completion;
		return false;
	}
}

bool DownloadFile(const char* _text, const char* fileName)
{
	FILE* pFile = 0;
	pFile = fopen(fileName, "rb");
	if (pFile)
	{
		fclose(pFile);
		return false;
	}
	else
	{
		for (int a = 0; a < DownloadNumber; a++)
		{
			if (m_PostHTTPConnect[a].m_ExternalState == DOWNLOAD_Completion)
			{
				m_PostHTTPConnect[a].m_ExternalState = DOWNLOAD_Failure;
				m_PostHTTPConnect[a].DownloadFileRequest(_text, DownloadSave, 0, fileName);
				break;
			}
		}
	}

	bool _RunState = true;
	while (_RunState)
	{

		gotoxy(0, 0);
		for (int a = 0; a < DownloadNumber; a++)
		{
			printf("开始下载:%s     \n", m_PostHTTPConnect[a].Downloadaddress);
			printf("下载进度:%d/%d  %0.1f%%  Reset:%d Error:%d Timeout:%d              \n", m_PostHTTPConnect[a].GetCurrentReceiveLength(), m_PostHTTPConnect[a].GetCurrentFileLength(),
				((float)m_PostHTTPConnect[a].GetCurrentReceiveLength() / (float)m_PostHTTPConnect[a].GetCurrentFileLength()) * 100.0f, m_PostHTTPConnect[a].m_RequestD,
				m_PostHTTPConnect[a].GetError(), (int)(m_PostHTTPConnect[a].GetTimeout() / 1000.0f));
			switch (m_PostHTTPConnect[a].m_ExternalState)
			{
			case DOWNLOAD_Failure:
				printf("重新连接                                  \n\n");
				break;
			case DOWNLOAD_Completion:
				printf("准备新连接                                  \n\n");
				_RunState = false;
				break;
			default:
				printf("正在连接                                  \n\n");
				break;
			}
		}
	}

	return true;
}

int main()
{
	int Y1, Y2;
	ifstream openfile;
	openfile.open("RECT.txt");
	if (!openfile)
	{
		Y1 = -89;
		Y2 = 89;
	}
	openfile >> Y1;
	openfile >> Y2;

	openfile.close();
	m_PostHTTPConnect = new PostHTTPConnect[DownloadNumber];

	while (!m_PostHTTPConnect->GetConnectState())
	{
		gotoxy(0, 0);
		printf("正在连接:%s            \n", WebsiteNAME);
		Sleep(1000);
	}

	gotoxy(0, 0);
	printf("连接成功:%s             \n", WebsiteNAME);

	char text[256];
	const char html[] = "http://opendemdata.info/data/srtm_contour/N61W101.zip";
	strcpy(text, html);
	int IndexPos = strlen(html) - 11;

	for (int y = Y1; y <= Y2; y++)
	{
		for (int x = -179; x <= 179; x++)
		{
			int IndexPos1;

			if (y >= 0)
				text[IndexPos] = 'N';
			else
				text[IndexPos] = 'S';
			if (y >= 10 || y <= -10)
				sprintf_s(&text[IndexPos + 1], 3, "%d", abs(y));
			else
				sprintf_s(&text[IndexPos + 1], 3, "0%d", abs(y));

			IndexPos1 = strlen(text);
			if (x >= 0)
				text[IndexPos1] = 'E';
			else
				text[IndexPos1] = 'W';

			if (x < 10 && x > -10)
				sprintf_s(&text[IndexPos1 + 1], 9, "00%d.zip", abs(x));
			else if (x < 100 && x > -100)
				sprintf_s(&text[IndexPos1 + 1], 9, "0%d.zip", abs(x));
			else
				sprintf_s(&text[IndexPos1 + 1], 9, "%d.zip", abs(x));


			if (!DownloadFile(text, &text[IndexPos]))
				continue;

			if (x < 100 && x > -100)
			{
				//情况2
				if (y >= 0)
					text[IndexPos] = 'N';
				else
					text[IndexPos] = 'S';
				if (y >= 10 || y <= -10)
					sprintf_s(&text[IndexPos + 1], 3, "%d", abs(y));
				else
					sprintf_s(&text[IndexPos + 1], 3, "0%d", abs(y));

				IndexPos1 = strlen(text);
				if (x >= 0)
					text[IndexPos1] = 'E';
				else
					text[IndexPos1] = 'W';

				if (x < 10 && x > -10)
					sprintf_s(&text[IndexPos1 + 1], 9, "0%d.zip", abs(x));
				else if (x < 100 && x > -100)
					sprintf_s(&text[IndexPos1 + 1], 9, "%d.zip", abs(x));
				else
					sprintf_s(&text[IndexPos1 + 1], 9, "%d.zip", abs(x));

				if (!DownloadFile(text, &text[IndexPos]))
					continue;
			}

			//情况3
			if (y >= 0)
				text[IndexPos] = 'n';
			else
				text[IndexPos] = 's';
			if (y >= 10 || y <= -10)
				sprintf_s(&text[IndexPos + 1], 3, "%d", abs(y));
			else
				sprintf_s(&text[IndexPos + 1], 3, "0%d", abs(y));

			IndexPos1 = strlen(text);
			if (x >= 0)
				text[IndexPos1] = 'e';
			else
				text[IndexPos1] = 'w';

			if (x < 10 && x > -10)
				sprintf_s(&text[IndexPos1 + 1], 9, "00%d.zip", abs(x));
			else if (x < 100 && x > -100)
				sprintf_s(&text[IndexPos1 + 1], 9, "0%d.zip", abs(x));
			else
				sprintf_s(&text[IndexPos1 + 1], 9, "%d.zip", abs(x));

			if (!DownloadFile(text, &text[IndexPos]))
				continue;
		}

	}

	bool _RunState = true;
	while (true)
	{
		_RunState = false;
		for (int a = 0; a < DownloadNumber; a++)
		{
			if (m_PostHTTPConnect[a].m_ExternalState != DOWNLOAD_Completion)
			{
				_RunState = true;
				break;
			}
		}
	}

	if (m_PostHTTPConnect)
	{
		delete[]m_PostHTTPConnect;
		m_PostHTTPConnect = 0;
	}

	return 0;
}