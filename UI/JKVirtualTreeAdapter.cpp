#include "stdafx.h"
#include "JKVirtualTreeAdapter.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include <QModelIndex>

JKVirtualModelAdapter::JKVirtualModelAdapter(BaseObjectConstRefPtr _root)
{
	this->setRoot(_root);
}

JKVirtualModelAdapter::~JKVirtualModelAdapter()
{

}

QVariant JKVirtualModelAdapter::data(BaseObjectConstRefPtr item, int role, const QModelIndex & index)
{
	return data(item, role, index.row(), index.column());
}

bool JKVirtualModelAdapter::setData(BaseObjectConstRefPtr item, const QVariant & value, int role /*= Qt::EditRole*/)
{
	return false;
}

Qt::ItemFlags JKVirtualModelAdapter::flags(const QModelIndex & index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int JKVirtualModelAdapter::indexOf(BaseObjectConstRefPtr parent, BaseObjectConstRefPtr item, int start)
{
	int count = getChildItemsCount(parent);
	for (int i = start; i < count; ++i)
	{
		if (getChildItem(parent, i) == item)
			return i;
	}
	return -1;
}

bool JKVirtualModelAdapter::hasItems(BaseObjectConstRefPtr parent)
{
  return getChildItemsCount(parent) > 0;
}

BaseObjectPtr JKVirtualModelAdapter::getItemParent(BaseObjectConstRefPtr item)
{
  return item;
}

void JKVirtualModelAdapter::beginUpdate()
{
  if (m_modelIntf)
    m_modelIntf->beginUpdate();
}

void JKVirtualModelAdapter::endUpdate()
{
  if (m_modelIntf)
    m_modelIntf->endUpdate();
}

void JKVirtualModelAdapter::QueuedUpdate()
{
  if (m_modelIntf)
    m_modelIntf->QueuedUpdate();
}

void JKVirtualModelAdapter::setRoot(BaseObjectConstRefPtr _root)
{
	if (root != _root)
		RootChanged = true;
	root = _root;
}

JKVirtualModelInterface::~JKVirtualModelInterface()
{

}

int JKVirtualModelStubAdapter::getChildItemsCount(void *parent)
{
  Q_UNUSED(parent)
  return 0;
}

void *JKVirtualModelStubAdapter::getChildItem(void *parent, int index)
{
  Q_UNUSED(parent)
  Q_UNUSED(index)
  return nullptr;
}

QVariant JKVirtualModelStubAdapter::data(void *item, int role)
{
  Q_UNUSED(item)
  Q_UNUSED(role)
  return QVariant();
}
