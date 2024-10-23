#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>

void MainWindow::generateData()
{
    ui->resultsText->setPlainText("");
    ui->cypherText->setPlainText("");
    ui->cypherBitText->setPlainText("");
    cypher.clear();
    cypherBit.clear();
    quint64 currentRandom = seed/*, blumCopy = blum*/;
    quint32 i = 0u;
    bitData.clear();
    //quint32 byteRange = 0u;
    //while (blumCopy != 1u)
    //{
        //blumCopy >>= 1u;
        //++byteRange;
    //}
    while (i != bitLength)
    {
        currentRandom *= currentRandom;
        currentRandom %= blum;
        quint64 currentRandomBits = currentRandom;
        //for (quint32 j = 0u; j != byteRange && i != bitLength; ++j)
        {
            if (currentRandomBits & 1)
                bitData += '1';
            else
                bitData += '0';
            //currentRandomBits >>= 1;
            ++i;
        }
    }
    if (bText)
        ui->resultsText->setPlainText(bitData);
    visualize();
    ui->testParamSpin->setMaximum(qMax(0, bitData.size() - 20000));
}

void MainWindow::encrypt()
{
    cypher.clear();
    cypherBit.clear();
    for (unsigned i = 0u; i != bitData.size(); ++i)
        cypherBit.append((bitData[i] & 1) ^ (plainBitText[i] & 1) + '0');

    for (unsigned i = 0u; i < bitData.size(); i += 8u)
        cypher.append(cypherBit.mid(i, 8u).toUInt(nullptr, 2));

    if (bText)
    {
        ui->cypherBitText->setPlainText(cypherBit);
        ui->cypherText->setPlainText(cypher);
    }
    visualize();
    ui->testParamSpin->setMaximum(qMax(0, cypherBit.size() - 20000));
}

//Liczby pierwsze mniejsze od 2022
//Dla szybkiego testu, czy jest sens przerzucać liczbę przez test Rabina Millera
quint32 firstPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011, 2017};

//Test naiwny, potem Rabin-Miller, a na koniec test wyczerpujący
//Powinno w miarę rozsądnym czasie stwierdzić pierwszość liczby
bool primalityTest(quint32 possiblePrime)
{
    if (possiblePrime == 1u || possiblePrime == 4u)
        return false;

    if (possiblePrime < 4u)
        return true;

    //,,sito''
    for (quint32 prime : firstPrimes)
        if (!(possiblePrime % prime))
            return false;

    quint32 possiblePrime_sub1 = possiblePrime - 1u;

    quint32 exponent = possiblePrime_sub1;
    while (!(exponent % 2u))
        exponent >>= 1u;

    //Rabin-Miller, 20 iteracji
    for (quint32 i = 0u; i != 20u; ++i)
    {
        quint64 ret = 1u;
        quint64 x = QRandomGenerator::global()->bounded(2u, possiblePrime_sub1);

        quint64 d = exponent;
        while (d != 0u)
        {
            if (d & 1u)
            {
                ret *= x;
                ret %= possiblePrime;
            }

            d >>= 1u;
            x *= x;
            x %= possiblePrime;
        }

        if (ret == 1u || ret == possiblePrime_sub1)
            continue;

        bool bPossiblePrime = false;
        d = exponent;
        while (d != possiblePrime_sub1)
        {
            ret *= ret;
            ret %= possiblePrime;
            d <<= 1;

            if (ret == 1)     return false;
            if (ret == possiblePrime_sub1)
            {
                bPossiblePrime = true;
                break;
            }
        }
        if (!bPossiblePrime)
        return false;
    }

    //Dokładne sprawdzenie
    for (quint32 div = 2027u; div * div <= possiblePrime; div += 6u)
        if (!(possiblePrime % div) || !(possiblePrime % (div + 2u)))
            return false;

   return true;
}

quint32 generatePrimeCo3Mod4()
{
    quint32 ret = QRandomGenerator::global()->bounded(1u << 12u, 1u << 15u);
    //Zapewniamy (ret % 4) == 3
    ret += 3u - (ret % 4u);
    while (!primalityTest(ret))
        ret += 4u;
    return ret;
}

//Korzystamy z zewnętrznego PRNG, żeby wylosować parametry naszego PRNG. Trochę ironiczne
void MainWindow::generateBlum()
{
    quint32 p = generatePrimeCo3Mod4();
    ui->pEdit->setText(QString::number(p));
    quint32 q = generatePrimeCo3Mod4();
    ui->qEdit->setText(QString::number(q));
    blum = p * q;
    ui->blumSpin->disconnect();
    ui->blumSpin->setValue(blum);
    connect(ui->blumSpin,       &BigSpin::valueChanged,             this, &MainWindow::changeBlum);
}

void MainWindow::generateSeed()
{
    seed = QRandomGenerator::global()->bounded(21u, qMax(22u, static_cast<quint32>(qSqrt(blum))));
    while (std::gcd(blum, seed) != 1)
        --seed;
    ui->seedSpin->setValue(seed);
}

void MainWindow::runTests()
{
    ui->singleBitStatText       ->setText("");
    ui->sequence1Text           ->setText("");
    ui->sequence2Text           ->setText("");
    ui->sequence3Text           ->setText("");
    ui->sequence4Text           ->setText("");
    ui->sequence5Text           ->setText("");
    ui->sequence6Text           ->setText("");
    ui->sequence1Text_2         ->setText("");
    ui->sequence2Text_2         ->setText("");
    ui->sequence3Text_2         ->setText("");
    ui->sequence4Text_2         ->setText("");
    ui->sequence5Text_2         ->setText("");
    ui->sequence6Text_2         ->setText("");
    ui->pokerText               ->setText("");

    ui->singleBitTestText       ->setText("TAK");
    ui->sequence1TestText       ->setText("TAK");
    ui->sequence2TestText       ->setText("TAK");
    ui->sequence3TestText       ->setText("TAK");
    ui->sequence4TestText       ->setText("TAK");
    ui->sequence5TestText       ->setText("TAK");
    ui->sequence6TestText       ->setText("TAK");
    ui->longSequenceTestText    ->setText("TAK");
    ui->pokerTestText           ->setText("TAK");

    bool    bCypher = cypherBit.size();
    quint32 size    = (bCypher ? cypherBit.size() : bitData.size());
    quint32 pos     = ui->testParamSpin->value();
    if (size < 20000u)
    {
        QMessageBox::critical(this, tr("Błąd"), tr("Zbyt krótki ciąg! Wymagana długość to 20000 bitów"));
        return;
    }
    const QByteArray tested = (bCypher ? cypherBit.mid(pos, 20000u) : bitData.mid(pos, 20000u));

    //singleBit
    quint32 count = 0u;
    for (quint32 i = 0u; i != 20000u; ++i)
        if (tested[i] == '1') ++count;

    ui->singleBitStatText->setText(QString::number(count));
    if (count < 9725u || count > 10275u)
        ui->singleBitTestText->setText("NIE");

    //sequence
    quint32 sequence[12u]{0u};
    quint32 index = 0u;
    bool parity = (tested[0u] == '1');

    for (quint32 i = 1u; i != 20000u; ++i)
        if (tested[i] == (parity ? '1' : '0'))
        {
            if (index != 5u)
                ++index;
        }
        else
        {
            ++sequence[(parity ? 6u : 0u) +index];
            parity = !parity;
            index = 0u;
        }
    ++sequence[index];
    ui->sequence1Text   ->setText(QString::number(sequence[0u]));
    ui->sequence2Text   ->setText(QString::number(sequence[1u]));
    ui->sequence3Text   ->setText(QString::number(sequence[2u]));
    ui->sequence4Text   ->setText(QString::number(sequence[3u]));
    ui->sequence5Text   ->setText(QString::number(sequence[4u]));
    ui->sequence6Text   ->setText(QString::number(sequence[5u]));
    ui->sequence1Text_2 ->setText(QString::number(sequence[6u]));
    ui->sequence2Text_2 ->setText(QString::number(sequence[7u]));
    ui->sequence3Text_2 ->setText(QString::number(sequence[8u]));
    ui->sequence4Text_2 ->setText(QString::number(sequence[9u]));
    ui->sequence5Text_2 ->setText(QString::number(sequence[10u]));
    ui->sequence6Text_2 ->setText(QString::number(sequence[11u]));
    if (sequence[0u] < 2315u || sequence[0u] > 2685u || sequence[6u]  < 2315u || sequence[6u]  > 2685u)
        ui->sequence1TestText->setText("NIE");
    if (sequence[1u] < 1114u || sequence[1u] > 1386u || sequence[7u]  < 1114u || sequence[7u]  > 1386u)
        ui->sequence2TestText->setText("NIE");
    if (sequence[2u] < 527u  || sequence[2u] > 723u  || sequence[8u]  < 527u  || sequence[8u]  > 723u)
        ui->sequence3TestText->setText("NIE");
    if (sequence[3u] < 240u  || sequence[3u] > 384u  || sequence[9u]  < 240u  || sequence[9u]  > 384u)
        ui->sequence4TestText->setText("NIE");
    if (sequence[4u] < 103u  || sequence[4u] > 209u  || sequence[10u] < 103u  || sequence[10u] > 209u)
        ui->sequence5TestText->setText("NIE");
    if (sequence[5u] < 103u  || sequence[5u] > 209u  || sequence[11u] < 103u  || sequence[11u] > 209u)
        ui->sequence6TestText->setText("NIE");

    //longSequence
    parity  = (tested[0u] == '1');
    quint32 len = 1u, maxLen = 1u;
    for (quint32 i = 1u; i != 20000u; ++i)
        if (tested[i] == (parity ? '1' : '0'))
        {
            if (++len > maxLen)
                maxLen = len;
            if (len >= 26u)
            {
                ui->longSequenceTestText->setText("NIE");
                break;
            }
        }
        else
        {
            parity = !parity;
            len = 1u;
        }
    ui->longSequenceText->setText(QString::number(maxLen));

    //pokerTest
    quint32 segment[16u]{0u};
    for (quint32 i = 0u; i < 20000u; i += 4u)
        ++segment[QString(tested.mid(i, 4u)).toUInt(nullptr, 2)];

    qreal poker = 0.0;
    for (quint32 i = 0u; i != 16u; ++i)
        poker += segment[i] * segment[i];

    poker *= 16.0/5000.0;
    poker -= 5000.0;

    ui->pokerText->setText(QString::number(poker));
    if (poker < 2.16 || poker > 46.17)
        ui->pokerTestText->setText("NIE");
}
