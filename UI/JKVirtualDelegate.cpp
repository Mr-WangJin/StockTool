#include "stdafx.h"
#include "JKVirtualDelegate.h"

#include <QComboBox>

ComboDelegate::ComboDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{}

QWidget *ComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	QStringList list;
	list << "工人" << "农民" << "军人" << "律师";

	QComboBox *pEditor = new QComboBox(parent);
	pEditor->addItems(list);
	pEditor->installEventFilter(const_cast<ComboDelegate*>(this));
	return pEditor;

}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString strText = index.model()->data(index).toString();
	QComboBox *pCombox = NULL;
	pCombox = static_cast<QComboBox*>(editor);
	if (pCombox != NULL)
	{
		int nIndex = pCombox->findText(strText);
		pCombox->setCurrentIndex(nIndex);
	}
}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const
{
	QComboBox *pCombox = NULL;
	pCombox = static_cast<QComboBox*>(editor);
	if (pCombox != NULL)
	{
		QString strText = pCombox->currentText();
		model->setData(index, strText);
	}
}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}