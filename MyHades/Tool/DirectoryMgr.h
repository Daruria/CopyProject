#pragma once
class CDirectoryMgr
{
private:
	CDirectoryMgr();
	~CDirectoryMgr();

public:
	static CString ConvertRelativePath(const TCHAR* pFullPath);
	static void ExtractPathInfo(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoList);
	static void ExtractPathInfoForState(const TCHAR* pFullPath, unordered_map<wstring, PATH_INFO*>& rumapPathInfo);
	static void ExtractPathInfo(const TCHAR* pFullPath, PATH_INFO& rPathInfo);
	static int GetFileCount(const TCHAR* pFullPath);

	static void ExtractFileInfo(const TCHAR* pFullPath, unordered_map<wstring, FILE_INFO*>& rumapFileInfo);
	static void ExtractFolders(const TCHAR* pFullPath, vector<wstring>& rvecObjectKey);
	static void ExtractFiles(const TCHAR* pFullPath, vector<wstring>& rvecFiles);
	
};

