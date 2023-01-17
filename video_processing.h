#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // Open video file
    VideoCapture cap("video.mp4");
    if (!cap.isOpened()) {
        cerr << "Error opening video file" << endl;
        return -1;
    }

    // Get video properties
    double fps = cap.get(CAP_PROP_FPS);
    int width = cap.get(CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    // Define the codec and create a video writer
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
    VideoWriter writer("output.avi", codec, fps, Size(width, height));

    // Read frames from the video
    Mat frame;
    while (cap.read(frame)) {
        // Perform image processing on the frame
        // Examples:
        // - Convert to grayscale: cvtColor(frame, frame, COLOR_BGR2GRAY);
        // - Apply threshold: threshold(frame, frame, 128, 255, THRESH_BINARY);
        // - Detect edges: Canny(frame, frame, 100, 200);
        // - Detect faces:
        //    CascadeClassifier face_cascade;
        //    face_cascade.load("haarcascade_frontalface_default.xml");
        //    vector<Rect> faces;
        //    face_cascade.detectMultiScale(frame, faces);
        //    for (auto face : faces) {
        //        rectangle(frame, face, Scalar(0, 255, 0));
        //    }

        // Write the processed frame to the output video
        writer.write(frame);
    }

    // Release resources
    cap.release();
    writer.release();
    return 0;
}
