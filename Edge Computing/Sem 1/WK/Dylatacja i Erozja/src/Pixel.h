#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std; //tylko dlatego, ze to maly program

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

                pixel.rgb[i] = static_cast<unsigned char>(val);
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
            this->rgb[i] = static_cast<unsigned char>(max(0, min(255, rgb[i])));
        return *this;
    }

    void setPixelMax(int pixelMax)
    {
        rgb[0] = static_cast<unsigned char>(min(static_cast<int>(rgb[0]), pixelMax));
        rgb[1] = static_cast<unsigned char>(min(static_cast<int>(rgb[1]), pixelMax));
        rgb[2] = static_cast<unsigned char>(min(static_cast<int>(rgb[2]), pixelMax));
    }

    unsigned char rgb[3]{0u, 0u, 0u};
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
            cout << number;
            int val = stoi(number);

            if (val > 255)
                val = 255;
            else if (val < 0)
                val = 0;

            pixel.a = static_cast<unsigned char>(val);
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
        this->a = static_cast<unsigned char>(max(0, min(255, a)));
        return *this;
    }

    void setPixelMax(int pixelMax)
    {
        a = static_cast<unsigned char>(min(static_cast<int>(a), pixelMax));
    }

    unsigned char a = 0u;
};
