#include "mainwindow.h"

const quint8 lookup1[256u] = {
    0x63u, 0x7cu, 0x77u, 0x7bu, 0xf2u, 0x6bu, 0x6fu, 0xc5u, 0x30u, 0x01u, 0x67u, 0x2bu, 0xfeu, 0xd7u, 0xabu, 0x76u,
    0xcau, 0x82u, 0xc9u, 0x7du, 0xfau, 0x59u, 0x47u, 0xf0u, 0xadu, 0xd4u, 0xa2u, 0xafu, 0x9cu, 0xa4u, 0x72u, 0xc0u,
    0xb7u, 0xfdu, 0x93u, 0x26u, 0x36u, 0x3fu, 0xf7u, 0xccu, 0x34u, 0xa5u, 0xe5u, 0xf1u, 0x71u, 0xd8u, 0x31u, 0x15u,
    0x04u, 0xc7u, 0x23u, 0xc3u, 0x18u, 0x96u, 0x05u, 0x9au, 0x07u, 0x12u, 0x80u, 0xe2u, 0xebu, 0x27u, 0xb2u, 0x75u,
    0x09u, 0x83u, 0x2cu, 0x1au, 0x1bu, 0x6eu, 0x5au, 0xa0u, 0x52u, 0x3bu, 0xd6u, 0xb3u, 0x29u, 0xe3u, 0x2fu, 0x84u,
    0x53u, 0xd1u, 0x00u, 0xedu, 0x20u, 0xfcu, 0xb1u, 0x5bu, 0x6au, 0xcbu, 0xbeu, 0x39u, 0x4au, 0x4cu, 0x58u, 0xcfu,
    0xd0u, 0xefu, 0xaau, 0xfbu, 0x43u, 0x4du, 0x33u, 0x85u, 0x45u, 0xf9u, 0x02u, 0x7fu, 0x50u, 0x3cu, 0x9fu, 0xa8u,
    0x51u, 0xa3u, 0x40u, 0x8fu, 0x92u, 0x9du, 0x38u, 0xf5u, 0xbcu, 0xb6u, 0xdau, 0x21u, 0x10u, 0xffu, 0xf3u, 0xd2u,
    0xcdu, 0x0cu, 0x13u, 0xecu, 0x5fu, 0x97u, 0x44u, 0x17u, 0xc4u, 0xa7u, 0x7eu, 0x3du, 0x64u, 0x5du, 0x19u, 0x73u,
    0x60u, 0x81u, 0x4fu, 0xdcu, 0x22u, 0x2au, 0x90u, 0x88u, 0x46u, 0xeeu, 0xb8u, 0x14u, 0xdeu, 0x5eu, 0x0bu, 0xdbu,
    0xe0u, 0x32u, 0x3au, 0x0au, 0x49u, 0x06u, 0x24u, 0x5cu, 0xc2u, 0xd3u, 0xacu, 0x62u, 0x91u, 0x95u, 0xe4u, 0x79u,
    0xe7u, 0xc8u, 0x37u, 0x6du, 0x8du, 0xd5u, 0x4eu, 0xa9u, 0x6cu, 0x56u, 0xf4u, 0xeau, 0x65u, 0x7au, 0xaeu, 0x08u,
    0xbau, 0x78u, 0x25u, 0x2eu, 0x1cu, 0xa6u, 0xb4u, 0xc6u, 0xe8u, 0xddu, 0x74u, 0x1fu, 0x4bu, 0xbdu, 0x8bu, 0x8au,
    0x70u, 0x3eu, 0xb5u, 0x66u, 0x48u, 0x03u, 0xf6u, 0x0eu, 0x61u, 0x35u, 0x57u, 0xb9u, 0x86u, 0xc1u, 0x1du, 0x9eu,
    0xe1u, 0xf8u, 0x98u, 0x11u, 0x69u, 0xd9u, 0x8eu, 0x94u, 0x9bu, 0x1eu, 0x87u, 0xe9u, 0xceu, 0x55u, 0x28u, 0xdfu,
    0x8cu, 0xa1u, 0x89u, 0x0du, 0xbfu, 0xe6u, 0x42u, 0x68u, 0x41u, 0x99u, 0x2du, 0x0fu, 0xb0u, 0x54u, 0xbbu, 0x16u };

const quint8 lookup2[256u] = {
    0x52u, 0x09u, 0x6au, 0xd5u, 0x30u, 0x36u, 0xa5u, 0x38u, 0xbfu, 0x40u, 0xa3u, 0x9eu, 0x81u, 0xf3u, 0xd7u, 0xfbu,
    0x7cu, 0xe3u, 0x39u, 0x82u, 0x9bu, 0x2fu, 0xffu, 0x87u, 0x34u, 0x8eu, 0x43u, 0x44u, 0xc4u, 0xdeu, 0xe9u, 0xcbu,
    0x54u, 0x7bu, 0x94u, 0x32u, 0xa6u, 0xc2u, 0x23u, 0x3du, 0xeeu, 0x4cu, 0x95u, 0x0bu, 0x42u, 0xfau, 0xc3u, 0x4eu,
    0x08u, 0x2eu, 0xa1u, 0x66u, 0x28u, 0xd9u, 0x24u, 0xb2u, 0x76u, 0x5bu, 0xa2u, 0x49u, 0x6du, 0x8bu, 0xd1u, 0x25u,
    0x72u, 0xf8u, 0xf6u, 0x64u, 0x86u, 0x68u, 0x98u, 0x16u, 0xd4u, 0xa4u, 0x5cu, 0xccu, 0x5du, 0x65u, 0xb6u, 0x92u,
    0x6cu, 0x70u, 0x48u, 0x50u, 0xfdu, 0xedu, 0xb9u, 0xdau, 0x5eu, 0x15u, 0x46u, 0x57u, 0xa7u, 0x8du, 0x9du, 0x84u,
    0x90u, 0xd8u, 0xabu, 0x00u, 0x8cu, 0xbcu, 0xd3u, 0x0au, 0xf7u, 0xe4u, 0x58u, 0x05u, 0xb8u, 0xb3u, 0x45u, 0x06u,
    0xd0u, 0x2cu, 0x1eu, 0x8fu, 0xcau, 0x3fu, 0x0fu, 0x02u, 0xc1u, 0xafu, 0xbdu, 0x03u, 0x01u, 0x13u, 0x8au, 0x6bu,
    0x3au, 0x91u, 0x11u, 0x41u, 0x4fu, 0x67u, 0xdcu, 0xeau, 0x97u, 0xf2u, 0xcfu, 0xceu, 0xf0u, 0xb4u, 0xe6u, 0x73u,
    0x96u, 0xacu, 0x74u, 0x22u, 0xe7u, 0xadu, 0x35u, 0x85u, 0xe2u, 0xf9u, 0x37u, 0xe8u, 0x1cu, 0x75u, 0xdfu, 0x6eu,
    0x47u, 0xf1u, 0x1au, 0x71u, 0x1du, 0x29u, 0xc5u, 0x89u, 0x6fu, 0xb7u, 0x62u, 0x0eu, 0xaau, 0x18u, 0xbeu, 0x1bu,
    0xfcu, 0x56u, 0x3eu, 0x4bu, 0xc6u, 0xd2u, 0x79u, 0x20u, 0x9au, 0xdbu, 0xc0u, 0xfeu, 0x78u, 0xcdu, 0x5au, 0xf4u,
    0x1fu, 0xddu, 0xa8u, 0x33u, 0x88u, 0x07u, 0xc7u, 0x31u, 0xb1u, 0x12u, 0x10u, 0x59u, 0x27u, 0x80u, 0xecu, 0x5fu,
    0x60u, 0x51u, 0x7fu, 0xa9u, 0x19u, 0xb5u, 0x4au, 0x0du, 0x2du, 0xe5u, 0x7au, 0x9fu, 0x93u, 0xc9u, 0x9cu, 0xefu,
    0xa0u, 0xe0u, 0x3bu, 0x4du, 0xaeu, 0x2au, 0xf5u, 0xb0u, 0xc8u, 0xebu, 0xbbu, 0x3cu, 0x83u, 0x53u, 0x99u, 0x61u,
    0x17u, 0x2bu, 0x04u, 0x7eu, 0xbau, 0x77u, 0xd6u, 0x26u, 0xe1u, 0x69u, 0x14u, 0x63u, 0x55u, 0x21u, 0x0cu, 0x7du };

const quint8 lookup3[11u] = {
    0x8du, 0x01u, 0x02u, 0x04u, 0x08u, 0x10u, 0x20u, 0x40u, 0x80u, 0x1bu, 0x36u };

quint8 rK[176u];
quint8 iRK[16u];

void kGen(const quint8* k)
{
    for (unsigned i = 0u; i != 4u; ++i)
    {
        rK[i << 2u]           = k[i << 2u];
        rK[(i << 2u) + 1u]    = k[(i << 2u) + 1u];
        rK[(i << 2u) + 2u]    = k[(i << 2u) + 2u];
        rK[(i << 2u) + 3u]    = k[(i << 2u) + 3u];
    }

    for (unsigned i = 4u; i != 44u; ++i)
    {
        quint8 t[4u];
        unsigned rKIndex = (i - 1u) << 2u;

        t[0u] = rK[rKIndex];
        t[1u] = rK[rKIndex + 1u];
        t[2u] = rK[rKIndex + 2u];
        t[3u] = rK[rKIndex + 3u];

        if (!(i % 4u))
        {
            quint8 shift = t[0u];
            t[0u] = lookup1[t[1u]];
            t[1u] = lookup1[t[2u]];
            t[2u] = lookup1[t[3u]];
            t[3u] = lookup1[shift];
            t[0u] = t[0u] ^ lookup3[i >> 2u];
        }

        unsigned rKI1 = i << 2u, rKI2 = (i - 4u) << 2u;
        rK[rKI1]      = rK[rKI2]        ^ t[0u];
        rK[rKI1 + 1u] = rK[rKI2 + 1u]   ^ t[1u];
        rK[rKI1 + 2u] = rK[rKI2 + 2u]   ^ t[2u];
        rK[rKI1 + 3u] = rK[rKI2 + 3u]   ^ t[3u];
    }
}

void addrK(quint8 r, result* res)
{
    for (quint8 i = 0u; i != 4u; ++i)
        for (quint8 j = 0u; j != 4u; ++j)
            (*res)[i][j] ^= rK[(r << 4u) + (i << 2u) + j];
}

void mB(result* res)
{
    for (qint8 i = 0u; i != 4u; ++i)
        for (qint8 j = 0u; j != 4u; ++j)
            (*res)[j][i] = lookup1[(*res)[j][i]];
}

void sR(result* res)
{
    quint8 t       = (*res)[0u][1u];
    (*res)[0u][1u] = (*res)[1u][1u];
    (*res)[1u][1u] = (*res)[2u][1u];
    (*res)[2u][1u] = (*res)[3u][1u];
    (*res)[3u][1u] = t;
    t              = (*res)[0u][2u];
    (*res)[0u][2u] = (*res)[2u][2u];
    (*res)[2u][2u] = t;
    t              = (*res)[1u][2u];
    (*res)[1u][2u] = (*res)[3u][2u];
    (*res)[3u][2u] = t;
    t              = (*res)[0u][3u];
    (*res)[0u][3u] = (*res)[3u][3u];
    (*res)[3u][3u] = (*res)[2u][3u];
    (*res)[2u][3u] = (*res)[1u][3u];
    (*res)[1u][3u] = t;
}

quint8 xPow(quint8 x)
{
    return ((x << 1u) ^ (((x >> 7u) & 1u) * 0x1bu)); //[& 1u] dla pewności, mimo że QT powinien zapewnić, że zmienna będzie 8bitowa
}

void mC(result* res)
{
    for (quint8 i = 0u; i != 4u; ++i)
    {
        quint8  t1  = (*res)[i][0u];
        quint8  t2  = (*res)[i][0u] ^ (*res)[i][1u] ^ (*res)[i][2u] ^   (*res)[i][3u];
        quint8  Tm  = (*res)[i][0u] ^ (*res)[i][1u];  Tm = xPow(Tm);    (*res)[i][0u] ^= Tm ^ t2;
                Tm  = (*res)[i][1u] ^ (*res)[i][2u];  Tm = xPow(Tm);    (*res)[i][1u] ^= Tm ^ t2;
                Tm  = (*res)[i][2u] ^ (*res)[i][3u];  Tm = xPow(Tm);    (*res)[i][2u] ^= Tm ^ t2;
                Tm  = (*res)[i][3u] ^ t1;             Tm = xPow(Tm);    (*res)[i][3u] ^= Tm ^ t2;
    }
}

void iMC(result* res)
{
    for (unsigned i = 0u; i != 4u; ++i)
    {
        quint8 a = (*res)[i][0u];
        quint8 b = (*res)[i][1u];
        quint8 c = (*res)[i][2u];
        quint8 d = (*res)[i][3u];

        #define bitMulti(x, y)                            \
          (((y      & 1u) * x) ^                          \
          ((y >> 1u & 1u) * xPow(x)) ^                    \
          ((y >> 2u & 1u) * xPow(xPow(x))) ^              \
          ((y >> 3u & 1u) * xPow(xPow(xPow(x)))) ^        \
          ((y >> 4u & 1u) * xPow(xPow(xPow(xPow(x))))))   \

        (*res)[i][0u] = bitMulti(a, 0x0eu) ^ bitMulti(b, 0x0bu) ^ bitMulti(c, 0x0du) ^ bitMulti(d, 0x09u);
        (*res)[i][1u] = bitMulti(a, 0x09u) ^ bitMulti(b, 0x0eu) ^ bitMulti(c, 0x0bu) ^ bitMulti(d, 0x0du);
        (*res)[i][2u] = bitMulti(a, 0x0du) ^ bitMulti(b, 0x09u) ^ bitMulti(c, 0x0eu) ^ bitMulti(d, 0x0bu);
        (*res)[i][3u] = bitMulti(a, 0x0bu) ^ bitMulti(b, 0x0du) ^ bitMulti(c, 0x09u) ^ bitMulti(d, 0x0eu);
    }
}

void imB(result* res)
{
    for (qint8 i = 0u; i != 4u; ++i)
        for (qint8 j = 0u; j != 4u; ++j)
            (*res)[j][i] = lookup2[(*res)[j][i]];
}

void isR(result* res)
{
    quint8 t       = (*res)[3u][1u];
    (*res)[3u][1u] = (*res)[2u][1u];
    (*res)[2u][1u] = (*res)[1u][1u];
    (*res)[1u][1u] = (*res)[0u][1u];
    (*res)[0u][1u] = t;
    t              = (*res)[0u][2u];
    (*res)[0u][2u] = (*res)[2u][2u];
    (*res)[2u][2u] = t;
    t              = (*res)[1u][2u];
    (*res)[1u][2u] = (*res)[3u][2u];
    (*res)[3u][2u] = t;
    t              = (*res)[0u][3u];
    (*res)[0u][3u] = (*res)[1u][3u];
    (*res)[1u][3u] = (*res)[2u][3u];
    (*res)[2u][3u] = (*res)[3u][3u];
    (*res)[3u][3u] = t;
}

void encryptStep(result *res, quint8 key[16u])
{
    kGen(key);
    addrK(0u, res);

    for (quint8 r = 1u;; ++r)
    {
        mB(res);
        sR(res);

        if (r == 10u)
            break;

        mC(res);
        addrK(r, res);
    }
    addrK(10u, res);
}

void encrypt(quint8 buf[16u], quint8 key[16u], blockMode mode, quint8 xorBuf[16u])
{
    result* res = reinterpret_cast<result*>(buf);

    switch(mode)
    {
    case CBC:
        for (unsigned i = 0u; i != 16u; ++i)
            buf[i] ^= xorBuf[i];
        break;
    case CFB:
        res = reinterpret_cast<result*>(xorBuf);
        break;
    case ECB:
    default:
        break;
    }

    encryptStep(res, key);

    switch(mode)
    {
    case CFB:
        for (unsigned i = 0u; i != 16u; ++i)
            buf[i] ^= xorBuf[i];
    case CBC:
        for (unsigned i = 0u; i != 16u; ++i)
            xorBuf[i] = buf[i];
        break;
    case ECB:
    default:
        break;
    }
}

void decryptStep(result *res, quint8 key[16u])
{
    kGen(key);
    addrK(10u, res);

    for (quint8 r = 9u;; --r)
    {
        isR(res);
        imB(res);
        addrK(r, res);
        if (r == 0u)
            break;

        iMC(res);
    }
}

void decrypt(quint8 buf[16u], quint8 key[16u], blockMode mode, quint8 xorBuf[16u])
{
    result* res = reinterpret_cast<result*>(buf);
    quint8 bufCopy[16u];
    memcpy(bufCopy, buf, 16u);

    switch(mode)
    {
    case CFB:
        res = reinterpret_cast<result*>(xorBuf);
        encryptStep(res, key);
        break;
    case ECB:
    case CBC:
    default:
        decryptStep(res, key);
        break;
    }


    switch(mode)
    {
    case CBC:
    case CFB:
        for (unsigned i = 0u; i != 16u; ++i)
        {
            buf[i] ^= xorBuf[i];
            xorBuf[i] = bufCopy[i];
        }
        break;
    case ECB:
    default:
        break;
    }

}