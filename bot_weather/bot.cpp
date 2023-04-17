#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>



#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cctype>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::stringstream;
using std::vector;



#include <iostream>
#include <string>
#include <map>

#include <iostream>
#include <string>
#include <map>

#include <string>
#include <sstream>
#include <iomanip>

void	print_sunny()
{
	std::cout << "\033[38;5;226m    \\   /    \033[0m" << std::endl;
	std::cout << "\033[38;5;226m     .-.     \033[0m" << std::endl;
	std::cout << "\033[38;5;226m  ‒ (   ) ‒  \033[0m" << std::endl;
	std::cout << "\033[38;5;226m     `-᾿     \033[0m" << std::endl;
	std::cout << "\033[38;5;226m    /   \\    \033[0m" << std::endl;
}

void print_ThunderyHeavyRai()
{
    std::cout << "\033[38;5;240;1m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚\033[38;5;228;5m⚡\033[38;5;21;25m‚ʻ   \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚ʻ   \033[0m" << std::endl;
}

void print_unknown()
{
    std::cout << "    .-.      " << std::endl;
    std::cout << "     __)     " << std::endl;
    std::cout << "    (        " << std::endl;
    std::cout << "     `-᾿     " << std::endl;
    std::cout << "      •      " << std::endl;
}

void print_cloudy()
{
    std::cout << "             " << std::endl;
    std::cout << "\033[38;5;250m     .--.    \033[0m" << std::endl;
    std::cout << "\033[38;5;250m  .-(    ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;250m (___.__)__) \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}

void print_fog()
{
    std::cout << "             " << std::endl;
    std::cout << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    std::cout << "\033[38;5;251m  _ - _ - _  \033[0m" << std::endl;
    std::cout << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}

void print_heavy_rain()
{
    std::cout << "\033[38;5;240;1m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m" << std::endl;
}

void print_heavy_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;240;1m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;240;1m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;240;1m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m   ‚ʻ‚ʻ‚ʻ‚ʻ  \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m   ‚ʻ‚ʻ‚ʻ‚ʻ  \033[0m" << std::endl;
}

void print_heavy_snow()
{
    std::cout << "\033[38;5;240;1m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m   * * * *   \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m  * * * *    \033[0m" << std::endl;
}

void print_heavy_snow_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;240;1m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;240;1m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;240;1m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m    * * * *  \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m   * * * *   \033[0m" << std::endl;
}

void print_light_rain()
{
    std::cout << "\033[38;5;250m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;250m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;250m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;111m    ʻ ʻ ʻ ʻ  \033[0m" << std::endl;
    std::cout << "\033[38;5;111m   ʻ ʻ ʻ ʻ   \033[0m" << std::endl;
}

void print_light_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;111m     ʻ ʻ ʻ ʻ \033[0m" << std::endl;
    std::cout << "\033[38;5;111m    ʻ ʻ ʻ ʻ  \033[0m" << std::endl;
}

void print_light_sleet()
{
    std::cout << "\033[38;5;250m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;250m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;250m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;111m    ʻ \033[38;5;255m*\033[38;5;111m ʻ \033[38;5;255m*  \033[0m" << std::endl;
    std::cout << "\033[38;5;255m   *\033[38;5;111m ʻ \033[38;5;255m*\033[38;5;111m ʻ   \033[0m" << std::endl;
}

void print_light_sleet_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;111m     ʻ \033[38;5;255m*\033[38;5;111m ʻ \033[38;5;255m* \033[0m" << std::endl;
    std::cout << "\033[38;5;255m    *\033[38;5;111m ʻ \033[38;5;255m*\033[38;5;111m ʻ  \033[0m" << std::endl;
}

void print_light_snow()
{
    std::cout << "\033[38;5;250m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;250m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;250m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;255m    *  *  *  \033[0m" << std::endl;
    std::cout << "\033[38;5;255m   *  *  *   \033[0m" << std::endl;
}

void print_light_snow_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;255m     *  *  * \033[0m" << std::endl;
    std::cout << "\033[38;5;255m    *  *  *  \033[0m" << std::endl;
}

void print_partly_cloudy()
{
    std::cout << "\033[38;5;226m   \\  /\033[0m      " << std::endl;
    std::cout << "\033[38;5;226m _ /\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   \\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}

void print_thundery_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;228;5m    ⚡\033[38;5;111;25mʻ ʻ\033[38;5;228;5m⚡\033[38;5;111;25mʻ ʻ \033[0m" << std::endl;
    std::cout << "\033[38;5;111m    ʻ ʻ ʻ ʻ  \033[0m" << std::endl;
}

void print_ThunderySnowShowers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;250m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;250m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;250m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;255m     *\033[38;5;228;5m⚡\033[38;5;255;25m *\033[38;5;228;5m⚡\033[38;5;255;25m * \033[0m" << std::endl;
    std::cout << "\033[38;5;255m    *  *  *  \033[0m" << std::endl;
}

void print_very_cloudy()
{
    std::cout << "             " << std::endl;
    std::cout << "\033[38;5;240;1m     .--.    \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m  .-(    ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m (___.__)__) \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}





std::string urlencode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
        // Any other characters are percent-encoded
        else {
            escaped << '%' << std::setw(2) << int((unsigned char) c);
        }
    }

    return escaped.str();
}


std::map<std::string, std::string> parse_json(std::string json_str) {
    std::map<std::string, std::string> result;

    // Recherche de la température
    std::size_t pos_temp = json_str.find("\"temp\":");
    if (pos_temp != std::string::npos) {
        std::size_t start_temp = pos_temp + 7;
        std::size_t end_temp = json_str.find(',', start_temp);
        std::string temp_str = json_str.substr(start_temp, end_temp - start_temp);
        result["temp"] = temp_str;
    }

    // Recherche de la température ressentie
    std::size_t pos_feels_like = json_str.find("\"feels_like\":");
    if (pos_feels_like != std::string::npos) {
        std::size_t start_feels_like = pos_feels_like + 13;
        std::size_t end_feels_like = json_str.find(',', start_feels_like);
        std::string feels_like_str = json_str.substr(start_feels_like, end_feels_like - start_feels_like);
        result["feels_like"] = feels_like_str;
    }

    // Recherche de la description
    std::size_t pos_description = json_str.find("\"description\":");
    if (pos_description != std::string::npos) {
        std::size_t start_description = pos_description + 15;
        std::size_t end_description = json_str.find('"', start_description);
        std::string description_str = json_str.substr(start_description, end_description - start_description);
        result["description"] = description_str;
    }

    // Recherche du nom de la ville
    std::size_t pos_name = json_str.find("\"name\":");
    if (pos_name != std::string::npos) {
        std::size_t start_name = pos_name + 8;
        std::size_t end_name = json_str.find('"', start_name);
        std::string name_str = json_str.substr(start_name, end_name - start_name);
        result["name"] = name_str;
    }

	std::size_t pos_id = json_str.find("\"id\":");
    if (pos_id != std::string::npos) {
        std::size_t start_id = pos_id + 5;
        std::size_t end_id = json_str.find(',', start_id);
        std::string id_str = json_str.substr(start_id, end_id - start_id);
        result["id"] = id_str;
    }

    return result;
}


size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    ((std::string*)userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string get_weather(const char* city, const char* key) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
	{
        std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + urlencode(city) + "&appid=" + std::string(key) + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
		return response;
    }
    return "";
}

int main() {
    const char* city = "Caen";
    const char* key = "b693c063ba0578ae1fc1907e4426a73f";

    std::string response = get_weather(city, key);
    std::cout << response << std::endl;
	// parse_weather_info(response);
	std::map<std::string, std::string> result = parse_json(response);
	std::cout << "Name: " << result["name"] << std::endl;
	std::cout << "Id: " << result["id"] << std::endl;
	std::cout << "Description: " << result["description"] << std::endl;
	std::cout << "Temperature: " << result["temp"] << "°C"<< std::endl;
	std::cout << "Feels_like: " << result["feels_like"] << "°C"<< std::endl;


    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@******/#@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@      ,@@@**********//@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@              ,@&*******//#@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@,                 @@********(@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@&                   @@   @@@******//@@@@" << std::endl;
    std::cout << "@@@@@@@@@@                    #@      @@*****//@@@" << std::endl;
    std::cout << "@@@@@@@@  @@                            @@***//@@@" << std::endl;
    std::cout << "@@@@@@@               @@@@(@@@@     /@@,    @@@@@@" << std::endl;
    std::cout << "@@@@@,    (@@       @@                         ,@@" << std::endl;
    std::cout << "@@@          @@                                  @" << std::endl;
    std::cout << "@@                                               @" << std::endl;
    std::cout << "@@        *@,,,,,,,//@@        @,,,,,,,/*@@     @@" << std::endl;
    std::cout << "@@@@      @/,,,,,,//@@        @&,,,,,,//@@    @@@@" << std::endl;
    std::cout << "@@@@@@@@@@@,,,,,,//@@@@@@@@@@@@,,,,,,//@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@,,,,,,,,,,/@@@@@@@@@,,,,,,,,,,/@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@,,,,,,*@@@@@@@@@@@@@,,,,,,*%@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@,,/@@@@@@@@@@@@@@@@@,,/@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@,/@@@@@@@@@@@@@@@@@@,/@@@@@@@@@@@@@@@" << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

std::cout << "unknown :" << std::endl;
print_unknown();
std::cout << "cloudy :" << std::endl;
print_cloudy();
std::cout << "fog :" << std::endl;
print_fog();
std::cout << "heavy rain :" << std::endl;
print_heavy_rain();
std::cout << "heavy showers :" << std::endl;
print_heavy_showers();
std::cout << "heavy snow :" << std::endl;
print_heavy_snow();
std::cout << "heavy snow showers :" << std::endl;
print_heavy_snow_showers();
std::cout << "light rain :" << std::endl;
print_light_rain();
std::cout << "light showers :" << std::endl;
print_light_showers();
std::cout << "light sleet :" << std::endl;
print_light_sleet();
std::cout << "light sleet showers :" << std::endl;
print_light_sleet_showers();
std::cout << "light snow :" << std::endl;
print_light_snow();
std::cout << "light snow showers :" << std::endl;
print_light_snow_showers();
std::cout << "partly cloudy :" << std::endl;
print_partly_cloudy();
std::cout << "sunny :" << std::endl;
print_sunny();
std::cout << "thundery heavy rain :" << std::endl;
print_ThunderyHeavyRai();
std::cout << "thundery showers :" << std::endl;
print_thundery_showers();
std::cout << "thundery snow showers :" << std::endl;
print_ThunderySnowShowers();
std::cout << "very cloudy :" << std::endl;
print_very_cloudy();




    return 0;
}

// thundery heavy rain :
// fog
// heavy rain :
// heavy snow :
// heavy snow showers :
// sunny
// cloudy
