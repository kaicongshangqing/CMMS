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
//* 模块名称：GetSysCurDate
//* 模块功能：得到系统当前时间
//* 返 回 值：系统当前时间
//* 参数说明：
//* 创 建 者: lj
//* 创建日期：2008年03月20日
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
//* 模块名称：GetMainDirectory()
//* 模块功能：得到本程序运行的主目录
//* 返 回 值：表示是否成功执行
//* 参数说明：1.strMainDir 取得的主目录路径
//* 创 建 者: lj
//* 创建日期：2008年03月20日
//********************************************************************
int  CCommonUtility::GetMainDirectory(CString& strMainDir)
{
	TCHAR   exeFullPath[MAX_PATH]; 
	CString strPath; 

	//取得模块运行的路径
	GetModuleFileName(NULL,exeFullPath,MAX_PATH); 

	strPath      = (CString)exeFullPath; 
	int position = strPath.ReverseFind('\\'); 

	strPath      = strPath.Left(position); 
	//
	strMainDir   = strPath;

	return true;
}

//*******************************************************************************
//* 模块名称：CreateID
//* 模块功能：产生ID
//* 返 回 值：
//* 参数说明：id 产生的ID地址
//* 创建日期：2008年03月20日
//* 修改日期：
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
//* 模块名称：DeleteDir
//* 模块功能：递归删除以整个目录
//* 返 回 值：
//* 参数说明：lpszDirName 名称
//* 创建日期：2008年03月20日
//* 修改日期：
//*******************************************************************************
// 递归删除以整个目录
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
//* 模块名称：BrowseDir()
//* 模块功能：递归遍历所有文件夹及文件，将其复制到指定文件夹下
//* 返 回 值：
//* 参数说明：1.strDir			浏览的文件夹
//*			  2.strFilePath		文件保存路径			
//* 创建日期：2008年07月9日
//* 修改日期：
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
		res = ff.FindNextFile();	//更新标志位
		if(ff.IsDirectory() && !ff.IsDots())	//文件夹
		{
			CString strPath = ff.GetFilePath();
			CString strTitle = ff.GetFileTitle();

			//创建文件夹
			BOOL bFlag = CreateDirectory((strFilePath + "\\" + strTitle),NULL);
			if(!bFlag)//判断文件夹创建是否成功
			{
				CString str;
				str.Format("创建文件夹%s失败！",strTitle);
				//CDKLog::Error(str);
				//CCommonUtility::ShowDlgYes(str);
			}

			//浏览下一级文件夹时对应的保存路径也应移到下一级
			BrowseDir(strPath,(strFilePath + "\\" + strTitle));
		}
		else if(!ff.IsDirectory() && !ff.IsDots())	//文件
		{
			CString strName = ff.GetFileName();

			BOOL bFlag = CopyFile((strDir + "\\" + strName),(strFilePath + "\\" + strName),FALSE);
			if(!bFlag)
			{
				CString str;
				str.Format("拷贝文件%s失败！",strName);
			//	CCommonUtility::ShowDlgYes(str);
			//	CDKLog::Error(str);
			}
		}
	}
	ff.Close();
}

//********************************************************************
//* 模块名称：RemoveDir
//* 模块功能：删除目录
//* 返 回 值：
//* 参数说明：1.strDir			浏览的文件夹
//*			  2.strFilePath		文件保存路径			
//* 创建日期：2008年07月9日
//* 修改日期：
//********************************************************************
////////////////////////////////
//删除文件夹(可以是多层文件夹)
//mode=LEAVING_ROOT 表示留下空的根目录
//mode=LEAVING_NOTHING 表示全部删除
void CCommonUtility::RemoveDir(CString strDirectory,int mode)
{
	//if(_access(strDirectory,0) == -1)
	//	return;
	if(strDirectory!="") //目录路径不能为空
	{
		CString strTemp;
		BOOL bResult;
		CFileFind finder;
		strDirectory.TrimRight("\\");
		bResult = finder.FindFile(strDirectory + "\\" + _T("*.*"));
		while (bResult)
		{
			bResult=finder.FindNextFile();
			if (finder.IsDots()) //排除.和..文件
				continue;
			if(finder.IsDirectory())
			{
				strTemp=finder.GetFilePath();
				RemoveDir(strTemp);				
			}
			else  //最底层则删除文件
				CFile::Remove(finder.GetFilePath());					
		}
		finder.Close();
		Sleep(200);
		//删除空文件夹
		if(mode == LEAVING_NOTHING)
			RemoveDirectory(strDirectory);
		Sleep(200);
	}	
}

//********************************************************************
//* 模块名称：IsNameInArrayWithExact
//* 模块功能：判断名称是否包含在数组内
//* 返 回 值：1.是TRUE 否FALSE
//* 参数说明：1.pStrNameArray		名称数组
//*			  2.strName				名称
//*			  3.iIndex				表示位置下标
//* 创 建 者: lj
//* 创建日期：2009年02月03日
//********************************************************************
//判断名称是否包含在数组内
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
//* 模块名称：IsNameInArray()
//* 模块功能：判断名称是否包含在数组内
//* 返 回 值：1.是TRUE 否FALSE
//* 参数说明：1.pStrNameArray		名称数组
//*			  2.strName				名称
//*			  3.iIndex				表示位置下标
//* 创 建 者: lj
//* 创建日期：2009年02月03日
//********************************************************************
//判断名称是否包含在数组内
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
//* 模块名称：IsNameInArray()
//* 模块功能：判断名称是否包含在数组内
//* 返 回 值：1.是TRUE 否FALSE
//* 参数说明：1.pStrNameArray		名称数组
//*			  2.strName				名称
//* 创 建 者: lj
//* 创建日期：2009年02月03日
//********************************************************************
//判断名称是否包含在数组内
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
//* 模块名称：ClearPtrArray
//* 模块功能：清空数据内容
//* 返 回 值：
//* 参数说明：
//*			 
//* 创 建 者: lj
//* 创建日期：2009年02月03日
//********************************************************************
//清空数据内容
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
//* 模块名称：IsFileExsit()
//* 模块功能：判断文件是否存在，如果存在，返回TRUE，否则，返回FALSE
//* 返 回 值：
//* 参数说明：1.strFilePath	文件路径
//* 创 建 者: lj
//* 创建日期：2008年03月21日
//********************************************************************
//判断文件是否存在，如果存在，返回TRUE，否则，返回FALSE
BOOL CCommonUtility::IsFileExist(CString strFileName)
{
	CFile file;
	CFileException ex;
	BOOL bRet = file.Open(strFileName,CFile::modeRead |  CFile::shareDenyNone,&ex);
	if(bRet == FALSE)	//不能打开文件
	{
		return FALSE;
	}
	file.Close();
	return TRUE;
}
//********************************************************************
//* 模块名称：GetFileData
//* 模块功能：获取文件信息内容
//* 返 回 值：文件内容
//* 参数说明：1.strFilePath	文件路径
//* 创 建 者: lj
//* 创建日期：2008年03月21日
//********************************************************************

//获取文件信息内容
CString CCommonUtility::GetFileData(CString strFilePath)
{
	CStdioFile file;
	CFileException ex;
	CString strData;

	try
	{	
		
		BOOL bRet = file.Open(strFilePath,CFile::modeRead |  CFile::shareExclusive,&ex);
		if(bRet == FALSE)	//不能打开文件
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
//* 模块名称：WriteDataIntoFile()
//* 模块功能：在调试文件中写入数据
//* 返 回 值：
//* 参数说明：1.strData	数据
//* 创 建 者: lj
//* 创建日期：2008年03月21日
//********************************************************************
//在调试文件中写入数据
void CCommonUtility::WriteDataIntoFile(CString strData)
{
	try
	{	
		//计算当前时间
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

		//获取当前程序运行路径
		CString strMainDir;
		GetMainDirectory(strMainDir);

		//判断当前程序运行路径下是否包括  xx_xx_xx.txt文件(其中xx_xx_xx为时间)
		CString strFileName;
		CFile file;
		CFileException ex;
		strFileName = strMainDir + "\\开发" + strDate + ".txt";
		BOOL bRet = file.Open(strFileName,CFile::modeWrite |  CFile::shareExclusive,&ex);
		if(bRet == FALSE)	//不能打开文件
		{
			//文件不存在，创建文件
			BOOL bRet = file.Open(strFileName,CFile::modeCreate | CFile::modeWrite| CFile::shareExclusive,&ex);
			if(bRet == FALSE)
			{
				return;
			}
			else
			{
				CString strTmp;
				strTmp = "运行信息文件--用于保存运行过程信息\r\n";
				int iLength = strTmp.GetLength();
				file.Write(strTmp,iLength);

				iLength = strData.GetLength();
				file.Write(strData,iLength);
				file.Close();
				return;
			}
		}

		file.SeekToEnd();
		//文件被打开，存储数据
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
//* 模块名称：DeleteDataFromArray()
//* 模块功能：从数组中删除指点位置的内容
//* 返 回 值：
//* 参数说明：1.pPtrArray		节点数组
//*			  2.pOrderArray		表示数组中待删除内容的位置信息
//* 创 建 者: lj
//* 创建日期：2008年11月30日
//********************************************************************
//从数组中删除指点位置的内容
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
//* 模块名称：DeleteDataFromArray()
//* 模块功能：从数组中删除指点位置的内容
//* 返 回 值：
//* 参数说明：1.pPtrArray		节点数组
//*			  2.pOrderArray		表示数组中待删除内容的位置信息
//*			  3.bSort           是否需要排序，默认值为TURE；
//* 创 建 者: ZLJ
//* 创建日期：2008年11月30日
//********************************************************************
//从数组中删除指点位置的内容
void CCommonUtility::DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray,BOOL bSort)
{
	if (bSort)
	{
		int iTmp ;	//用于进行交换的临时变量
		int i,j;
		BOOL exchange; //交换标志
		for(i=0;i<pOrderArray->GetSize()-1;i++)
		{
			//最多做n-1趟排序
			exchange=FALSE;//本趟排序开始前，交换标志应为假
			for(j=pOrderArray->GetSize()-2;j>=i;j--) //对当前无序区R[i..n]自下向上扫描
			{
				int iAfter = pOrderArray->GetAt(j+1);
				int iBefore = pOrderArray->GetAt(j);
				if(iAfter < iBefore)
				{
					//交换记录
					iTmp = iAfter;//R[j+1]; //R[0]不是哨兵，仅做暂存单元
					//	pAfter = pBefore;
					pOrderArray->SetAt(j,iAfter);
					pOrderArray->SetAt(j+1,iBefore);
					// 				R[j+1]=R[j];
					// 				R[j]=R[0];
					exchange=TRUE; //发生了交换，故将交换标志置为真
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
//* 模块名称：DeleteProcess
//* 模块功能：删除进程信息
//* 返 回 值：
//* 参数说明：1.strProcessName	进程名称信息
//* 创 建 者: lj
//* 创建日期：2008年03月21日
//********************************************************************
//删除进程信息
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
// 			//查找相应的窗口 
// 			CMainWindowIterator itw(dwProcessID);
// 
// 			for(HWND hwnd = itw.First(); hwnd ;hwnd=itw.Next())
// 			{
// 				DWORD pidwin;
// 				GetWindowThreadProcessId(hwnd, &pidwin);
// 				if(pidwin == dwProcessID)
// 				{
// 					//向窗口发送消息 
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
 //* 模块名称：ShowDlgYes()
 //* 模块功能：警告提示框
 //* 返 回 值：
 //* 参数说明：1.strInfo 提示信息
 //* 创 建 者: lj
 //* 创建日期：2008年03月20日
 //********************************************************************
  void CCommonUtility::ShowDlgYes(CString strInfo,CString strTitle/*=_T("警告")*/)
  {
//   	 CDlgYes dlg(strInfo,strTitle);
//   	 dlg.DoModal();
  }

 //********************************************************************
 //* 模块名称：SetComboxWithString()
 //* 模块功能：按照字符串字面值设置Combox的选项
 //* 返 回 值：
 //* 参数说明：
 //* 创 建 者: ZLJ
 //* 创建日期：2011年03月13日
 //********************************************************************
 //按照字符串字面值设置Combox的选项；
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
 //* 模块名称：OnFilecopy
 //* 模块功能：拷贝文件夹；
 //* 返 回 值：
 //* 参数说明：1.strSrcPath -- 源文件夹路径
 //*           2.strDstPath -- 目标文件夹路径
 //* 创建日期：2010年11月15日
 //* 修改日期：
 //* 备    注：GP
 //********************************************************************
 //拷贝文件夹
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

	 //设置目的路径 
	 char strTitle[]="正在导出工程，请稍后..."; 
	 //进度题头 
	 SHFILEOPSTRUCT FileOp; 
	 FileOp.hwnd=NULL; 
	 FileOp.wFunc=FO_COPY; 
	 //执行文件拷贝 
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
 //* 模块名称：CombinationStr
 //* 模块功能：合并字符串
 //* 返 回 值：
 //* 参数说明：
 //* 创建日期：2010年11月15日
 //* 修改日期：
 //* 备    注：ZLJ
 //********************************************************************
 //合并字符串
CString CCommonUtility::CombinationStr(CStringArray *subArray,CString sep)
{
	CString strRet;

	//合并字符串，以sep为分隔符;
	for (int i=0;i<subArray->GetCount();++i)
	{
		strRet = strRet + subArray->GetAt(i) + sep;
	}

	//删除最后一个sep字符；
	strRet.Delete(strRet.GetLength()-1,1);

	return strRet;
}
//********************************************************************
//* 模块名称：SpliteStr
//* 模块功能：将str按照分隔符号sep分隔成字符串组,分隔后的字符串清除首尾空格
//* 返 回 值：
//* 参数说明：1.str					待分割字符串
//*			  2.sep					分割字符串
//*			  3.subArray            分割后的字符串
//* 创 建 者: lj
//* 创建日期：2009年09月18日
//********************************************************************
//功能：将str按照分隔符号sep分隔成字符串组,分隔后的字符串清除首尾空格,分隔符号
//可以是1个或连续多个
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
//* 模块名称：GetAllDirsName
//* 模块功能：获得指定路径下的所有文件夹名称
//* 返 回 值：
//* 参数说明：
//* 创 建 者: GP
//* 创建日期：2011年03月17日
//********************************************************************
//获得指定路径下的所有文件夹名称
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
//* 模块名称：ExcludeStr
//* 模块功能：删除字符数组中的字符串
//* 返 回 值：
//* 参数说明：
//* 创 建 者: lj
//* 创建日期：2009年09月18日
//********************************************************************
//删除字符数组中的字符串；
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
//* 模块名称：GetPictureSize
//* 模块功能：获取cell中图片的尺寸 
//* 返 回 值：
//* 参数说明：strPicturePath 图片的路径
//* 创 建 者: GP
//* 创建日期：2011年05月23日
//********************************************************************
//获取cell中图片的尺寸 返回格式为 “高，宽”
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
//* 模块名称：GetAllFileName
//* 模块功能：获取制定文件夹下面的所有文件
//* 返 回 值：
//* 参数说明：
//* 创 建 者: GP
//* 创建日期：2011年05月23日
//********************************************************************
//获取制定文件夹下面的所有文件
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
//	名称：GetFolder
//	功能：目录选择对话框
//  参数：
//	返回：选择成功返回true，否则返回false
//  备注：
/******************************************************************************/
//获取目录选择对话框的路径
BOOL CCommonUtility::GetFolder(CString &Folder)
{
	BOOL bRet = FALSE;
	BROWSEINFO	bi; 
	ITEMIDLIST	*pidl; 
	char		Dir[256];

	memset(&bi, 0, sizeof(bi));
	bi.pszDisplayName = Dir; 
	bi.lpszTitle = "请选择目录"; 
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
//	名称：Parse
//	功能：解析字符串
//  参数：strDes为源字符串，strFirst/strSecond/strThird为解析出的字符串，nType为解析类型，
//			nType=1时为阈值解析, nType=2为新建数据模块时字符串判断
//	返回：选择成功返回true，否则返回false
//  备注：
//  add by yin 20110921
/******************************************************************************/
//解析字符串
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
//路径下文件夹是否存在
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
//	名称：Parse
//	功能：解析字符串
//  参数：strDes为源字符串，strFirst/strSecond/strThird为解析出的字符串，nType为解析类型，
//			nType=1时为阈值解析, nType=2为新建数据模块时字符串判断
//	返回：选择成功返回true，否则返回false
//  备注：
//  add by yin 20110921
/******************************************************************************/
//解析字符串
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
//	名称：IsNumber
//	功能：判断给定的字符串是否为数字
//  参数：字符串
//	返回：选择成功返回true，否则返回false
//  备注：
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
//	名称：CheckData
//	功能：去除字符串中非数字
//  参数：strNum为源字符串，
//	返回：检索出的数字字符
//  备注：
//  add by szy 2011-11-01
/******************************************************************************/
//解析字符串
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
//	名称：GetDateNum
//	功能：获取日期中的年月日（2011年2月3日）
//  参数：str时间字符串，oArray返回值
//	返回：
//  备注：
//  add by szy 2011-11-08
/******************************************************************************/
//解析字符串
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
//* 模块名称：IsContainChar
//* 模块功能：判断是否有给定字符
//* 返 回 值：
//* 参数说明：1.strData		给定字符串
//*			 2.strTmp		查找的字符
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//判断是否有给定字符
int CCommonUtility::IsContainChar(CString strData,CString strTmp,int iPos/*起始位置*/)
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
//* 模块名称：ConvertTwoCharToOne
//* 模块功能：将两个字符转化为一个利用16进制表示的字符
//* 返 回 值：
//* 参数说明：1.buf		给定的两个长度
//*			  2.oneChar 返回的一个字符
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//将两个字符转化为一个利用16进制表示的字符
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
//* 模块名称：stringConvert
//* 模块功能：将source中的位置用0位补足
//* 返 回 值：
//* 参数说明：1.source		数据源
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//将source中的位置用0位补足
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
//* 模块名称：Convert16IPToASCII
//* 模块功能：将16进制的IP转化为字符串型的IP
//* 返 回 值：
//* 参数说明：1.str16IP		16进制IP
//*			  2.strIP		ASCII的IP
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//将16进制的IP转化为字符串型的IP
void CCommonUtility::Convert16IPToASCII(CString str16IP,CString& strIP )
{
	//转化内容
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
//将4个BYTE表示的是ID装换为字符串型的Ip
void CCommonUtility::Conver16IDToAscii(CString str16ID, CString& strID)
{
	int iHouse;
	int iHund;
	int iTen;
	int iIndividual;
	//转化内容
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
//* 模块名称：ConvertCurTimeToASCII
//* 模块功能：将当前时间转化为可用内容
//* 返 回 值：1.buf		返回的数据内容
//* 参数说明：
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//将当前时间转化为可用内容
void CCommonUtility::ConvertCurTimeToASCII(CString& strBuf)
{
	//获取当前时间，并组成数据帧
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString strYearH,strYearL,strMonth,strDay,strHour,strMinute,strSecond;
	//年转化
	strYearH.Format("%4d",sysTime.wYear);
	

	//月
	strMonth.Format("%2d",sysTime.wMonth);

	//日
	strDay.Format("%2d",sysTime.wDay);

	//时
	strHour.Format("%2d",sysTime.wHour);


	//分
	strMinute.Format("%2d",sysTime.wMinute);
	

	//秒
	strSecond.Format("%2d",sysTime.wSecond);

	strBuf = strBuf + strYearH + strYearL + strMonth + strDay ;
	strBuf = strBuf + strHour + strMinute + strSecond;
}

//********************************************************************
//* 模块名称：ConvertCurTimeTo16
//* 模块功能：将当前时间转化为可用内容
//* 返 回 值：1.buf		返回的数据内容
//* 参数说明：
//* 创建日期：2012年02月25日
//* 修改日期：
//* 备    注：added by lj 
//********************************************************************
//将当前时间转化为可用内容
void CCommonUtility::ConvertCurTimeTo16(CString& strBuf)
{
	//获取当前时间，并组成数据帧
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString strYearH,strYearL,strMonth,strDay,strHour,strMinute,strSecond;
	//年转化
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

	//月
	strMonth.Format("%2X",sysTime.wMonth);
	CCommonUtility::ConvertTwoCharToOne(strMonth.GetBuffer(),charYearH);
	strMonth.Format("%c",charYearH);
	if(strMonth.GetLength() > 0 && strMonth.GetAt(0) == 0x00)
	{
		strMonth= "0";
	}

	//日
	strDay.Format("%2X",sysTime.wDay);
	CCommonUtility::ConvertTwoCharToOne(strDay.GetBuffer(),charYearH);
	strDay.Format("%c",charYearH);
	if(strDay.GetLength() > 0 && strDay.GetAt(0) == 0x00)
	{
		strDay= "0";
	}

	//时
	strHour.Format("%2X",sysTime.wHour);
	CCommonUtility::ConvertTwoCharToOne(strHour.GetBuffer(),charYearH);
	strHour.Format("%c",charYearH);
	if(strHour.GetLength() > 0 && strHour.GetAt(0) == 0x00)
	{
		strHour= "0";
	}

	//分
	strMinute.Format("%2X",sysTime.wMinute);
	CCommonUtility::ConvertTwoCharToOne(strMinute.GetBuffer(),charYearH);
	strMinute.Format("%c",charYearH);
	if(strMinute.GetLength() > 0 && strMinute.GetAt(0) == 0x00)
	{
		strMinute= "0";
	}

	//秒
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
//单字符串转换为宽字符.
//在服务器端接收到数据是，进行UTF-8到UNICODE的转换  C++  -->  java
wchar_t * CCommonUtility::singlestring_to_wide(const char * pastring)
{
	int wchar_count = MultiByteToWideChar(CP_UTF8, 0, pastring, -1,   NULL, 0); 
	wchar_t * pwstring = (wchar_t *)malloc(wchar_count * sizeof(wchar_t));      
	::MultiByteToWideChar(CP_UTF8, 0, pastring, -1, pwstring, wchar_count * sizeof(wchar_t)); 
	return pwstring;  
}
//宽字符串转换为单字符串
//在服务器发送数据给客户端时，进行UNICODE到UTF-8的转换：JAVA --> C++
char* CCommonUtility::widestring_to_single(wchar_t * pwstring)
{
	DWORD char_count = WideCharToMultiByte(CP_UTF8, NULL, pwstring, -1,NULL, 0, NULL, FALSE);  
	char * pastring = (char*)malloc(char_count);  
	WideCharToMultiByte(CP_UTF8, 0, pwstring, -1,pastring, char_count, NULL, NULL);  
	return pastring;  
}