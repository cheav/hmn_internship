#ifndef DOC_WINDOW_H__
#define DOC_WINDOW_H__

#include <QWidget>
#include <set>

class TreeView;
class TreeModel;
class ComboDelegate;

class DocWindowTree : public QWidget
{
    Q_OBJECT
public:
    DocWindowTree(QWidget *pParent = nullptr);
public slots:
    //
    void on_actionAddRow_triggered();
    void on_actionInsertRow_triggered();
    void on_actionInsertChildRow_triggered();
    void on_actionDeleteSelectedRows_triggered();
    void on_actionMoveSelectedRowsFront_triggered();
    //
private:
    //
    TreeModel *m_pTreeModel;
    TreeView *m_pTreeView;
    //
    typedef std::set <int> IntSet;
    //
    void getSelectedRows(IntSet &rRows);
    QModelIndex getParent_of_SelectedIndexes();
};

#endif // DOC_WINDOW_H__
