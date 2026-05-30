#include "Config.h"

#include <XephTools/AppData.h>
#include <yaml-cpp/yaml.h>

#include <Windows.h>

#include <fstream>

#ifdef _DEBUG
#define CONFIG_PATH ".\\appdata\\tc\\config.yaml"
#else
#define CONFIG_PATH  _APPDATA_ + "\\tc\\config.yaml"
#endif

Config Config::Load()
{
    Config result;

    std::filesystem::path configPath = CONFIG_PATH;

    std::ifstream configFile(configPath);
    if (!configFile.is_open())
    {
        std::cout << "No config file found, creating... Please fill out the timecard location and pay rate within `config.yaml`";
        if (!std::filesystem::exists(configPath.parent_path()))
        {
            std::filesystem::create_directories(configPath.parent_path());
        }

        std::ofstream fileOut(configPath);
        fileOut << "timecard-path: \"\"\npay-rate: 0\nperiod-index: -1\n";
        
        fileOut.close();
        return Config();
    }

    YAML::Node root;

    try
    {
        root = YAML::Load(configFile);
    }
    catch (std::exception)
    {
        std::cout << "Error -- YAML syntax error in config file." << std::endl;
        return Config();
    }

    try
    {
        result.timecardFile = root["timecard-path"].as<std::string>();
        result.payRate = root["pay-rate"].as<float>();
        result.periodIndex = root["period-index"].as<int>();
    }
    catch (std::exception)
    {
        std::cout << "Error -- Could not parse config file." << std::endl;
        return Config();
    }

    if (result.timecardFile.empty())
    {
        std::cout << "A timecard file path must be specified...";
        return Config();
    }

    return result;
}

void Config::OpenPath()
{
    std::filesystem::path path = std::filesystem::absolute(CONFIG_PATH);
    std::string param = path.parent_path().string();
    ShellExecuteA(NULL, "open", "explorer", param.c_str(), NULL, SW_SHOWDEFAULT);
}

void Config::Save()
{
    YAML::Node root;
    root["timecard-path"] = timecardFile.string();
    root["pay-rate"] = payRate;
    root["period-index"] = periodIndex;

    std::ofstream fileOut(CONFIG_PATH);
    if (!fileOut.is_open())
    {
        std::cout << "Error -- Could not create or open config file at: " << CONFIG_PATH << std::endl;
        return;
    }

    fileOut << root;
}
