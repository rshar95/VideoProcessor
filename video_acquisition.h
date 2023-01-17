/**
 * Video Acquisition module
 * This module is responsible for acquiring video streams from youtube and handle it's properties
 * @param filename: The name of the video file to be read
 * @param fps: The frames per second of the video
 * @param width: The width of the video frame
 * @param height: The height of the video frame
 * @return: return -1 in case of error in opening the video file
 */


#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include <string>
#include <fstream>

using namespace std;

string youtube_api_key = "YOUR_API_KEY_HERE";

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    string data((const char*) ptr, (size_t) size * nmemb);
    *((stringstream*) stream) << data << endl;
    return size * nmemb;
}

string get_video_url(string video_id) {
    CURL *curl;
    CURLcode res;
    stringstream out;

    curl = curl_easy_init();
    if(curl) {
        string url = "https://www.googleapis.com/youtube/v3/videos?part=snippet&id=" + video_id + "&key=" + youtube_api_key;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return "";
        }

        Json::Value jsonData;
        Json::Reader jsonReader;
        if(!jsonReader.parse(out.str(), jsonData)) {
            cerr << "Error parsing JSON" << endl;
            return "";
        }

        string url = jsonData["items"][0]["snippet"]["thumbnails"]["default"]["url"].asString();
        return url;
    }
    return "";
}

int download_video(string video_id, string file_name) {
    string video_url = get_video_url(video_id);

    if(video_url.empty()) {
        cerr << "Error getting video URL" << endl;
        return 1;
    }

    string command = "ffmpeg -i \"" + video_url + "\" -c copy \"" + file_name + "\"";
    int result = system(command.c_str());
    if(result != 0) {
        cerr << "Error downloading video" << endl;
        return result;
    }
    return 0;
}

int main() {
    string video_id = "VIDEO_ID_HERE";
    string file_name = "video.mp4";
    int result = download_video(video_id, file_name);
    if(result == 0) {
        cout << "Video downloaded successfully" << endl;
    } else
