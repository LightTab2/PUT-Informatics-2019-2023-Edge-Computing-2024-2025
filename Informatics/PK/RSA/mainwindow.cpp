#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Zarządzanie plikami
    ui->actionLoad  ->setShortcut(QKeySequence(tr("Ctrl+O")));
    ui->actionSaveAs->setShortcut(QKeySequence(tr("Ctrl+S")));

    connect(ui->actionLoad,                     &QAction::triggered,            this,       [this]{ MainWindow::loadFile(&text1Data); if (bCypher) ui->text1Text->setPlainText(text1Data); else ui->text1Text->setPlainText(text1Data.toHex(' ')); bTextChanged = false; });
    connect(ui->actionSaveAs,                   &QAction::triggered,            this,       [this]{ MainWindow::saveAsFile(text1Data); });
    connect(ui->actionSaveAs2,                  &QAction::triggered,            this,       [this]{ MainWindow::saveAsFile(text2Data); });
    connect(ui->text1Text,                      &QPlainTextEdit::textChanged,   this,       [this]{ bTextChanged = true; });
    connect(ui->nSpin,                          &BigSpin::valueChanged,         this,       [this](int1024_t val) { n = val; ui->pEdit->setText("???"); ui->qEdit->setText("???"); ui->phiEdit->setPlainText("??????");});
    connect(ui->workButton,                     &QPushButton::clicked,          this,       &MainWindow::crypt);

    auto loadTwoNumbers = [this](int1024_t *number1, int1024_t *number2)
    {
        QByteArray data;
        if (!MainWindow::loadFile(&data))
            return;

        QString numbersStr = QString(data);
        QStringList numbers = numbersStr.split(" ");
        if (numbers.size() != 2)
        {
            QMessageBox::information(this, tr("Błąd"), tr("Niepoprawna liczba argumentów"));
            return;
        }

        if (numbers.size() != 2)
        {
            QMessageBox::information(this, tr("Błąd"), tr("Niepoprawna ilość liczb"));
            return;
        }

        int1024_t num1, num2;
        bool ok;
        num1 = numbers[0].toUInt(&ok);
        if (!ok)
        {
            QMessageBox::information(this, tr("Błąd"), "Niepoprawna liczba \"" + numbers[0] +  "\"");
            return;
        }

        num2 = numbers[1].toUInt(&ok);
        if (!ok)
        {
            QMessageBox::information(this, tr("Błąd"), "Niepoprawna liczba \"" + numbers[0] +  "\"");
            return;
        }

        (*number1) = num1;
        (*number2) = num2;
    };

    auto saveTwoNumbers = [this](int1024_t number1, int1024_t number2)
    {
        std::string toWrite = lexical_cast<std::string>(number1) + " " + lexical_cast<std::string>(number2);
        MainWindow::saveAsFile(QByteArray::fromRawData(toWrite.c_str(), toWrite.size()));
    };

    connect(ui->actionSenderPrivateKeyLoad,         &QAction::triggered,        this,       [this, loadTwoNumbers]
    {
        loadTwoNumbers(&d, &n);

        ui->dSpin->setValue(d);
        ui->nSpin->setValue(n);
    });

    connect(ui->actionSenderPrivateKeySaveAs,       &QAction::triggered,        this,       [this, saveTwoNumbers]
    {
        saveTwoNumbers(d, n);
    });

    connect(ui->actionSenderPublicKeyLoad,          &QAction::triggered,        this,       [this, loadTwoNumbers]
    {
        loadTwoNumbers(&e, &n);

        ui->eSpin->setValue(e);
        ui->nSpin->setValue(n);
    });

    connect(ui->actionSenderPublicKeySaveAs,        &QAction::triggered,        this,       [this, saveTwoNumbers]
    {
        saveTwoNumbers(e, n);
    });

    connect(ui->actionReceiverPrivateKeyLoad,       &QAction::triggered,        this,       [this, loadTwoNumbers]
    {
        loadTwoNumbers(&receiver_d, &receiver_n);
        ui->dReceiverSpin->setValue(receiver_d);
        ui->nReceiverSpin->setValue(receiver_n);
    });

    connect(ui->actionReceiverPrivateKeySaveAs,     &QAction::triggered,        this,       [this, saveTwoNumbers]
    {
        saveTwoNumbers(receiver_d, receiver_n);
    });

    connect(ui->actionReceiverPublicKeyLoad,        &QAction::triggered,        this,       [this, loadTwoNumbers]
    {
        loadTwoNumbers(&receiver_e, &receiver_n);

        ui->eReceiverSpin->setValue(receiver_e);
        ui->nReceiverSpin->setValue(receiver_n);
    });

    connect(ui->actionReceiverPublicKeySaveAs,      &QAction::triggered,        this,       [this, saveTwoNumbers]
    {
        saveTwoNumbers(receiver_e, receiver_n);
    });

    connect(ui->generateButton,                     &QPushButton::clicked,      this,       [this]
    {
        int512_t p, q;
        generate_pq(&p, &q);
        n   = static_cast<int1024_t>(p) * q;
        phi = static_cast<int1024_t>(p - 1u) * (q - 1u);
        ui->nSpin->setValue(n);
        ui->phiEdit->setPlainText(lexical_cast<std::string>(phi).c_str());
        ui->pEdit->setText(lexical_cast<std::string>(p).c_str());
        ui->qEdit->setText(lexical_cast<std::string>(q).c_str());

        generate_e(phi, &e);
        ui->eSpin->setValue(e);

        generate_d(phi, e, &d);
        ui->dSpin->setValue(d);
    });
    connect(ui->generateReceiverButton,         &QPushButton::clicked,          this,       [this]{ int512_t pr, qr; generate_pq(&pr, &qr); receiver_n = pr * qr; receiver_phi = static_cast<int1024_t>(pr - 1) * (qr - 1); generate_e(receiver_phi, &receiver_e); generate_d(receiver_phi, receiver_e, &receiver_d); ui->eReceiverSpin->setValue(receiver_e); ui->dReceiverSpin->setValue(receiver_d); ui->nReceiverSpin->setValue(receiver_n); });
    connect(ui->copyReceiverButton,             &QPushButton::clicked,          this,       [this]{ receiver_e = e; ui->eReceiverSpin->setValue(receiver_e); receiver_d = d; ui->dReceiverSpin->setValue(receiver_d); receiver_n = n; ui->nReceiverSpin->setValue(receiver_n); });
    connect(ui->signButton,                     &QPushButton::clicked,          this,       &MainWindow::sign);
    connect(ui->verifyButton,                   &QPushButton::clicked,          this,       &MainWindow::verify);

    connect(ui->modeCheck,                      &QCheckBox::toggled,            this,       [this](bool toggle)
    {
        this->bCypher = toggle;
        ui->text1Text->setReadOnly(!toggle);
        if (bCypher)
        {
            ui->menuFile  ->setTitle("Tekst jawny");
            ui->menuCypher->setTitle("Szyfrogram");
            ui->text1Label->setText("Tekst jawny");
            ui->text2Label->setText("Szyfrogram");
            ui->workButton->setText("Szyfruj");
        }
        else
        {
            ui->menuCypher->setTitle("Tekst jawny");
            ui->menuFile  ->setTitle("Szyfrogram");
            ui->text2Label->setText("Tekst jawny");
            ui->text1Label->setText("Szyfrogram");
            ui->workButton->setText("Odszyfruj");
        }
    });
    connect(ui->eSpin,                          &BigSpin::valueChanged,            this,       [this](int1024_t val) { e = val; });
    connect(ui->dSpin,                          &BigSpin::valueChanged,            this,       [this](int1024_t val) { d = val; });

    connect(ui->eReceiverSpin,                  &BigSpin::valueChanged,            this,       [this](int1024_t val) { receiver_e = val; });
    connect(ui->dReceiverSpin,                  &BigSpin::valueChanged,            this,       [this](int1024_t val) { receiver_d = val; });
    connect(ui->nReceiverSpin,                  &BigSpin::valueChanged,            this,       [this](int1024_t val) { receiver_n = val; });
}

void MainWindow::saveFile(QByteArray array, QString fileName)
{
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
    out.writeRawData(array, array.size());
}

void MainWindow::saveAsFile(QByteArray array)
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Zapisz plik jako"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return;

    saveFile(array, fileName);
}

bool MainWindow::loadFile(QByteArray *array)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Wczytaj plik"), "",
                                                 tr("Wszystkie pliki (*)"));
    if (fileName.isEmpty())
        return false;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Nie udało się otworzyć pliku"),
            file.errorString());

        return false;
    }
    (*array) = file.readAll();
    return true;
}


MainWindow::~MainWindow()
{
    delete ui;
}
