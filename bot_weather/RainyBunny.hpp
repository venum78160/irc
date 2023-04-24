#pragma once

#include "../ft_irc.hpp"
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iomanip>


std::string urlencode(const std::string& str);
std::map<std::string, std::string> parse_json(std::string json_str);
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
std::string get_weather(const char* city, const char* key);
void    display_weather(std::string id);
void	start_bot();
void	start_bot(std::string request);
void	print_sunny();
void    print_ThunderyHeavyRai();
void    print_unknown();
void    print_cloudy();
void    print_fog();
void    print_heavy_rain();
void    print_heavy_showers();
void    print_heavy_snow();
void    print_heavy_snow_showers();
void    print_light_rain();
void    print_light_showers();
void    print_light_sleet();
void    print_light_sleet_showers();
void    print_light_snow();
void    print_light_snow_showers();
void    print_partly_cloudy();
void    print_thundery_showers();
void    print_ThunderySnowShowers();
void    print_very_cloudy();
