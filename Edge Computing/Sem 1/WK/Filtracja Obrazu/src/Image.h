#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std; //tylko dlatego, ze to maly program

template <typename PixelUnit>
class Image final
{
public:
    friend ostream& operator<< (ostream& os, const Image& image)
    {
        //todo: dodac kolejny argument szablonu klasy do sprawdzenia formatu
        os << "P2\n" << image.width_ << " " << image.height_ << "\n255\n";

        for (int i = 0; i != image.height_; ++i)
        {
            for (int j = 0; j != image.width_; ++j)
                os << image.A_[i][j];

            os << "\n";
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
                is >> image.A_[i][j];

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
    Image(Image&& ) = default;
    Image& operator=(const Image& image)
    {
        dealloc();
        alloc(image.width_, image.height_);
        for (int i = 0; i != height_; ++i)
            for (int j = 0; j != width_; ++j)
                A_[i][j] = image.A_[i][j];
        return *this;
    }
    ~Image()
    {
        dealloc();
    }

    const PixelUnit** data_c() const
    {
        return const_cast<const PixelUnit**>(A_);
    }

    PixelUnit** data() const
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
        for (int i = 0; i != height_; ++i)
            for (int j = 0; j != width_; ++j)
                A_[i][j].setPixelMax(pixelMax);
    }

private:
    void alloc(int width, int height)
    {
        if (A_ != nullptr)
            dealloc();
        width_  = width;
        height_ = height;

        A_ = new PixelUnit*[height_];
        for (int i = 0; i != height_; ++i)
            A_[i] = new PixelUnit[width_];
    }

    void dealloc()
    {
        if (A_ == nullptr)
            return;

        for (int i = 0; i != height_; ++i)
            delete[] A_[i];
        delete[] A_;

        A_ = nullptr;
    }

    PixelUnit** A_ = nullptr;
    int width_ = -1, height_ = -1;
};
