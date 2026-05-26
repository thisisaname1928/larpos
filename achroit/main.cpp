#include "achroit.hpp"
#include <exception>
#include <iostream>
int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "invalid agruments!\n";
    return -1;
  }
  try {
    ArchiotSrcManager s(argv[1]);
  } catch (std::exception &e) {
    std::cout << e.what() << '\n';
    return -1;
  }
  return 0;
}