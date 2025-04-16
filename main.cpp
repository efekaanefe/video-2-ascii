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

const std::string ASCII_CHARS = "@%#*+=-:. ";
const int ASCII_SCREEN_WIDTH = 97; // Can be changed to any value

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
    if (!ret) {
        std::cout << "Error: Could not read the first frame." << std::endl;
        return -1;
    }
    
    int video_width = frame.cols;
    int video_height = frame.rows;
    float aspect_ratio = static_cast<float>(video_width) / video_height;
    
    int ASCII_SCREEN_HEIGHT = std::round((float)ASCII_SCREEN_WIDTH / aspect_ratio / 2.0f);
    
    // Calculate patch size
    float delta_video_width = static_cast<float>(video_width) / ASCII_SCREEN_WIDTH;
    float delta_video_height = static_cast<float>(video_height) / ASCII_SCREEN_HEIGHT;
    
    // Debug
    std::cout << "Video dimensions: " << video_width << "x" << video_height << std::endl;
    std::cout << "ASCII dimensions: " << ASCII_SCREEN_WIDTH << "x" << ASCII_SCREEN_HEIGHT << std::endl;
    std::cout << "Patch size: " << delta_video_width << "x" << delta_video_height << std::endl;
    std::cout << "Starting video playback in 2 seconds..." << std::endl;
    
    usleep(1000000);  
    
    // Clear screen 
    std::cout << "\033[2J\033[H";
    
    while (true) {
        ret = cap.read(frame);
        if (!ret) {
            std::cout << "End of video or error occurred." << std::endl;
            break;
        }
        
        std::string ascii_frame;
        
        // Generate the ASCII frame using row and column iteration
        for (int r = 0; r < ASCII_SCREEN_HEIGHT; r++) {
            for (int c = 0; c < ASCII_SCREEN_WIDTH; c++) {
                int start_x = static_cast<int>(c * delta_video_width);
                int start_y = static_cast<int>(r * delta_video_height);
                int width = static_cast<int>(delta_video_width);
                int height = static_cast<int>(delta_video_height);

                
                cv::Rect patchRegion(start_x, start_y, width, height);
                cv::Mat patch = frame(patchRegion);
                
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
    
    int index = static_cast<int>(std::round((avg_color / 255.0f) * (ascii_chars.length() - 1)));
    
    // Ensure index is within bounds
    index = std::max(0, std::min(index, static_cast<int>(ascii_chars.length() - 1)));
    
    return ascii_chars.at(index);
}
