#include "achroit.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>

#define ACHROIT_FN "/achroit.json"

int buildComponent(ArchiotJson &comp, std::string buildDir) {
  std::string cmd = "cd \"" + buildDir + "\" && " + comp.buildCommand;
  int retcode = std::system(cmd.c_str());

  return retcode;
}

bool ArchiotSrcManager::recursiveBuild(std::string component) {
  if (componentsBuildCheck[component]) {
    return true;
  }

  if (isOrderBuild[component]) {
    std::cout << "[ERROR] Circular dependency detected on: " << component
              << "! Build stopped.\n";
    return false;
  }

  isOrderBuild[component] = true;

  std::vector<std::string> currentDeps = components[component].dependencies;
  for (const auto &comp : currentDeps) {
    if (!recursiveBuild(comp)) {
      return false;
    } else
      componentsBuildCheck[comp] = true;
  }

  isOrderBuild[component] = false;

  std::cout << "[INFO] Building component " << component << "\n";
  int buildSuccess =
      buildComponent(components[component], components[component].buildPath);

  if (buildSuccess == 0) {
    componentsBuildCheck[component] = true;
    return true;
  }

  return false;
}

ArchiotSrcManager::ArchiotSrcManager(const std::string &path) : path(path) {
  try {
    curComponent = parseArchoitJson(this->path + ACHROIT_FN);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    throw std::runtime_error("ERROR::BAD_ROOT_COMPONENT");
  }

  std::cout << "[INFO] Starting indexing " << curComponent.componentName
            << "'s sub components\n";

  // parsing sub components
  for (auto dir : std::filesystem::directory_iterator(this->path + "/")) {
    if (!dir.is_directory())
      continue;
    ArchiotJson curIComp;
    std::string fn = static_cast<std::string>(dir.path()) + ACHROIT_FN;
    try {
      curIComp = parseArchoitJson(fn);

    } catch (...) {
      if (std::filesystem::exists(fn)) {
        std::cout << "[WARN] Component indexing is failed on " << dir.path()
                  << "! pass\n";
      }
      continue;
    }

    std::cout << "[INFO] Found component " << curIComp.componentName << "!\n";
    components[curIComp.componentName] = curIComp;
  }

  // do a check on dependencies of root component
  for (auto i : curComponent.dependencies) {
    if (components.find(i) == components.cend()) {
      std::cout << "[ERRO] Component " << i << " is missing!";
      throw std::runtime_error("ERROR::COMPONENT_MISSING");
    }
  }

  // build
  for (auto i : curComponent.dependencies) {
    bool res = recursiveBuild(i);

    if (!res) {
      std::cout << "[ERRO] Build failed!\n";
      break;
    }
  }

  int retcode = buildComponent(curComponent, curComponent.buildPath);
  if (retcode == 0)
    std::cout << "[INFO] Build success!\n";
  else
    std::cout << "[ERRO] Build failed!\n";
}
