#ifndef INPUT_WIN_H__
#define INPUT_WIN_H__
#include <QDialog>
class QLineEdit;

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    InputDialog(QWidget *pParent = nullptr);
    ~InputDialog();
    double Radius() const;
    double radius() const;
    double distance() const;
    double thetaMax() const;
signals:
    void dataChanged();
private slots:
    void sInputDataChanged();
private:
    QLineEdit *m_pLine_R;
    QLineEdit *m_pLine_r;
    QLineEdit *m_pLine_d;
    QLineEdit *m_pLine_th;
};

#endif // INPUT_WIN_H__
