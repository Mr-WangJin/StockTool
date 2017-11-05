#include "stdafx.h"
#include "JKCrawlPrice.h"
#include <future>
#include <JKThread/JKThreadPool.h>
#include <iostream>
#include <qtimer.h>
#include <qdatetime.h>
#include <JKFile/JKFileIO.h>
#include <json\reader.h>

#include <Python.h>


void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended ----------------" << std::this_thread::get_id << std::endl;
}



bool requestStockPrice(JKCrawlPrice* pCrawlPrice, JKRef_Ptr<JKStockCodeBLL> refStockCode)
{
	refStockCode->setLatestPrice(refStockCode->getLatestPrice() + .01);
	pCrawlPrice->stockCodeChanged(refStockCode);

	Py_Initialize();


	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��
	string path = "D:/Programs/JK/JKPython/scrapy/tutorial/";
	string chdir_cmd = string("sys.path.append('") + path + "')";
	const char* cstr_cmd = chdir_cmd.c_str();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString(cstr_cmd);
	
	// ����ģ��
	PyObject* moduleName = PyString_FromString("runCrawlStockPrice"); //ģ�����������ļ���
	PyObject* pModule = PyImport_Import(moduleName);
	if (!pModule) // ����ģ��ʧ��
	{
		cout << "[ERROR] Python get module failed." << endl;
		return false;
	}
	cout << "[INFO] Python get module succeed." << endl;
	
	// ���غ���
	PyObject* pv = PyObject_GetAttrString(pModule, "crawlPrice");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
		cout << "[ERROR] Can't find funftion (crawlPrice)" << endl;
		return false;
	}
	cout << "[INFO] Get function (test_add) succeed." << endl;
	
	// ���ò���
	PyObject* args = PyTuple_New(2);   // 2������
	PyObject* arg1 = PyString_FromString("https://gupiao.baidu.com/stock/sh600100.html");    // ����һ��Ϊ4
	PyObject* arg2 = PyString_FromString("600100");    // ��������Ϊ3
	PyTuple_SetItem(args, 0, arg1);
	PyTuple_SetItem(args, 1, arg2);
	
	// ���ú���
	PyObject* pRet = PyObject_CallObject(pv, args);
	
	// ��ȡ����
	if (pRet)  // ��֤�Ƿ���óɹ�
	{
		//PyTupleObject* tupleObj = (PyTupleObject*)(pRet);
		//int s = tupleObj->ob_size;

		JKString result = PyString_AsString(pRet);
		cout << "result:" << result;
		if (result == "1")
		{
			JKString data;
			size_t dataLen = 0;
			JKString price;
			if (JKFileIO::ReadFile("D:/Programs/JK/JKPython/scrapy/tutorial/600100", JKFileIO::Read, data, dataLen))
			{
				Json::Reader reader;
				Json::Value result;
				reader.parse(data, result);
				Json::Value p = result["price"];
				if (p.type() == Json::ValueType::arrayValue)
				{
					for (int i = 0; i < p.size(); ++i)
						price = p[i].asString();
				}
				
			}
		}
	}
	
	
	PyRun_SimpleString("print 'hello'");

	Py_Finalize();


	return true;

}

void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice)
{
	//while (true)
	{
		pCrawlPrice->mtxReadProject.lock();

		if (!pCrawlPrice->refProject.valid())
		{
			pCrawlPrice->mtxReadProject.unlock();
			//pause_thread(2);
			return;
		}
		vector<JKRef_Ptr<JKStockCodeBLL>> vecStockCode = pCrawlPrice->refProject->getAllStockCode();
		for (auto &var : vecStockCode)
		{
			//JKSingleton<JKThreadPool>::GetInstance().enqueue(requestStockPrice, pCrawlPrice, var);
			requestStockPrice(pCrawlPrice, var);
		}

		pCrawlPrice->mtxReadProject.unlock();
		//pause_thread(2);
	}
}


JKCrawlPrice::JKCrawlPrice(JKRef_Ptr<JKProjectBLL> _refProject, QObject* parent/* = nullptr*/)
	: QObject(parent)
	, refProject(_refProject)
{
	//threadCrawler[0] = std::thread(runCrawlPriceThread, this);

	QTimer *m_timer;

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(reCrawlPrice()));
	m_timer->start(4 * 1000); //every 2 minutes 


	//runCrawlPriceThread(this);

	//JKSingleton<JKThreadPool>::GetInstance().enqueue(runCrawlPriceThread, this);

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
	std::unique_lock<mutex> ulck(mtxReadProject);

	return refProject.valid();
}

void JKCrawlPrice::HandleEvent(const JKCustomUIEvent* event)
{

}

void JKCrawlPrice::beforeProjectChanged()
{
	std::unique_lock<mutex> ulckReadProject(mtxReadProject);
	
	refProject = nullptr;
}

void JKCrawlPrice::afterProjectChanged(JKRef_Ptr<JKProjectBLL> _refProject)
{
	std::unique_lock<mutex> ulckReadProject(mtxReadProject);
	refProject = _refProject;
}

void JKCrawlPrice::reCrawlPrice()
{
	runCrawlPriceThread(this);
}

void JKCrawlPrice::initCrawler()
{
}