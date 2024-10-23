#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QRegularExpressionValidator>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QRandomGenerator>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QRegularExpressionValidator>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/integer/mod_inverse.hpp>
#include <boost/endian/arithmetic.hpp>

#include <sha256.h>

using boost::multiprecision::int512_t;
using boost::multiprecision::int1024_t;
using boost::lexical_cast;

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

    void                generate_pq             (int512_t   *p,     int512_t    *q);
    void                generate_e              (int1024_t  phi,    int1024_t   *e);
    void                generate_d              (int1024_t  phi,    int1024_t   e,      int1024_t   *d);
    void                crypt                   ();
    void                sign                    ();
    void                verify                  ();

    void                saveFile                (QByteArray array,  QString     fileName);
    void                saveAsFile              (QByteArray array);
    bool                loadFile                (QByteArray *array);
    QByteArray          rsa                     (QByteArray in,     bool        bMode,   bool       bLoad,    int1024_t   e,      int1024_t   d,      int1024_t   n,      bool    bAppend = true);



    int1024_t           e, d, n, phi, receiver_e, receiver_d, receiver_n, receiver_phi;

    bool                bText = true, bTextChanged = false, bCypher = true, bHash = true;

    QByteArray          text1Data, text2Data, signature;
};
