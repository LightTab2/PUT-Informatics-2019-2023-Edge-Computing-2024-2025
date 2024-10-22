#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

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

    for (int y = 0; y != height; ++y)
        for (int x = 0; x != width; ++x)
        {
            const Pixel& pixel = pixels[y][x];
            greyPixels[y][x] = static_cast<int>(lround((static_cast<int>(pixel.rgb[0]) + static_cast<int>(pixel.rgb[1]) + static_cast<int>(pixel.rgb[2])) / 3.0));
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

    for (int y = 0; y != height; ++y)
        for (int x = 0; x != width; ++x)
        {
            const Pixel& pixel = pixels[y][x];
            greyPixels[y][x] = static_cast<int>(lround(0.299 * pixel.rgb[0] + 0.587 * pixel.rgb[1] + 0.114 * pixel.rgb[2]));
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
    for (int y = 0; y != height; ++y)
        for (int x = 0; x != width; ++x)
            ++histogram[pixels[y][x].a];

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

    for (int y = 0; y != height; ++y)
        for (int x = 0; x != width; ++x) 
            equalizedPixels[y][x] = transform[pixels[y][x].a];

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

    for (int y = 0; y != height; ++y)
        for (int x = 0; x != width; ++x)
            correctedPixels[y][x] = static_cast<int>(lround(pow(pixels[y][x].a / 255.0, gamma) * 255.0));

    return correctedImage;
}

Image<GreyscalePixel> convolution(const Image<GreyscalePixel>& image, const vector<vector<double>>& kernel, const double kernelConstant = 1.0)
{
    int imageWidth = image.width();
    int imageHeight = image.height();
    int kernelHeight = static_cast<int>(kernel.size());
    int kernelWidth = static_cast<int>(kernel[0].size());

    Image<GreyscalePixel> result(imageWidth, imageHeight);
    GreyscalePixel** resultPixels = result.data();
    const GreyscalePixel** imagePixels = image.data_c();

    for (int y = 0; y != imageHeight; ++y)
        for (int x = 0; x != imageWidth; ++x) 
        {
            double sum = 0.0;
            for (int ky = 0; ky != kernelHeight; ++ky)
            {
                int ky_flip = kernelHeight - ky - 1;

                for (int kx = 0; kx != kernelWidth; ++kx)
                {
                    int kx_flip = kernelWidth - kx - 1;

                    int imageX = x + kx;
                    int imageY = y + ky;

                    if ((imageX >= 0 && imageX < imageWidth) && (imageY >= 0 && imageY < imageHeight))
                        sum += imagePixels[imageY][imageX].a * kernel[ky_flip][kx_flip];
                }
            }
            sum *= kernelConstant;
            resultPixels[y][x] = lround(sum);
        }

    return result;
}

const vector<vector<double>> createAveragingKernel(int width, int height)
{
    vector<vector<double>> kernel;
    double kernelConstant = 1.0/(width * height); //Zamiast ustawiac tego w funkcji [convolution2D], zrobie to tutaj, zeby to ladnie przekazac jako output funkcji bez argumentow typu in/out
    for (int i = 0; i != height; ++i)
    {
        kernel.push_back(vector<double>());
        for (int j = 0; j != width; ++j)
            kernel.back().push_back(kernelConstant);
    }

    return kernel;
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
        auto averageKernel = createAveragingKernel(3, 3);
        vector<vector<double>> mexicanHat = { 
                                              {  0,  0, -1,  0,  0 },
                                              {  0, -1, -2, -1,  0 },
                                              { -1, -2, 16, -2, -1 },
                                              {  0, -1, -2, -1,  0 },
                                              {  0,  0, -1,  0,  0 } 
                                            };

        vector<vector<double>> sobel = { 
                                         {  3,  3,  3 },
                                         {  3,  0,  3 },
                                         { -5, -5, -5 } 
                                       };
        Image<GreyscalePixel> blurredImage = convolution(gammaImage, averageKernel);

        Image<GreyscalePixel> filteredImage = convolution(blurredImage, mexicanHat);

        string outputFileName = argv[2];
        ofstream outputFile(outputFileName);
        if (!outputFile)
            throw runtime_error("Blad przy otwieraniu \"" + outputFileName + "\"!");

        //outputFile << greyImage;
        //outputFile << eqImage;
        //outputFile << gammaImage;
        //outputFile << blurredImage;
        outputFile << filteredImage;
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
