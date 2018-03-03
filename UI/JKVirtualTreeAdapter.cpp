#include "stdafx.h"
#include "JKVirtualTreeAdapter.h"

int JKVirtualModelAdapter::indexOf(BaseObjectConstRefPtr parent, BaseObjectConstRefPtr item, int start)
{
  int count = getItemsCount(parent);
  for (int i = start; i < count; ++i)
    if (getItem(parent, i) == item)
      return i;
  return -1;
}

bool JKVirtualModelAdapter::hasItems(BaseObjectConstRefPtr parent)
{
  return getItemsCount(parent) > 0;
}

BaseObjectConstRefPtr JKVirtualModelAdapter::getItemParent(BaseObjectConstRefPtr item)
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


JKVirtualModelInterface::~JKVirtualModelInterface()
{

}


int JKVirtualModelStubAdapter::getItemsCount(void *parent)
{
  Q_UNUSED(parent)
  return 0;
}

void *JKVirtualModelStubAdapter::getItem(void *parent, int index)
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
