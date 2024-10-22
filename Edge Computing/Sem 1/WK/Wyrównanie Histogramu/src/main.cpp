#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Image.h"
#include "Pixel.h"

using namespace std; //tylko dlatego, ze to maly program

constexpr double GAMMA = 1.0;

Image<GreyscalePixel> greyscaleAverage(const Image<Pixel>& image)
{
    int width  = image.width();
    int height = image.height();

    Image<GreyscalePixel> greyImage(width, height);
    GreyscalePixel** greyPixels = greyImage.data();
    const Pixel** pixels = image.data_c();

    for (int i = 0; i != height; ++i)
        for (int j = 0; j != width; ++j)
        {
            const Pixel& pixel = pixels[i][j];
            greyPixels[i][j] = static_cast<int>(lround((static_cast<int>(pixel.rgb[0]) + static_cast<int>(pixel.rgb[1]) + static_cast<int>(pixel.rgb[2])) / 3.0));
        }

    return greyImage;
}

Image<GreyscalePixel> greyscaleWeight(const Image<Pixel>& image)
{
    int width  = image.width();
    int height = image.height();

    Image<GreyscalePixel> greyImage(width, height);
    GreyscalePixel** greyPixels = greyImage.data();
    const Pixel** pixels = image.data_c();

    for (int i = 0; i != height; ++i)
        for (int j = 0; j != width; ++j)
        {
            const Pixel& pixel = pixels[i][j];
            greyPixels[i][j] = static_cast<int>(lround(0.299 * pixel.rgb[0] + 0.587 * pixel.rgb[1] + 0.114 * pixel.rgb[2]));
        }

    return greyImage;
}

Image<GreyscalePixel> histogramEq(const Image<GreyscalePixel>& image)
{
    int width = image.width();
    int height = image.height();

    Image<GreyscalePixel> equalizedImage(width, height);
    GreyscalePixel** equalizedPixels = equalizedImage.data();
    const GreyscalePixel** pixels = image.data_c();

    int histogram[256] = { 0 };
    for (int i = 0; i != height; ++i)
        for (int j = 0; j != width; ++j)
            ++histogram[pixels[i][j].a];

    int histogramMin = histogram[0];

    int cdf[256];
    cdf[0] = histogram[0];
    for (int i = 1; i != 256; ++i)
    {
        histogramMin = min(histogramMin, histogram[i]);
        cdf[i] = cdf[i - 1] + histogram[i];
    }
    int imageSize = width * height;

    int transform[256];
    for (int i = 0; i != 256; ++i)
        transform[i] = static_cast<int>(lround((cdf[i] - histogramMin) / static_cast<double>(imageSize - histogramMin) * 255.0));

    for (int i = 0; i != height; ++i)
        for (int j = 0; j != width; ++j) 
            equalizedPixels[i][j] = transform[pixels[i][j].a];

    return equalizedImage;
}

Image<GreyscalePixel> gammaCorrect(const Image<GreyscalePixel>& image, double gamma)
{
    if (gamma <= 0.0)
        throw runtime_error("Gamma musi byc wieksza od 0");
    int width = image.width();
    int height = image.height();

    Image<GreyscalePixel> correctedImage(width, height);
    GreyscalePixel** correctedPixels = correctedImage.data();
    const GreyscalePixel** pixels = image.data_c();

    for (int i = 0; i != height; ++i)
        for (int j = 0; j != width; ++j)
            correctedPixels[i][j] = static_cast<int>(lround(pow(pixels[i][j].a / 255.0, gamma) * 255.0));

    return correctedImage;
}

int main(int argc, char* argv[]) 
{
    try 
    {
        if (argc != 3) 
            throw invalid_argument("Nieprawidlowa ilosc parametrow, poprawne parametry to: <input_file> <output_file>");

        string inputFileName = argv[1];
        ifstream inputFile(inputFileName);
        if (!inputFile)
            throw runtime_error("Blad przy otwieraniu \"" + inputFileName + "\"!");

        Image<Pixel> image;
        inputFile >> image;

        inputFile.close();

        //TUTAJ ZMIANA FUNKCJI
        //Image<GreyscalePixel> greyImage = greyscaleAverage(image);
        Image<GreyscalePixel> greyImage = greyscaleWeight(image);
        Image<GreyscalePixel> eqImage = histogramEq(greyImage);
        Image<GreyscalePixel> gammaImage = gammaCorrect(eqImage, GAMMA);


        string outputFileName = argv[2];
        ofstream outputFile(outputFileName);
        if (!outputFile)
            throw runtime_error("Blad przy otwieraniu \"" + outputFileName + "\"!");

        //outputFile << greyImage;
        //outputFile << eqImage;
        outputFile << gammaImage;
        cout << "Konwersja zakonczona." << endl;
    }
    catch (const exception& e) 
    {
        cerr << "Krytyczny blad: " << e.what() << endl;
        //MessageBoxA(NULL, e.what(), "Krytyczny blad", MB_ICONERROR | MB_OK);
        return 1;
    }

    return 0;
}
