#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

// 计算两条线段的交点
bool calculateIntersection(cv::Point2f o1, cv::Point2f p1, cv::Point2f o2, cv::Point2f p2, cv::Point2f& r) {
    cv::Point2f x = o2 - o1;
    cv::Point2f d1 = p1 - o1;
    cv::Point2f d2 = p2 - o2;

    float cross = d1.x * d2.y - d1.y * d2.x;
    if (fabs(cross) < 1e-8) {
        return false;
    }

    double t1 = (x.x * d2.y - x.y * d2.x) / cross;
    r = o1 + d1 * t1;
    return true;
}

// 计算多边形的中心
cv::Point2f calculatePolygonCenter(const vector<cv::Point>& polygon) {
    cv::Moments m = cv::moments(polygon);
    return cv::Point2f(m.m10 / m.m00, m.m01 / m.m00);
}

// 检测箭头方向的函数
string getArrowDirection(cv::Point2f intersection, cv::Point2f polyCenter) {
    float dx = intersection.x - polyCenter.x;
    float dy = intersection.y - polyCenter.y;

    if (fabs(dx) > fabs(dy)) {
        return (dx > 0) ? "Right" : "Left";
    }
    else {
        return (dy > 0) ? "Down" : "Up";
    }
}

vector<string> getArrowDirections(int blurKernelSize, const cv::Mat& image) {
    // 确保高斯模糊核的大小为奇数并且大于1
    if (blurKernelSize % 2 == 0) {
        blurKernelSize++;
    }
    if (blurKernelSize < 3) {
        blurKernelSize = 3;
    }

    int lowerH = 37, lowerS = 0, lowerV = 0;
    int upperH = 143, upperS = 255, upperV = 220;
    int cannyLowThreshold = 50, cannyHighThreshold = 150;

    // 转换为HSV颜色空间
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    // 定义HSV颜色阈值
    cv::Scalar lowerBound(lowerH, lowerS, lowerV);
    cv::Scalar upperBound(upperH, upperS, upperV);

    // 颜色范围筛选
    cv::Mat mask1;
    cv::inRange(hsvImage, lowerBound, upperBound, mask1);

    // 高斯模糊
    cv::Mat blur1;
    cv::GaussianBlur(mask1, blur1, cv::Size(blurKernelSize, blurKernelSize), 0);

    // Canny边缘检测
    cv::Mat edge1;
    cv::Canny(blur1, edge1, cannyLowThreshold, cannyHighThreshold);

    // 寻找轮廓
    vector<vector<cv::Point>> contours;
    cv::findContours(edge1, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    vector<cv::Rect> arrowBoxes; // 用于保存箭头的外接矩形
    vector<cv::Vec4i> longestLines1; // 用于保存最长的边1
    vector<cv::Vec4i> longestLines2; // 用于保存最长的边2
    vector<cv::Point2f> intersections; // 用于保存交点
    vector<cv::Point2f> polyCenters; // 用于保存多边形中心
    vector<string> directions; // 用于保存箭头方向

    // 筛选指定面积的轮廓
    for (size_t i = 0; i < contours.size(); i++) {
        cv::Rect boundingBox = cv::boundingRect(contours[i]);
        if (boundingBox.width >= 23 && boundingBox.width <= 32 &&
            boundingBox.height >= 23 && boundingBox.height <= 32) {
            // 多边形近似
            vector<cv::Point> polygon;
            cv::approxPolyDP(contours[i], polygon, 0.01 * cv::arcLength(contours[i], true), true);

            // 检查多边形面积是否大于300
            if (cv::contourArea(polygon) <= 300) {
                continue;
            }

            // 找出最长的两条边
            cv::Vec4i longestLine1, longestLine2;
            double maxLen1 = 0, maxLen2 = 0;

            for (size_t j = 0; j < polygon.size(); ++j) {
                cv::Point pt1 = polygon[j];
                cv::Point pt2 = polygon[(j + 1) % polygon.size()];
                double len = cv::norm(pt1 - pt2);
                if (len > maxLen1) {
                    maxLen2 = maxLen1;
                    longestLine2 = longestLine1;
                    maxLen1 = len;
                    longestLine1 = cv::Vec4i(pt1.x, pt1.y, pt2.x, pt2.y);
                }
                else if (len > maxLen2) {
                    maxLen2 = len;
                    longestLine2 = cv::Vec4i(pt1.x, pt1.y, pt2.x, pt2.y);
                }
            }

            // 计算多边形中心
            cv::Point2f polyCenter = calculatePolygonCenter(polygon);

            // 计算两条最长边的交点
            cv::Point2f intersection;
            if (calculateIntersection(cv::Point2f(longestLine1[0], longestLine1[1]), cv::Point2f(longestLine1[2], longestLine1[3]),
                cv::Point2f(longestLine2[0], longestLine2[1]), cv::Point2f(longestLine2[2], longestLine2[3]), intersection)) {

                // 检查交点到多边形中心的距离
                float distanceToCenter = cv::norm(intersection - polyCenter);

                // 如果距离大于17像素，则舍弃此多边形
                if (distanceToCenter > 17) {
                    continue;
                }

                // 检查多边形中心点之间的距离，去掉太近的多边形
                if (!polyCenters.empty() && cv::norm(polyCenter - polyCenters.back()) < 10) {
                    continue;
                }

                // 添加箭头信息
                polyCenters.push_back(polyCenter); // 添加多边形中心
                intersections.push_back(intersection); // 添加交点
                arrowBoxes.push_back(boundingBox);
                longestLines1.push_back(longestLine1);
                longestLines2.push_back(longestLine2);

                string direction = getArrowDirection(intersection, polyCenter);
                directions.push_back(direction); // 添加箭头方向
            }
        }
    }

    // 如果箭头数量为5或6，保留Y轴最接近平均值的四个箭头
    if (directions.size() == 5 || directions.size() == 6) {
        double meanY = accumulate(polyCenters.begin(), polyCenters.end(), 0.0,
            [](double sum, const cv::Point2f& p) { return sum + p.y; }) / polyCenters.size();

        vector<int> indices(directions.size());
        iota(indices.begin(), indices.end(), 0); // 生成索引

        // 按与平均Y值的距离排序
        sort(indices.begin(), indices.end(), [&](int a, int b) {
            return fabs(polyCenters[a].y - meanY) < fabs(polyCenters[b].y - meanY);
            });

        // 选择与平均Y值最接近的四个箭头
        indices.resize(4);

        // 按索引的X值从小到大排序
        sort(indices.begin(), indices.end(), [&](int a, int b) {
            return polyCenters[a].x < polyCenters[b].x;
            });

        // 精简 directions
        vector<string> filteredDirections;
        for (int idx : indices) {
            filteredDirections.push_back(directions[idx]);
        }
        directions = filteredDirections; // 更新 directions
    }
    else {
        // 按多边形中心点的X值排序
        vector<int> indices(directions.size());
        iota(indices.begin(), indices.end(), 0); // 生成索引
        sort(indices.begin(), indices.end(), [&](int a, int b) {
            return polyCenters[a].x < polyCenters[b].x;
            });

        // 更新 directions 为按X值排序后的顺序
        vector<string> sortedDirections;
        for (int idx : indices) {
            sortedDirections.push_back(directions[idx]);
        }
        directions = sortedDirections;
    }

    return directions;
}