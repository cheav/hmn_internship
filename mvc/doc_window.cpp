#include "doc_window.h"
#include "combo_delegate.h"
#include "tree_model.h"
#include "tree_view.h"
#include "tree_item.h"
#include <QDebug>

DocWindowTree::DocWindowTree(QWidget *pParent) : QWidget(pParent)
{
    setMinimumSize(450, 450);
    //---------------------------------------------------------------------------------------------
    QStringList headers;
    headers << tr("String") << tr("Number") << tr("Bool") << tr("Enum");
    QString data = tr("item1") + "\n" + tr("item2") + "\n" + tr("item3");

    m_pTreeModel = new TreeModel(headers, data);
    m_pTreeView = new TreeView(this);
    m_pTreeView->setMinimumSize(size());
    ComboDelegate *pDelegate = new ComboDelegate(m_pTreeModel);
    m_pTreeView->setItemDelegateForColumn(3, pDelegate);
    m_pTreeView->setModel(m_pTreeModel);

    m_pTreeModel->insertRows(0, 3, m_pTreeModel->index(0, 0));
    m_pTreeModel->insertRows(0, 3, m_pTreeModel->index(1, 0));
    m_pTreeModel->insertRows(0, 3, m_pTreeModel->index(2, 0));

    QModelIndex index;
    for(int j = 0; j < 3; ++j)
        for(int i = 0; i < 3; ++i)
        {
            // set strings
            index = m_pTreeModel->index(i, 0, m_pTreeModel->index(j, 0));
            QString str("string_" + QString::number(i));
            m_pTreeModel->setData(index, str);

            // set ints
            index = m_pTreeModel->index(i, 1, m_pTreeModel->index(j, 0));
            m_pTreeModel->setData(index, i);

            // set bools
            index = m_pTreeModel->index(i, 2, m_pTreeModel->index(j, 0));
            m_pTreeModel->setData(index, (bool)i);

            // set enums
            index = m_pTreeModel->index(i, 3, m_pTreeModel->index(j, 0));
            m_pTreeModel->setData(index, (size_t)i);
        }

    m_pTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_pTreeView->setDragEnabled(true);
    m_pTreeView->setAcceptDrops(true);
    m_pTreeView->setDropIndicatorShown(true);
    m_pTreeView->setSortingEnabled(true);
    m_pTreeView->sortByColumn(0, Qt::AscendingOrder);

    // context menu:
    connect(m_pTreeView, SIGNAL(addRow()), SLOT(on_actionAddRow_triggered()));
    connect(m_pTreeView, SIGNAL(insertRow()), SLOT(on_actionInsertRow_triggered()));
    connect(m_pTreeView, SIGNAL(insertChildRow()), SLOT(on_actionInsertChildRow_triggered()));
    connect(m_pTreeView, SIGNAL(deleteSelectedRows()), SLOT(on_actionDeleteSelectedRows_triggered()));
    connect(m_pTreeView, SIGNAL(moveSelectedRowsFront()), SLOT(on_actionMoveSelectedRowsFront_triggered()));
}
//-------------------------------------------------------------------------------------------------
void DocWindowTree::on_actionAddRow_triggered()
{
    const int cnRow = m_pTreeModel->rowCount();
    m_pTreeModel->insertRow(cnRow);
    QModelIndex indexNew = m_pTreeModel->index(cnRow, 0);
    m_pTreeView->setCurrentIndex(indexNew);
}
void DocWindowTree::on_actionInsertRow_triggered()
{
    QModelIndex currentIndex = m_pTreeView->currentIndex();
    const int cnCurrentRow = currentIndex.row();
    QModelIndex parentIndex = getParent_of_SelectedIndexes();
    m_pTreeModel->insertRow(cnCurrentRow + 1, parentIndex);
}
void DocWindowTree::on_actionInsertChildRow_triggered()
{
    QModelIndex parentIndex = m_pTreeView->currentIndex();
    m_pTreeModel->insertRows(0, 1, parentIndex);
}
void DocWindowTree::on_actionDeleteSelectedRows_triggered()
{
    IntSet rows;
    getSelectedRows(rows);
    //
    int row = *(rows.begin());
    QModelIndex parent = getParent_of_SelectedIndexes();
    m_pTreeModel->removeRows(row, rows.size(), parent);
}
void DocWindowTree::on_actionMoveSelectedRowsFront_triggered()
{
    IntSet rows;
    getSelectedRows(rows);
    //
    QModelIndex sourceParent = getParent_of_SelectedIndexes();
    //
    IntSet::iterator i = rows.begin(), e = rows.end();
    for (int n = 0; i != e; ++i, ++n)
    {
        bool bMoved = m_pTreeModel->moveRow(sourceParent, *i, sourceParent, n);
        if(!bMoved)
            qDebug() << "not moved ";
    }
}
void DocWindowTree::getSelectedRows(IntSet &rRows)
{
    QModelIndexList indexes = m_pTreeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, indexes)
        rRows.insert(index.row());
}
QModelIndex DocWindowTree::getParent_of_SelectedIndexes()
{
    QModelIndexList indexes = m_pTreeView->selectionModel()->selectedIndexes();
    return indexes[0].parent();
}

//-------------------------------------------------------------------------------------------------
