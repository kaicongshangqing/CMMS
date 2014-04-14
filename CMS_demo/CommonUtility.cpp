#include "StdAfx.h"
#include "CommonUtility.h"
#include "Tlhelp32.h"
//#include "EnumProc.h"



CCommonUtility::CCommonUtility(void)
{
}

CCommonUtility::~CCommonUtility(void)
{
}

//********************************************************************
//* ģ�����ƣ�GetSysCurDate
//* ģ�鹦�ܣ��õ�ϵͳ��ǰʱ��
//* �� �� ֵ��ϵͳ��ǰʱ��
//* ����˵����
//* �� �� ��: lj
//* �������ڣ�2008��03��20��
//********************************************************************
CString CCommonUtility::GetSysCurDate()
{
	int iDay,iMonth,iYear,iHour,iMin,iSecond;
	CString strDate; 
	CTime oCurTime;

	oCurTime=CTime::GetCurrentTime();
	iDay=oCurTime.GetDay();
	iMonth=oCurTime.GetMonth();
	iYear=oCurTime.GetYear();
	iHour=oCurTime.GetHour();
	iMin=oCurTime.GetMinute();
	iSecond = oCurTime.GetSecond();
	strDate.Format(_T("%d%s%.2d%s%.2d %.2d%s%.2d%s%.2d"),iYear,"-",iMonth,"-",iDay,iHour,":",iMin,":",iSecond);

	return strDate;
}


//********************************************************************
//* ģ�����ƣ�GetMainDirectory()
//* ģ�鹦�ܣ��õ����������е���Ŀ¼
//* �� �� ֵ����ʾ�Ƿ�ɹ�ִ��
//* ����˵����1.strMainDir ȡ�õ���Ŀ¼·��
//* �� �� ��: lj
//* �������ڣ�2008��03��20��
//********************************************************************
int  CCommonUtility::GetMainDirectory(CString& strMainDir)
{
	TCHAR   exeFullPath[MAX_PATH]; 
	CString strPath; 

	//ȡ��ģ�����е�·��
	GetModuleFileName(NULL,exeFullPath,MAX_PATH); 

	strPath      = (CString)exeFullPath; 
	int position = strPath.ReverseFind('\\'); 

	strPath      = strPath.Left(position); 
	//
	strMainDir   = strPath;

	return true;
}

//*******************************************************************************
//* ģ�����ƣ�CreateID
//* ģ�鹦�ܣ�����ID
//* �� �� ֵ��
//* ����˵����id ������ID��ַ
//* �������ڣ�2008��03��20��
//* �޸����ڣ�
//*******************************************************************************
void CCommonUtility::CreateID(CString &strID)
{
	wchar_t lpszString[128];
	GUID *pguid;
	pguid = new(GUID);
	CoCreateGuid(pguid);
	StringFromGUID2(*pguid,lpszString,sizeof(lpszString));
	//	wcscpy(id,lpszString);
	//	strID.Format(_T("%s"),lpszString);
	strID = lpszString;
	delete pguid;
}
//*******************************************************************************
//* ģ�����ƣ�DeleteDir
//* ģ�鹦�ܣ��ݹ�ɾ��������Ŀ¼
//* �� �� ֵ��
//* ����˵����lpszDirName ����
//* �������ڣ�2008��03��20��
//* �޸����ڣ�
//*******************************************************************************
// �ݹ�ɾ��������Ŀ¼
BOOL CCommonUtility::DeleteDir(LPCTSTR lpszDirName)
{
	CFileFind ff;

	CString sFileToFind;
	sFileToFind.Format(_T("%s\\*.*"), lpszDirName);

	BOOL bFind;

	// find & delete files/sub dir
	bFind = ff.FindFile(sFileToFind);
	while(bFind)
	{
		bFind = ff.FindNextFile();
		if(ff.IsDots()) // if it's the ./.. dir
			continue;

		if(ff.IsDirectory())
		{ // if it's a dir
			DeleteDir(ff.GetFilePath());
		}
		else
		{ // if it's a file
			::SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_ARCHIVE);
			::DeleteFile(ff.GetFilePath());
			DWORD dwError = GetLastError();
		}
	}
	ff.Close();

	// begin to delete the dir
	if(!RemoveDirectory(lpszDirName))
		return FALSE;

	return TRUE;
}

//********************************************************************
//* ģ�����ƣ�BrowseDir()
//* ģ�鹦�ܣ��ݹ���������ļ��м��ļ������临�Ƶ�ָ���ļ�����
//* �� �� ֵ��
//* ����˵����1.strDir			������ļ���
//*			  2.strFilePath		�ļ�����·��			
//* �������ڣ�2008��07��9��
//* �޸����ڣ�
//********************************************************************
void CCommonUtility::BrowseDir(CString strDir,CString strFilePath) 
{
	CFileFind ff;
	CString szDir = strDir;

	if(szDir.Right(1) != "\\")
		szDir += "\\";

	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while( res )
	{
		res = ff.FindNextFile();	//���±�־λ
		if(ff.IsDirectory() && !ff.IsDots())	//�ļ���
		{
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileTitle();

			//�����ļ���
			BOOL bFlag = CreateDirectory((strFilePath + "\\" + strTitle),NULL);
			if(!bFlag)//�ж��ļ��д����Ƿ�ɹ�
			{
				CString str;
				str.Format("�����ļ���%sʧ�ܣ�",strTitle);
				//CDKLog::Error(str);
				//CCommonUtility::ShowDlgYes(str);
			}

			//�����һ���ļ���ʱ��Ӧ�ı���·��ҲӦ�Ƶ���һ��
			BrowseDir(strPath,(strFilePath + "\\" + strTitle));
		}
		else if(!ff.IsDirectory() && !ff.IsDots())	//�ļ�
		{
			CString strName = ff.GetFileName();

			BOOL bFlag = CopyFile((strDir + "\\" + strName),(strFilePath + "\\" + strName),FALSE);
			if(!bFlag)
			{
				CString str;
				str.Format("�����ļ�%sʧ�ܣ�",strName);
			//	CCommonUtility::ShowDlgYes(str);
			//	CDKLog::Error(str);
			}
		}
	}
	ff.Close();
}

//********************************************************************
//* ģ�����ƣ�RemoveDir
//* ģ�鹦�ܣ�ɾ��Ŀ¼
//* �� �� ֵ��
//* ����˵����1.strDir			������ļ���
//*			  2.strFilePath		�ļ�����·��			
//* �������ڣ�2008��07��9��
//* �޸����ڣ�
//********************************************************************
////////////////////////////////
//ɾ���ļ���(�����Ƕ���ļ���)
//mode=LEAVING_ROOT ��ʾ���¿յĸ�Ŀ¼
//mode=LEAVING_NOTHING ��ʾȫ��ɾ��
void CCommonUtility::RemoveDir(CString strDirectory,int mode)
{
	//if(_access(strDirectory,0) == -1)
	//	return;
	if(strDirectory!="") //Ŀ¼·������Ϊ��
	{
		CString strTemp;
		BOOL bResult;
		CFileFind finder;
		strDirectory.TrimRight("\\");
		bResult = finder.FindFile(strDirectory + "\\" + _T("*.*"));
		while (bResult)
		{
			bResult=finder.FindNextFile();
			if (finder.IsDots()) //�ų�.��..�ļ�
				continue;
			if(finder.IsDirectory())
			{
				strTemp=finder.GetFilePath();
				RemoveDir(strTemp);				
			}
			else  //��ײ���ɾ���ļ�
				CFile::Remove(finder.GetFilePath());					
		}
		finder.Close();
		Sleep(200);
		//ɾ�����ļ���
		if(mode == LEAVING_NOTHING)
			RemoveDirectory(strDirectory);
		Sleep(200);
	}	
}

//********************************************************************
//* ģ�����ƣ�IsNameInArrayWithExact
//* ģ�鹦�ܣ��ж������Ƿ������������
//* �� �� ֵ��1.��TRUE ��FALSE
//* ����˵����1.pStrNameArray		��������
//*			  2.strName				����
//*			  3.iIndex				��ʾλ���±�
//* �� �� ��: lj
//* �������ڣ�2009��02��03��
//********************************************************************
//�ж������Ƿ������������
BOOL CCommonUtility::IsNameInArrayWithExact(CStringArray *pStrNameArray,CString strName,int &iIndex)
{
	iIndex = -1;
	for (int i = 0;i< pStrNameArray->GetSize() ;i++)
	{
		CString strTmp = pStrNameArray->GetAt(i);
		if(strTmp == strName)
		{
			iIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

//********************************************************************
//* ģ�����ƣ�IsNameInArray()
//* ģ�鹦�ܣ��ж������Ƿ������������
//* �� �� ֵ��1.��TRUE ��FALSE
//* ����˵����1.pStrNameArray		��������
//*			  2.strName				����
//*			  3.iIndex				��ʾλ���±�
//* �� �� ��: lj
//* �������ڣ�2009��02��03��
//********************************************************************
//�ж������Ƿ������������
BOOL CCommonUtility::IsNameInArray(CStringArray *pStrNameArray,CString strName,int &iIndex)
{
	iIndex = -1;
	for (int i = 0;i< pStrNameArray->GetSize() ;i++)
	{
		CString strTmp = pStrNameArray->GetAt(i);
		if(strTmp.Find(strName) != -1)
		{
			iIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

//********************************************************************
//* ģ�����ƣ�IsNameInArray()
//* ģ�鹦�ܣ��ж������Ƿ������������
//* �� �� ֵ��1.��TRUE ��FALSE
//* ����˵����1.pStrNameArray		��������
//*			  2.strName				����
//* �� �� ��: lj
//* �������ڣ�2009��02��03��
//********************************************************************
//�ж������Ƿ������������
BOOL CCommonUtility::IsNameInArray(CStringArray *pStrNameArray,CString strName)
{
	for (int i = 0;i< pStrNameArray->GetSize() ;i++)
	{
		CString strTmp = pStrNameArray->GetAt(i);
		if(strTmp.Find(strName) != -1)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//********************************************************************
//* ģ�����ƣ�ClearPtrArray
//* ģ�鹦�ܣ������������
//* �� �� ֵ��
//* ����˵����
//*			 
//* �� �� ��: lj
//* �������ڣ�2009��02��03��
//********************************************************************
//�����������
void CCommonUtility::ClearPtrArray(CPtrArray *pArray)
{
	if(pArray == NULL)
		return;

	for(int i = 0;i< pArray->GetSize();i++)
	{
		delete pArray->GetAt(i);
	}
	if(pArray->GetSize() > 0)
		pArray->RemoveAll();

	//	pArray = NULL;
}

//********************************************************************
//* ģ�����ƣ�IsFileExsit()
//* ģ�鹦�ܣ��ж��ļ��Ƿ���ڣ�������ڣ�����TRUE�����򣬷���FALSE
//* �� �� ֵ��
//* ����˵����1.strFilePath	�ļ�·��
//* �� �� ��: lj
//* �������ڣ�2008��03��21��
//********************************************************************
//�ж��ļ��Ƿ���ڣ�������ڣ�����TRUE�����򣬷���FALSE
BOOL CCommonUtility::IsFileExist(CString strFileName)
{
	CFile file;
	CFileException ex;
	BOOL bRet = file.Open(strFileName,CFile::modeRead |  CFile::shareDenyNone,&ex);
	if(bRet == FALSE)	//���ܴ��ļ�
	{
		return FALSE;
	}
	file.Close();
	return TRUE;
}
//********************************************************************
//* ģ�����ƣ�GetFileData
//* ģ�鹦�ܣ���ȡ�ļ���Ϣ����
//* �� �� ֵ���ļ�����
//* ����˵����1.strFilePath	�ļ�·��
//* �� �� ��: lj
//* �������ڣ�2008��03��21��
//********************************************************************

//��ȡ�ļ���Ϣ����
CString CCommonUtility::GetFileData(CString strFilePath)
{
	CStdioFile file;
	CFileException ex;
	CString strData;

	try
	{	
		
		BOOL bRet = file.Open(strFilePath,CFile::modeRead |  CFile::shareExclusive,&ex);
		if(bRet == FALSE)	//���ܴ��ļ�
		{
			return "";
		}

		CString strSingleLine = "";
		file.ReadString(strSingleLine);
		strData = strData + strSingleLine;
		while(strSingleLine != "")
		{
			CString strTmp;
// 			strTmp.Format("%c",0x0D);
// 			strData = strData + strTmp;
			strTmp.Format("%c",0x0A);
			strData = strData + strTmp;

			file.ReadString(strSingleLine);
			strData = strData + strSingleLine;
		}

		file.Close();

		return strData;

	}
	catch (CFileException *ex)	
	{
		TCHAR szError[1024];
		ex->GetErrorMessage(szError, 1024);
		//		CDKLog::Error(szError);
		//WriteDataIntoFile("strFilePath error:" + szError);
		TRACE("There is an WriteDataFile: %s\n",strData);
		return "";
	}
}

//********************************************************************
//* ģ�����ƣ�WriteDataIntoFile()
//* ģ�鹦�ܣ��ڵ����ļ���д������
//* �� �� ֵ��
//* ����˵����1.strData	����
//* �� �� ��: lj
//* �������ڣ�2008��03��21��
//********************************************************************
//�ڵ����ļ���д������
void CCommonUtility::WriteDataIntoFile(CString strData)
{
	try
	{	
		//���㵱ǰʱ��
		int iDay,iMonth,iYear,iHour,iMin,iSecond;
		CString strDate; 
		CTime oCurTime;

		oCurTime=CTime::GetCurrentTime();
		iDay=oCurTime.GetDay();
		iMonth=oCurTime.GetMonth();
		iYear=oCurTime.GetYear();
		iHour=oCurTime.GetHour();
		iMin=oCurTime.GetMinute();
		iSecond = oCurTime.GetSecond();
		strDate.Format("%d%s%d%s%d",iYear,"_",iMonth,"_",iDay);

		//��ȡ��ǰ��������·��
		CString strMainDir;
		GetMainDirectory(strMainDir);

		//�жϵ�ǰ��������·�����Ƿ����  xx_xx_xx.txt�ļ�(����xx_xx_xxΪʱ��)
		CString strFileName;
		CFile file;
		CFileException ex;
		strFileName = strMainDir + "\\����" + strDate + ".txt";
		BOOL bRet = file.Open(strFileName,CFile::modeWrite |  CFile::shareExclusive,&ex);
		if(bRet == FALSE)	//���ܴ��ļ�
		{
			//�ļ������ڣ������ļ�
			BOOL bRet = file.Open(strFileName,CFile::modeCreate | CFile::modeWrite| CFile::shareExclusive,&ex);
			if(bRet == FALSE)
			{
				return;
			}
			else
			{
				CString strTmp;
				strTmp = "������Ϣ�ļ�--���ڱ������й�����Ϣ\r\n";
				int iLength = strTmp.GetLength();
				file.Write(strTmp,iLength);

				iLength = strData.GetLength();
				file.Write(strData,iLength);
				file.Close();
				return;
			}
		}

		file.SeekToEnd();
		//�ļ����򿪣��洢����
		CString strTmp;
		strTmp = ("\r\n");
		int iLength = strTmp.GetLength();
		file.Write(strTmp,iLength);

		iLength = strData.GetLength();
		file.Write(strData,iLength);
		file.Close();

	}
	catch (...)	
	{
//		TCHAR szError[1024];
//		ex->GetErrorMessage(szError, 1024);
//		CDKLog::Error(szError);
		TRACE("There is an WriteDataFile: %s\n",strData);
		return;
	}
}

//********************************************************************
//* ģ�����ƣ�DeleteDataFromArray()
//* ģ�鹦�ܣ���������ɾ��ָ��λ�õ�����
//* �� �� ֵ��
//* ����˵����1.pPtrArray		�ڵ�����
//*			  2.pOrderArray		��ʾ�����д�ɾ�����ݵ�λ����Ϣ
//* �� �� ��: lj
//* �������ڣ�2008��11��30��
//********************************************************************
//��������ɾ��ָ��λ�õ�����
void CCommonUtility::DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray)
{
	int iTotal = 0 ;
	for(int i = 0 ;i< pOrderArray->GetSize() ;i++)
	{
		int iOrder = pOrderArray->GetAt(i);
		pPtrArray->RemoveAt(iOrder - iTotal);
		iTotal = iTotal + 1;
	}
}
//********************************************************************
//* ģ�����ƣ�DeleteDataFromArray()
//* ģ�鹦�ܣ���������ɾ��ָ��λ�õ�����
//* �� �� ֵ��
//* ����˵����1.pPtrArray		�ڵ�����
//*			  2.pOrderArray		��ʾ�����д�ɾ�����ݵ�λ����Ϣ
//*			  3.bSort           �Ƿ���Ҫ����Ĭ��ֵΪTURE��
//* �� �� ��: ZLJ
//* �������ڣ�2008��11��30��
//********************************************************************
//��������ɾ��ָ��λ�õ�����
void CCommonUtility::DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray,BOOL bSort)
{
	if (bSort)
	{
		int iTmp ;	//���ڽ��н�������ʱ����
		int i,j;
		BOOL exchange; //������־
		for(i=0;i<pOrderArray->GetSize()-1;i++)
		{
			//�����n-1������
			exchange=FALSE;//��������ʼǰ��������־ӦΪ��
			for(j=pOrderArray->GetSize()-2;j>=i;j--) //�Ե�ǰ������R[i..n]��������ɨ��
			{
				int iAfter = pOrderArray->GetAt(j+1);
				int iBefore = pOrderArray->GetAt(j);
				if(iAfter < iBefore)
				{
					//������¼
					iTmp = iAfter;//R[j+1]; //R[0]�����ڱ��������ݴ浥Ԫ
					//	pAfter = pBefore;
					pOrderArray->SetAt(j,iAfter);
					pOrderArray->SetAt(j+1,iBefore);
					// 				R[j+1]=R[j];
					// 				R[j]=R[0];
					exchange=TRUE; //�����˽������ʽ�������־��Ϊ��
				}
			}
		}  
		DeleteDataFromArray(pPtrArray,pOrderArray);
	}
	else
	{
		DeleteDataFromArray(pPtrArray,pOrderArray);
	}
}

//********************************************************************
//* ģ�����ƣ�DeleteProcess
//* ģ�鹦�ܣ�ɾ��������Ϣ
//* �� �� ֵ��
//* ����˵����1.strProcessName	����������Ϣ
//* �� �� ��: lj
//* �������ڣ�2008��03��21��
//********************************************************************
//ɾ��������Ϣ
BOOL CCommonUtility::DeleteProcess(CString strProcessName)
{
// 	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);	
// 	PROCESSENTRY32 pe;
// 	pe.dwSize = sizeof(PROCESSENTRY32);
// 	if(!Process32First(hSnapshot,&pe))
// 	{
// 		return FALSE;
// 	}
// 
// 	//CString strProcessName = ;
// 	strProcessName.MakeLower();
// 	while(Process32Next(hSnapshot,&pe))
// 	{
// 		CString strTmp = pe.szExeFile;
// 		strTmp.MakeLower();
// 		if(!strTmp.Compare(strProcessName))
// 		{
// 			DWORD dwProcessID = pe.th32ProcessID;
// 			// 			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
// 			// 			::TerminateProcess(hProcess,0);
// 
// 			//������Ӧ�Ĵ��� 
// 			CMainWindowIterator itw(dwProcessID);
// 
// 			for(HWND hwnd = itw.First(); hwnd ;hwnd=itw.Next())
// 			{
// 				DWORD pidwin;
// 				GetWindowThreadProcessId(hwnd, &pidwin);
// 				if(pidwin == dwProcessID)
// 				{
// 					//�򴰿ڷ�����Ϣ 
// 					::SendMessage(hwnd,WM_CLOSE,0,0);// ->PostMessage(WM_CLOSE); 
// 
// 					HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
// 					::TerminateProcess(hProcess,0);
// 				}
// 			}
// 			//::EnumWindows(EnumProc, (LPARAM)this);
// 
// 			//	CloseHandle(hProcess);
// 			//	return TRUE;
// 		}
// 
// 		strTmp.ReleaseBuffer();
// 	}

	return TRUE;
}

 //********************************************************************
 //* ģ�����ƣ�ShowDlgYes()
 //* ģ�鹦�ܣ�������ʾ��
 //* �� �� ֵ��
 //* ����˵����1.strInfo ��ʾ��Ϣ
 //* �� �� ��: lj
 //* �������ڣ�2008��03��20��
 //********************************************************************
  void CCommonUtility::ShowDlgYes(CString strInfo,CString strTitle/*=_T("����")*/)
  {
//   	 CDlgYes dlg(strInfo,strTitle);
//   	 dlg.DoModal();
  }

 //********************************************************************
 //* ģ�����ƣ�SetComboxWithString()
 //* ģ�鹦�ܣ������ַ�������ֵ����Combox��ѡ��
 //* �� �� ֵ��
 //* ����˵����
 //* �� �� ��: ZLJ
 //* �������ڣ�2011��03��13��
 //********************************************************************
 //�����ַ�������ֵ����Combox��ѡ�
void CCommonUtility::SetComboxWithString(CComboBox *pCombox,CString strText)
{
	int nIndex = 0;
	for (nIndex;nIndex<pCombox->GetCount();++nIndex)
	{
		CString strLbtext;
		pCombox->GetLBText(nIndex,strLbtext);
		if (strText == strLbtext)
		{
			pCombox->SetCurSel(nIndex);
			break;
		}
	}
}
 //********************************************************************
 //* ģ�����ƣ�OnFilecopy
 //* ģ�鹦�ܣ������ļ��У�
 //* �� �� ֵ��
 //* ����˵����1.strSrcPath -- Դ�ļ���·��
 //*           2.strDstPath -- Ŀ���ļ���·��
 //* �������ڣ�2010��11��15��
 //* �޸����ڣ�
 //* ��    ע��GP
 //********************************************************************
 //�����ļ���
 BOOL CCommonUtility::OnFilecopy(CString  strSrcPath, CString  strDstPath) 
 {
	 // TODO: Add your command handler code here
	 //CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	 int nOk; 
	 char *strSrc = strSrcPath.GetBuffer(strSrcPath.GetLength()+2);
	 strSrc[strSrcPath.GetLength()] = '\0';
	 strSrc[strSrcPath.GetLength()+1] = '\0';

	 char *strDst = strDstPath.GetBuffer(strDstPath.GetLength()+2);
	 strDst[strDstPath.GetLength()] = '\0';
	 strDst[strDstPath.GetLength()+1] = '\0';

	 //����Ŀ��·�� 
	 char strTitle[]="���ڵ������̣����Ժ�..."; 
	 //������ͷ 
	 SHFILEOPSTRUCT FileOp; 
	 FileOp.hwnd=NULL; 
	 FileOp.wFunc=FO_COPY; 
	 //ִ���ļ����� 
	 FileOp.pFrom= strSrc; 
	 FileOp.pTo=strDst; 
	 FileOp.fFlags=FOF_ALLOWUNDO | FOF_SIMPLEPROGRESS; 
	 FileOp.hNameMappings=NULL; 
	 FileOp.lpszProgressTitle=strTitle; 
	
	 nOk=SHFileOperation(&FileOp); 

	 
	 DWORD dwError = GetLastError();
	 if(nOk) 
	 {
		 TRACE("There is an error: %d\n",nOk); 
		 return FALSE;
	 }
	 else 
	 {
		 TRACE("SHFileOperation finished successfully\n"); 
		 return TRUE;
	 }
	 if(FileOp.fAnyOperationsAborted) 
	 {
		 TRACE("Operation was aborted!\n"); 
		 return FALSE;
	 }
	 return TRUE;
 }
 //********************************************************************
 //* ģ�����ƣ�CombinationStr
 //* ģ�鹦�ܣ��ϲ��ַ���
 //* �� �� ֵ��
 //* ����˵����
 //* �������ڣ�2010��11��15��
 //* �޸����ڣ�
 //* ��    ע��ZLJ
 //********************************************************************
 //�ϲ��ַ���
CString CCommonUtility::CombinationStr(CStringArray *subArray,CString sep)
{
	CString strRet;

	//�ϲ��ַ�������sepΪ�ָ���;
	for (int i=0;i<subArray->GetCount();++i)
	{
		strRet = strRet + subArray->GetAt(i) + sep;
	}

	//ɾ�����һ��sep�ַ���
	strRet.Delete(strRet.GetLength()-1,1);

	return strRet;
}
//********************************************************************
//* ģ�����ƣ�SpliteStr
//* ģ�鹦�ܣ���str���շָ�����sep�ָ����ַ�����,�ָ�����ַ��������β�ո�
//* �� �� ֵ��
//* ����˵����1.str					���ָ��ַ���
//*			  2.sep					�ָ��ַ���
//*			  3.subArray            �ָ����ַ���
//* �� �� ��: lj
//* �������ڣ�2009��09��18��
//********************************************************************
//���ܣ���str���շָ�����sep�ָ����ַ�����,�ָ�����ַ��������β�ո�,�ָ�����
//������1�����������
BOOL CCommonUtility::SpliteStr(CString str,CString sep,CStringArray& subArray)
{
	int nStart = 0;
	int nIdx;
	subArray.RemoveAll();
	CString sTemp;
	while((nIdx = str.Find(sep,nStart)) != -1){
		sTemp = str.Mid(nStart,nIdx-nStart);
		sTemp.TrimLeft();
		sTemp.TrimRight();
		if(!sTemp.IsEmpty())
			subArray.Add(sTemp);
		nStart = nIdx + sep.GetLength();
	} 
	sTemp = str.Mid(nStart);
	sTemp.TrimLeft();
	sTemp.TrimRight();
	if(!sTemp.IsEmpty())
		subArray.Add(sTemp);
	return TRUE;
}
//********************************************************************
//* ģ�����ƣ�GetAllDirsName
//* ģ�鹦�ܣ����ָ��·���µ������ļ�������
//* �� �� ֵ��
//* ����˵����
//* �� �� ��: GP
//* �������ڣ�2011��03��17��
//********************************************************************
//���ָ��·���µ������ļ�������
void CCommonUtility::GetAllDirsName(CString strDirPath,CList<CString,CString>& saveAllDirsName)
{
	CFileFind finder;
	strDirPath += "\\*.*";
	BOOL bWorking = finder.FindFile(strDirPath);
	while  (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			CString strDirName = finder.GetFileName();
			saveAllDirsName.AddTail(strDirName);
		}
	}
	finder.Close();
}
//********************************************************************
//* ģ�����ƣ�ExcludeStr
//* ģ�鹦�ܣ�ɾ���ַ������е��ַ���
//* �� �� ֵ��
//* ����˵����
//* �� �� ��: lj
//* �������ڣ�2009��09��18��
//********************************************************************
//ɾ���ַ������е��ַ�����
void CCommonUtility::ExcludeStr(CStringArray *pArray,CString ExcludeStr)
{
	for (int i= pArray->GetCount()-1;i>=0;--i)
	{
		if (pArray->GetAt(i) == ExcludeStr)
		{
			pArray->RemoveAt(i);
		}
	}
}
//********************************************************************
//* ģ�����ƣ�GetPictureSize
//* ģ�鹦�ܣ���ȡcell��ͼƬ�ĳߴ� 
//* �� �� ֵ��
//* ����˵����strPicturePath ͼƬ��·��
//* �� �� ��: GP
//* �������ڣ�2011��05��23��
//********************************************************************
//��ȡcell��ͼƬ�ĳߴ� ���ظ�ʽΪ ���ߣ���
void CCommonUtility::GetPictureSize(CString strPicturePath,long& lHight,long& lWidth)
{
// 	Gdiplus::Bitmap *m_pImage;
// 	m_pImage = Gdiplus::Bitmap::FromFile(_bstr_t(strPicturePath));
// 	if (m_pImage != NULL)
// 	{
// 		lHight =	m_pImage->GetHeight();
// 		lWidth = 	m_pImage->GetWidth();
// 		delete m_pImage;
// 	}
	
}
//********************************************************************
//* ģ�����ƣ�GetAllFileName
//* ģ�鹦�ܣ���ȡ�ƶ��ļ�������������ļ�
//* �� �� ֵ��
//* ����˵����
//* �� �� ��: GP
//* �������ڣ�2011��05��23��
//********************************************************************
//��ȡ�ƶ��ļ�������������ļ�
void CCommonUtility::GetAllFileName(CString strDirPath,CStringArray *pArray)
{
	CFileFind finder;
	strDirPath += "\\*.*";
	BOOL bWorking = finder.FindFile(strDirPath);
	while  (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory() && !finder.IsDots())
		{
			CString strDirName = finder.GetFileName();
			pArray->Add(strDirName);
		}
	}
	finder.Close();
}
/******************************************************************************/
//	���ƣ�GetFolder
//	���ܣ�Ŀ¼ѡ��Ի���
//  ������
//	���أ�ѡ��ɹ�����true�����򷵻�false
//  ��ע��
/******************************************************************************/
//��ȡĿ¼ѡ��Ի����·��
BOOL CCommonUtility::GetFolder(CString &Folder)
{
	BOOL bRet = FALSE;
	BROWSEINFO	bi; 
	ITEMIDLIST	*pidl; 
	char		Dir[256];

	memset(&bi, 0, sizeof(bi));
	bi.pszDisplayName = Dir; 
	bi.lpszTitle = "��ѡ��Ŀ¼"; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 

	pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		SHGetPathFromIDList(pidl,Dir);
		Folder = Dir;
		bRet = TRUE;
	} 	
	else
	{
		Folder = "";
		bRet = FALSE;
	}

	GlobalFree(pidl);
	return bRet;
}


/******************************************************************************/
//	���ƣ�Parse
//	���ܣ������ַ���
//  ������strDesΪԴ�ַ�����strFirst/strSecond/strThirdΪ���������ַ�����nTypeΪ�������ͣ�
//			nType=1ʱΪ��ֵ����, nType=2Ϊ�½�����ģ��ʱ�ַ����ж�
//	���أ�ѡ��ɹ�����true�����򷵻�false
//  ��ע��
//  add by yin 20110921
/******************************************************************************/
//�����ַ���
BOOL	CCommonUtility::Parse(CString strSource, CString& strFirst, CString& strSecond, CString& strThird, int nType/* = 1*/)
{
	if(strSource.IsEmpty())
	{
		return FALSE;
	}

	int i = 0;

	switch(nType)
	{
	case 1:
		{
			int nPos = 0;
			
			nPos = strSource.Find(' ');
			strFirst = strSource.Left(nPos);

			nPos = strSource.GetLength() - 1 - strSource.ReverseFind(' ');
			CString strTemp = strSource.Right(nPos);

			for(i = 0; i < strTemp.GetLength(); ++i)
			{
				if((strTemp.GetAt(i) >= '0' && strTemp.GetAt(i) <= '9') || 
					strTemp.GetAt(i) == '-' || strTemp.GetAt(i) == '+' ||
					strTemp.GetAt(i) == '.')
				{
					strSecond += strTemp.GetAt(i);
				}
				else
					break;
			}

			nPos = strTemp.GetLength() - i;
			strThird = strTemp.Right(nPos);
		}
		break;
	case 2:
		{
			char ch;

			for(i = 0; i < strSource.GetLength(); ++i)
			{
				ch = strSource.GetAt(i);
				if( !(isupper(ch) || isdigit(ch)) )
				{
					return FALSE;
				}
			}
		}
		break;
	case 3:
		{
			char ch;

			for(i = 0; i < strSource.GetLength(); ++i)
			{
				ch = strSource.GetAt(i);
				if( !(isupper(ch)) )
				{
					return FALSE;
				}
			}
		}
		break;
	default:
		break;
	}

	return TRUE;

}
//·�����ļ����Ƿ����
BOOL CCommonUtility::IsDirExist(CString strDirPath,CString strDirName)
{
	CFileFind finder;

	CString strWildcard(strDirPath);
	strWildcard += _T("*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			CString str = finder.GetFileName();
			if (strDirName == str)
			{
				return TRUE;
			}
		}
		else
			continue;
	}
	finder.Close();
	return FALSE;
}
/******************************************************************************/
//	���ƣ�Parse
//	���ܣ������ַ���
//  ������strDesΪԴ�ַ�����strFirst/strSecond/strThirdΪ���������ַ�����nTypeΪ�������ͣ�
//			nType=1ʱΪ��ֵ����, nType=2Ϊ�½�����ģ��ʱ�ַ����ж�
//	���أ�ѡ��ɹ�����true�����򷵻�false
//  ��ע��
//  add by yin 20110921
/******************************************************************************/
//�����ַ���
BOOL CCommonUtility::ParseIfNumber(CString strDes)
{
	if(strDes.IsEmpty())
	{
		return FALSE;
	}

	int i = 0;

	for(i = 0; i < strDes.GetLength(); ++i)
	{
		if((strDes.GetAt(i) >= '0' && strDes.GetAt(i) <= '9'))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;

}

/******************************************************************************/
//	���ƣ�IsNumber
//	���ܣ��жϸ������ַ����Ƿ�Ϊ����
//  �������ַ���
//	���أ�ѡ��ɹ�����true�����򷵻�false
//  ��ע��
//  add by lj 20110921
/******************************************************************************/
BOOL CCommonUtility::IsNumber(CString strDes)
{
	if(strDes.IsEmpty())
	{
		return FALSE;
	}

	int i = 0;

	for(i = 0; i < strDes.GetLength(); ++i)
	{
		if((strDes.GetAt(i) >= '0' && strDes.GetAt(i) <= '9'))
		{
			//return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;

}

/******************************************************************************/
//	���ƣ�CheckData
//	���ܣ�ȥ���ַ����з�����
//  ������strNumΪԴ�ַ�����
//	���أ��������������ַ�
//  ��ע��
//  add by szy 2011-11-01
/******************************************************************************/
//�����ַ���
CString CCommonUtility::CheckData(CString strNum)
{
	CString strTemp[2]={"",""};
	for (int i=0;i<strNum.GetLength();i++)
	{
		char num = strNum.GetAt(i);
		if (num>='0'&&num<='9')
		{
			strTemp[0].Format("%c",strNum.GetAt(i));
			strTemp[1] += strTemp[0];
		}
	}
	return strTemp[1];
}
/******************************************************************************/
//	���ƣ�GetDateNum
//	���ܣ���ȡ�����е������գ�2011��2��3�գ�
//  ������strʱ���ַ�����oArray����ֵ
//	���أ�
//  ��ע��
//  add by szy 2011-11-08
/******************************************************************************/
//�����ַ���
CString CCommonUtility::GetDateNum(CString str,CStringArray &oArray)
{
	char date[14];
	CString strYear,strMonth,strDate;

	for (int i=0;i<str.GetLength();i++)
		date[i] = str.GetAt(i);

	strYear.Format("%c%c%c%c",date[0],date[1],date[2],date[3]);

	if ('0'<=date[7]&&date[7]<='9')
	{
		strMonth.Format("%c%c",date[6],date[7]);
		if ('0'<=date[11]&&date[11]<='9')
			strDate.Format("%c%c",date[10],date[11]);
		else
			strDate.Format("%c",date[10]);
	}
	else
	{
		strMonth.Format("%c",date[6]);
		if ('0'<=date[10]&&date[10]<='9')
			strDate.Format("%c%c",date[9],date[10]);
		else
			strDate.Format("%c",date[9]);
	}

	oArray.Add(strYear);
	oArray.Add(strMonth);
	oArray.Add(strDate);

	return strYear+"-"+strMonth+"-"+strDate;
}

//********************************************************************
//* ģ�����ƣ�IsContainChar
//* ģ�鹦�ܣ��ж��Ƿ��и����ַ�
//* �� �� ֵ��
//* ����˵����1.strData		�����ַ���
//*			 2.strTmp		���ҵ��ַ�
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//�ж��Ƿ��и����ַ�
int CCommonUtility::IsContainChar(CString strData,CString strTmp,int iPos/*��ʼλ��*/)
 {
// 	for(int i = iPos;i < strData.GetLength();i++)
// 	{
// 		CString strTData = strData.GetAt(i);
// 		if(strTData == strTmp)
// 		{
// 			return i;
// 		}
// 	}

	return -1;
}

//********************************************************************
//* ģ�����ƣ�ConvertTwoCharToOne
//* ģ�鹦�ܣ��������ַ�ת��Ϊһ������16���Ʊ�ʾ���ַ�
//* �� �� ֵ��
//* ����˵����1.buf		��������������
//*			  2.oneChar ���ص�һ���ַ�
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//�������ַ�ת��Ϊһ������16���Ʊ�ʾ���ַ�
void CCommonUtility::ConvertTwoCharToOne(char* buf,char &oneChar)
{
	int valueNumH = 0;
	int valueNumL = 0;
	if(buf[0] >= '0' && buf[0] <= '9'  )
	{
		valueNumH = buf[0] - '0';
	}
	else if(buf[0] >= 'A' && buf[0] <= 'F'  )
	{
		valueNumH = buf[0] - 'A' + 10;
	}

	if(buf[1] >= '0' && buf[1] <= '9'  )
	{
		valueNumL = buf[1] - '0';
	}
	else if(buf[1] >= 'A' && buf[1] <= 'F'  )
	{
		valueNumL = buf[1] - 'A'+ 10;
	}

	oneChar = (valueNumH<<4) + valueNumL;
}

//********************************************************************
//* ģ�����ƣ�stringConvert
//* ģ�鹦�ܣ���source�е�λ����0λ����
//* �� �� ֵ��
//* ����˵����1.source		����Դ
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//��source�е�λ����0λ����
// void CCommonUtility::stringConvert(char *source)
// {
// 	int length=strlen(source);
// 	//	TCHAR temp[10];
// 	for(int i=0;i<length;i++)
// 	{
// 		if(source[i]==' ')
// 		{
// 			source[i]='0';
// 		}
// 		else if(source[i]<='z'&&source[i]>='a')
// 		{
// 			source[i]=source[i]-'a'+'A';
// 		}
// 	}
// }

//********************************************************************
//* ģ�����ƣ�Convert16IPToASCII
//* ģ�鹦�ܣ���16���Ƶ�IPת��Ϊ�ַ����͵�IP
//* �� �� ֵ��
//* ����˵����1.str16IP		16����IP
//*			  2.strIP		ASCII��IP
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//��16���Ƶ�IPת��Ϊ�ַ����͵�IP
void CCommonUtility::Convert16IPToASCII(CString str16IP,CString& strIP )
{
	//ת������
	for(int i = 0;i< 4;i++)
	{
		BYTE   szValue  =  str16IP.GetAt(i) ;     
		CString strTmp;
		strTmp.Format(_T("%d"),szValue);
		if(strTmp=="48")
			strTmp = "0";
		strIP += strTmp;
		if (i<3)
		{
			strIP += ".";
		}
	}
	
}
//��4��BYTE��ʾ����IDװ��Ϊ�ַ����͵�Ip
void CCommonUtility::Conver16IDToAscii(CString str16ID, CString& strID)
{
	int iHouse;
	int iHund;
	int iTen;
	int iIndividual;
	//ת������
	for(int i = 0;i< 4;i++)
	{
		BYTE   szValue  =  str16ID.GetAt(i) ;     
		CString strTmp;
		strTmp.Format(_T("%d"),szValue);
		int iTmp = atoi(strTmp);
		if (i==0)
	        iHouse = iTmp*256*256*256;
		else if (i==1)
	        iHund = iTmp*256*256;
		else if (i==2)
		    iTen = iTmp*256;
		else if(i==3)
			iIndividual = iTmp;
	}
	int iId = iHouse + iHund + iTen + iIndividual;
	strID.Format("%d",iId);
}

//********************************************************************
//* ģ�����ƣ�ConvertCurTimeToASCII
//* ģ�鹦�ܣ�����ǰʱ��ת��Ϊ��������
//* �� �� ֵ��1.buf		���ص���������
//* ����˵����
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//����ǰʱ��ת��Ϊ��������
void CCommonUtility::ConvertCurTimeToASCII(CString& strBuf)
{
	//��ȡ��ǰʱ�䣬���������֡
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString strYearH,strYearL,strMonth,strDay,strHour,strMinute,strSecond;
	//��ת��
	strYearH.Format("%4d",sysTime.wYear);
	

	//��
	strMonth.Format("%2d",sysTime.wMonth);

	//��
	strDay.Format("%2d",sysTime.wDay);

	//ʱ
	strHour.Format("%2d",sysTime.wHour);


	//��
	strMinute.Format("%2d",sysTime.wMinute);
	

	//��
	strSecond.Format("%2d",sysTime.wSecond);

	strBuf = strBuf + strYearH + strYearL + strMonth + strDay ;
	strBuf = strBuf + strHour + strMinute + strSecond;
}

//********************************************************************
//* ģ�����ƣ�ConvertCurTimeTo16
//* ģ�鹦�ܣ�����ǰʱ��ת��Ϊ��������
//* �� �� ֵ��1.buf		���ص���������
//* ����˵����
//* �������ڣ�2012��02��25��
//* �޸����ڣ�
//* ��    ע��added by lj 
//********************************************************************
//����ǰʱ��ת��Ϊ��������
void CCommonUtility::ConvertCurTimeTo16(CString& strBuf)
{
	//��ȡ��ǰʱ�䣬���������֡
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString strYearH,strYearL,strMonth,strDay,strHour,strMinute,strSecond;
	//��ת��
	strYearH.Format("%2X",sysTime.wYear>>8);
	strYearL.Format("%2X",(sysTime.wYear&0x00FF));

	char charYearH,charYearL;
	CCommonUtility::ConvertTwoCharToOne(strYearH.GetBuffer(),charYearH);
	CCommonUtility::ConvertTwoCharToOne(strYearL.GetBuffer(),charYearL);

	strYearH.Format("%c",charYearH);
	if(strYearH.GetLength() > 0 && strYearH.GetAt(0) == 0x00)
	{
		strYearH= "0";
	}

	strYearL.Format("%c",charYearL);
	if(strYearL.GetLength() > 0 && strYearL.GetAt(0) == 0x00)
	{
		strYearL= "0";
	}

	//��
	strMonth.Format("%2X",sysTime.wMonth);
	CCommonUtility::ConvertTwoCharToOne(strMonth.GetBuffer(),charYearH);
	strMonth.Format("%c",charYearH);
	if(strMonth.GetLength() > 0 && strMonth.GetAt(0) == 0x00)
	{
		strMonth= "0";
	}

	//��
	strDay.Format("%2X",sysTime.wDay);
	CCommonUtility::ConvertTwoCharToOne(strDay.GetBuffer(),charYearH);
	strDay.Format("%c",charYearH);
	if(strDay.GetLength() > 0 && strDay.GetAt(0) == 0x00)
	{
		strDay= "0";
	}

	//ʱ
	strHour.Format("%2X",sysTime.wHour);
	CCommonUtility::ConvertTwoCharToOne(strHour.GetBuffer(),charYearH);
	strHour.Format("%c",charYearH);
	if(strHour.GetLength() > 0 && strHour.GetAt(0) == 0x00)
	{
		strHour= "0";
	}

	//��
	strMinute.Format("%2X",sysTime.wMinute);
	CCommonUtility::ConvertTwoCharToOne(strMinute.GetBuffer(),charYearH);
	strMinute.Format("%c",charYearH);
	if(strMinute.GetLength() > 0 && strMinute.GetAt(0) == 0x00)
	{
		strMinute= "0";
	}

	//��
	strSecond.Format("%2X",sysTime.wSecond);
	CCommonUtility::ConvertTwoCharToOne(strSecond.GetBuffer(),charYearH);
	strSecond.Format("%c",charYearH);
	if(strSecond.GetLength() > 0 && strSecond.GetAt(0) == 0x00)
	{
		strMinute= "0";
	}


	strBuf = strBuf + strYearH + strYearL + strMonth + strDay ;
	strBuf = strBuf + strHour + strMinute + strSecond;
}
//���ַ���ת��Ϊ���ַ�.
//�ڷ������˽��յ������ǣ�����UTF-8��UNICODE��ת��  C++  -->  java
wchar_t * CCommonUtility::singlestring_to_wide(const char * pastring)
{
	int wchar_count = MultiByteToWideChar(CP_UTF8, 0, pastring, -1,   NULL, 0); 
	wchar_t * pwstring = (wchar_t *)malloc(wchar_count * sizeof(wchar_t));      
	::MultiByteToWideChar(CP_UTF8, 0, pastring, -1, pwstring, wchar_count * sizeof(wchar_t)); 
	return pwstring;  
}
//���ַ���ת��Ϊ���ַ���
//�ڷ������������ݸ��ͻ���ʱ������UNICODE��UTF-8��ת����JAVA --> C++
char* CCommonUtility::widestring_to_single(wchar_t * pwstring)
{
	DWORD char_count = WideCharToMultiByte(CP_UTF8, NULL, pwstring, -1,NULL, 0, NULL, FALSE);  
	char * pastring = (char*)malloc(char_count);  
	WideCharToMultiByte(CP_UTF8, 0, pwstring, -1,pastring, char_count, NULL, NULL);  
	return pastring;  
}