#include "achroit.hpp"
#include "third-party/json.hpp"
#include <cerrno>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::string trynaGetSmthStr(nlohmann::json &data, const std::string &wut) {
  if (data.contains(wut) && data[wut].is_string()) {
    return data[wut].get<std::string>();
  }
  throw std::runtime_error("ERROR:PARSE_ERROR");
}

std::vector<std::string> trynaGetSmthStrArr(nlohmann::json &data,
                                            const std::string &wut) {
  if (data.contains(wut) && data[wut].is_array()) {
    return data[wut].get<std::vector<std::string>>();
  }
  throw std::runtime_error("ERROR:PARSE_ERROR");
}

ArchiotJson parseArchoitJson(std::string path) {
  // shadow copy
  std::string fnPath = path + ".tmp";
  std::filesystem::copy(path, fnPath,
                        std::filesystem::copy_options::overwrite_existing);

  std::ifstream file(fnPath);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR::CANT_OPEN_DEST_FILE");
  }

  try {
    nlohmann::json data;

    file >> data;

    ArchiotJson s{.componentName = trynaGetSmthStr(data, "componentName"),
                  .dependencies = trynaGetSmthStrArr(data, "dependencies"),
                  .buildCommand = trynaGetSmthStr(data, "buildCommand"),
                  .buildPath = std::filesystem::path(fnPath).parent_path()};

    return s;
  } catch (std::exception &e) {
    throw std::runtime_error(e.what());
  }

  return ArchiotJson{};
}