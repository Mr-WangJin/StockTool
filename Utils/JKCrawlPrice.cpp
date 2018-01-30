#include "stdafx.h"
#include "JKCrawlPrice.h"
#include "BLL/JKStockCodeBLL.h"
#include "WebRequest\JKRequestWebData.h"
#include <iostream>
#include "Utils\XMLParser\JKParserHtmlData.h"
#include "Utils\XMLParser\JKTagTextContext.h"
#include "Utils\TransferData\JKWebData.h"
#include "JKUtil\JKStringUtil.h"

//bool requestStockPrice(JKCrawlPrice* pCrawlPrice, JKRef_Ptr<JKStockCodeBLL> refStockCode)
//{
//	refStockCode->setLatestPrice(refStockCode->getLatestPrice() + .01);
//	pCrawlPrice->stockCodeChanged(refStockCode);
//
//	Py_Initialize();
//
//
//	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��
//	string path = "D:/Programs/JK/JKPython/scrapy/tutorial/";
//	string chdir_cmd = string("sys.path.append('") + path + "')";
//	const char* cstr_cmd = chdir_cmd.c_str();
//	PyRun_SimpleString("import sys");
//	PyRun_SimpleString(cstr_cmd);
//	
//	// ����ģ��
//	PyObject* moduleName = PyString_FromString("runCrawlStockPrice"); //ģ�����������ļ���
//	PyObject* pModule = PyImport_Import(moduleName);
//	if (!pModule) // ����ģ��ʧ��
//	{
//		cout << "[ERROR] Python get module failed." << endl;
//		return false;
//	}
//	cout << "[INFO] Python get module succeed." << endl;
//	
//	// ���غ���
//	PyObject* pv = PyObject_GetAttrString(pModule, "crawlPrice");
//	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
//	{
//		cout << "[ERROR] Can't find funftion (crawlPrice)" << endl;
//		return false;
//	}
//	cout << "[INFO] Get function (test_add) succeed." << endl;
//	
//	// ���ò���
//	PyObject* args = PyTuple_New(2);   // 2������
//	PyObject* arg1 = PyString_FromString("https://gupiao.baidu.com/stock/sh600100.html");    // ����һ��Ϊ4
//	PyObject* arg2 = PyString_FromString("600100");    // ��������Ϊ3
//	PyTuple_SetItem(args, 0, arg1);
//	PyTuple_SetItem(args, 1, arg2);
//	
//	// ���ú���
//	PyObject* pRet = PyObject_CallObject(pv, args);
//	
//	// ��ȡ����
//	if (pRet)  // ��֤�Ƿ���óɹ�
//	{
//		//PyTupleObject* tupleObj = (PyTupleObject*)(pRet);
//		//int s = tupleObj->ob_size;
//
//		JKString result = PyString_AsString(pRet);
//		cout << "result:" << result;
//		if (result == "1")
//		{
//			JKString data;
//			size_t dataLen = 0;
//			JKString price;
//			if (JKFileIO::ReadFile("D:/Programs/JK/JKPython/scrapy/tutorial/600100", JKFileIO::Read, data, dataLen))
//			{
//				Json::Reader reader;
//				Json::Value result;
//				reader.parse(data, result);
//				Json::Value p = result["price"];
//				if (p.type() == Json::ValueType::arrayValue)
//				{
//					for (int i = 0; i < p.size(); ++i)
//						price = p[i].asString();
//				}
//				
//			}
//		}
//	}
//	
//	
//	PyRun_SimpleString("print 'hello'");
//
//	Py_Finalize();
//
//
//	return true;
//
//}

QString getUrlByCode(QString code)
{
	if (code.isEmpty())
		return "";
	if (code[0] == '0')
		return QString("https://gupiao.baidu.com/stock/sz%1.html").arg(code);
	else 
		return QString("https://gupiao.baidu.com/stock/sh%1.html").arg(code);
}

void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice)
{
	//JKCrawlPrice* pCrawlPrice = (JKCrawlPrice*)ptr;

	while (true)
	{
		pCrawlPrice->mtxRunCraw_Mtx.lock();
		std::unique_lock<std::mutex> uLck(pCrawlPrice->mtxRunCraw);
		pCrawlPrice->mtxRunCraw_Mtx.unlock();

		if (pCrawlPrice->getIsDelete())
			break;

		const std::list<JKRef_Ptr<JKStockCodeBLL>> listStockCodes = pCrawlPrice->getStockCodes();
		for (auto var : listStockCodes)
		{
			JKRequestWebData requestWebData;
			QString url = getUrlByCode(QString::fromStdString(var->getCode()));
			JKHtmlData* htmlData = requestWebData.getHtmlData(url.toStdString());
			if (htmlData == nullptr)
			{
				std::cout << "request error ! " << url.toStdString();
				continue;
			}

#ifdef _DEBUG
			JKString strHtmlData = JKString(htmlData->getMemory());
			strHtmlData = JKStringUtil::UTF8ToANSI(strHtmlData);
#endif // _DEBUG

			JKTagTextContext tagTextContext("strong");
			if (JKParserHtmlData::parserTagTextAttribute(htmlData, &tagTextContext))
			{
				const std::list<JKString> tagText = tagTextContext.getListTagText();
				std::cout << "parser tag " << tagTextContext.getTag() << " size : " << tagText.size() << std::endl;
				if (tagText.size() > 0)
				{
					JKString s = *(tagText.begin());
					pCrawlPrice->fire(s);
				}
			}

			JK_FreeAndNullptr(htmlData);
		}
		pause_thread(1000);
	}
}


JKCrawlPrice::JKCrawlPrice(QObject* parent/* = nullptr*/)
	: QObject(parent)
{
	varIsDelete = new JKVariableRWMtx<bool, std::mutex>(false);
	bIsStartCraw = true;
	this->stopRunCraw();

	threadCrawler[0] = std::thread(runCrawlPriceThread, this);

	//QTimer *m_timer;
	//m_timer = new QTimer(this);
	//connect(m_timer, SIGNAL(timeout()), this, SLOT(reCrawlPrice()));
	//m_timer->start(4 * 1000); //every 2 minutes 
}

JKCrawlPrice::~JKCrawlPrice()
{
	this->setIsDelete(true);
	if (!bIsStartCraw)
		this->startRunCraw();
	threadCrawler[0].join();
	JK_FreeAndNullptr(varIsDelete);
}

const std::list<JKRef_Ptr<JKStockCodeBLL>>& JKCrawlPrice::getStockCodes()
{
	std::unique_lock<std::mutex> ulck(mtxListStockCode);
	return listStockCode;
}

void JKCrawlPrice::addStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (_refStockCode.valid() == false)
		return;
	std::unique_lock<std::mutex> ulck(mtxListStockCode);

	if (std::find(listStockCode.begin(), listStockCode.end(), _refStockCode) == listStockCode.end())
	{
		listStockCode.push_back(_refStockCode);
	}
}

void JKCrawlPrice::removeStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	std::unique_lock<std::mutex> ulck(mtxListStockCode);

	std::list<JKRef_Ptr<JKStockCodeBLL>>::iterator iter = std::find(listStockCode.begin(), listStockCode.end(), _refStockCode);
	if (iter != listStockCode.end())
	{
		listStockCode.remove(_refStockCode);
	}
}

void JKCrawlPrice::clearStoclCode()
{
	std::unique_lock<std::mutex> ulck(mtxListStockCode);

	listStockCode.clear();
}


void JKCrawlPrice::startRunCraw()
{
	if (!bIsStartCraw)
		mtxRunCraw.unlock();

	bIsStartCraw = true;
	//std::unique_lock<std::mutex> uLck(mtxRunCraw_Mtx);
}

void JKCrawlPrice::stopRunCraw()
{
	if (bIsStartCraw)
	{
		std::unique_lock<std::mutex> uLck(mtxRunCraw_Mtx);
		mtxRunCraw.lock();
	}
	bIsStartCraw = false;
}

bool JKCrawlPrice::getIsDelete() 
{
	return varIsDelete->getVariable();
}

void JKCrawlPrice::setIsDelete(const bool & isDelete)
{
	varIsDelete->setVariable(isDelete);
}

void JKCrawlPrice::fire(JKString t)
{
	emit stockCodePriceChanged(t);
}

void JKCrawlPrice::HandleEvent(const JKCustomUIEvent* event)
{

}


void JKCrawlPrice::initCrawler()
{
}