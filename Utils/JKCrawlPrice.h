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

	const std::list<JKRef_Ptr<JKStockCodeBLL>> &getStockCodes();
	void addStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	void removeStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	void clearStoclCode();

	void startRunCraw();
	void stopRunCraw();

	bool getIsDelete();
	void setIsDelete(const bool &);

	void fire(JKString t);
	virtual void HandleEvent(const JKCustomUIEvent* event) override;

signals:
	void stockCodePriceChanged(JKString);

private:
	void initCrawler();


private:
	std::thread threadCrawler[1];
	bool bIsStartCraw = false;

	JKVariableRWMtx<bool, std::mutex>* varIsDelete;

	std::mutex mtxRunCraw;
	std::mutex mtxRunCraw_Mtx;

	std::mutex mtxListStockCode;
	std::list<JKRef_Ptr<JKStockCodeBLL>> listStockCode;
};

