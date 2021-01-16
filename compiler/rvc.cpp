#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "Parser.hpp"
#include "Checker.hpp"

int main(int argc, char *argv[]) {
  // TODO: add argument for non-stop compilation and stop on first error if not given
  if(argc == 1) {
    std::cout << "No file given" << std::endl;
    return 1;
  }
  
  std::ifstream in(argv[1]);
  int linenum = 1;
  for(std::string line; std::getline(in, line); linenum++) {
    if(line == "")
      continue;
    ParseNode parseNode = parse(line);
    auto error = check(parseNode);
    if (error.has_value())
      std::cout << "LINE " << linenum << ": " << error.value() << std::endl;
    std::cout << line << std::endl;
  }

  std::string outName(argc > 1 ? argv[1] : "prog.out");
  
  return 0;
}
