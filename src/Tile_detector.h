#ifndef TILE_DETECTOR_H
#define TILE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>

class Tile_detector
{
public:
    std::vector<cv::Mat> detect_tiles(const cv::Mat &input_image);

private:
    std::vector<cv::Point2f> order_points(const std::vector<cv::Point> &points);

    static const int c_min_tile_area = 2000;
};

#endif // TILE_DETECTOR_H