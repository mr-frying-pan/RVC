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
    // Parse line, get instruction and its parameters.
    ParseNode parseNode = parse(line);

    // Check instruction and parameter types.
    auto error = check(parseNode);
    if (error.get() != nullptr)
      std::cout << "LINE " << linenum << ": " << error.get()->to_string() << std::endl;
    std::cout << line << std::endl;
  }

  std::string outName(argc > 1 ? argv[1] : "prog.out");
  
  return 0;
}
