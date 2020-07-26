#include "stdafx.h"
#include "DirectoryMgr.h"


CDirectoryMgr::CDirectoryMgr()
{
}


CDirectoryMgr::~CDirectoryMgr()
{
}

CString CDirectoryMgr::ConvertRelativePath(const TCHAR* pFullPath)
{
	// From���� To�� ���� ����θ� Output�� �����ϴ� �Լ�.
	//::PathRelativePathTo(Output, From, FILE_ATTRIBUTE_DIRECTORY, To, FILE_ATTRIBUTE_DIRECTORY);

	TCHAR szCurrent[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrent);
	::PathRelativePathTo(szRelative, szCurrent, FILE_ATTRIBUTE_DIRECTORY, pFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelative);
}

void CDirectoryMgr::ExtractPathInfo(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoList)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// ���� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ����� �˻��ؼ� �����Ѵٸ� TRUE�� ��ȯ�Ѵ�.
	// ���� "Test\\*.*" ���·� ���ڸ� �����ϸ� Test���� �ȿ� ���� �ϳ��� �����ϸ� TRUE ��ȯ.
	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		// FindFile �Լ� ȣ�� �� ��󿡰� �����ϰ��� �Ѵٸ� �ݵ�� FindNextFile�Լ��� ȣ�����־�� �Ѵ�.
		// ���� FindFile �Լ��� "Test\\*.*" ���·� ���ڸ� ������ ��쿡 FindNextFile�Լ��� ȣ���ϸ�
		// ���� ù��° ��󿡰� ������ �� �ִ�.
		bIsFind = find.FindNextFile(); // ����� ���ų� ������ ��󿡰� ������ ��� FALSE�� ��ȯ.

		if (find.IsDots()) // ������ ����� .�̳� ..�� ���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ������
			ExtractPathInfo(find.GetFilePath(), rPathInfoList);
		else // �����̳�
		{
			if (find.IsSystem()) // �ý��� �����̸� �ǳʶ�.
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			// ���� ���� ����.
			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath());
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->iImgCount = GetFileCount(szFilePath);

			// ��� ��� ����
			CString strRelative = ConvertRelativePath(find.GetFilePath());
			strRelative.Replace(L"(1).", L"(%d)."); // (old, new)

			pPathInfo->wstrRelative = strRelative;

			// ������Ʈ Ű ����
			lstrcpy(szFilePath, strRelative);
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->wstrStateKey = ::PathFindFileName(szFilePath);

			// ������Ʈ Ű ����
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->wstrObjectKey = ::PathFindFileName(szFilePath);

			rPathInfoList.push_back(pPathInfo);
			break;
		}
	}
}

void CDirectoryMgr::ExtractPathInfoForState(const TCHAR* pFullPath, unordered_map<wstring, PATH_INFO*>& rumapPathInfo)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// ���� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ����� �˻��ؼ� �����Ѵٸ� TRUE�� ��ȯ�Ѵ�.
	// ���� "Test\\*.*" ���·� ���ڸ� �����ϸ� Test���� �ȿ� ���� �ϳ��� �����ϸ� TRUE ��ȯ.
	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		// FindFile �Լ� ȣ�� �� ��󿡰� �����ϰ��� �Ѵٸ� �ݵ�� FindNextFile�Լ��� ȣ�����־�� �Ѵ�.
		// ���� FindFile �Լ��� "Test\\*.*" ���·� ���ڸ� ������ ��쿡 FindNextFile�Լ��� ȣ���ϸ�
		// ���� ù��° ��󿡰� ������ �� �ִ�.
		bIsFind = find.FindNextFile(); // ����� ���ų� ������ ��󿡰� ������ ��� FALSE�� ��ȯ.

		if (find.IsDots()) // ������ ����� .�̳� ..�� ���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ������
			ExtractPathInfoForState(find.GetFilePath(), rumapPathInfo);
		else // �����̳�
		{
			if (find.IsSystem()) // �ý��� �����̸� �ǳʶ�.
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			// ���� ���� ����.
			TCHAR szFilePath[MAX_STR] = L"";
			lstrcpy(szFilePath, find.GetFilePath());
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->iImgCount = GetFileCount(szFilePath);

			// ��� ��� ����
			CString strRelative = ConvertRelativePath(find.GetFilePath());
			strRelative.Replace(L"(1).", L"(%d)."); // (old, new)

			pPathInfo->wstrRelative = strRelative;

			// ������Ʈ Ű ����
			lstrcpy(szFilePath, strRelative);
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->wstrStateKey = ::PathFindFileName(szFilePath);

			// ������Ʈ Ű ����
			::PathRemoveFileSpec(szFilePath);

			pPathInfo->wstrObjectKey = ::PathFindFileName(szFilePath);


			auto iter = rumapPathInfo.find(pPathInfo->wstrStateKey);

			if (rumapPathInfo.end() == iter)
				rumapPathInfo.insert(make_pair(pPathInfo->wstrStateKey, pPathInfo));
			
			//rPathInfoList.push_back(pPathInfo);
			break;
		}
	}
}

void CDirectoryMgr::ExtractFileInfo(const TCHAR * pFullPath, unordered_map<wstring, FILE_INFO*>& rumapFileInfo)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	CFileFind	find;

	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		bIsFind = find.FindNextFile(); 

		if (find.IsDots()) 
			continue;
		else if (find.IsDirectory()) // ������
			ExtractFileInfo(find.GetFilePath(), rumapFileInfo);
		else // �����̳�
		{
			if (find.IsSystem()) // �ý��� �����̸� �ǳʶ�.
				continue;

			FILE_INFO* pFileInfo = new FILE_INFO;

			// ���� ���� ����.
			TCHAR szFilePath[MAX_STR] = L"";
			pFileInfo->wstrFileName = find.GetFileName();

			lstrcpy(szFilePath, find.GetFilePath());
			::PathRemoveFileSpec(szFilePath);

			// ��� ��� ����
			CString strRelative = ConvertRelativePath(find.GetFilePath());
			//strRelative.Replace(L"(1).", L"(%d)."); // (old, new)

			pFileInfo->wstrRelative = strRelative;

			// ������Ʈ Ű ����
			lstrcpy(szFilePath, strRelative);
			::PathRemoveFileSpec(szFilePath);

			pFileInfo->wstrStateKey = ::PathFindFileName(szFilePath);

			// ������Ʈ Ű ����
			::PathRemoveFileSpec(szFilePath);

			pFileInfo->wstrObjectKey = ::PathFindFileName(szFilePath);

			auto iter = rumapFileInfo.find(find.GetFileName().GetString());

			if (rumapFileInfo.end() != iter)
				SafeDelete(pFileInfo);
			else
				rumapFileInfo.insert(make_pair(pFileInfo->wstrFileName, pFileInfo));
		}
	}
}

void CDirectoryMgr::ExtractFolders(const TCHAR * pFullPath, vector<wstring>& rvecObjectKey)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// ���� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ����� �˻��ؼ� �����Ѵٸ� TRUE�� ��ȯ�Ѵ�.
	// ���� "Test\\*.*" ���·� ���ڸ� �����ϸ� Test���� �ȿ� ���� �ϳ��� �����ϸ� TRUE ��ȯ.
	BOOL bIsFind = find.FindFile(strFindPath);
	while (bIsFind)
	{
		// FindFile �Լ� ȣ�� �� ��󿡰� �����ϰ��� �Ѵٸ� �ݵ�� FindNextFile�Լ��� ȣ�����־�� �Ѵ�.
		// ���� FindFile �Լ��� "Test\\*.*" ���·� ���ڸ� ������ ��쿡 FindNextFile�Լ��� ȣ���ϸ�
		// ���� ù��° ��󿡰� ������ �� �ִ�.
		bIsFind = find.FindNextFile(); // ����� ���ų� ������ ��󿡰� ������ ��� FALSE�� ��ȯ.

		if (find.IsDots()) // ������ ����� .�̳� ..�� ���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ������
			rvecObjectKey.push_back(find.GetFileName().GetString());
	}
}

void CDirectoryMgr::ExtractFiles(const TCHAR * pFullPath, vector<wstring>& rvecFiles)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	// ���� Ž�� ���� MFC Ŭ����.
	CFileFind	find;

	// ����� �˻��ؼ� �����Ѵٸ� TRUE�� ��ȯ�Ѵ�.
	// ���� "Test\\*.*" ���·� ���ڸ� �����ϸ� Test���� �ȿ� ���� �ϳ��� �����ϸ� TRUE ��ȯ.
	BOOL bIsFind = find.FindFile(strFindPath);
	while (bIsFind)
	{
		// FindFile �Լ� ȣ�� �� ��󿡰� �����ϰ��� �Ѵٸ� �ݵ�� FindNextFile�Լ��� ȣ�����־�� �Ѵ�.
		// ���� FindFile �Լ��� "Test\\*.*" ���·� ���ڸ� ������ ��쿡 FindNextFile�Լ��� ȣ���ϸ�
		// ���� ù��° ��󿡰� ������ �� �ִ�.
		bIsFind = find.FindNextFile(); // ����� ���ų� ������ ��󿡰� ������ ��� FALSE�� ��ȯ.

		if (find.IsDots()) // ������ ����� .�̳� ..�� ���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ������
			continue;
		else
			rvecFiles.push_back(find.GetFileName().GetString());
		
	}
}

void CDirectoryMgr::ExtractPathInfo(const TCHAR * pFullPath, PATH_INFO& rPathInfo)
{
	// ���� ���� ����.
	TCHAR szFilePath[MAX_STR] = L"";

	// ��� ��� ����
	CString strRelative = ConvertRelativePath(pFullPath);
	strRelative.Replace(L"0.", L"%d."); // (old, new)

	rPathInfo.wstrRelative = strRelative;

	// ������Ʈ Ű ����
	lstrcpy(szFilePath, strRelative);
	::PathRemoveFileSpec(szFilePath);

	rPathInfo.wstrStateKey = ::PathFindFileName(szFilePath);

	// ������Ʈ Ű ����
	::PathRemoveFileSpec(szFilePath);

	rPathInfo.wstrObjectKey = ::PathFindFileName(szFilePath);
}

int CDirectoryMgr::GetFileCount(const TCHAR* pFullPath)
{
	CString strFindPath = CString(pFullPath) + L"\\*.*";

	CFileFind	find;
	int iCount = 0;

	BOOL bIsFind = find.FindFile(strFindPath);

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
