#include "stdafx.h"
#include "JKVirtualTreeAdapter.h"

int JKVirtualModelAdapter::indexOf(void *parent, void *item, int start)
{
  int count = getItemsCount(parent);
  for (int i = start; i < count; ++i)
    if (getItem(parent, i) == item)
      return i;
  return -1;
}

bool JKVirtualModelAdapter::hasItems(void *parent)
{
  return getItemsCount(parent) > 0;
}

void *JKVirtualModelAdapter::getItemParent(void *item)
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
