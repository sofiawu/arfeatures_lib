//
//  Features.cpp
//  features_lib
//
//  Created by sofiawu on 2017/5/2.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#include "Features.hpp"

#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

namespace libar
{
    ARFeature::ARFeature() {}
    
    ARFeature::~ARFeature() {}
    
    int ARFeature::GetFeatures(const std::string& img_id, const std::string& img, uint32_t width, uint32_t height, std::string* feature_buff )
    {
        //create orb detector
        cv::Ptr<cv::Feature2D> detector = cv::ORB::create(1000, 1.3, 10);
        
        //detect features
        std::vector<cv::KeyPoint> kpts;
        cv::Mat desc;
        
        //string -> unsigned char*
        const unsigned char* pcBuffer =  (const unsigned char*)img.c_str();
        unsigned char* pBuffer = const_cast<unsigned char*> (pcBuffer);
        
        int size = (int)img.size();
        if(size != width * height) return -1;

        cv::Mat gray_img(height, width, CV_8U, pBuffer);
        detector->detectAndCompute(gray_img, cv::noArray(), kpts, desc);
        
        //save features
        std::stringstream ss;
        int marker_type = 0;
        ss.write((char*)&marker_type, sizeof(marker_type));
        
        int num = 1;
        ss.write((char*)&num, sizeof(num));
        
        int img_id_size = (int)img_id.length();
        ss.write((char*)&img_id_size, sizeof(img_id_size));
        ss.write((char*)img_id.c_str(), sizeof(char) * img_id_size);
        
        ss.write((char*)&width, sizeof(width));
        ss.write((char*)&height, sizeof(height));
        
        int pt_num = (int)kpts.size();
        ss.write((char*)&pt_num, sizeof(pt_num));
        for(auto kpt : kpts)
        {
            ss.write((char*)&kpt.pt.x, sizeof(float));
            ss.write((char*)&kpt.pt.y, sizeof(float));
            ss.write((char*)&kpt.angle, sizeof(float));
            ss.write((char*)&kpt.octave, sizeof(int));
        }
        
        int rows = desc.rows;
        int cols = desc.cols;
        int type = desc.type();
        ss.write((char*)&rows, sizeof(rows));
        ss.write((char*)&cols, sizeof(cols));
        ss.write((char*)&type, sizeof(type));
        
        ss.write((char*)desc.data, sizeof(char) * desc.step * rows);
        
        //return
        *feature_buff = ss.str();
        return 0;
    }
    
    int ARFeature::GetFeatures(const std::string& img_id, const std::string& img, std::string* feature_buff )
    {
        //create orb detector
        cv::Ptr<cv::Feature2D> detector = cv::ORB::create(1000, 1.3, 10);
        
        //detect features
        std::vector<cv::KeyPoint> kpts;
        cv::Mat desc;
        
        //string -> unsigned char*
        std::vector<uchar> data(img.begin(), img.end());
        cv::Mat decode_img = cv::imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
        
        detector->detectAndCompute(decode_img, cv::noArray(), kpts, desc);
        int width = decode_img.cols;
        int height = decode_img.rows;
        
        //save features
        std::stringstream ss;
        int marker_type = 0;
        ss.write((char*)&marker_type, sizeof(marker_type));
        
        int num = 1;
        ss.write((char*)&num, sizeof(num));
        
        int img_id_size = (int)img_id.length();
        ss.write((char*)&img_id_size, sizeof(img_id_size));
        ss.write((char*)img_id.c_str(), sizeof(char) * img_id_size);
        
        ss.write((char*)&width, sizeof(width));
        ss.write((char*)&height, sizeof(height));
        
        int pt_num = (int)kpts.size();
        ss.write((char*)&pt_num, sizeof(pt_num));
        for(auto kpt : kpts)
        {
            ss.write((char*)&kpt.pt.x, sizeof(float));
            ss.write((char*)&kpt.pt.y, sizeof(float));
            ss.write((char*)&kpt.angle, sizeof(float));
            ss.write((char*)&kpt.octave, sizeof(int));
        }
        
        int rows = desc.rows;
        int cols = desc.cols;
        int type = desc.type();
        ss.write((char*)&rows, sizeof(rows));
        ss.write((char*)&cols, sizeof(cols));
        ss.write((char*)&type, sizeof(type));
        
        ss.write((char*)desc.data, sizeof(char) * desc.step * rows);
        
        //return
        *feature_buff = ss.str();
        return 0;
    }
    
    int ARFeature::Grayscale(const std::string &jpeg, std::string *gray) {
        //decode
        std::vector<uchar> data(jpeg.begin(), jpeg.end());
        cv::Mat decode_img = cv::imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
        
        if(decode_img.data == NULL || decode_img.cols <= 0 || decode_img.rows <= 0) return -1;
        
        //encode
        std::vector<uchar> buf;
        cv::imencode(".jpg", decode_img, buf);
        
        //unsigned char* -> string
        *gray = std::string(buf.begin(), buf.end());
        //uchar* p_gray_buff = (&buf[0]);
        //gray->assign((char*)p_gray_buff, buf.size());
        return 0;
    }
    
    int ARFeature::Resize(const std::string &src_gray, int max_length, std::string *des_gray) {
        //decode
        std::vector<uchar>data(src_gray.begin(), src_gray.end());
        cv::Mat decode_img = cv::imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
        
        if(decode_img.data == NULL) return -1;
        
        //获取图像的高和宽
        int height = decode_img.rows;
        int width  = decode_img.cols;
        
        //获取图像最大边长度
        int long_side = std::max(height, width);
        
        cv::Mat resize_img;
        if ( long_side <= max_length )  //如果长边在设定的阈值内，则不做压缩
            resize_img = decode_img;
        else if (long_side == height ) {  //如果高为长边
            cv::resize(decode_img, resize_img, cv::Size((int)((max_length * width) / height), max_length));
        } else if(long_side == width){   //如果宽为长边
            cv::resize(decode_img, resize_img, cv::Size(max_length, (int)((max_length * height) / width)));
        }
        
        //encode and ->string
        std::vector<uchar> buf;
        cv::imencode(".jpg", resize_img, buf);
        
        //unsigned char* -> string
        *des_gray = std::string(buf.begin(), buf.end());
        return 0;
    }
    
    int ARFeature::GetSize(const std::string &gray, int *width, int *height) {
        //decode
        std::vector<uchar> data(gray.begin(), gray.end());
        cv::Mat decode_img = cv::imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
        if(decode_img.data == NULL) return -1;
        
        *width = decode_img.cols;
        *height = decode_img.rows;
        return 0;
    }
    
    int ARFeature::Decode(const std::string &gray, std::string *raw_data) {
        //decode
        std::vector<uchar> data(gray.begin(), gray.end());
        cv::Mat decode_img = cv::imdecode(data, CV_LOAD_IMAGE_GRAYSCALE);
        if(decode_img.data == NULL) return -1;
        
        //char* -> string
        raw_data->assign((char*)decode_img.data, decode_img.step * decode_img.rows);
        return 0;
    }
    
    int ARFeature::Encode(const std::string &raw_data, int width, int height, std::string *gray) {
        int size = (int)raw_data.size();
        if(size != width * height) return -1;
        
        //string -> unsigned char*
        const unsigned char* pcBuffer =  (const unsigned char*)raw_data.c_str();
        unsigned char* pBuffer = const_cast<unsigned char*> (pcBuffer);
        cv::Mat gray_img(height, width, CV_8U, pBuffer);
        
        //encode
        std::vector<uchar> buf;
        cv::imencode(".jpg", gray_img, buf);
        
        //unsigned char* -> string
        *gray = std::string(buf.begin(), buf.end());
        return 0;
    }
}
