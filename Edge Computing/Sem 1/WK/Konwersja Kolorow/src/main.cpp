#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Image.h"
#include "Pixel.h"

using namespace std; //tylko dlatego, ze to maly program

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
            greyPixels[i][j] = static_cast<unsigned char>(lround((static_cast<int>(pixel.rgb[0]) + static_cast<int>(pixel.rgb[1]) + static_cast<int>(pixel.rgb[2])) / 3.0));
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
            greyPixels[i][j] = static_cast<unsigned char>(lround(0.299 * pixel.rgb[0] + 0.587 * pixel.rgb[1] + 0.114 * pixel.rgb[2]));
        }

    return greyImage;
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
        Image<GreyscalePixel> greyImage = greyscaleAverage(image);
        //Image<GreyscalePixel> greyImage = greyscaleWeight(image);

        string outputFileName = argv[2];
        ofstream outputFile(outputFileName);
        if (!outputFile)
            throw runtime_error("Blad przy otwieraniu \"" + outputFileName + "\"!");

        outputFile << greyImage;

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
