#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "Tile_detector.h"

int main(int argc, char **argv)
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if (argc < 2)
    {
        std::cerr << "CL usage: MeldsFind <image_path>\n";
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    if (!image.data)
    {
        std::cerr << "No image data" << std::endl;
        return -1;
    }

    Tile_detector detector;
    auto tiles = detector.detect_tiles(image);

    std::cout << "Detected " << tiles.size() << " tiles\n";

    if (std::filesystem::exists("Detected_tiles"))
    {
        std::filesystem::remove_all("Detected_tiles"); // Clear directory before writing detected files
    }
    std::filesystem::create_directories("Detected_tiles");
    for (size_t i = 0; i < tiles.size(); ++i)
    {
        std::string filename = "Detected_tiles/output_tile_" + std::to_string(i) + ".png";
        cv::imwrite(filename, tiles[i]);
    }
    return 0;
}