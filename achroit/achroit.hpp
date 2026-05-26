#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct {
  std::string componentName;
  std::vector<std::string> dependencies;
  std::string buildCommand;
  std::string buildPath;
} ArchiotJson;

ArchiotJson parseArchoitJson(std::string path);

class ArchiotSrcManager {
public:
  ArchiotSrcManager(const std::string &path);
  bool recursiveBuild(std::string component);

private:
  std::string path;
  ArchiotJson curComponent;
  std::unordered_map<std::string, ArchiotJson> components;
  std::unordered_map<std::string, bool> componentsBuildCheck;
  std::unordered_map<std::string, bool> isOrderBuild;
};