#include <cmath>
#include <iostream>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <ostream>
#include <string>
#include <strings.h>
#include <unistd.h>
#include <vector>

const std::string ASCII_CHARS = "@%#*+=-:. ";
const int ASCII_SCREEN_WIDTH = 80;

char get_char_from_patch(cv::Mat patch, std::string ascii_chars);

int main() {
    cv::VideoCapture cap("video.mp4");
    if (!cap.isOpened()) {
        std::cout << "Error: Could not open video file." << std::endl;
        return -1;
    } else {
        std::cout << "Video file opened successfully!" << std::endl;
    }
    
    cv::Mat frame;
    bool ret = cap.read(frame);
    int video_width = frame.cols;
    int video_height = frame.rows;
    float aspect_ratio = static_cast<float>(video_width) / video_height;
    int ASCII_SCREEN_HEIGHT = std::round((float)ASCII_SCREEN_WIDTH / aspect_ratio);
    int delta_video_width = video_width / ASCII_SCREEN_WIDTH;
    int delta_video_height = video_height / ASCII_SCREEN_HEIGHT;
    
    // Pre-compute the patch positions to ensure consistency
    std::vector<cv::Rect> patches;
    for (int y = 0; y <= video_height - delta_video_height; y += delta_video_height) {
        for (int x = 0; x <= video_width - delta_video_width; x += delta_video_width) {
            patches.push_back(cv::Rect(x, y, delta_video_width, delta_video_height));
        }
    }
    
    int rows = (video_height - delta_video_height) / delta_video_height + 1;
    
    // DEBUG
    std::cout << video_width << "x" << video_height << std::endl;
    std::cout << "ASCII dimensions: " << ASCII_SCREEN_WIDTH << "x" << ASCII_SCREEN_HEIGHT << std::endl;
    std::cout << "Patch size: " << delta_video_width << "x" << delta_video_height << std::endl;
    std::cout << "Starting video playback..." << std::endl;
    
    usleep(1000000);  
    
    std::cout << "\033[2J\033[H";  // Clear screen 
    
    while (true) {
        ret = cap.read(frame);
        if (!ret) {
            std::cout << "End of video or error occurred." << std::endl;
            break;
        }
        
        std::string ascii_frame;
        int patch_index = 0;
        
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < ASCII_SCREEN_WIDTH && patch_index < patches.size(); c++, patch_index++) {
                cv::Mat patch = frame(patches[patch_index]);
                char patch_char = get_char_from_patch(patch, ASCII_CHARS);
                ascii_frame += patch_char;
            }
            ascii_frame += '\n';
        }
        
        // Move cursor to home position 
        std::cout << "\033[H";
        std::cout << ascii_frame;
        std::cout.flush();
        
        usleep(33000); // ~30 FPS
    }
    
    cap.release();
    return 0;
}

char get_char_from_patch(cv::Mat patch, std::string ascii_chars) {
    cv::Scalar mean = cv::mean(patch);
    float avg_color = (mean[0] + mean[1] + mean[2]) / 3.0f;
    
    int index = std::round((avg_color / 255.0f) * (ascii_chars.length() - 1));
    index = std::max(0, std::min(index, static_cast<int>(ascii_chars.length() - 1)));
    return ascii_chars.at(index);
}
