#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <arm_neon.h>

using namespace std; //tylko dlatego, ze to maly program

enum class SaveType
{
    PGM = 0,
    PBM = 1
};

struct Pixel
{
    friend ostream& operator<< (ostream& os, const Pixel& pixel)
    {
        os << static_cast<int>(pixel.rgb[0]) << ' ' << static_cast<int>(pixel.rgb[1]) << ' ' << static_cast<int>(pixel.rgb[2]) << ' ';
        return os;
    }

    friend istream& operator>>(istream& is, Pixel& pixel)
    {
        for (int i = 0; i != 3; ++i)
        {
            string number;
            if (!(is >> number))
                throw runtime_error("Niespodziewany koniec pliku");
            try
            {
                int val = stoi(number);

                if (val > 255)
                    val = 255;
                else if (val < 0)
                    val = 0;

                pixel.rgb[i] = static_cast<uint8_t>(val);
            }
            catch (const invalid_argument&)
            {
                pixel.rgb[i] = 0;
            }
            catch (const out_of_range&)
            {
                pixel.rgb[i] = 255;
            }
        }
        return is;
    }

    Pixel& operator=(int rgb[3])
    {
        for (int i = 0; i != 3; ++i)
            this->rgb[i] = static_cast<uint8_t>(max(0, min(255, rgb[i])));
        return *this;
    }

    void setPixelMax(int pixelMax)
    {
        rgb[0] = static_cast<uint8_t>(min(static_cast<int>(rgb[0]), pixelMax));
        rgb[1] = static_cast<uint8_t>(min(static_cast<int>(rgb[1]), pixelMax));
        rgb[2] = static_cast<uint8_t>(min(static_cast<int>(rgb[2]), pixelMax));
    }

    explicit operator uint8_t*()
    {
        return rgb;
    }

    uint8_t rgb[3]{ 0u, 0u, 0u };
};

struct GreyscalePixel
{
    friend ostream& operator<< (ostream& os, const GreyscalePixel& pixel)
    {
        os << static_cast<int>(pixel.a) << ' ';
        return os;
    }

    friend istream& operator>>(istream& is, GreyscalePixel& pixel)
    {
        string number;
        if (!(is >> number))
            throw runtime_error("Niespodziewany koniec pliku");
        try
        {
            int val = stoi(number);

            if (val > 255)
                val = 255;
            else if (val < 0)
                val = 0;

            pixel.a = static_cast<uint8_t>(val);
        }
        catch (const invalid_argument&)
        {
            pixel.a = 0;
        }
        catch (const out_of_range&)
        {
            pixel.a = 255;
        }
        return is;
    }

    GreyscalePixel& operator=(int a)
    {
        this->a = static_cast<uint8_t>(max(0, min(255, a)));
        return *this;
    }

    void setPixelMax(int pixelMax)
    {
        a = static_cast<uint8_t>(min(static_cast<int>(a), pixelMax));
    }

    uint8_t a = 0u;
};

template <typename PixelUnit, SaveType type = SaveType::PGM>
class Image final
{
public:
    friend ostream& operator<< (ostream& os, const Image& image)
    {
        switch (type)
        {
        case SaveType::PGM:
            //todo: dodac kolejny argument szablonu klasy do sprawdzenia formatu
            os << "P2\n" << image.width_ << " " << image.height_ << "\n255\n";

            for (int i = 0; i != image.height_ * image.width_; i += image.width_)
            {
                for (int j = 0; j != image.width_; ++j)
                    os << image.A_[i + j];

                os << "\n";
            }
            break;
        case SaveType::PBM:
            os << "P1\n" << image.width_ << " " << image.height_ << '\n';

            for (int i = 0; i != image.height_ * image.width_; i += image.width_)
            {
                for (int j = 0; j != image.width_; ++j)
                    os << ((image.A_[i + j].a == 0) ? 0 : 1) << ' ';

                os << "\n";
            }
            break;
        default:
            break;
        }
        return os;
    }

    friend istream& operator>>(istream& is, Image& image)
    {
        string line = "";
        while (is)
        {
            getline(is, line);
            if (line[0] == '#')
                continue;
            if (line == "P3")
                break;
        }
        //todo: dodac kolejny argument szablonu klasy do sprawdzenia formatu
        if (line != "P3")
            throw invalid_argument("Wczytany plik nie jest w formacie P3!");

        while (is)
        {
            getline(is, line);
            if (line[0] == '#')
                continue;
            break;
        }
        if (!is)
            throw runtime_error("Blad przy wczytywaniu!");

        istringstream sstream(line);
        int width, height, pixelMax = 255; //ImageMagick nie pozwala zmienic pixelMax, co bugowalo moj program...
        sstream >> width >> height; //>> pixelMax;

        image.alloc(width, height);
        for (int i = 0; i != height; ++i)
            for (int j = 0; j != width; ++j)
                is >> image.A_[i * width + j];

        image.setPixelMax(pixelMax);
        return is;
    }

    Image() = default;
    Image(int width, int height)
    {
        alloc(width, height);
    }
    Image(const Image& image)
    {
        *this = image;
    }
    Image(Image&&) = default;
    Image& operator=(const Image& image)
    {
        dealloc();
        alloc(image.width_, image.height_);
        for (int i = 0; i != height_ * width_; i += width_)
            for (int j = 0; j != width_; ++j)
                A_[i + j] = image.A_[i + j];
        return *this;
    }
    ~Image()
    {
        dealloc();
    }

    const PixelUnit* data_c() const
    {
        return const_cast<const PixelUnit*>(A_);
    }

    PixelUnit* data() const
    {
        return A_;
    }

    const int height() const
    {
        return height_;
    }

    const int width() const
    {
        return width_;
    }

    void setPixelMax(int pixelMax)
    {
        for (int i = 0; i != height_ * width_; i += width_)
            for (int j = 0; j != width_; ++j)
                A_[i + j].setPixelMax(pixelMax);
    }

private:
    void alloc(int width, int height)
    {
        if (A_ != nullptr)
            dealloc();
        width_  = width;
        height_ = height;

        A_ = new PixelUnit [height_ * width_];
    }

    void dealloc()
    {
        if (A_ == nullptr)
            return;

        delete[] A_;

        A_ = nullptr;
    }

    PixelUnit* A_ = nullptr;
    int width_ = -1, height_ = -1;
};

void sum(const uint8_t addend1[8], const uint8_t addend2[8], uint8_t dest[8])
{
    for (int i = 0; i != 8; ++i)
        dest[i] = addend1[i] + addend2[i];
}

void sumNeon(const uint8_t addend1[8], const uint8_t addend2[8], uint8_t dest[8])
{
    uint8x8_t addend1_neon = vld1_u8(addend1);
    uint8x8_t addend2_neon = vld1_u8(addend2);
    uint8x8_t result = vadd_u8(addend1_neon, addend2_neon);
    vst1_u8(dest, result);
}

// Wersja dla preprocessowanych danych pod Neon
void sumNeon(const uint8x8_t addend1, const uint8x8_t addend2, uint8x8_t dest)
{
    dest = vadd_u8(addend1, addend2);
}

void mul(const uint8_t factor1[8], const uint8_t factor2[8], uint8_t dest[8])
{
    for (int i = 0; i != 8; ++i)
        dest[i] = factor1[i] * factor2[i];
}

void mulNeon(const uint8_t factor1[8], const uint8_t factor2[8], uint8_t dest[8])
{
    uint8x8_t neon_src1 = vld1_u8(factor1);
    uint8x8_t neon_src2 = vld1_u8(factor2);
    uint8x8_t result = vmul_u8(neon_src1, neon_src2);
    vst1_u8(dest, result);
}

// Wersja dla preprocessowanych danych pod Neon
void mulNeon(const uint8x8_t factor1, const uint8x8_t factor2, uint8x8_t dest)
{
    dest = vmul_u8(factor1, factor2);
}

Image<GreyscalePixel> greyscaleWeightNeon(const Image<Pixel>& image)
{
    Image<GreyscalePixel> greyImage(image.width(), image.height());
    uint8x8_t r_ratio = vdup_n_u8(77);
    uint8x8_t g_ratio = vdup_n_u8(151);
    uint8x8_t b_ratio = vdup_n_u8(28);
    const uint8_t* data = reinterpret_cast<const uint8_t*>(image.data_c());
    uint8_t* dest = reinterpret_cast<uint8_t*>(greyImage.data());

    int imageSize_neon = (image.width() * image.height()) / 8;

    for (int i = 0; i != imageSize_neon; ++i)
    {
        uint16x8_t temp;
        uint8x8x3_t rgb = vld3_u8(data);
        uint8x8_t result;

        temp = vmull_u8(rgb.val[0], r_ratio);
        temp = vmlal_u8(temp, rgb.val[1], g_ratio);
        temp = vmlal_u8(temp, rgb.val[2], b_ratio);

        result = vshrn_n_u16(temp, 8);
        vst1_u8(dest, result);

        data += 8 * 3;
        dest += 8;
    }
    return greyImage;
}

// Pozwala tylko na kernele [dowolny_rozmiar]x4, bo nie chcialo mi sie tego generalizowac bardziej
// Mozna by flippedKernel rozszerzyc na vector<vector<int16x4_t>> i sie bawic
Image<GreyscalePixel> convolutionNeon(const Image<GreyscalePixel>& image, const vector<int16x4_t>& flippedKernel)
{
    int imageWidth  = image.width();
    int imageHeight = image.height();
    int kernelSize  = static_cast<int>(flippedKernel.size());

    Image<GreyscalePixel> result(imageWidth, imageHeight);
    const uint8_t* data = reinterpret_cast<const uint8_t*>(image.data_c());
    uint8_t* dest       = reinterpret_cast<uint8_t*>(result.data());

    for (int y = 0; y != imageHeight; ++y)
        for (int x = 0; x != imageWidth; ++x)
        {
            bool bIgnore = false;
            int32x4_t sumOneRow{ 0 };
            for (int ky = 0; ky != kernelSize; ++ky)
            {
                int imageY = y + ky;

                // Nie robie paddingu, wiec zignoruje pixel, gdzie jest za malo danych
                if (x + 8 >= imageWidth || imageY >= imageHeight)
                {
                    bIgnore = true;
                    break;
                }
                // Tez da sie zoptymalizowac indeksowanie tutaj, jezeli kompilator sam na to nie wpadnie
                // Tutaj tez przydaloby sie wczytac tylko 4 pixele, bo przez to robie ramke dookola obrazu, ktora nie jest przetwarzana
                // Da sie jakos?
                uint8x8_t rawData = vld1_u8(data + imageY * imageWidth + x);
                uint16x8_t extendedData = vmovl_u8(rawData);
                int16x8_t oneRow = vreinterpretq_s16_u16(extendedData);
                int16x4_t oneRowx4 = vget_low_s16(oneRow);

                sumOneRow = vmlal_s16(sumOneRow, oneRowx4, flippedKernel[ky]);
            }
            if (bIgnore) 
                continue;

            int32x2_t sumOneRowx2 = vadd_s32(vget_low_s32(sumOneRow), vget_high_s32(sumOneRow));
            int32x2_t finalSum = vpadd_s32(sumOneRowx2, sumOneRowx2);
            int32_t finalSumScalar = vget_lane_s32(finalSum, 0);

            dest[y * imageWidth + x] = static_cast<uint8_t>(finalSumScalar > 255 ? 255 : (finalSumScalar < 0 ? 0 : finalSumScalar));
        }

    return result;
}

int main(int argc, char* argv[])
{
    try
    {
        uint8_t numbers1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        uint8_t numbers2[8] = { 9, 10, 11, 12, 13, 14, 15, 16 };
        uint8_t dest[8];

        // Debug
        cout << "numbers1 = [ " << static_cast<int>(numbers1[0]);
        for (int i = 1; i != 8; ++i)
            cout << ", " << static_cast<int>(numbers1[i]);
        cout << " ]" << endl;

        cout << "numbers2 = [ " << static_cast<int>(numbers2[0]);
        for (int i = 1; i != 8; ++i)
            cout << ", " << static_cast<int>(numbers2[i]);
        cout << " ]" << endl;

        sumNeon(numbers1, numbers2, dest);

        cout << "sum = [ " << static_cast<int>(dest[0]);
        for (int i = 1; i != 8; ++i)
            cout << ", " << static_cast<int>(dest[i]);
        cout << " ]" << endl;

        mulNeon(numbers1, numbers2, dest);

        cout << "mul = [ " << static_cast<int>(dest[0]);
        for (int i = 1; i != 8; ++i)
            cout << ", " << static_cast<int>(dest[i]);
        cout << " ]" << endl;

        ifstream inputFile("input.ppm");
        if (!inputFile)
            throw runtime_error("Blad przy otwieraniu \"input.ppg\"!");

        Image<Pixel> image;
        inputFile >> image;
        inputFile.close();

        Image<GreyscalePixel> greyImage = greyscaleWeightNeon(image);

        vector<int16_t*> sobel =
        {
            new int16_t[4]{  3,  3,  3,  0 },
            new int16_t[4]{  3,  0,  3,  0 },
            new int16_t[4]{ -5, -5, -5,  0 }
        };

        vector<int16x4_t> sobelNeon = 
        {
           vld1_s16(sobel[0]),
           vld1_s16(sobel[1]),
           vld1_s16(sobel[2])
        };

        Image<GreyscalePixel> filteredImage = convolutionNeon(greyImage, sobelNeon);

        //ofstream outputGreyFile("grey.pgm");
        //if (!outputGreyFile)
        //    throw runtime_error("Blad przy otwieraniu \"grey.pgm\"!");

        ofstream outputFile("output.pgm");
        if (!outputFile)
            throw runtime_error("Blad przy otwieraniu \"output.pgm\"!");

        //outputGreyFile << greyImage;
        outputFile << filteredImage;

        for (int16_t* sobelRow : sobel)
            delete[] sobelRow;

        cout << "Praca zakonczona." << endl;
    }
    catch (const exception& e) 
    {
        cerr << "Krytyczny blad: " << e.what() << endl;
        //MessageBoxA(NULL, e.what(), "Krytyczny blad", MB_ICONERROR | MB_OK);
        return 1;
    }

    return 0;
}
