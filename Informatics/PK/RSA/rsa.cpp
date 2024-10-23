#include "mainwindow.h"
#include "ui_mainwindow.h"

quint32 firstPrimes[] = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011, 2017};
bool primalityTest(int1024_t possiblePrime)
{
    if (possiblePrime == 1u || possiblePrime == 4u)
        return false;

    if (possiblePrime < 4u)
        return true;

    //,,sito''
    for (quint32 prime : firstPrimes)
        if (!(possiblePrime % prime))
            return false;

    return boost::multiprecision::miller_rabin_test(possiblePrime, 40);
}

int512_t generatePrime()
{
    static unsigned add = 0u;
    boost::random::independent_bits_engine<boost::random::mt19937, 500, int512_t> prng;
    prng.seed(time(NULL) + ++add);
    int512_t ret = prng();
    ret -= (ret % 6u) + 1u;
    while (!primalityTest(ret))
    {
        ret -= 4u;
        if (primalityTest(ret))
            break;
        ret -= 2u;
    }
    return ret;
}

void MainWindow::generate_pq(int512_t *p, int512_t *q)
{
    (*p) = generatePrime();
    (*q) = generatePrime();
}

void MainWindow::generate_e(int1024_t phi, int1024_t *e)
{
    if (phi == 0)
        return;
    do
    {
         (*e) = generatePrime();
    }
    while (boost::math::gcd((*e), phi) != 1);
}

void MainWindow::generate_d(int1024_t phi, int1024_t e, int1024_t *d)
{
    if (phi == 0 || e == 0)
        return;
    (*d) = static_cast<int1024_t>(boost::integer::mod_inverse<boost::multiprecision::int1024_t>(e, phi));
}

QByteArray MainWindow::rsa(QByteArray in, bool bMode, bool bLoad, int1024_t e, int1024_t d, int1024_t n, bool bAppend)
{
    std::vector<unsigned char> sizeTest;
    export_bits(n, std::back_inserter(sizeTest), 8);
    int increment = sizeTest.size() - !bLoad;
    if (increment <= 0)
    {
        QMessageBox::critical(this, tr("Błąd"), tr("N jest zbyt małe, by cokolwiek zaszyfrować"));
        return "";
    }
    QByteArray ret = "";
    for (unsigned i = 0; i < in.size(); i += increment)
    {
        int size = qMin(increment, in.size() - i);
        QByteArray numberBits = in.mid(i, size);
        int1024_t toCrypt = 0;
        import_bits(toCrypt, numberBits.begin(), numberBits.end(), 8, true);

        int1024_t crypted = boost::multiprecision::powm(toCrypt, (bMode ? e : d), n);

        std::vector<boost::endian::big_uint8_t> v;
        export_bits(crypted, std::back_inserter(v), 8, true);
        if (bAppend)
            for (unsigned i = 0u; i != (increment + !bLoad) - v.size(); ++i)
                ret.append('\0');
        export_bits(crypted, std::back_inserter(ret), 8, true);
    }
    return ret;
}


void MainWindow::crypt()
{
    if (n == 0 || d == 0 || e == 0)
        return;

    //todo: refactorize
    if (bCypher && bTextChanged)
    {
        text1Data = ui->text1Text->toPlainText().toLatin1();
        bTextChanged = false;
    }

    //text1Data.append(QString(text1Data.size() % 1024, '\0').toLatin1());
    text2Data.clear();
    text2Data = rsa(text1Data, bCypher, !bCypher, e, d, n);
    ui->text2Text->setPlainText(bCypher ? text2Data.toHex(' ') : QString::fromLocal8Bit(text2Data));
}

void MainWindow::sign()
{
    if (!text1Data.size())
        return;

    SHA256_CTX ctx;
    char hash[32];

    sha256_init(&ctx);
    sha256_update(&ctx, text1Data.begin(), text1Data.size());
    sha256_final(&ctx, hash);

    QByteArray hashByte = QByteArray::fromRawData(hash, 32);
    ui->hashText->setPlainText(hashByte.toHex(' '));
    signature = rsa(hashByte, false, false, e, d, n);
    ui->signatureText->setPlainText(signature.toHex(' '));
}

void MainWindow::verify()
{
    if (receiver_n == 0 || receiver_d == 0 || receiver_e == 0)
        return;

    if (signature.size())
    {
        QByteArray hash = rsa(signature, true, true, receiver_e, receiver_d, receiver_n, false);
        ui->signatureVerText->setPlainText(hash.toHex(' '));
    }
    QByteArray temp = rsa(text2Data, !bCypher, true, receiver_e, receiver_d, receiver_n);
    ui->messageVerText->setPlainText(temp);
}
