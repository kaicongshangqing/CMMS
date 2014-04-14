#pragma once


enum{
	RELATIVE_PATH,
	ABSOLUTE_PATH,
	LEAVING_ROOT,
	LEAVING_NOTHING
};
//#include "..\Element\DlgYes\DlgYes.h"
// #include "..\Core\ProjectManage.h"
// #include "..\Core\GlobalManage.h"
//#include "..\IETMDevNew\MainFrm.h"
#include <afx.h>
class CCommonUtility
{
public:
	CCommonUtility(void);
	~CCommonUtility(void);

	//得到本程序运行的主目录
	static int  GetMainDirectory(CString& strMainDir);

	//拷贝文件夹
	static void BrowseDir(CString strDir,CString strFilePath);

	//获取系统当前时间
	static CString CCommonUtility::GetSysCurDate();

	////////////////////////////////
	//删除文件夹(可以是多层文件夹)
	//mode=LEAVING_ROOT 表示留下空的根目录
	//mode=LEAVING_NOTHING 表示全部删除
	static void RemoveDir(CString strDirectory,int mode  = LEAVING_NOTHING);

	//从数组中删除指点位置的内容
	static void DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray);

	//从数组中删除指点位置的内容
	static void DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray,BOOL bSort);

	//功能：将str按照分隔符号sep分隔成字符串组,分隔后的字符串清除首尾空格,分隔符号
	//可以是1个或连续多个
	static BOOL SpliteStr(CString str,CString sep,CStringArray& subArray);

	//合并字符串
	static CString CombinationStr(CStringArray *subArray,CString sep);

	//生成GUID号
	static CString GenGUID();


	//判断名称是否包含在数组内
	static BOOL IsNameInArray(CStringArray *pStrNameArray,CString strName);

	//判断名称是否包含在数组内
	static BOOL IsNameInArrayWithExact(CStringArray *pStrNameArray,CString strName,int &iIndex);

	//判断名称是否包含在数组内
	static BOOL IsNameInArray(CStringArray *pStrNameArray,CString strName,int &iIndex);

	//清空数据内容
	static void ClearPtrArray(CPtrArray *pArray);

	//判断文件是否存在，如果存在，返回TRUE，否则，返回FALSE
	static BOOL IsFileExist(CString strFileName);

	//在调试文件中写入数据
	static void WriteDataIntoFile(CString strData);

	//删除进程信息
	static BOOL DeleteProcess(CString strProcessName);

	//创建GUID
	static void CreateID(CString &strID);

	// 递归删除以整个目录
	static BOOL DeleteDir(LPCTSTR lpszDirName);

	//警告提示框
	static void ShowDlgYes(CString strInfo,CString strTitle=_T("警告"));

	//按照字符串字面值设置Combox的选项；
	static void SetComboxWithString(CComboBox *pCombox,CString strText);

	//调用微软复制文件夹函数复制文件
	static BOOL OnFilecopy(CString  strSrcPath, CString  strDstPath);

	//获得指定路径下的所有文件夹名称
	static void GetAllDirsName(CString strDirPath,CList<CString,CString>& saveAllDirsName);

	//删除字符数组中的字符串；
	static void ExcludeStr(CStringArray *pArray,CString ExcludeStr);

	//获取cell中图片的尺寸 
	static void GetPictureSize(CString strPicturePath,long& lHight,long& lWidth);

	//获取制定文件夹下面的所有文件
	static void GetAllFileName(CString strDirPath,CStringArray *pArray);
	
	//获取目录选择对话框
	static  BOOL  GetFolder(CString &Folder);
	//路径下文件夹是否存在
	static BOOL IsDirExist(CString strDirPath,CString strDirName);

	//解析字符串 1为阈值
	static	BOOL	Parse(CString strSource, CString& strFirst, CString& strSecond, CString& strThird, int nType = 1);
	
	//判断是否为字符串型
	static BOOL ParseIfNumber(CString strDes);

	//过滤非数字字符串
	static CString CheckData(CString strNum);

	//得到时间当中的数字。例如：2011年2月3日
	static CString GetDateNum(CString str,CStringArray &oArray);

	//判断给定的字符串是否为数字
	static BOOL IsNumber(CString strDes);

	//获取文件信息内容
	static CString GetFileData(CString strFilePath);

	//判断是否有给定字符
	static int IsContainChar(CString strData,CString strTmp,int iPos = 0/*起始位置*/);

	//将两个字符转化为一个利用16进制表示的字符
	static void ConvertTwoCharToOne(char* buf,char &oneChar);

	//将当前时间转化为可用内容
	static void ConvertCurTimeTo16(CString& strBuf);

	//将当前时间转化为可用内容
	static void ConvertCurTimeToASCII(CString& strBuf);

	//将16进制的IP转化为字符串型的IP
	static void Convert16IPToASCII(CString str16IP,CString& strIP );
	//将16进制的ID装换为字符串型的Ip
	static void Conver16IDToAscii(CString str16ID, CString& strID);


	//单字符串转换为宽字符
	static wchar_t * singlestring_to_wide(const char * pastring);
	//宽字符串转换为单字符串
    static char* widestring_to_single(wchar_t * pwstring);
};
