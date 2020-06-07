#include <QMenu>

#include "tree_view.h"
#include "tree_model.h"

TreeView::TreeView(QWidget *pwgt) : QTreeView(pwgt)
{
    m_pactAddRow = new QAction("&Добавить строку", nullptr);
    m_pactInsertRow = new QAction("&Вставить строку", nullptr);
    m_pactInsertChildRow = new QAction("&Вставить дочернюю строку", nullptr);
    m_pactDeleteSelectedRows = new QAction("&Удалить выделенные строки", nullptr);
    m_pactMoveSelectedRowsFront = new QAction("&Переместить выделенные строки", nullptr);

    m_pContextMenu = new QMenu(this);
    m_pContextMenu->addAction(m_pactAddRow);
    m_pContextMenu->addAction(m_pactInsertRow);
    m_pContextMenu->addAction(m_pactInsertChildRow);
    m_pContextMenu->addAction(m_pactDeleteSelectedRows);
    m_pContextMenu->addAction(m_pactMoveSelectedRowsFront);

    connect(m_pContextMenu, SIGNAL(triggered(QAction*)), SLOT(sEmitContextMenuAction(QAction*)));
}
void TreeView::contextMenuEvent(QContextMenuEvent*)
{
    m_pContextMenu->exec(QCursor::pos());
}
//-------------------------------------------------------------------------------------------------
void TreeView::sEmitContextMenuAction(QAction* pact)
{
    if(pact == m_pactAddRow)
        emit addRow();
    else if(pact == m_pactInsertRow)
        emit insertRow();
    else if(pact == m_pactInsertChildRow)
        emit insertChildRow();
    else if(pact == m_pactDeleteSelectedRows)
        emit deleteSelectedRows();
    else if(pact == m_pactMoveSelectedRowsFront)
        emit moveSelectedRowsFront();
}
//-------------------------------------------------------------------------------------------------
