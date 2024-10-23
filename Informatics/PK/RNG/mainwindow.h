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

    void                generateData            ();
    void                generateBlum            ();
    void                generateSeed            ();
    void                encrypt                 ();

    void                changeBlum              ();
    void                runTests                ();
    void                visualize               ();

    void                saveFile                ();
    void                saveAsFile              ();
    void                loadFile                ();

    void                saveAsPlainText         ();
    void                loadPlainText           ();

    QString             savedFileName;

    quint32             seed, bitLength = 0u;
    quint64             blum = 21u;

    bool                bText = true, bVis = true;

    QByteArray          bitData, plainText, plainBitText, cypher, cypherBit;
};
