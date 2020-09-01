#include "stdafx.h"
#include "../col/col.h"

int __col_stream_index__ = std::ios_base::xalloc();

string g_strDirectoryA;
wstring g_strDirectoryW;

BOOL GetModuleDirectory(std::string& strDirectory)
{
	BOOL bRet;
	if (!(bRet = g_strDirectoryA.size() != 0))
	{
		char szFilePath[MAX_PATH];
		if (bRet = (GetModuleFileNameA(NULL, szFilePath, MAX_PATH) != 0))
		{
			*strrchr(szFilePath, '\\') = NULL;
			g_strDirectoryA = szFilePath;
		}
	}
	strDirectory = g_strDirectoryA;
	return bRet;
}

BOOL GetModuleDirectory(std::wstring& strDirectory)
{

	BOOL bRet;
	if (!(bRet = g_strDirectoryW.size() != 0))
	{
		wchar_t szFilePath[MAX_PATH];
		if (bRet = (GetModuleFileNameW(NULL, szFilePath, MAX_PATH) != 0))
		{
			*wcsrchr(szFilePath, L'\\') = NULL;
			g_strDirectoryW = szFilePath;
		}
	}
	strDirectory = g_strDirectoryW;
	return bRet;
}


std::basic_string< TCHAR > GetPrivateProfileString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszDefault, LPCTSTR lpszFileName)
{
	TCHAR szString[128];
	memset(szString, 0, 128 * sizeof(TCHAR));

	::GetPrivateProfileString(lpszAppName, lpszKeyName, lpszDefault, szString, 128, lpszFileName);
	return std::basic_string< TCHAR >(szString);
}
std::basic_string< TCHAR > GetUserID()
{
	tstring m_strInfoFilePath;
	GetModuleDirectory(m_strInfoFilePath);
	m_strInfoFilePath += TEXT("\\");
	//m_strInfoFilePath += STR_BASIC_INFO_FILE_NAME;
	m_strInfoFilePath += TEXT("hscrelay.cfg");

	return GetPrivateProfileString(TEXT("user"), TEXT("id"), TEXT(""), m_strInfoFilePath);
}



void showhex(const unsigned char* pData, int cb)
{
	std::cout << std::hex << std::uppercase;
	for (int i = 0; i < cb; i++)
		std::cout << (int)(pData[i]) << " ";
	std::cout << std::nouppercase << std::dec << std::endl;

	std::cout << "string size : " << cb << std::endl;
}

template< typename _Elem >
void showhex(const std::basic_string< _Elem >& str)
{
	showhex((const unsigned char*)str.c_str(), (int)(str.size() * sizeof(_Elem)));
}


BOOL IsMyUserID(LPCTSTR lpszUserId)
{

	return (FALSE
		|| (_tcsicmp(lpszUserId, GetUserID().c_str()) == 0)
		|| (_tcsicmp(lpszUserId, tstring(TEXT("MyClient@sheriff.co.kr")).c_str()) == 0)
		|| (_tcslen(lpszUserId) == 0)
		);

}

int main()
{
	setlocale(LC_ALL, "");

	bwinarybuf *bibuf = new bwinarybuf;

	bwostream bos(bibuf);
	bwistream bis(bibuf);

	std::tuple<std::vector<wstring>, std::map<int, wstring>> tuo;
	std::tuple<std::vector<wstring>, std::map<int, wstring>> tui;


	std::get<0>(tuo).push_back(L"AAA");
	std::get<0>(tuo).push_back(L"BBB");
	std::get<0>(tuo).push_back(L"CCC");

	std::get<1>(tuo).insert(std::make_pair(1, L"¼Û¹Î±Ô"));
	std::get<1>(tuo).insert(std::make_pair(2, L"123123"));
	std::get<1>(tuo).insert(std::make_pair(3, L"343434343456565"));

	bos << tuo;
	bis >> tui;

	for (const auto & item : std::get<1>(tui))
	{
		std::wcout << item.first << std::endl;
		//showhex(item.second.toutf8());
	}


	setlocale(LC_ALL, "");

	string m_Userid = tstring(GetUserID()).toutf8();
	
	std::cout << "Wstring To UTF8 : ";
	showhex(m_Userid);
	
	std::cout << "UTF8 To Wstring : ";
	showhex(tstring().fromutf8(m_Userid));

	string strUserID, strConnectID;	
	strUserID = "Çï·Î¿ì";
	strConnectID.format("%s[%s]", strUserID.toutf8().c_str(), m_Userid.c_str());

	std::cout << "Çï·Î¿ì : ";
	LPSTR m_lpszUserID = (LPSTR)malloc(strlen(strConnectID.c_str()) + 1);
	strcpy(m_lpszUserID, strConnectID.c_str());
	string userid = m_lpszUserID;
	showhex(userid);

	if (IsMyUserID(tstring().fromutf8(m_Userid).c_str()))
	{
		std::wcout << "USER ID : "<< tstring().fromutf8(m_Userid).c_str() << std::endl;
	}
	
	tstring ts;

	ts.format("%s,%s", "song", "min");
	std::wcout << ts.c_str() << std::endl;
}

