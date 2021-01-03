#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "Parser.hpp"

std::string& istring(std::string str);

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
    auto parseResult = parse(line);
    if(std::holds_alternative<std::string>(parseResult))
      std::cout << "LINE " << linenum << ": " << std::get<std::string>(parseResult) << std::endl;
    else {
      ParseResult pr = std::get<ParseResult>(parseResult);
      std::cout << " ( " <<  pr.instr.value.to_string() << ", '" << pr.label << "' ) " << line << std::endl;
    }
  }

  std::string outName(argc > 1 ? argv[1] : "prog.out");
  
  return 0;
}

std::string& istring(std::string str) {
  return str.insert(32 - 7, 1, ' ')
    .insert(32 - 12, 1, ' ')
    .insert(32 - 15, 1, ' ')
    .insert(32 - 20, 1, ' ');
}
