#include "stdafx.h"
#include "PostHTTPConnect.h"
#include <fstream>
#include <iostream>

using namespace std;

PostHTTPConnect::PostHTTPConnect()
{
	sockClientTCP = 0;
	Handle = 0;
	pBuffer = 0;
	ConnectState = 0;
	SimAppUpdataState = 0;
	ReceiveType = 0;
	pFileBuffer = 0;
	m_CallbackFun = 0;
	HttpHeadComplete = false;
	memset(SimAppVer, 0x00, 8);
	memset(CESSNA172Ver, 0x00, 8);
	memset(AUDIOVer, 0x00, 8);
	memset(PFDMFDVer, 0x00, 8);
	memset(CESSNA172Add, 0x00, 256);
	memset(AUDIOAdd, 0x00, 256);
	memset(PFDMFDAdd, 0x00, 256);
	pbufferP = 0;
	m_WaitNumber = 4;
	m_FileLength = m_NowPos = 0;

	Initialize();
}


PostHTTPConnect::~PostHTTPConnect()
{
	Shutdown();
}


void PostHTTPConnect::SendHTTPMessage(char* _message, int _size)
{
	send(sockClientTCP, _message, _size, 0);
}

DWORD WINAPI  PostHTTPConnect::Updata(_In_ LPVOID lpParameter)
{
	DWORD result = 0;
	float LastTime = 0.0f, DiffTime = 0.0f, NowTime = 0.0f;
	float WaitTime = 600.0f, WaitDiffTime = 0.0f;
	PostHTTPConnect* AppDialog = (PostHTTPConnect*)lpParameter;

	if (AppDialog->ConnectHTTP())
	{
		AppDialog->ConnectState = true;
		AppDialog->SendVersionRequest();
	}
	else
		AppDialog->ConnectState = false;

	while (true)
	{
		AppDialog->RecvMessage();
		result = WaitForSingleObject(AppDialog->hEvent, 200);
		if (result == WAIT_OBJECT_0)
		{
			return true;
		}
		NowTime = clock();
		DiffTime = NowTime - LastTime;
		LastTime = NowTime;

		if (AppDialog->ReceiveType == 0)
		{
			WaitDiffTime += (DiffTime / 1000.0f);
			if (WaitTime < WaitDiffTime)
			{
				WaitDiffTime = 0.0f;
				WaitTime = ((float)rand() / (float)RAND_MAX)*600.0f + 600.0f;
				AppDialog->m_WaitNumber = 4;
				AppDialog->SendVersionRequest();
			}
		}
		else if (AppDialog->ReceiveType == 1)
		{
			AppDialog->OvertimeReconnect(DiffTime);
		}

		if (!AppDialog->ConnectState && AppDialog->ReceiveType == 0)
		{
			AppDialog->SendVersionRequest();
		}
	}

	return true;
}

bool PostHTTPConnect::SendVersionRequest()
{
	if (!ConnectHTTP())
	{
		//		::MessageBoxEx(0, TEXT_ConnectSeverFail, L"Error", MB_OK, NULL);
		ConnectState = false;
		return false;
	}
	ConnectState = true;

	char* req = "GET /Single/Downloads.html HTTP/1.1\r\n"
		"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET4.0C; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\n"
		"Host:www.generalsimulator.com\r\n\r\n";

	memset(SimAppVer, 0x00, 8);
	memset(CESSNA172Ver, 0x00, 8);
	memset(AUDIOVer, 0x00, 8);
	memset(PFDMFDVer, 0x00, 8);

	this->SendHTTPMessage(req, strlen(req));

	ReceiveType = WebpageData;

	return true;
}

bool PostHTTPConnect::DownloadFileRequest(char* _pth, CallbackFun _backfun, void* _P)
{
	char text[512];
	memset(text, 0x00, 512);
	m_CallbackFun = _backfun;
	pbufferP = _P;
	char* TCPMe = " HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET4.0C; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\n"
		"Host:www.generalsimulator.com\r\n\r\n";

	if (!ConnectHTTP())
	{
		//		::MessageBoxEx(0, TEXT_ConnectSeverFail, L"Error", MB_OK, NULL);
		ConnectState = false;
		return false;
	}
	ConnectState = true;

	strcpy(text, "GET ");
	strcpy(&text[4], _pth);
	strcpy(&text[strlen(text)], TCPMe);
	this->SendHTTPMessage(text, strlen(text));

	ReceiveType = DOWNLOADFile;

	return true;
}

bool PostHTTPConnect::GetSIMAPPVersionState()
{
	return SimAppUpdataState;
}

void PostHTTPConnect::GetSIMAPPVersion(char* _ver)
{
	memcpy(_ver, SimAppVer, 8);
}

void PostHTTPConnect::GetCESSNA172Version(char* _ver)
{
	memcpy(_ver, CESSNA172Ver, 8);
}

void PostHTTPConnect::GetAUDIOVersion(char* _ver)
{
	memcpy(_ver, AUDIOVer, 8);
}

void PostHTTPConnect::GetPFDMFDVersion(char* _ver)
{
	memcpy(_ver, PFDMFDVer, 8);
}

void PostHTTPConnect::GetCESSNA172Add(char* _Add)
{
	strcpy(_Add, CESSNA172Add);
}

void PostHTTPConnect::GetAUDIOAdd(char* _Add)
{
	strcpy(_Add, AUDIOAdd);
}

void PostHTTPConnect::GetPFDMFDAdd(char* _Add)
{
	strcpy(_Add, PFDMFDAdd);
}

void PostHTTPConnect::CloseDownload()
{
	m_CloseMessage = true;
}

bool PostHTTPConnect::GetConnectState(char* _text)
{
	LPHOSTENT lphostent;
	lphostent = gethostbyname(_text);
	if (lphostent == NULL)
		return false;
	else
		return true;
}

bool PostHTTPConnect::RecvMessage()
{
	int result;
	char pSimAppVersion[8];

	if (ReceiveType == 0)
	{
		result = recv(sockClientTCP, pBuffer, 1048576, 0);
		return result;
	}
	else if (ReceiveType == WebpageData)
	{
		result = DataTransmission();
		m_Error = result;
		if (result >= DOWNLOAD_Completion)
		{
			result = GetALLVersion(pFileBuffer, m_FileLength);
			if (!result)
			{
				return false;
				ClearData();
			}

			memcpy(pSimAppVersion, SimAppVer, 8);
			pSimAppVersion[1] = pSimAppVersion[2];
			pSimAppVersion[2] = pSimAppVersion[4];
			pSimAppVersion[3] = pSimAppVersion[6];
			pSimAppVersion[4] = 0x00;

			char buffer[] = SIMAPP_VERSION;
			buffer[1] = buffer[2];
			buffer[2] = buffer[4];
			buffer[3] = buffer[6];
			buffer[4] = 0x00;
			if (atoi(buffer) < atoi(pSimAppVersion))
				SimAppUpdataState = true;
			else
				SimAppUpdataState = false;

			ClearData();
		}
		return result;
	}
	else if (ReceiveType == DOWNLOADFile)
	{
		result = DataTransmission();
		m_Error = result;
		if (result >= DOWNLOAD_Completion)
		{
			if (m_CallbackFun)
				m_CallbackFun(pFileBuffer, m_FileLength, pbufferP);

			ClearData();
		}
		return result;
	}

	return true;
}

int PostHTTPConnect::DataTransmission()
{
	int result;

	if (m_CloseMessage)
		return DOWNLOAD_Failure;

	memset(pBuffer, 0x00, 1048576);
	result = recv(sockClientTCP, pBuffer, 262144, 0);
	if (result <= 0)
	{
		if (result == 0)
		{
			return DOWNLOAD_Failure;
		}
		else
			return false;
	}
	unsigned int Index = 0;
	Index = GetFileHeaderLength(pBuffer, result);
	if (!pFileBuffer)
	{
		m_NowPos = 0;
		m_FileLength = GetFileLength(pBuffer);
		if (!m_FileLength)
			return DOWNLOAD_GetLengthF;
		HttpHeadComplete = true;
		Index = GetFileHeaderLength(pBuffer, result);
		if (m_FileLength < result)
			m_FileLength = result - Index;
		pFileBuffer = new char[m_FileLength];
		if (!pFileBuffer)
			return DOWNLOAD_CreateMemoryF;
		memset(pFileBuffer, 0x00, m_FileLength);
	}
	if (pFileBuffer)
	{
		if (m_NowPos + result - Index > m_FileLength)
			return DOWNLOAD_Failure;
		memcpy(&pFileBuffer[m_NowPos], &pBuffer[Index], result - Index);
		m_NowPos += result - Index;
		if (m_NowPos >= m_FileLength)
		{
			return DOWNLOAD_Completion;
		}
	}

	return DOWNLOAD_NOW;
}

bool PostHTTPConnect::GetALLVersion(char* _text, int _size)
{
	bool _SimApp = 0, _CESSNA172 = 0, _Audio = 0, PFD_MFD = 0,
		_CESSNA172Add = 0, _AudioAdd = 0, PFD_MFDAdd = 0;

	unsigned int CESSNA172Len = strlen("G.S CESSNA172 FIRMWARE V");
	unsigned int PFDMFDLen = strlen("G1K&G1K PLUS (PFD&MFD) FIRMWARE V");
	unsigned int AUDIO2Len = strlen("G1K&G1K PLUS (AUDIO PANEL) FIRMWARE V");

	unsigned int Len = strlen("<a class=\"dn_a1 marl_30 lf\" href=\"");

	for (int a = 0; a < _size; a++)
	{
		if (!_SimApp)
		{
			if (memcmp(&_text[a], "SIMAPP VERSION ", 15) == 0)
			{
				a += 15;
				memset(SimAppVer, 0x00, 8);
				memcpy(SimAppVer, &_text[a], 7);
				a += 7;
				_SimApp = true;
			}
		}

		if (!_CESSNA172)
		{
			if (memcmp(&_text[a], "G.S CESSNA172 FIRMWARE V", CESSNA172Len) == 0)
			{
				a += CESSNA172Len;
				memset(CESSNA172Ver, 0x00, 8);
				memcpy(CESSNA172Ver, &_text[a], 4);
				a += 4;
				_CESSNA172 = true;
			}
		}
		else
		{
			if (!_CESSNA172Add)
			{
				if (memcmp(&_text[a], "<a class=\"dn_a1 marl_30 lf\" href=\"", Len) == 0)
				{
					a += Len;
					memset(CESSNA172Add, 0x00, 256);
					int b = a;
					while (_text[a] != '"')
					{
						a++;
						if (a >= _size)
							return false;
					}
					memcpy(CESSNA172Add, &_text[b], a - b);
					_CESSNA172Add = true;
				}
			}
		}

		if (!_Audio)
		{
			if (memcmp(&_text[a], "G1K&G1K PLUS (AUDIO PANEL) FIRMWARE V", AUDIO2Len) == 0)
			{
				a += AUDIO2Len;
				memset(AUDIOVer, 0x00, 8);
				memcpy(AUDIOVer, &_text[a], 4);
				a += 4;
				_Audio = true;
			}
		}
		else
		{
			if (!_AudioAdd)
			{
				if (memcmp(&_text[a], "<a class=\"dn_a1 marl_30 lf\" href=\"", Len) == 0)
				{
					a += Len;
					memset(AUDIOAdd, 0x00, 256);
					int b = a;
					while (_text[a] != '"')
					{
						a++;
						if (a >= _size)
							return false;
					}
					memcpy(AUDIOAdd, &_text[b], a - b);
					_AudioAdd = true;
				}
			}
		}

		if (!PFD_MFD)
		{
			if (memcmp(&_text[a], "G1K&G1K PLUS (PFD&MFD) FIRMWARE V", PFDMFDLen) == 0)
			{
				a += PFDMFDLen;
				memset(PFDMFDVer, 0x00, 8);
				memcpy(PFDMFDVer, &_text[a], 4);
				a += 4;
				PFD_MFD = true;
			}
		}
		else
		{
			if (!PFD_MFDAdd)
			{
				if (memcmp(&_text[a], "<a class=\"dn_a1 marl_30 lf\" href=\"", Len) == 0)
				{
					a += Len;
					memset(PFDMFDAdd, 0x00, 256);
					int b = a;
					while (_text[a] != '"')
					{
						a++;
						if (a >= _size)
							return false;
					}
					memcpy(PFDMFDAdd, &_text[b], a - b);
					PFD_MFDAdd = true;
				}
			}
		}
		if (PFD_MFD&& _Audio&&_CESSNA172&&_SimApp&&
			_CESSNA172Add &&_AudioAdd&& PFD_MFDAdd)
		{
			return true;
		}
	}
	return false;
}


bool PostHTTPConnect::ConnectHTTP()
{
	SOCKADDR_IN addrSrv;
	LPHOSTENT lphostent;

	if (sockClientTCP)
	{
		closesocket(sockClientTCP);
		sockClientTCP = 0;
	}

	sockClientTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	lphostent = gethostbyname(WebsiteNAME);
	if (lphostent == NULL)
		return false;

	addrSrv.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(*(in_addr*)*lphostent->h_addr_list));  //本函数将一个32位数从主机字节顺序转换成网络字节顺序。
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PostHTTP_ACCESS);

	if (connect(sockClientTCP, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) != 0)
		return false;

	int iMode = 1; //0：阻塞
	ioctlsocket(sockClientTCP, FIONBIO, (u_long FAR*) &iMode);

	return true;
}

unsigned int PostHTTPConnect::GetFileLength(char* _buffer)
{
	char text[80];
	int charsize = strlen(_buffer);
	for (int a = 0; a < charsize; a++)
	{
		if (_buffer[a] == 0x0A)
		{
			if (memcmp(&_buffer[++a], "Content-Length:", strlen("Content-Length:")) == 0)
			{
				a += strlen("Content-Length:");
				int b = 0;
				while (_buffer[a] != 0x0D)
				{
					text[b] = _buffer[a];
					b++;
					a++;
				}
				text[b] = 0x00;
				return atoi(text);
			}
		}
	}

	return 0;
}

int PostHTTPConnect::GetFileHeaderLength(char* _buffer, unsigned int _size)
{
	if (HttpHeadComplete)
	{
		char text[4] = { 0x0D, 0x0A, 0x0D, 0x0A };
		for (unsigned int a = 0; a < _size; a++)
		{
			if (_buffer[a] == 0x0D && _buffer[++a] == 0x0A)
			{
				if (memcmp(&_buffer[++a], "Date:", 5) == 0)
				{
					for (; a < _size; a++)
					{
						if (_buffer[a] == 0x0D)
						{
							if (memcmp(&_buffer[a], text, 4) == 0)
							{
								HttpHeadComplete = false;
								return a + 4;
							}
						}
					}
				}
			}
		}
		return _size;
	}
	return 0;
}

void PostHTTPConnect::OvertimeReconnect(float _DiffTime)
{
	static float WaitTime = 0.0f;
	WaitTime += _DiffTime;
	if (WaitTime >= 10000)
	{
		WaitTime = 0.0f;
		m_WaitNumber--;
		if (m_WaitNumber <= 0)
		{
			ReceiveType = 0;
			return;
		}
		SendVersionRequest();

	}
}

void PostHTTPConnect::ClearData()
{
	if (pFileBuffer)
		delete[] pFileBuffer;
	pFileBuffer = 0;
	m_FileLength = 0;
	m_NowPos = 0;
	ReceiveType = 0;
	m_CloseMessage = false;
}

bool PostHTTPConnect::Initialize()
{
	pBuffer = new char[1048576];
	if (!pBuffer)
		return false;
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	Handle = CreateThread(NULL, 0, this->Updata, this, 0, NULL);

	m_CloseMessage = false;

	return true;
}


void PostHTTPConnect::Shutdown()
{
	SetEvent(hEvent);
	WaitForSingleObject(Handle, INFINITE);
	if (pBuffer)
	{
		delete[] pBuffer;
		pBuffer = 0;
	}

	closesocket(sockClientTCP);
	sockClientTCP = 0;
}