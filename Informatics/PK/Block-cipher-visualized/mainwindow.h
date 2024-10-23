#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QRegularExpressionValidator>
#include <helpwindow.h>
#include <manipulatewindow.h>
#include <QElapsedTimer>
#include "global.h"

void decrypt(quint8 res[16u], quint8 key[16u], blockMode mode, quint8 xorBuf[16u]);
void encrypt(quint8 res[16u], quint8 key[16u], blockMode mode, quint8 xorBuf[16u]);

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    enum validateState
    {
        Invalid,
        Valid
    };

    enum readMode
    {
        Hex,
        Text,
        BASE64
    };

    void                execute         (bool execute = true);

    bool                callAES         ();

    void                validateInput   ();
    void                setupGuiLabels  (bool cypherMode);
    void                manageBlock     (int row, int col);

    void                saveFile        ();
    void                saveAsFile      ();
    void                loadFile        ();

    QString             savedFileName;

    bool                cypherMode      = false; //false - szyfrowanie, true - deszyfrowanie

    QValidator          *alphabetValidator,
                        *alphabetKeyValidator,
                        *hexValidator;

    HelpWindow          help;
    ManipulateWindow    manipulate;
    qint64              elapsedTime = 0;
    QByteArray          rawData,
                        processedRawData;
};
