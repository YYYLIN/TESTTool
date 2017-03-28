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
����:1.����
     2.���ݳ���
	 3.����Ŀ�ָ��
*/
typedef void(*CallbackFun)(char* ,unsigned int ,void* );

class PostHTTPConnect
{
public:
	PostHTTPConnect();
	~PostHTTPConnect();
	/*
	������:SendHTTPMessage
	����:1.��Ϣ    
	     2.��Ϣ����
    ����:������ҳ������Ϣ
	����ֵ:
	*/
	void SendHTTPMessage(char* _message, int _size);
	/*
	������:SendVersionRequest
	����:
	����:�����ȡSimApp�汾
	����ֵ:����ʧ����Ϊ0������ɹ�
	*/
	bool SendVersionRequest();
	/*
	������:DownloadFileRequest
	����:1.���ص���ҳ·��(����Ҫ��ͷ��ַ.��:/upfile/Upload/2016729/G.S%20CESSNA172%20UpdateFile_V1.62.zip)
	     2.�ص�����,���ļ���������Ժ�����������
	����:�������������������һ���ļ���������ҳ
	����ֵ:����ʧ����Ϊ0������ɹ�
	*/
	bool DownloadFileRequest(char*, CallbackFun, void*);
	/*
	������:GetSIMAPPVersionState
	����:
	����:�����������ȡ��SimApp�汾�Ժ󣬿���ͨ�����ȷ���Ƿ����°汾��
	����ֵ:���°�Ϊ1������Ϊ0
	*/
	bool GetSIMAPPVersionState();
	/*
	������:GetSIMAPPVersion
	����:
	����:�����������ȡ��SimApp�汾�Ժ󣬿���ͨ�������ȡ�°汾�š�
	����ֵ:���°�Ϊ1������Ϊ0
	*/
	void GetSIMAPPVersion(char* _ver);
	/*
	������:GetCESSNA172Version
	����:
	����:�����������ȡ��CESSNA172�汾�Ժ󣬿���ͨ�������ȡ�°汾�š�
	����ֵ:
	*/
	void GetCESSNA172Version(char* _ver);
	/*
	������:GetAUDIOVersion
	����:
	����:�����������ȡ��AUDIO�汾�Ժ󣬿���ͨ�������ȡ�°汾�š�
	����ֵ:
	*/
	void GetAUDIOVersion(char* _ver);
	/*
	������:GetPFDMFDVersion
	����:
	����:�����������ȡ��PFDMFD�汾�Ժ󣬿���ͨ�������ȡ�°汾�š�
	����ֵ:
	*/
	void GetPFDMFDVersion(char* _ver);
	/*
	������:GetCESSNA172Add
	����:
	����:�����������ȡ��CESSNA172�汾�Ժ󣬿���ͨ�������ȡ�°汾�������ӡ�
	����ֵ:
	*/
	void GetCESSNA172Add(char* _Add);
	/*
	������:GetAUDIOAdd
	����:
	����:�����������ȡ��AUDIO�汾�Ժ󣬿���ͨ�������ȡ�°汾�������ӡ�
	����ֵ:
	*/
	void GetAUDIOAdd(char* _Add);
	/*
	������:GetPFDMFDAdd
	����:
	����:�����������ȡ��PFDMFD�汾�Ժ󣬿���ͨ�������ȡ�°汾�������ӡ�
	����ֵ:
	*/
	void GetPFDMFDAdd(char* _Add);
	/*
	������:GetCurrentReceiveType
	����:
	����:��ȡ��ǰ����״̬ DOWNLOAD_Completion Ϊ��������
	����ֵ:����0Ϊû������
	*/
	char GetCurrentReceiveType(){ return ReceiveType; }
	/*
	������:GetCurrentFileLength
	����:
	����:����������أ����Ի�ȡ��ǰ�ļ��ܳ���
	����ֵ:�ļ��ܵ��ֽ���
	*/
	unsigned int GetCurrentFileLength(){ return m_FileLength; }
	/*
	������:GetCurrentReceiveLength
	����:
	����:����ļ��������أ����Ի�ȡ��ǰ�����ص��ֽ���
	����ֵ:��ǰ�����ص��ֽ���
	*/
	unsigned int GetCurrentReceiveLength(){ return m_NowPos; }
	/*
	������:GetError
	����:
	����:����ļ��������أ����Ի�ȡ��ǰ���صĴ������
	����ֵ:�������
	       DOWNLOAD_NOW            ��������
	       DOWNLOAD_Completion     �������
	       DOWNLOAD_Failure        ����ʧ��
		   DOWNLOAD_CreateMemoryF  �����ڴ�ʧ��
		   DOWNLOAD_GetLengthF     ��ȡ�ļ�����ʧ��
	*/
	int GetError(){ return m_Error; }
	/*
	������:CloseDownload
	����:
	����:����ļ��������أ����ô˺����ر�����
	����ֵ:
	*/
	void CloseDownload();
	/*
	������:GetConnectState
	����:��վ������: 
	     #define  WebsiteNAME   "www.generalsimulator.com" 
	����:��ȡ��վ����״̬
	����ֵ:���ӳɹ�����true ���� false
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

