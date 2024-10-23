#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_manipulatewindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), help((ui->setupUi(this), this)), manipulate(ui->firstText, ui->hexRadio, this)
{

    //Zmienia tekst podpisów
    connect(ui->modeCheck,      &QCheckBox::toggled,            this, &MainWindow::setupGuiLabels);

    //Zapewnij możliwość wpisywania tekstu wyłącznie z alfabetu
    hexValidator                = new QRegularExpressionValidator(QRegularExpression(HEX));
    alphabetValidator           = new QRegularExpressionValidator(QRegularExpression(ALPHABET));
    alphabetKeyValidator        = new QRegularExpressionValidator(QRegularExpression(ALPHABET_CHAR));
    connect(ui->firstText,      &QPlainTextEdit::textChanged,   this, &MainWindow::validateInput);
    connect(ui->keyText,        &QLineEdit::textEdited,         this, [this](){execute();});
    connect(ui->modeCheck,      &QCheckBox::toggled,            this, [this](){execute();});
    connect(ui->visCheck,       &QCheckBox::toggled,            this, [this](){execute();});
    connect(ui->ecbRadio,       &QRadioButton::toggled,         this, &MainWindow::execute);
    connect(ui->cbcRadio,       &QRadioButton::toggled,         this, &MainWindow::execute);
    connect(ui->cfbRadio,       &QRadioButton::toggled,         this, &MainWindow::execute);
    connect(ui->hexRadio,       &QRadioButton::toggled,         this, [this](bool toggled)
    {
        if (toggled)
        {
            ui->firstText->setReadOnly(false);
            ui->firstText->disconnect();
            ui->firstText->setPlainText(rawData.toHex().toUpper());
            connect(ui->firstText,      &QPlainTextEdit::textChanged,   this, &MainWindow::validateInput);
            execute();
        }
    });
    connect(ui->textRadio,      &QRadioButton::toggled,         this, [this](bool toggled)
    {
        if (toggled)
        {
            ui->firstText->setReadOnly(true);
            ui->firstText->disconnect();
            ui->firstText->setPlainText(QByteArray::fromRawData(rawData, rawData.size()));
            connect(ui->firstText,      &QPlainTextEdit::textChanged,   this, &MainWindow::validateInput);
            execute();
        }
    });
    connect(ui->timeButton,     &QPushButton::pressed,         this, [this](){ QMessageBox::information(this, tr("Czas"),
                                                                                                        tr("Wykonanie szyfrowania/deszyfrowania tekstu za pomocą AES-128 zajęło ") + std::to_string(elapsedTime).c_str() + "ms."); });
    //connect(ui->base64Radio,    &QRadioButton::toggled,         this, &MainWindow::execute);

    ui->keyText     ->setValidator(alphabetValidator);

    //Zarządzanie plikami
    ui->actionLoad  ->setShortcut(QKeySequence(tr("Ctrl+O")));
    ui->actionSave  ->setShortcut(QKeySequence(tr("Ctrl+S")));
    ui->actionSaveAs->setShortcut(QKeySequence(tr("Ctrl+Shift+S")));

    connect(ui->actionLoad,     &QAction::triggered,            this, &MainWindow::loadFile);
    connect(ui->actionSave,     &QAction::triggered,            this, &MainWindow::saveFile);
    connect(ui->actionSaveAs,   &QAction::triggered,            this, &MainWindow::execute);

    //Wyświetlanie pomocy
    connect(ui->actionInfo,     &QAction::triggered,            this, [this]() { help.showMaximized(); help.activateWindow(); });

    //connect(ui->visTable,       &QTableWidget::cellEntered,     this, ???);
    connect(ui->visTable,       &QTableWidget::cellClicked,     this, &MainWindow::manageBlock);
}

//Sprawdza poprawność wprowadzonego tekstu
void MainWindow::validateInput()
{
    QString     text = QString(ui->firstText->toPlainText());
    readMode    read = Hex;
    if (ui->textRadio->isChecked())
        read = Text;
    else if (ui->base64Radio->isChecked())
        read = BASE64;

    text.removeIf([this, read](QChar c)
    {
        int i = 0;
        QString t = c;
        if (read == Text)
            return  false;//alphabetKeyValidator->validate(t, i) == QRegularExpressionValidator::Invalid; //psuje wczytywanie z plików
        return      hexValidator        ->validate(t, i) == QRegularExpressionValidator::Invalid;
    });

    QTextCursor cursor      = ui->firstText->textCursor();
    int         position    = cursor.position();
    bool        bChanged    = text.compare(ui->firstText->toPlainText());

    if (bChanged)
        cursor.setPosition(position - 1);
    else cursor.setPosition(position);
    ui->firstText->disconnect();
    ui->firstText->setPlainText(text.toUpper());
    ui->firstText->setTextCursor(cursor);
    connect(ui->firstText,      &QPlainTextEdit::textChanged,   this, &MainWindow::validateInput);
    rawData = text.toLatin1();
    if (!bChanged) execute();
}

//Odpowiada za zmiany napisów na formatkach
void MainWindow::setupGuiLabels(bool cypherMode)
{
    if (cypherMode) //deszyfrowanie
    {
        ui->firstTextLabel  ->setText("Szyfr");
        ui->secondTextLabel ->setText("Tekst");
    }
    else            //szyfrowanie
    {
        ui->firstTextLabel  ->setText("Tekst");
        ui->secondTextLabel ->setText("Szyfr");
    }
}

void MainWindow::manageBlock(int row, int col)
{
    if (row > ui->visTable->rowCount() / 2u || !ui->visTable->item(row, col))
        return;

    unsigned blockIndex = (row * ui->visTable->columnCount() + col) / 16u;
    if (ui->hexRadio->isChecked())
    {
        if ((!(ui->firstText->toPlainText().size() % 32u) || ((ui->firstText->toPlainText().size() % 32u) == 31u)) && blockIndex == (ui->firstText->toPlainText().size()+31u) / 32u)
            return;
    }
    else
    {
        if (!(ui->firstText->toPlainText().size() % 16u) && blockIndex == (15u + ui->firstText->toPlainText().size()) / 16u)
            return;
    }
    manipulate.showBlock(blockIndex);
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

    readMode    read = Hex;
    QByteArray  data;

    if (ui->textRadio->isChecked())
        read = Text;
    else if (ui->base64Radio->isChecked())
        read = BASE64;

    switch(read)
    {
    case Hex:
        data = QByteArray::fromHex(ui->secondText->toPlainText().toLatin1());
        break;

    case Text:
    default:
        data = ui->secondText->toPlainText().toLatin1();
        break;
    }
    QDataStream out(&file);
    out << data;
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

    QByteArray  arr = file.readAll();
    readMode    read = Hex;

    if (ui->textRadio->isChecked())
        read = Text;
    else if (ui->base64Radio->isChecked())
        read = BASE64;

    switch(read)
    {
    case Hex:
        arr = arr.toHex();
        break;

    case Text:
    default:
        break;
    }
    ui->firstText->setPlainText(arr);

    savedFileName = fileName;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete alphabetValidator;
    delete alphabetKeyValidator;
    delete hexValidator;
}
