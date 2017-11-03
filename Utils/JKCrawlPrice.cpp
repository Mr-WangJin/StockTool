#include "stdafx.h"
#include "JKCrawlPrice.h"
#include <future>
#include <JKThread/JKThreadPool.h>
#include "BLL/JKProjectBLL.h"
#include <iostream>

#include <Python.h>


void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended\n";
}



void requestStockPrice(JKCrawlPrice* pCrawlPrice, JKRef_Ptr<JKStockCodeBLL> refStockCode)
{
	refStockCode->setLatestPrice(refStockCode->getLatestPrice() + .01);
	pCrawlPrice->stockCodeChanged(refStockCode);
}

void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice)
{
	while (pCrawlPrice->isContinue())
	{
		std::unique_lock<mutex> ulck(pCrawlPrice->mtxProjectChanged);

		if (!pCrawlPrice->refProject.valid())
			pause_thread(2);

		vector<JKRef_Ptr<JKStockCodeBLL>> vecStockCode = pCrawlPrice->refProject->getAllStockCode();
		for (auto &var : vecStockCode)
		{
			JKSingleton<JKThreadPool>::GetInstance().enqueue(requestStockPrice, pCrawlPrice, var);
		}
		pause_thread(5);
	}

	
}


JKCrawlPrice::JKCrawlPrice(JKRef_Ptr<JKProjectBLL> _refProject, QObject* parent/* = nullptr*/)
	: QObject(parent)
	, refProject(_refProject)
{
	JKSingleton<JKThreadPool>::GetInstance().enqueue(runCrawlPriceThread, this);

}

JKCrawlPrice::~JKCrawlPrice()
{
}

void JKCrawlPrice::stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> refStockCode)
{
	emit stockCodePriceChanged(refStockCode);

	std::cout << refStockCode->getLatestPrice() << std::endl;
}

bool JKCrawlPrice::isContinue()
{

}

void JKCrawlPrice::HandleEvent(const JKCustomUIEvent* event)
{

}

void JKCrawlPrice::beforeProjectChanged()
{
	std::unique_lock<mutex> ulck(mtxProjectChanged);
	
	refProject = nullptr;

}

void JKCrawlPrice::afterProjectChanged(JKRef_Ptr<JKProjectBLL> _refProject)
{

}

void JKCrawlPrice::initCrawler()
{
}


//
//Py_Initialize();
//
//	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性
//	string path = "D:/projects/temp/Project1/Project1/test/";
//	string chdir_cmd = string("sys.path.append('") + path + "')";
//	const char* cstr_cmd = chdir_cmd.c_str();
//	PyRun_SimpleString("import sys");
//	PyRun_SimpleString(cstr_cmd);
//
//	// 加载模块
//	PyObject* moduleName = PyString_FromString("test_aa"); //模块名，不是文件名
//	PyObject* pModule = PyImport_Import(moduleName);
//	if (!pModule) // 加载模块失败
//	{
//		cout << "[ERROR] Python get module failed." << endl;
//		return 0;
//	}
//	cout << "[INFO] Python get module succeed." << endl;
//
//	// 加载函数
//	PyObject* pv = PyObject_GetAttrString(pModule, "test_add");
//	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
//	{
//		cout << "[ERROR] Can't find funftion (test_add)" << endl;
//		return 0;
//	}
//	cout << "[INFO] Get function (test_add) succeed." << endl;
//
//	// 设置参数
//	PyObject* args = PyTuple_New(2);   // 2个参数
//	PyObject* arg1 = PyInt_FromLong(4);    // 参数一设为4
//	PyObject* arg2 = PyInt_FromLong(3);    // 参数二设为3
//	PyTuple_SetItem(args, 0, arg1);
//	PyTuple_SetItem(args, 1, arg2);
//
//	// 调用函数
//	PyObject* pRet = PyObject_CallObject(pv, args);
//
//	// 获取参数
//	if (pRet)  // 验证是否调用成功
//	{
//		long result = PyInt_AsLong(pRet);
//		cout << "result:" << result;
//	}
//
//
//	PyRun_SimpleString("print 'hello'");
//	Py_Finalize();
//
//	return 0;