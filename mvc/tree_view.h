#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QTreeView>

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget* pwgt = nullptr);
public slots:
    void sEmitContextMenuAction(QAction* pact);
protected:
    void contextMenuEvent(QContextMenuEvent*) override;
private:
    QAction *m_pactAddRow;
    QAction *m_pactInsertRow;
    QAction *m_pactInsertChildRow;
    QAction *m_pactDeleteSelectedRows;
    QAction *m_pactMoveSelectedRowsFront;
    //
    QMenu *m_pContextMenu;
signals:
    void addRow();
    void insertRow();
    void insertChildRow();
    void deleteSelectedRows();
    void moveSelectedRowsFront();
};

#endif // TREE_VIEW_H
