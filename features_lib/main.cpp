//
//  main.cpp
//  features_lib
//
//  Created by sofiawu on 2017/5/2.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include <iostream>
#include "Features.hpp"

#include <opencv2/opencv.hpp>

int ParseBuffer(const std::string& feature_buff)
{
    if(feature_buff.empty()) return 0;
    
    std::stringstream ss(feature_buff);
    int marker_type = 0;
    ss.read((char*)&marker_type, sizeof(marker_type));
    
    int num = 0;
    ss.read((char*)&num, sizeof(num));
    
    int size = 0;
    ss.read((char*)&size, sizeof(size));
    char* temp = new char[size + 1];
    memset(temp, 0, sizeof(char) * (size + 1));
    ss.read((char*)temp, sizeof(char) * size);
    std::string md5 = std::string(temp); // doc_id_
    delete[] temp;
    
    uint32_t width = 0, height = 0;
    ss.read((char*)&width, sizeof(width));
    ss.read((char*)&height, sizeof(height));
    
    std::vector<cv::KeyPoint> kpts;
    int pt_num = 0;
    ss.read((char*)&pt_num, sizeof(pt_num));
    for(int i = 0; i < pt_num; ++i)
    {
        cv::KeyPoint kpt;
        ss.read((char*)&kpt.pt.x, sizeof(float));
        ss.read((char*)&kpt.pt.y, sizeof(float));
        ss.read((char*)&kpt.angle, sizeof(float));
        ss.read((char*)&kpt.octave, sizeof(int));
        kpts.push_back(kpt);
    }
    
    int rows = 0, cols = 0, type = 0;
    ss.read((char*)&rows, sizeof(int));
    ss.read((char*)&cols, sizeof(int));
    ss.read((char*)&type, sizeof(int));
    cv::Mat desc = cv::Mat(rows, cols, type);
    ss.read((char*)desc.data, sizeof(char) * desc.step * rows);
    
    return 0;
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

std::string GetFileName(const std::string& file_name) {
    size_t pos = file_name.find_last_of('/');
    return file_name.substr(pos + 1);
}

int GetFile(const std::string& file_name, std::string* content) {
    int fd = open(file_name.c_str(), O_RDONLY);
    if ( fd <= 0) return 1;
    
    size_t size = lseek(fd, 0, SEEK_END);
    if(size == -1) return 1; if(size == 0) return 0;
    
    content->resize(size);
    
    size_t ret = lseek(fd, 0, SEEK_SET);
    if(ret != 0) return 1;
    
    ret = read(fd, &((*content)[0]), size);
    if(ret == -1) return 1;
    
    return 0;
}

int main() {
    std::string img_str;
    GetFile("/Users/sofiawu/Work/Projects/AR/Test/0361/train.jpg", &img_str);
    
    std::string gray_str;
    libar::ARFeature arfeature;
    arfeature.Grayscale(img_str, &gray_str);
    std::string resize_str;
    arfeature.Resize(gray_str, 640, &resize_str);
    
    int width, height;
    arfeature.GetSize(resize_str, &width, &height);
    std::cout << width << " " << height << std::endl;
    
    std::string raw_data;
    arfeature.Decode(resize_str, &raw_data);
    
    std::string gray;
    arfeature.Encode(raw_data, width, height, &gray);
    arfeature.Decode(gray, &raw_data);
    return 0;
}

int mainx(int argc, const char * argv[]) {
    // insert code here...
    cv::Mat img = cv::imread("/Users/sofiawu/Work/Projects/AR/Test/0361/train.jpg", 0);
    std::string img_str;
    img_str.assign((char*)img.data, img.step * img.rows);
    
    libar::ARFeature feature;
    std::string feature_buff;
    feature.GetFeatures("test", img_str, img.cols, img.rows, &feature_buff);
    
    std::string img_str1;
    GetFile("/Users/sofiawu/Work/Projects/AR/Test/0361/train.jpg", &img_str1);
    std::string gray_str;
    feature.Grayscale(img_str1, &gray_str);
    std::string feature_buff1;
    feature.GetFeatures("test1", gray_str, &feature_buff1);
    
    std::ofstream fs("/Users/sofiawu/Work/feature.dat", std::ios::out | std::ios::binary);
    fs.write(feature_buff.c_str(), feature_buff.size());
    fs.close();
    
    ParseBuffer(feature_buff);
    return 0;
}
