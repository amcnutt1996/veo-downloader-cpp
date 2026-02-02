//
// Created by Gregory McNutt on 1/29/26.
//
#include <iostream>
#include "web_scraper.h"
#include <curl/curl.h>
#include <string>

// Callback when CURL runs and returns data, this catches it for use.
size_t web_scraper::WriteCallBack(void *content, const size_t size, const size_t amount, std::string *response) {
    const size_t total_size = size * amount;
    response -> append(static_cast<char *>(content), total_size);
    return total_size;
}

std::string web_scraper::get_website(const std::string &url) {
    CURL *curl = curl_easy_init();
    CURLcode result = CURLE_FAILED_INIT;
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            std::cerr <<"CURL Error: " << curl_easy_strerror(result) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::string web_scraper::extract_primary(const std::string &data) {
    const std::string search_start = "https://app.veo.co/matches/";
    const std::string search_end = "><";
    const std::string proceeding = "https://app.veo.co/api/app/matches/";
    std::string primary_url = extract_between(data, search_start, search_end);
    if (!primary_url.empty()) {
        primary_url.pop_back();
    }
    return proceeding + primary_url + "videos/";
}

std::string web_scraper::extract_secondary(const std::string &data) {
    //https://c.veocdn.com/9733ddad-9a2d-4155-8b92-76c6afe80ce5/panorama/transcode-4abfbb61-64a2-4bea-ad81-9eec4c420a10.mp4
    std::string search_text = "panorama/transcode";

    size_t search_pos = data.find(search_text);
    if (search_pos == std::string::npos) {
        return "";
    }

    size_t start_search = data.rfind("\"", search_pos);
    if (start_search == std::string::npos) {
        return "";
    }

    size_t end_search = data.find("\"", search_pos);
    if (end_search == std::string::npos) {
        return "";
    }

    return data.substr(start_search+1, end_search - start_search -1);
}

std::string web_scraper::extract_between(const std::string &data, const std::string &start, const std::string &end, size_t start_offset) {
    auto start_pos = data.find(start, start_offset);
    if (start_pos == std::string::npos) {
        return "";
    }
    start_pos += start.length();
    auto end_pos = data.find(end, start_pos);
    if (end_pos == std::string::npos) {
        return"";
    }
    return data.substr(start_pos, end_pos - start_pos);
}

std::string web_scraper::get_video_url(const std::string &input_url) {
    //first fetch primary url that is input
    const std::string primary_fetch = get_website(input_url);
    std::cout << "Fetching URL: " << input_url << std::endl;

    //from that data, it gives you an api request that contains the video link.
    const std::string first_url = extract_primary(primary_fetch);
    std::cout << "Found Video Being Loaded at:" << first_url << ". Fetching source..." << std::endl;

    //then from the api page, you parse it and get the source of the video
    const std::string secondary_fetch = get_website(first_url);
    std::cout << "Fetching Second URL: " << first_url << ". Finding Video Source" << std::endl;

    //then once you parse that api page and get the source of the video you can use it.
    const std::string video_url = extract_secondary(secondary_fetch);
    std::cout << "Found Source Video At: " << video_url << std::endl;

    return video_url;
}

