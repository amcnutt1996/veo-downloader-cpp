//
// Created by Gregory McNutt on 1/29/26.
//

#ifndef VIDEODOWNLOADERGUI_WEB_SCRAPER_H
#define VIDEODOWNLOADERGUI_WEB_SCRAPER_H
#include <curl/curl.h>


class web_scraper {
    static size_t WriteCallBack(void *content, size_t size, size_t amount, std::string *response);
public:

    static std::string get_video_url(const std::string &input_url);

private:
    static std::string get_website(const std::string &url);

    static std::string extract_primary(const std::string &data);

    static std::string extract_secondary(const std::string &data);

    static std::string extract_between(const std::string &data, const std::string &start, const std::string &end, size_t start_offset = 0);

};


#endif //VIDEODOWNLOADERGUI_WEB_SCRAPER_H