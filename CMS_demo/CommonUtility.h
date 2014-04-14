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

	//�õ����������е���Ŀ¼
	static int  GetMainDirectory(CString& strMainDir);

	//�����ļ���
	static void BrowseDir(CString strDir,CString strFilePath);

	//��ȡϵͳ��ǰʱ��
	static CString CCommonUtility::GetSysCurDate();

	////////////////////////////////
	//ɾ���ļ���(�����Ƕ���ļ���)
	//mode=LEAVING_ROOT ��ʾ���¿յĸ�Ŀ¼
	//mode=LEAVING_NOTHING ��ʾȫ��ɾ��
	static void RemoveDir(CString strDirectory,int mode  = LEAVING_NOTHING);

	//��������ɾ��ָ��λ�õ�����
	static void DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray);

	//��������ɾ��ָ��λ�õ�����
	static void DeleteDataFromArray(CPtrArray *pPtrArray,CUIntArray *pOrderArray,BOOL bSort);

	//���ܣ���str���շָ�����sep�ָ����ַ�����,�ָ�����ַ��������β�ո�,�ָ�����
	//������1�����������
	static BOOL SpliteStr(CString str,CString sep,CStringArray& subArray);

	//�ϲ��ַ���
	static CString CombinationStr(CStringArray *subArray,CString sep);

	//����GUID��
	static CString GenGUID();


	//�ж������Ƿ������������
	static BOOL IsNameInArray(CStringArray *pStrNameArray,CString strName);

	//�ж������Ƿ������������
	static BOOL IsNameInArrayWithExact(CStringArray *pStrNameArray,CString strName,int &iIndex);

	//�ж������Ƿ������������
	static BOOL IsNameInArray(CStringArray *pStrNameArray,CString strName,int &iIndex);

	//�����������
	static void ClearPtrArray(CPtrArray *pArray);

	//�ж��ļ��Ƿ���ڣ�������ڣ�����TRUE�����򣬷���FALSE
	static BOOL IsFileExist(CString strFileName);

	//�ڵ����ļ���д������
	static void WriteDataIntoFile(CString strData);

	//ɾ��������Ϣ
	static BOOL DeleteProcess(CString strProcessName);

	//����GUID
	static void CreateID(CString &strID);

	// �ݹ�ɾ��������Ŀ¼
	static BOOL DeleteDir(LPCTSTR lpszDirName);

	//������ʾ��
	static void ShowDlgYes(CString strInfo,CString strTitle=_T("����"));

	//�����ַ�������ֵ����Combox��ѡ�
	static void SetComboxWithString(CComboBox *pCombox,CString strText);

	//����΢�����ļ��к��������ļ�
	static BOOL OnFilecopy(CString  strSrcPath, CString  strDstPath);

	//���ָ��·���µ������ļ�������
	static void GetAllDirsName(CString strDirPath,CList<CString,CString>& saveAllDirsName);

	//ɾ���ַ������е��ַ�����
	static void ExcludeStr(CStringArray *pArray,CString ExcludeStr);

	//��ȡcell��ͼƬ�ĳߴ� 
	static void GetPictureSize(CString strPicturePath,long& lHight,long& lWidth);

	//��ȡ�ƶ��ļ�������������ļ�
	static void GetAllFileName(CString strDirPath,CStringArray *pArray);
	
	//��ȡĿ¼ѡ��Ի���
	static  BOOL  GetFolder(CString &Folder);
	//·�����ļ����Ƿ����
	static BOOL IsDirExist(CString strDirPath,CString strDirName);

	//�����ַ��� 1Ϊ��ֵ
	static	BOOL	Parse(CString strSource, CString& strFirst, CString& strSecond, CString& strThird, int nType = 1);
	
	//�ж��Ƿ�Ϊ�ַ�����
	static BOOL ParseIfNumber(CString strDes);

	//���˷������ַ���
	static CString CheckData(CString strNum);

	//�õ�ʱ�䵱�е����֡����磺2011��2��3��
	static CString GetDateNum(CString str,CStringArray &oArray);

	//�жϸ������ַ����Ƿ�Ϊ����
	static BOOL IsNumber(CString strDes);

	//��ȡ�ļ���Ϣ����
	static CString GetFileData(CString strFilePath);

	//�ж��Ƿ��и����ַ�
	static int IsContainChar(CString strData,CString strTmp,int iPos = 0/*��ʼλ��*/);

	//�������ַ�ת��Ϊһ������16���Ʊ�ʾ���ַ�
	static void ConvertTwoCharToOne(char* buf,char &oneChar);

	//����ǰʱ��ת��Ϊ��������
	static void ConvertCurTimeTo16(CString& strBuf);

	//����ǰʱ��ת��Ϊ��������
	static void ConvertCurTimeToASCII(CString& strBuf);

	//��16���Ƶ�IPת��Ϊ�ַ����͵�IP
	static void Convert16IPToASCII(CString str16IP,CString& strIP );
	//��16���Ƶ�IDװ��Ϊ�ַ����͵�Ip
	static void Conver16IDToAscii(CString str16ID, CString& strID);


	//���ַ���ת��Ϊ���ַ�
	static wchar_t * singlestring_to_wide(const char * pastring);
	//���ַ���ת��Ϊ���ַ���
    static char* widestring_to_single(wchar_t * pwstring);
};
