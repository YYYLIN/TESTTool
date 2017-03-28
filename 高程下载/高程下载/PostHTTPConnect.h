#pragma once

#define  PostHTTP_ACCESS   80
#define  WebsiteNAME   "www.generalsimulator.com" 
#define  WebpageData   1
#define  DOWNLOADFile  2

#define  DOWNLOAD_NOW             1
#define  DOWNLOAD_Completion      2
#define  DOWNLOAD_Failure         3
#define  DOWNLOAD_CreateMemoryF   4
#define  DOWNLOAD_GetLengthF      5

/*
参数:1.数据
     2.数据长度
	 3.传入的空指针
*/
typedef void(*CallbackFun)(char* ,unsigned int ,void* );

class PostHTTPConnect
{
public:
	PostHTTPConnect();
	~PostHTTPConnect();
	/*
	函数名:SendHTTPMessage
	参数:1.消息    
	     2.消息长度
    功能:向发送网页发送消息
	返回值:
	*/
	void SendHTTPMessage(char* _message, int _size);
	/*
	函数名:SendVersionRequest
	参数:
	功能:请求获取SimApp版本
	返回值:发送失败者为0，否则成功
	*/
	bool SendVersionRequest();
	/*
	函数名:DownloadFileRequest
	参数:1.下载的网页路径(不需要开头地址.例:/upfile/Upload/2016729/G.S%20CESSNA172%20UpdateFile_V1.62.zip)
	     2.回调函数,当文件下载完成以后调用这个函数
	功能:向服务器发送请求下载一个文件，或者网页
	返回值:发送失败者为0，否则成功
	*/
	bool DownloadFileRequest(char*, CallbackFun, void*);
	/*
	函数名:GetSIMAPPVersionState
	参数:
	功能:当发送请求获取到SimApp版本以后，可以通过这个确认是否有新版本。
	返回值:有新版为1，否则为0
	*/
	bool GetSIMAPPVersionState();
	/*
	函数名:GetSIMAPPVersion
	参数:
	功能:当发送请求获取到SimApp版本以后，可以通过这个获取新版本号。
	返回值:有新版为1，否则为0
	*/
	void GetSIMAPPVersion(char* _ver);
	/*
	函数名:GetCESSNA172Version
	参数:
	功能:当发送请求获取到CESSNA172版本以后，可以通过这个获取新版本号。
	返回值:
	*/
	void GetCESSNA172Version(char* _ver);
	/*
	函数名:GetAUDIOVersion
	参数:
	功能:当发送请求获取到AUDIO版本以后，可以通过这个获取新版本号。
	返回值:
	*/
	void GetAUDIOVersion(char* _ver);
	/*
	函数名:GetPFDMFDVersion
	参数:
	功能:当发送请求获取到PFDMFD版本以后，可以通过这个获取新版本号。
	返回值:
	*/
	void GetPFDMFDVersion(char* _ver);
	/*
	函数名:GetCESSNA172Add
	参数:
	功能:当发送请求获取到CESSNA172版本以后，可以通过这个获取新版本下载链接。
	返回值:
	*/
	void GetCESSNA172Add(char* _Add);
	/*
	函数名:GetAUDIOAdd
	参数:
	功能:当发送请求获取到AUDIO版本以后，可以通过这个获取新版本下载链接。
	返回值:
	*/
	void GetAUDIOAdd(char* _Add);
	/*
	函数名:GetPFDMFDAdd
	参数:
	功能:当发送请求获取到PFDMFD版本以后，可以通过这个获取新版本下载链接。
	返回值:
	*/
	void GetPFDMFDAdd(char* _Add);
	/*
	函数名:GetCurrentReceiveType
	参数:
	功能:获取当前连接状态 DOWNLOAD_Completion 为正在下载
	返回值:返回0为没有连接
	*/
	char GetCurrentReceiveType(){ return ReceiveType; }
	/*
	函数名:GetCurrentFileLength
	参数:
	功能:如果正在下载，可以获取当前文件总长度
	返回值:文件总的字节数
	*/
	unsigned int GetCurrentFileLength(){ return m_FileLength; }
	/*
	函数名:GetCurrentReceiveLength
	参数:
	功能:如果文件正在下载，可以获取当前所下载的字节数
	返回值:当前所下载的字节数
	*/
	unsigned int GetCurrentReceiveLength(){ return m_NowPos; }
	/*
	函数名:GetError
	参数:
	功能:如果文件正在下载，可以获取当前下载的错误参数
	返回值:错误参数
	       DOWNLOAD_NOW            正在下载
	       DOWNLOAD_Completion     下载完成
	       DOWNLOAD_Failure        下载失败
		   DOWNLOAD_CreateMemoryF  创建内存失败
		   DOWNLOAD_GetLengthF     获取文件长度失败
	*/
	int GetError(){ return m_Error; }
	/*
	函数名:CloseDownload
	参数:
	功能:如果文件正在下载，调用此函数关闭下载
	返回值:
	*/
	void CloseDownload();
	/*
	函数名:GetConnectState
	参数:网站名字例: 
	     #define  WebsiteNAME   "www.generalsimulator.com" 
	功能:获取网站链接状态
	返回值:链接成功返回true 否则 false
	*/
	bool GetConnectState(char* );

private:
	static DWORD WINAPI  Updata(_In_ LPVOID lpParameter);

	bool RecvMessage();

	int DataTransmission();

	bool GetALLVersion(char* _text, int _size);

	bool ConnectHTTP();

	unsigned int GetFileLength(char* );

	int GetFileHeaderLength(char* ,unsigned int);

	void OvertimeReconnect(float);

	void ClearData();

	bool Initialize();
	void Shutdown();

private:
	bool HttpHeadComplete;
	bool m_CloseMessage;
	int m_Error;
	char m_WaitNumber;
	unsigned int m_FileLength,m_NowPos;
	char ReceiveType;
	bool ConnectState;
	char* pBuffer;
	char* pFileBuffer;
	char SimAppVer[8];
	char CESSNA172Ver[8];
	char AUDIOVer[8];
	char PFDMFDVer[8];
	char CESSNA172Add[256];
	char AUDIOAdd[256];
	char PFDMFDAdd[256];

	bool SimAppUpdataState;
	CallbackFun m_CallbackFun;
	void* pbufferP;

	HANDLE Handle;
	SOCKET sockClientTCP;
	HANDLE hEvent;
};

