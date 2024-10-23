#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::execute(bool bExecute)
{
    //Niektóre przyciski wywołują tą funkcję, gdy są wyłączane, a w ich miejsce włączane inne, które ponownie
    //wywołują tę funkcję (np. QRadioButton) i nie powinny tego robić drugi raz
    if (!bExecute)
        return;
    //Najpierw trzeba posprzątać po poprzednim uruchomieniu programu, o ile to się wydarzyło
    ui->secondText  ->clear();
    ui->visTable    ->setColumnCount(0u);
    ui->visTable    ->setRowCount(0u);

    if (rawData.isEmpty())
        return;

    bool        decypher    = ui->modeCheck->isChecked(),
                visualize   = ui->visCheck ->isChecked();

    unsigned    textSize    = rawData.size(),
                blockSize;

    readMode    read        = Hex;
    if (ui->textRadio->isChecked())
                read        = Text;
    else if (ui->base64Radio->isChecked())
                read        = BASE64;

    QByteArray  rawText     = rawData;

    if ((textSize % 16u) || !decypher)
    {
        rawText.append('\1');
        rawText.append(std::string(15u - (textSize % 16u), '\0'));
    }
    blockSize = rawText.size();

    //Ustawiamy strukturę do wizualizacji
    unsigned    maxRows     = ceil(sqrt(blockSize / 2u)),
                maxColumns  = maxRows * 2u;

    if (visualize)
    {
        ui->visTable->setRowCount(maxRows * 2u + 1u);
        ui->visTable->setColumnCount(maxColumns);
    }
    quint8      key[16u],
                cbcBuf[16u];

    std::string keyText = ui->keyText->text().toStdString();
    //Pobieramy klucz i zapisujemy w [key]
    memset(key, 0u, 16u);
    memcpy(key, keyText.c_str(), qMin(keyText.length(), 16u));
    memcpy(cbcBuf, key, 16u);

    QColor      blockColor(0u, 0u, 0u);
    unsigned    row     = 0u,   col     = 0u,
                pRow    = 0u,   pCol    = 0u;

    QElapsedTimer timer;
    quint8* arrData = new quint8[blockSize];
    memcpy(arrData, rawText.toStdString().c_str(), blockSize);
    timer.start();

    unsigned    dataSize = 16u;
    for (unsigned index = 0u; index < blockSize; )
    {
        quint8 *str = arrData + index;
        if (visualize)
        {
            unsigned    diffSum = 0u,
                        r, g, b;

            while (diffSum < 100u)
            {
                diffSum = 0u;
                diffSum += qAbs(blockColor.red()     - (r = QRandomGenerator::global()->bounded(128u)));
                diffSum += qAbs(blockColor.green()   - (g = QRandomGenerator::global()->bounded(128u)));
                diffSum += qAbs(blockColor.blue()    - (b = QRandomGenerator::global()->bounded(128u)));
            }
            blockColor = QColor(127u + r, 127u + g, 127u + b);

            for (unsigned i = 0u; i != 16u; ++i)
            {
                QByteArray  newItemText;
                if (read == Hex)
                    newItemText = QByteArray::fromRawData(reinterpret_cast<const char*>(str) + i, 1u).toHex().toUpper();
                else
                    newItemText = QByteArray::fromRawData(reinterpret_cast<const char*>(str) + i, 1u);
                QTableWidgetItem *newItem = new QTableWidgetItem(newItemText);
                newItem->setBackground(QBrush(blockColor));
                ui->visTable->setItem(row, col, newItem);
                if (++col == maxColumns)
                {
                    col = 0u;
                    ++row;
                }
            }
        }

        blockMode mode = ECB;
        if (ui->cbcRadio->isChecked())
            mode = CBC;
        else if (ui->cfbRadio->isChecked())
            mode = CFB;

        if (decypher)
            decrypt(str, key, mode, cbcBuf);
        else
            encrypt(str, key, mode, cbcBuf);

        index += 16u;

        if (decypher && index == rawText.length())
        {
            QByteArray end = QByteArray::fromRawData(reinterpret_cast<const char*>(str), 16u);

            for (; dataSize != 0u; --dataSize)
                if (end[dataSize - 1u] == '1')
                    break;
        }
        if (visualize)
            for (unsigned i = 0u; i != 16u; ++i)
            {
                QByteArray  newItemText;
                if (read == Hex)
                    newItemText = QByteArray::fromRawData(reinterpret_cast<const char*>(str) + i, 1u).toHex().toUpper();
                else
                    newItemText = QByteArray::fromRawData(reinterpret_cast<const char*>(str) + i, 1u);
                QTableWidgetItem *newItem = new QTableWidgetItem(newItemText);
                newItem->setBackground(QBrush(blockColor));
                ui->visTable->setItem(maxRows + 1u + pRow, pCol, newItem);
                if (++pCol == maxColumns)
                {
                    pCol = 0u;
                    ++pRow;
                }
            }
    }
    elapsedTime = timer.elapsed();
    processedRawData = QByteArray::fromRawData(reinterpret_cast<const char*>(arrData), rawText.length() + ((dataSize == 0) ? dataSize : (dataSize - 16u)));
    if (read == Hex)
        processedRawData = processedRawData.toHex();

    ui->secondText->setPlainText(processedRawData);
    delete[] arrData;
}
