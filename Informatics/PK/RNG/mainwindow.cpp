#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <bitset>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->bitSpin     ->setMaximum(std::numeric_limits<int>::max());
    ui->seedSpin    ->setMaximum(std::numeric_limits<int>::max());

    //Zarządzanie plikami
    ui->actionLoad  ->setShortcut(QKeySequence(tr("Ctrl+O")));
    ui->actionSave  ->setShortcut(QKeySequence(tr("Ctrl+S")));
    ui->actionSaveAs->setShortcut(QKeySequence(tr("Ctrl+Shift+S")));

    connect(ui->actionLoad,     &QAction::triggered,                this, &MainWindow::loadFile);
    connect(ui->actionSave,     &QAction::triggered,                this, &MainWindow::saveFile);
    connect(ui->actionSaveAs,   &QAction::triggered,                this, &MainWindow::saveAsFile);
    connect(ui->actionLoadText, &QAction::triggered,                this, &MainWindow::loadPlainText);
    connect(ui->actionSaveAsText,&QAction::triggered,               this, &MainWindow::saveAsPlainText);
    connect(ui->bitGenerate,    &QPushButton::clicked,              this, &MainWindow::generateData);
    connect(ui->blumGenerate,   &QPushButton::clicked,              this, &MainWindow::generateBlum);
    connect(ui->seedGenerate,   &QPushButton::clicked,              this, &MainWindow::generateSeed);
    connect(ui->blumSpin,       &BigSpin::valueChanged,             this, &MainWindow::changeBlum);
    connect(ui->seedSpin,       &QSpinBox::valueChanged,            this, [this](int value)     { this->seed        = value; });
    connect(ui->bitSpin,        &QSpinBox::valueChanged,            this, [this](int value)     { this->bitLength   = value; });
    connect(ui->visCheck,       &QCheckBox::toggled,                this, [this](bool toggle)   { this->bVis        = toggle; this->ui->visTable    ->setEnabled(toggle); visualize(); });
    connect(ui->showCheck,      &QCheckBox::toggled,                this, [this](bool toggle)   { this->bText       = toggle; this->ui->resultsText ->setEnabled(toggle); });
    connect(ui->testButton,     &QPushButton::clicked,              this, &MainWindow::runTests);
    connect(ui->cypherButton,   &QPushButton::clicked,              this, &MainWindow::encrypt);
}

void MainWindow::visualize()
{
    ui->visTable->clear();
    ui->visTable->setRowCount(0);
    ui->visTable->setColumnCount(0);
    if (!bVis)
        return;

    bool        bCypher     = cypherBit.size();
    quint32     bitLength   = (bCypher ? cypherBit.size() : bitData.size()),
                maxRows     = ceil(sqrt(bitLength / 2u)),
                maxColumns  = maxRows * 2u;
    ui->visTable->setRowCount(maxRows);
    ui->visTable->setColumnCount(maxColumns);
    quint32    row     = 0u,   col     = 0u;

    for (quint32 i = 0u; i != bitLength; ++i)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem("");
        if ((bCypher ? cypherBit[i] : bitData[i]) == '1')
            newItem->setBackground(QBrush(QColor(255,0,0)));
        else
            newItem->setBackground(QBrush(QColor(0,0,0)));
        ui->visTable->setItem(row, col, newItem);
        if (++col == maxColumns)
        {
            col = 0u;
            ++row;
        }
    }
}

void MainWindow::saveFile()
{
    if (savedFileName == "")
        return;

    QFile file(savedFileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Nie udało się otworzyć pliku"),
            file.errorString());

        return;
    }

    QDataStream out(&file);
    out.writeRawData(bitData, bitData.size());
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Zapisz plik jako"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    savedFileName = fileName;
    saveFile();
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Wczytaj plik"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Nie udało się otworzyć pliku"),
            file.errorString());

        return;
    }

    bitData         = file.readAll();
    bitLength       = bitData.size();
    ui->bitSpin     ->setValue(bitLength);
    if (bText)
        ui->resultsText->setPlainText(bitData);
    visualize();
    savedFileName   = fileName;
}

void MainWindow::saveAsPlainText()
{
    if (!cypher.size())
        return;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Zapisz plik jako"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Nie udało się otworzyć pliku"),
            file.errorString());

        return;
    }

    QDataStream out(&file);
    out.writeRawData(cypher, cypher.size());
}

void MainWindow::loadPlainText()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Wczytaj plik"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Nie udało się otworzyć pliku"),
            file.errorString());

        return;
    }

    cypher.clear();
    cypherBit.clear();
    ui->plainText->setPlainText(cypher);
    ui->plainText->setPlainText(cypherBit);
    plainText       = file.readAll();
    plainBitText.clear();
    for (quint32 i = 0u; i != plainText.size(); ++i)
        plainBitText.append(std::bitset<8>(plainText.at(i)).to_string().c_str());

    if (bText)
        ui->plainText->setPlainText(plainText);
    if (bVis)
        visualize();
}

void MainWindow::changeBlum()
{
    ui->pEdit->setText("?????");
    ui->qEdit->setText("?????");
    blum = ui->blumSpin->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}
