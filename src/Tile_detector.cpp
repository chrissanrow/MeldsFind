#include "Tile_detector.h"
#include <algorithm>

// Detects tiles within input image and returns a vector of clipped images
std::vector<cv::Mat> Tile_detector::detect_tiles(const cv::Mat &input_image)
{
    std::vector<cv::Mat> tiles;

    // Preprocessing pipeline: grayscale, apply gaussian blur, and canny edge detect
    cv::Mat grayscaled, blurred, edged;
    cv::cvtColor(input_image, grayscaled, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(grayscaled, blurred, cv::Size(5, 5), 0);
    cv::Canny(blurred, edged, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edged, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (auto &contour : contours)
    {
        double perimeter = cv::arcLength(contour, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * perimeter, true);

        // DP approx should yield quadrilateral for mahjong tiles
        if (approx.size() == 4 && cv::contourArea(approx) > c_min_tile_area)
        {
            auto corners = order_points(approx);

            float width = 100;
            float height = 150;
            std::vector<cv::Point2f> target_tile_shape =
                {{0.0f, 0.0f},
                 {width - 1, 0.0f},
                 {width - 1, height - 1},
                 {0.0f, height - 1}};
            cv::Mat M = cv::getPerspectiveTransform(corners, target_tile_shape);
            cv::Mat warped;
            cv::warpPerspective(input_image, warped, M, cv::Size(static_cast<int>(width), static_cast<int>(height)));
            tiles.push_back(warped);
        }
    }
    return tiles;
}

// order quadrilateral points in order: top-left, top-right, bottom-right, bottom-left
std::vector<cv::Point2f> Tile_detector::order_points(const std::vector<cv::Point> &points)
{
    std::vector<cv::Point2f> pointsf;
    for (auto &point : points)
    {
        pointsf.emplace_back(point);
    }

    // sort by inc. x coord
    std::sort(pointsf.begin(), pointsf.end(),
              [](const cv::Point2f a, cv::Point2f b)
              {
                  return a.x < b.x;
              });
    std::vector<cv::Point2f> left_corners(pointsf.begin(), pointsf.begin() + 2);
    std::vector<cv::Point2f> right_corners(pointsf.begin() + 2, pointsf.end());

    std::sort(left_corners.begin(), left_corners.end(),
              [](const cv::Point2f &a, cv::Point2f &b)
              {
                  return a.y < b.y;
              });
    auto top_left = left_corners.at(0);
    auto bottom_left = left_corners.at(1);

    std::sort(right_corners.begin(), right_corners.end(),
              [](const cv::Point2f &a, cv::Point2f &b)
              {
                  return a.y < b.y;
              });
    auto top_right = right_corners.at(0);
    auto bottom_right = right_corners.at(1);

    auto dist = [](const cv::Point2f &a, const cv::Point2f &b)
    {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    };
    if (dist(top_left, bottom_right) < dist(top_left, top_right))
    {
        std::swap(bottom_left, top_right);
    }

    return {top_left, top_right, bottom_right, bottom_left};
}