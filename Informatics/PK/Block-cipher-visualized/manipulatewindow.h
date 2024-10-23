#pragma once
#include <QWidget>
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class manipulateWindow; }
QT_END_NAMESPACE

class ManipulateWindow : public QWidget
{
    Q_OBJECT
    public:
        ManipulateWindow(QPlainTextEdit *firstText, QRadioButton *hexRadio, QWidget *parent = nullptr);
        void showBlock(unsigned blockId);
        ~ManipulateWindow();

        Ui::manipulateWindow *ui;
private:
        virtual void showEvent(QShowEvent *e);
        virtual void closeEvent(QCloseEvent *e);

        QPlainTextEdit *firstText = nullptr;
        QRadioButton *hexRadio = nullptr;
        unsigned blockId;

        QValidator *hexStringValidator;
};
