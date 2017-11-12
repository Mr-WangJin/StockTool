#pragma once

#include <thread>
#include <mutex>
#include <QObject>
#include "JKEventType.h"
#include "ThreadUtil\JKThreadUtil.h"

class JKStockCodeBLL;
class JKCrawlPrice;
extern void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice);

class JKCrawlPrice : public QObject, public JKUiEventHandler
{
	friend void runCrawlPriceThread(JKCrawlPrice*);
	Q_OBJECT
public:
	JKCrawlPrice(QObject* parent = nullptr);
	~JKCrawlPrice();

	void addStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	void removeStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

	void startRunCraw();
	void stopRunCraw();

	bool getIsDelete();
	void setIsDelete(const bool &);

	virtual void HandleEvent(const JKCustomUIEvent* event) override;

signals:
	//void stockCodePriceChanged(JKRef_Ptr<JKStockCodeBLL>);

private:
	void initCrawler();


private:
	std::thread threadCrawler[1];

	JKVariableMtx<bool, std::mutex>* varIsDelete;

	std::mutex mtxRunCraw;
	std::mutex mtxRunCraw_Mtx;

	std::list<JKRef_Ptr<JKStockCodeBLL>> listStockCode;
};

