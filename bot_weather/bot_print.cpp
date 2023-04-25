#include "../ft_irc.hpp"

void	print_sunny()
{
	std::cout << "\033[38;5;226m    \\   /    \033[0m" << std::endl;
	std::cout << "\033[38;5;226m     .-.     \033[0m" << std::endl;
	std::cout << "\033[38;5;226m  ‒ (   ) ‒  \033[0m" << std::endl;
	std::cout << "\033[38;5;226m     `-᾿     \033[0m" << std::endl;
	std::cout << "\033[38;5;226m    /   \\    \033[0m" << std::endl;
}

std::string get_sunny()
{
    std::string result;
    result += "\033[38;5;226m    \\   /    \033[0m\n";
    result += "\033[38;5;226m     .-.     \033[0m\n";
    result += "\033[38;5;226m  ‒ (   ) ‒  \033[0m\n";
    result += "\033[38;5;226m     `-᾿     \033[0m\n";
    result += "\033[38;5;226m    /   \\    \033[0m\n";
    return result;
}

void print_ThunderyHeavyRai()
{
    std::cout << "\033[38;5;240;1m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚\033[38;5;228;5m⚡\033[38;5;21;25m‚ʻ   \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚ʻ   \033[0m" << std::endl;
}

std::string get_ThunderyHeavyRai()
{
    std::stringstream ss;
    ss << "\033[38;5;240;1m     .-.     \033[0m\n";
    ss << "\033[38;5;240;1m    (   ).   \033[0m\n";
    ss << "\033[38;5;240;1m   (___(__)  \033[0m\n";
    ss << "\033[38;5;21;1m  ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚\033[38;5;228;5m⚡\033[38;5;21;25m‚ʻ   \033[0m\n";
    ss << "\033[38;5;21;1m  ‚ʻ‚ʻ\033[38;5;228;5m⚡\033[38;5;21;25mʻ‚ʻ   \033[0m\n";
    return ss.str();
}

void print_unknown()
{
    std::cout << "    .-.      " << std::endl;
    std::cout << "     __)     " << std::endl;
    std::cout << "    (        " << std::endl;
    std::cout << "     `-᾿     " << std::endl;
    std::cout << "      •      " << std::endl;
}

std::string get_unknown()
{
    std::stringstream ss;
    ss << "    .-.      \n";
    ss << "     __)     \n";
    ss << "    (        \n";
    ss << "     `-᾿     \n";
    ss << "      •      \n";
    return ss.str();
}

void print_cloudy()
{
    std::cout << "             " << std::endl;
    std::cout << "\033[38;5;250m     .--.    \033[0m" << std::endl;
    std::cout << "\033[38;5;250m  .-(    ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;250m (___.__)__) \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}

std::string get_cloudy()
{
    std::string output;
    output += "             \n";
    output += "\033[38;5;250m     .--.    \033[0m\n";
    output += "\033[38;5;250m  .-(    ).  \033[0m\n";
    output += "\033[38;5;250m (___.__)__) \033[0m\n";
    output += "             \n";
    return output;
}

void print_fog()
{
    std::cout << "             " << std::endl;
    std::cout << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    std::cout << "\033[38;5;251m  _ - _ - _  \033[0m" << std::endl;
    std::cout << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    std::cout << "             " << std::endl;
}

std::string get_fog()
{
    std::stringstream output;
    output << "             " << std::endl;
    output << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    output << "\033[38;5;251m  _ - _ - _  \033[0m" << std::endl;
    output << "\033[38;5;251m _ - _ - _ - \033[0m" << std::endl;
    output << "             " << std::endl;
    return output.str();
}

void print_heavy_rain()
{
    std::cout << "\033[38;5;240;1m     .-.     \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m    (   ).   \033[0m" << std::endl;
    std::cout << "\033[38;5;240;1m   (___(__)  \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m" << std::endl;
    std::cout << "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m" << std::endl;
}

std::string get_heavy_rain()
{
    std::string output;
    output += "\033[38;5;240;1m     .-.     \033[0m\n";
    output += "\033[38;5;240;1m    (   ).   \033[0m\n";
    output += "\033[38;5;240;1m   (___(__)  \033[0m\n";
    output += "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m\n";
    output += "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m\n";
    return output;
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

std::string get_heavy_snow()
{
    std::string output = "";
    output += "\033[38;5;240;1m     .-.     \033[0m\n";
    output += "\033[38;5;240;1m    (   ).   \033[0m\n";
    output += "\033[38;5;240;1m   (___(__)  \033[0m\n";
    output += "\033[38;5;255;1m   * * * *   \033[0m\n";
    output += "\033[38;5;255;1m  * * * *    \033[0m\n";
    return output;
}

void print_heavy_snow_showers()
{
    std::cout << "\033[38;5;226m _`/\"\"\033[38;5;240;1m.-.    \033[0m" << std::endl;
    std::cout << "\033[38;5;226m  ,\\_\033[38;5;240;1m(   ).  \033[0m" << std::endl;
    std::cout << "\033[38;5;226m   /\033[38;5;240;1m(___(__) \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m    * * * *  \033[0m" << std::endl;
    std::cout << "\033[38;5;255;1m   * * * *   \033[0m" << std::endl;
}

std::string get_heavy_snow_showers()
{
    std::stringstream ss;
    ss << "\033[38;5;226m _`/\"\"\033[38;5;240;1m.-.    \033[0m" << std::endl;
    ss << "\033[38;5;226m  ,\\_\033[38;5;240;1m(   ).  \033[0m" << std::endl;
    ss << "\033[38;5;226m   /\033[38;5;240;1m(___(__) \033[0m" << std::endl;
    ss << "\033[38;5;255;1m    * * * *  \033[0m" << std::endl;
    ss << "\033[38;5;255;1m   * * * *   \033[0m" << std::endl;
    return ss.str();
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