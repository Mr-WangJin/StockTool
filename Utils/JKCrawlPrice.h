#pragma once

#include <thread>
#include <mutex>
#include <QObject>
#include "JKEventType.h"
#include "BLL/JKStockCodeBLL.h"

class JKStockCodeBLL;
class JKCrawlPrice;
class JKProjectBLL;


void requestStockPrice(JKCrawlPrice* pCrawlPrice, JKRef_Ptr<JKStockCodeBLL> refStockCode);

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

signals:
	void stockCodePriceChanged(JKRef_Ptr<JKStockCodeBLL>);

private:
	void initCrawler();


private:
	//std::thread threadCrawler;

	JKRef_Ptr<JKProjectBLL> refProject;

	friend void runCrawlPriceThread(JKCrawlPrice* pCrawlPrice);

	std::mutex mtxProjectChanged;
};

