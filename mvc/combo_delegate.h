#ifndef COMBO_DELEGATE_H__
#define COMBO_DELEGATE_H__
#include <QStyledItemDelegate>

class ComboDelegate : public QStyledItemDelegate
{
public:
    ComboDelegate(QObject *pParent = nullptr);
public:
    //
    virtual QWidget *createEditor(QWidget *pParent, const QStyleOptionViewItem &rcOption,
                                  const QModelIndex &rcIndex) const;

    virtual void setEditorData(QWidget *pEditor, const QModelIndex &rcIndex) const;

    virtual void setModelData(QWidget *pEditor, QAbstractItemModel *pModel,
                              const QModelIndex &rcIndex) const;

    virtual void updateEditorGeometry(QWidget *pEditor, const QStyleOptionViewItem &rcOption,
                                      const QModelIndex &rcIndex) const;
};

#endif // COMBO_DELEGATE_H__
