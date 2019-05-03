/****************************************************************************
 *
 * NLPIR Chinese Keyword Extraxt System Copyright (c) 2000-2013
 *     Dr. Kevin Zhang (Hua-Ping Zhang)
 *     All rights reserved.
 *
 * This file is the confidential and proprietary property of 
 * Kevin Zhang and the possession or use of this file requires 
 * a written license from the author.
 * Filename: 
 * Abstract:
 *          KeyExtract.h: definition of the NLPIR lexical analysis system API
 * Author:   Kevin Zhang 
 *          Email: pipy_zhang@msn.com kevinzhang@bit.edu.cn
 *			Weibo: http://weibo.com/drkevinzhang
 * Date:     2013-3-1
 *
 * Notes:
 *                
 ****************************************************************************/
#if !defined(__KEYEXTRACT_2013_LIB_INCLUDED__)
#define __KEYEXTRACT_2013_LIB_INCLUDED__

#define OS_LINUX

#ifdef OS_LINUX
#define KEYEXTRACT_API extern "C" 
#else
#ifdef KEYEXTRACT_EXPORTS
#define KEYEXTRACT_API extern "C" __declspec(dllexport)
#else
#define KEYEXTRACT_API extern "C" __declspec(dllimport)
#endif
#endif

#define GBK_CODE 0//Ĭ��֧��GBK����
#define UTF8_CODE GBK_CODE+1//UTF8����
#define BIG5_CODE GBK_CODE+2//BIG5����
#define GBK_FANTI_CODE GBK_CODE+3//GBK���룬�������������

/*********************************************************************
 *
 *  Func Name  : Init
 *
 *  Description: Init KeyExtract
 *               The function must be invoked before any operation listed as following
 *
 *  Parameters : const char * sInitDirPath=NULL
 *				 sDataPath:  Path where Data directory stored.
 *				 the default value is NULL, it indicates the initial directory is current working directory path
 *
 *  Returns    : success or fail
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2013-2-6
 *********************************************************************/
KEYEXTRACT_API int KeyExtract_Init(const char * sDataPath=0,int encode=GBK_CODE,const char*sLicenceCode=0);

/*********************************************************************
 *
 *  Func Name  : KeyExtract_Exit
 *
 *  Description: Exist KeyExtract and free related buffer
 *               Exit the program and free memory
 *				 The function must be invoked while you needn't any lexical anlysis
 *
 *  Parameters : None
 *
 *  Returns    : success or fail
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2002-8-6
 *********************************************************************/
KEYEXTRACT_API bool KeyExtract_Exit();

/*********************************************************************
*
*  Func Name  : KeyExtract_GetKeyWords
*
*  Description: Extract keyword from sLine
*
*  Parameters : sLine, the input paragraph 
				bArguOut,whether  the keyword weight output
				nMaxKeyLimt:maximum of key words, up to 50
*  Returns    : keywords list like:
*               "��ѧ��չ�� ��۾��� " or
				"��ѧ��չ��/n/23.80 ��۾���/n/12.20" with weight
				�ֱ��ʾ �ؼ���/�ؼ��ʴ���/Ȩ��

*
*  Author     :   
*  History    : 
*              1.create 2012/11/12
*********************************************************************/
KEYEXTRACT_API const char * KeyExtract_GetKeyWords(const char *sLine,int nMaxKeyLimit=50,bool bWeightOut=false);

/*********************************************************************
*
*  Func Name  : KeyExtract_GetFileKeyWords
*
*  Description: Extract keyword from a text file
*
*  Parameters : sFilename, the input text file name 
				bArguOut,whether  the keyword weight output
				nMaxKeyLimt:maximum of key words, up to 50
*  Returns    : keywords list like:
*               "��ѧ��չ�� ��۾��� " or
*				"��ѧ��չ��/n/23.80 ��۾���/n/12.20" with weight
*				�ֱ��ʾ �ؼ���/�ؼ��ʴ���/Ȩ��

*
*  Author     :   
*  History    : 
*              1.create 2012/11/12
*********************************************************************/
KEYEXTRACT_API const char * KeyExtract_GetFileKeyWords(const char *sFilename,int nMaxKeyLimit=50,bool bWeightOut=false);

/*********************************************************************
 *
 *  Func Name  : KeyExtract_ImportUserDict
 *
 *  Description: Import keyword user defined dictionary
 *  Parameters : Text filename for user dictionary, each line for a imported keyword
 *				 
 *  Returns    : The  number of  lexical entry imported successfully
 *  Author     : Kevin Zhang
 *  History    : 
 *              1.create 2014-6-26
 *********************************************************************/
KEYEXTRACT_API unsigned int KeyExtract_ImportUserDict(const char *sFilename,bool bOverwrite=false);
/*********************************************************************
*
*  Func Name  : KeyExtract_AddUserWord
*
*  Description: add a word to the user dictionary ,example:
*				һ��һ·  key
*				��Ҫ��Ϊ�ؼ��ʵģ�����ǰ׺����Ϊkey
*  Parameters : const char *sWord: ���뵽��ʱ�û��ʵ��ص������ԣ��ÿո�ָ�
*
*  Returns    : 1,true ; 0,false
*
*  Author     :
*  History    :
*              1.create 2017/6/3
*********************************************************************/
KEYEXTRACT_API int KeyExtract_AddUserWord(const char *sWord);
/*********************************************************************
*
*  Func Name  : KeyExtract_CleanUserWord
*
*  Description: Clean all temporary added user words
*
*  Parameters : 
*  Returns    : 1,true ; 0,false
*
*  Author     :
*  History    :
*              1.create 2017/6/3
*********************************************************************/
KEYEXTRACT_API int KeyExtract_CleanUserWord();
/*********************************************************************
*
*  Func Name  : KeyExtract_SaveTheUsrDic
*
*  Description: Save dictionary to file
*
*  Parameters :
*
*  Returns    : 1,true; 2,false
*
*  Author     :
*  History    :
*              1.create 2017/6/3
*********************************************************************/
KEYEXTRACT_API int KeyExtract_SaveTheUsrDic();

/*********************************************************************
*
*  Func Name  : KeyExtract_DelUsrWord
*
*  Description: delete a word from the  user dictionary
*
*  Parameters :
*  Returns    : -1, the word not exist in the user dictionary; else, the handle of the word deleted
*
*  Author     :
*  History    :
*              1.create 2017/6/3
*********************************************************************/
KEYEXTRACT_API int KeyExtract_DelUsrWord(const char *sWord);

/*********************************************************************
 *
 *  Func Name  : KeyExtract_ImportKeyBlackList
 *
 *  Description: Import keyword black list 
 *  Parameters : sFilename: Text filename for user defined blacklist dictionary, each line for a stop keyword
 *				 sPOSBlacklist: ͣ�õĴ����б�������Ϊ�ô����б����ʵĴʲ�����ؼ��ʷ�Χ��
 *								������Ϊ#nr#ns#��ʾnr,ns����Ϊ�ؼ���
 *  Returns    : The  number of  lexical entry imported successfully
 *  Author     : Kevin Zhang
 *  History    :
 *              1.create 2003-11-28
 *********************************************************************/
KEYEXTRACT_API unsigned int KeyExtract_ImportKeyBlackList(const char *sFilename, const char *sPOSBlacklist = 0);
/*********************************************************************
 *
 *  Func Name  : KeyExtract_GetLastErrorMsg
 *
 *  Description: GetLastErrorMessage
 *    
 *
 *  Parameters : void
 *               
 *				  
 *  Returns    : the result buffer pointer 
 *
 *  Author     : Kevin Zhang  
 *  History    : 
 *              1.create 2014-2-27
 *********************************************************************/
KEYEXTRACT_API const char * KeyExtract_GetLastErrorMsg();


/*********************************************************************
*
*  ���º���Ϊ2013�汾ר����Թؼ����������ֵĹ��̣�һ�㽨���ѻ�ʵ�֣��������ߴ���
*********************************************************************/
/*********************************************************************
*
*  Func Name  : KeyExtract_Batch_Start
*
*  Description: �����ؼ���ʶ��

*
*  Parameters : None
*  Returns    : bool, true:success, false:fail
*
*  Author     : Kevin Zhang
*  History    :
*              1.create 2013/11/23
*********************************************************************/
KEYEXTRACT_API int KeyExtract_Batch_Start();//New Word Indentification Start
/*********************************************************************
*
*  Func Name  : KeyExtract_Batch_AddFile
*
*  Description: ���ؼ���ʶ��ϵͳ�����Ӵ�ʶ��ؼ��ʵ��ı��ļ�
*				��Ҫ������KeyExtract_Batch_Start()֮�󣬲���Ч
*
*  Parameters : const char *sFilename���ļ���
*  Returns    : bool, true:success, false:fail
*
*  Author     : Kevin Zhang
*  History    :
*              1.create 20132/11/23
*********************************************************************/
KEYEXTRACT_API unsigned long  KeyExtract_Batch_AddFile(const char *sFilename);
/*********************************************************************
*
*  Func Name  : KeyExtract_Batch_AddMem
*
*  Description: ���ؼ���ʶ��ϵͳ������һ�δ�ʶ��ؼ��ʵ��ڴ�
*				��Ҫ������KeyExtract_Batch_Start()֮�󣬲���Ч
*
*  Parameters : const char *sFilename���ļ���
*  Returns    : bool, true:success, false:fail
*
*  Author     : Kevin Zhang
*  History    :
*              1.create 2013/11/23
*********************************************************************/
KEYEXTRACT_API unsigned long KeyExtract_Batch_AddMem(const char *sText);
/*********************************************************************
*
*  Func Name  : KeyExtract_Batch_Complete
*
*  Description: �ؼ���ʶ���������ݽ���
*				��Ҫ������KeyExtract_Batch_Start()֮�󣬲���Ч
*
*  Parameters : None
*  Returns    : bool, true:success, false:fail
*
*  Author     : Kevin Zhang
*  History    :
*              1.create 2013/11/23
*********************************************************************/
KEYEXTRACT_API int KeyExtract_Batch_Complete();//�ļ������ڴ浼�����

/*********************************************************************
*
*  Func Name  : KeyExtract_Batch_GetResult
*
*  Description: ��ȡ�ؼ���ʶ��Ľ��
*				��Ҫ������KeyExtract_Batch_Complete()֮�󣬲���Ч
*
*  Parameters : bWeightOut���Ƿ���Ҫ���ÿ���ؼ��ʵ�Ȩ�ز���
*
*  Returns    : �����ʽΪ
*				���ؼ���1�� ��Ȩ��1�� ���ؼ���2�� ��Ȩ��2�� ...
*
*  Author     : Kevin Zhang
*  History    :
*              1.create 2013/11/23
*********************************************************************/
KEYEXTRACT_API const char * KeyExtract_Batch_GetResult(bool bWeightOut = false);//����ؼ���ʶ����

#endif//__KEYEXTRACT_2013_LIB_INCLUDED__
