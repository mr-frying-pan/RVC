#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "Instruction.hpp"
#include "constants.hpp"

std::variant<std::tuple<Instruction, std::string>, std::string> parse(const std::string line);
std::string errline(const std::string expected, const std::string got);

std::tuple<uint8_t, uint8_t, uint16_t> parseR(std::istringstream &linestream);
std::variant<IType, std::string> parseI(std::istringstream &linestream, const uint8_t opcode, const uint8_t funct3);
std::tuple<uint8_t, uint8_t, uint16_t> parseS(std::istringstream &linestream);
std::tuple<uint8_t, uint8_t, uint16_t> parseB(std::istringstream &linestream);
std::tuple<uint8_t, uint8_t, uint16_t> parseU(std::istringstream &linestream);
std::tuple<uint8_t, uint8_t, uint16_t> parseJ(std::istringstream &linestream);

std::optional<uint8_t> parseRegister(const std::string str);
std::optional<uint32_t> parseImmediate(const std::string str);

int main(int argc, char *argv[]) {
  if(argc == 1) {
    std::cout << "No file given" << std::endl;
    return 1;
  }

  //std::vector<Instruction> code;
  
  std::ifstream in(argv[1]);
  for(std::string line; std::getline(in, line); ) {
    if(line == "")
      continue;
    auto result = parse(line);
    if(std::holds_alternative<std::string>(result))
      std::cerr << std::get<std::string>(result) << std::endl;
    else {
      auto [ instr, label ] = std::get<std::tuple<Instruction, std::string>>(result);
      std::cout << "( " << instr.getValue() << ", '" << label << "' )" << std::endl;
    }
  }

  //for(Instruction instr : code) {
    
  //}

  std::string outName(argc > 1 ? argv[1] : "prog.out");
  
  return 0;
}

std::variant<std::tuple<Instruction, std::string>, std::string> parse(const std::string line) {
  std::istringstream linestream(line);
  std::string token;
  linestream >> token;
  if(token.back() == ':')
    return std::make_tuple(Instruction::zero, token.substr(0, token.size() - 1));
  else if (token == "ADDI")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::ADDI);
  else if (token == "SLTI")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::SLTI);
  else if (token == "SLTIU")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::SLTIU);
  else if (token == "ANDI")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::ANDI);
  else if (token == "ORI")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::ORI);
  else if (token == "XORI")
    return parseI(linestream, constants::opcode::OP_IMM, constants::funct3::XORI);
  return "Unrecognized instruction: '" + token + "'";
}

std::variant<std::tuple<IType, std::string>, std::string> parseI(std::istringstream &linestream, const uint8_t opcode, const uint8_t funct3) {
  std::string srd, srs1, simm;
  linestream >> srd >> srs1 >> simm;
  auto rd = parseRegister(srd),
    rs1 = parseRegister(srs1);
  auto imm = parseImmediate(simm);
  if(!rd.has_value())
    return errline("register", srd);
  if(!rs1.has_value())
    return errline("register", srs1);
  if(!imm.has_value())
    return errline("number", simm);

  // TODO: add immediate size checks
  
  IType instr(opcode, funct3, rd.value(), rs1.value(), imm.value());
  return std::make_tuple(instr, "");
}

std::optional<uint8_t> parseRegister(const std::string str) {
  if(str.length() < 2 || str.length() > 3 || str[0] != 'x')
    return std::nullopt;

  std::string num = str.substr(1);
  uint32_t reg;
  try {
    reg = std::stoul(num);
    if (reg > 32) // number is bigger than there are registers.
      return std::nullopt;
  }
  catch(std::invalid_argument& e) {
    // could not convert to number. Not a proper register.
    return std::nullopt;
  }
  return reg;
}

std::optional<uint32_t> parseImmediate(const std::string str) {
  // TODO: add literal formats
  uint32_t reg;
  try {
    reg = std::stol(str);
  }
  catch(std::invalid_argument& e) {
    return std::nullopt;
  }
  return reg;
}

std::string errline(const std::string expected, const std::string got) {
  return "Expected " + expected + ", got '" + got + "'.";
}
