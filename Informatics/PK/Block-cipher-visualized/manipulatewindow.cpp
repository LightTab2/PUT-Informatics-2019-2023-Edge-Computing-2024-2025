#include "manipulatewindow.h"
#include "ui_manipulatewindow.h"

void ManipulateWindow::showBlock(unsigned blockId)
{
    this->blockId = blockId;
    ui->blockNumber->setValue(blockId);

    QByteArray text;
    if (hexRadio->isChecked())
        text = firstText->toPlainText().toLatin1();
    else
        text = firstText->toPlainText().toLatin1().toHex();

    ui->blockText->setText(text.mid(blockId * 32u, qMin(32ll, text.size() - blockId * 32ll)));
    this->show();
}

ManipulateWindow::ManipulateWindow(QPlainTextEdit *firstText, QRadioButton *hexRadio, QWidget *parent)
    : QWidget(parent), ui(new Ui::manipulateWindow), firstText(firstText), hexRadio(hexRadio)
{
    ui->setupUi(this);

    hexStringValidator          = new QRegularExpressionValidator(QRegularExpression(HEX_STRING));
    ui->changeText->setValidator(hexStringValidator);
    connect(ui->blockText, &QLineEdit::textChanged, this, [this]
    {
        QString text = this->firstText->toPlainText();
        if (this->hexRadio->isChecked())
            text.replace(this->blockId * 32u, 32u, ui->blockText->text());
        else
            text.replace(this->blockId * 16u, 16u, QByteArray::fromHex(ui->blockText->text().toLatin1()));
        this->firstText->setPlainText(text);
    });

    connect(ui->changeByteButton,     &QPushButton::pressed,         this, [this]()
    {
        QString text = ui->blockText->text();
        if (text.size() < 2)
            return;
        QString replaceText = ui->changeText->text();
        replaceText.append(QString('0').repeated(2u - replaceText.size()));
        text.replace(QRandomGenerator::global()->bounded(text.size()/2u) * 2u, 2u, replaceText);
        ui->blockText->setText(text);
    });

    connect(ui->deleteByteButton,     &QPushButton::pressed,         this, [this]()
    {
        QString text = ui->blockText->text();
        if (text.size() < 2)
            return;
        QString replaceText = "00";
        text.replace(QRandomGenerator::global()->bounded(text.size()/2u) * 2u, 2u, replaceText);
        ui->blockText->setText(text);
    });

    connect(ui->shuffleBytesButton,   &QPushButton::pressed,         this, [this]()
    {
        QString text = ui->blockText->text();
        if (text.size() < 2)
            return;
        std::vector<unsigned> freePos;
        for (unsigned i = 0u; i != text.size()/2u; ++i)
            freePos.push_back(i);
        QString shuffledText;
        while (!freePos.empty())
        {
            unsigned index = QRandomGenerator::global()->bounded(freePos.size());
            shuffledText.append(text.mid(freePos[index]*2u, 2u));
            freePos.erase(freePos.begin() + index);
        }
        if (text.size() % 2)
            shuffledText.append(text.right(1));
        ui->blockText->setText(shuffledText);
    });

    connect(ui->shuffleBytesButton,   &QPushButton::pressed,         this, [this]()
    {
        QString text = ui->blockText->text();
        if (text.size() < 2u)
            return;
        std::vector<unsigned> freePos;
        for (unsigned i = 0u; i != text.size()/2u; ++i)
            freePos.push_back(i);
        QString shuffledText;
        while (!freePos.empty())
        {
            unsigned index = QRandomGenerator::global()->bounded(freePos.size());
            shuffledText.append(text.mid(freePos[index]*2u, 2u));
            freePos.erase(freePos.begin() + index);
        }
        if (text.size() % 2u)
            shuffledText.append(text.right(1u));
        ui->blockText->setText(shuffledText);
    });

    connect(ui->deleteBlockButton,   &QPushButton::pressed,         this, [this]()
    {
        QString inputText = this->firstText->toPlainText();
        if (ui->blockText->text().isEmpty())
            return;

        if (this->hexRadio->isChecked())
            inputText.remove(this->blockId * 32u, 32u);
        else
            inputText.remove(this->blockId * 16u, 16u);

        this->firstText->setPlainText(inputText);
        this->close();
    });

    connect(ui->exchangeBlockButton,   &QPushButton::pressed,         this, [this]()
    {
        QString inputText = this->firstText->toPlainText();
        if (ui->blockText->text().isEmpty())
            return;

        if (this->hexRadio->isChecked())
        {
            if ((ui->blockText->text().size() + 31u) / 32u < 2)
                return;
            inputText.remove(this->blockId * 32u, 32u);
        }
        else
        {
            if ((ui->blockText->text().size() + 15u) / 16u < 2)
                return;
            inputText.remove(this->blockId * 16u, 16u);
        }
        this->firstText->setPlainText(inputText);
        this->close();
    });

    connect(ui->duplicateBlockButton,   &QPushButton::pressed,         this, [this]()
    {
        QString inputText = this->firstText->toPlainText();
        QString text = ui->blockText->text();
        int id = blockId;

        if (ui->poRadio->isChecked())
            ++id;

        if (text.size() < 32)
            return;

        if (this->hexRadio->isChecked())
            inputText.insert(id * 32u, text);
        else
            inputText.insert(id * 16u, QByteArray::fromHex(text.toStdString().c_str()));
        if (!ui->poRadio->isChecked())
            ++blockId;

        ui->blockNumber->setValue(blockId);
        this->firstText->setPlainText(inputText);
    });

    connect(ui->newBlockButton,         &QPushButton::pressed,         this, [this]()
    {
        QString inputText = this->firstText->toPlainText();
        QString text;
        int id = blockId;
        if (ui->poRadio->isChecked())
        {
            ++id;
            if (text.size() < 32)
            return;
        }

        if (this->hexRadio->isChecked())
        {
            for (unsigned i = 0; i != 32u; ++i)
            {
                unsigned rand = QRandomGenerator::global()->bounded(16u);
                if (rand < 10)
                    text += static_cast<char>('0' + rand);
                else
                    text += static_cast<char>('A' + rand - 10);
            }
            inputText.insert(id * 32u, text);
        }
        else
            inputText.insert(id * 16u, QByteArray::fromHex(text.toStdString().c_str()));

        if (!ui->poRadio->isChecked())
            ++blockId;

        ui->blockNumber->setValue(blockId);
        this->firstText->setPlainText(inputText);
    });

    this->setWindowFlag(Qt::Window);
    this->close();
}

void ManipulateWindow::showEvent(QShowEvent *e)
{
    if (this->firstText)
        this->firstText->setReadOnly(true);
}

void ManipulateWindow::closeEvent(QCloseEvent *e)
{
    if (this->firstText)
        this->firstText->setReadOnly(false);
}

ManipulateWindow::~ManipulateWindow()
{
    delete hexStringValidator;
}
