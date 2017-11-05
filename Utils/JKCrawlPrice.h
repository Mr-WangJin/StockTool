#pragma once

#include <thread>
#include <mutex>
#include <QObject>
#include "JKEventType.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKProjectBLL.h"

class JKCrawlPrice;


bool requestStockPrice(JKCrawlPrice* pCrawlPrice, JKRef_Ptr<JKStockCodeBLL> refStockCode);

void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice);

class JKCrawlPrice : public QObject, public JKUiEventHandler
{
	Q_OBJECT
public:
	JKCrawlPrice(JKRef_Ptr<JKProjectBLL> _refProject, QObject* parent = nullptr);
	~JKCrawlPrice();

	void stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> refStockCode);
	bool isContinue();

	virtual void HandleEvent(const JKCustomUIEvent* event) override;

	public slots:
	void beforeProjectChanged();
	void afterProjectChanged(JKRef_Ptr<JKProjectBLL> _refProject);

	void reCrawlPrice();

signals:
	void stockCodePriceChanged(JKRef_Ptr<JKStockCodeBLL>);

private:
	void initCrawler();


private:
	std::thread threadCrawler[1];

	JKRef_Ptr<JKProjectBLL> refProject;

	friend void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice);

	std::mutex mtxProjectChanged;
	std::mutex mtxReadProject;
};

