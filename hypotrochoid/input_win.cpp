#include "input_win.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QPushButton>

InputDialog::InputDialog(QWidget *pParent) : QDialog(pParent)
{
    setWindowTitle("Входные данные: ");
    resize(200, 200);

    QLabel *plbl_R = new QLabel("R ");
    QLabel *plbl_r = new QLabel("r ");
    QLabel *plbl_d = new QLabel("d ");
    QLabel *plbl_th = new QLabel("th ");

    m_pLine_R = new QLineEdit;
    m_pLine_r = new QLineEdit;
    m_pLine_d = new QLineEdit;
    m_pLine_th = new QLineEdit;

    QDoubleValidator *pdblValidator_R = new QDoubleValidator;
    QDoubleValidator *pdblValidator_r = new QDoubleValidator;
    QDoubleValidator *pdblValidator_d = new QDoubleValidator;
    QDoubleValidator *pdblValidator_th = new QDoubleValidator;
    m_pLine_R->setValidator(pdblValidator_R);
    m_pLine_r->setValidator(pdblValidator_r);
    m_pLine_d->setValidator(pdblValidator_d);
    m_pLine_th->setValidator(pdblValidator_th);

    QPushButton *pOk = new QPushButton("Ok");
    QPushButton *pCancel = new QPushButton("Cancel");
    connect(pOk, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(pOk, SIGNAL(clicked(bool)), this, SLOT(sInputDataChanged()));
    connect(pCancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    QGridLayout *pgrdLayout = new QGridLayout;
    pgrdLayout->addWidget(plbl_R, 0, 0);
    pgrdLayout->addWidget(plbl_r, 1, 0);
    pgrdLayout->addWidget(plbl_d, 2, 0);
    pgrdLayout->addWidget(plbl_th, 3, 0);
    pgrdLayout->addWidget(pOk, 4, 0);
    pgrdLayout->addWidget(m_pLine_R, 0, 1);
    pgrdLayout->addWidget(m_pLine_r, 1, 1);
    pgrdLayout->addWidget(m_pLine_d, 2, 1);
    pgrdLayout->addWidget(m_pLine_th, 3, 1);
    pgrdLayout->addWidget(pCancel, 4, 1);

    setLayout(pgrdLayout);
}
InputDialog::~InputDialog(){}
//-------------------------------------------------------------------------------------------------
double InputDialog::Radius() const
{
    return (m_pLine_R->text()).toDouble();
}
double InputDialog::radius() const
{
    return (m_pLine_r->text()).toDouble();
}
double InputDialog::distance() const
{
    return (m_pLine_d->text()).toDouble();
}
double InputDialog::thetaMax() const
{
    return (m_pLine_th->text()).toDouble();
}
//-------------------------------------------------------------------------------------------------
void InputDialog::sInputDataChanged(){ /* Ok clicked */ emit dataChanged(); }
//-------------------------------------------------------------------------------------------------

