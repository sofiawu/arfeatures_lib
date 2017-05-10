//
//  Features.hpp
//  features_lib
//
//  Created by sofiawu on 2017/5/2.
//  Copyright © 2017年 sofiawu. All rights reserved.
//

#ifndef Features_hpp
#define Features_hpp

#include <stdio.h>
#include <string>

namespace libar
{
    /**
     * @class ARFeature
     * Extract ARFeatures for AR applications.
     */
    class ARFeature
    {
    public:
        ARFeature();
        ~ARFeature();
        
        /**
         * Extract features for an image.
         * @param img_id the key id of image
         * @param img image buffer（uncompressed).
         * @param width image width.
         * @param height image height.
         * @param feature_buff (out) the extracted feature buffer
         * @return the state of GetFeatures(0 : success, -1 : failed).
         */
        int GetFeatures(const std::string& img_id, const std::string& img, uint32_t width, uint32_t height, std::string* feature_buff );
        
        /**
         * Extract features for an image.
         * @param img_id the key id of image
         * @param img image buffer（compressed).
         * @param feature_buff (out) the extracted feature buffer
         * @return the state of GetFeatures(0 : success, -1 : failed).
         */
        int GetFeatures(const std::string& img_id, const std::string& img, std::string* feature_buff );
        
        /**
         * Grayscale an image.
         * @param jpeg the jpeg format of image(compressed).
         * @param gray (out) the jpeg format of grayscale image buffer.
         * @return the state of Grayscale(0 : success, -1 : jpeg buffer problem, -2 : failed).
         */
        int Grayscale(const std::string &jpeg, std::string *gray);
        
        /**
         * Resize an image.
         * @param src_gray the jpeg format of gray image(compressed).
         * @param max_length the max length of resized image.
         * @param des_gray (out) the jpeg format of resized gray image(compressed).
         * @return the state of Resize(0 : success, -1 : failed).
         */
        int Resize(const std::string &src_gray, int max_length, std::string *des_gray);

        /**
         * Resize an image.
         * @param gray the jpeg format of gray image(compressed).
         * @param width (out) the width of gray image.
         * @param height (out) the height of gray image.
         * @return the state of GetSize(0 : success, -1 : failed).
         */
        int GetSize(const std::string &gray, int *width, int *height);
        
        /**
         * Decode a jpeg format image.
         * @param gray the jpeg format of gray image(compressed).
         * @param raw_data (out) the raw data of gray image.
         * @return the state of Decode(0 : success, -1 : failed).
         */
        int Decode(const std::string &gray, std::string *raw_data);
        
        /**
         * Encode a grayscale image raw buffer into a jpeg format image.
         * @param raw_data the raw data of gray image(uncompressed).
         * @param width the width of gray image(uncompressed).
         * @param height the height of gray image(uncompressed).
         * @param gray (out) the the jpeg format of gray image.
         * @return the state of Encode(0 : success, -1 : failed).
         */
        int Encode(const std::string &raw_data, int width, int height, std::string *gray);
    };
}

#endif /* Features_hpp */
