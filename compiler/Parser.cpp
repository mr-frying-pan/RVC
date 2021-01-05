#include "Parser.hpp"

#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "constants.hpp"

RegisterOperand::RegisterOperand(const uint8_t val) noexcept : val(val) {}

uint8_t RegisterOperand::value() const noexcept {
  return val;
}

std::optional<RegisterOperand> RegisterOperand::tryConstruct(const std::string& str) noexcept {
  if(str.length() < 2 || str.length() > 3 || str[0] != 'x')
    return std::nullopt;

  std::string num = str.substr(1);
  uint32_t reg;
  try {
    reg = std::stoul(num);
    if (reg > 31) // number is bigger than there are registers.
      return std::nullopt;
  }
  catch(std::invalid_argument& e) {
    // could not convert to number. Not a proper register.
    return std::nullopt;
  }
  return RegisterOperand { static_cast<uint8_t>(reg) };
}

ImmediateOperand::ImmediateOperand(const uint32_t val) noexcept : val(val) {}

uint32_t ImmediateOperand::value() const noexcept {
  return val;
}

std::optional<ImmediateOperand> ImmediateOperand::tryConstruct(const std::string& str) noexcept {
  // TODO: add literal formats
  uint32_t reg;
  try {
    reg = std::stol(str);
  }
  catch(std::invalid_argument& e) {
    return std::nullopt;
  }
  return ImmediateOperand { reg };
}

LabelOperand::LabelOperand(const std::string &lbl) noexcept : val(lbl) {}

std::string LabelOperand::value() const noexcept {
  return val;
}

ParseNode parse(const std::string& line) noexcept {
  std::vector<std::string> tokens;
  std::istringstream linestream(line);
  ParseNode node;
  std::string instr;
  linestream >> instr;

  if (instr.back() == ':') {
    node.label = true;
    node.instr = instr.substr(0, instr.size() - 1);
    return node;
  }
  node.instr = instr;
  for(std::string token; linestream >> token; ) {
    auto opt_reg = RegisterOperand::tryConstruct(token);
    if (opt_reg.has_value()) {
      node.operands.push_back(opt_reg.value());
      continue;
    }
    auto opt_imm = ImmediateOperand::tryConstruct(token);
    if (opt_imm.has_value()) {
      node.operands.push_back(opt_imm.value());
      continue;
    }
    node.operands.push_back(LabelOperand { token });
  }
  return node;
}

// FOR USE IN COMPILATION STEP

// std::string errline(const std::string expected, const std::string got) {
//   return "Expected " + expected + ", got '" + got + "'.";
// }

// std::optional<uint8_t> parseRegister(const std::string str) {

// }

// std::optional<uint32_t> parseImmediate(const std::string str) {

// }

// std::variant<ParseResult, std::string> parseR(const std::vector<std::string> &tokens, const uint8_t opcode, const uint8_t funct3, const uint32_t funct7) {
//   if (tokens.size() != 4)
//     return errline("4 arguments", std::to_string(tokens.size()));
//   auto rd = parseRegister(tokens[1]),
//     rs1 = parseRegister(tokens[2]),
//     rs2 = parseRegister(tokens[3]);
//   if (!rd.has_value())
//     return errline("register", tokens[1]);
//   if (!rs1.has_value())
//     return errline("register", tokens[2]);
//   if (!rs2.has_value())
//     return errline("register", tokens[3]);

//   return ParseResult { Instruction { opcode, funct3, funct7, rd.value(), rs1.value(), rs2.value() }, "" };
// }

// std::variant<ParseResult, std::string> parseI(const std::vector<std::string> &tokens, const uint8_t opcode, const uint8_t funct3) {
//   if (tokens.size() != 4)
//     return errline("4 arguments", std::to_string(tokens.size()));
//   auto rd = parseRegister(tokens[1]),
//     rs1 = parseRegister(tokens[2]);
//   auto imm = parseImmediate(tokens[3]);
//   if(!rd.has_value())
//     return errline("register", tokens[1]);
//   if(!rs1.has_value())
//     return  errline("register", tokens[2]);
//   if(!imm.has_value())
//     return errline("number", tokens[3]);

//   // TODO: add immediate size checks

//   return ParseResult { Instruction { opcode, funct3, rd.value(), rs1.value(), imm.value() }, "" };
// }

// std::variant<ParseResult, std::string> parseShift(const std::vector<std::string> &tokens, const uint8_t opcode, const uint8_t funct3, const uint8_t funct7) {
//   auto res = parseI(tokens, opcode, funct3);
//   if (std::holds_alternative<ParseResult>(res)) {
//     ParseResult r = std::get<ParseResult>(res);
//     r.instr.setFunct7(funct7);
//     return r;
//   }
//   return res;
// }

// std::variant<ParseResult, std::string> parseU(const std::vector<std::string> &tokens, const uint8_t opcode) {
//   if (tokens.size() != 3)
//     return errline("3 arguments", std::to_string(tokens.size()));

//   auto rd = parseRegister(tokens[1]);
//   auto imm = parseImmediate(tokens[2]);
//   if (!rd.has_value())
//     return errline("register", tokens[1]);
//   if (!imm.has_value())
//     return errline("number", tokens[1]);

//   // TODO: immediate checks and warnings

//   return ParseResult { Instruction { opcode, rd.value(), imm.value() }, "" };
// }

// const std::unordered_map<std::string, std::function<std::variant<ParseResult, std::string>(const std::vector<std::string>&)>> parseFunctions = {
//   {"ADDI",  [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::ADDI);
//   }},
//   {"SLTI",  [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::SLTI);
//   }},
//   {"SLTIU", [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::SLTIU);
//   }},
//   {"ANDI", [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::ANDI);
//   }},
//   {"ORI", [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::ORI);
//   }},
//   {"XORI", [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP_IMM, constants::funct3::XORI);
//   }},
//   {"SLLI", [] (const std::vector<std::string> &tokens) {
//       return parseShift(tokens, constants::opcode::OP_IMM, constants::funct3::SLLI, constants::funct7::ZERO);
//     }},
//   {"SRLI", [] (const std::vector<std::string> &tokens) {
//       return parseShift(tokens, constants::opcode::OP_IMM, constants::funct3::SRLI, constants::funct7::ZERO);
//     }},
//   {"SRAI", [] (const std::vector<std::string> &tokens) {
//       return parseShift(tokens, constants::opcode::OP_IMM, constants::funct3::SRAI, constants::funct7::BIT30);
//     }},
//   {"LUI", [] (const std::vector<std::string> &tokens) {
//     return parseU(tokens,constants::opcode::LUI);
//   }},
//   {"AUIPC", [] (const std::vector<std::string> &tokens) {
//     return parseU(tokens,constants::opcode::AUIPC);
//   }},
//   {"ADD", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::ADD, constants::funct7::ZERO);
//   }},
//   {"SLT", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SLT, constants::funct7::ZERO);
//   }},
//   {"SLTU", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SLTU, constants::funct7::ZERO);
//   }},
//   {"AND", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::AND, constants::funct7::ZERO);
//   }},
//   {"OR", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::OR, constants::funct7::ZERO);
//   }},
//   {"XOR", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::XOR, constants::funct7::ZERO);
//   }},
//   {"SLL", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SLL, constants::funct7::ZERO);
//   }},
//   {"SRL", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SRL, constants::funct7::ZERO);
//   }},
//   {"SUB", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SUB, constants::funct7::BIT30);
//   }},
//   {"SRA", [] (const std::vector<std::string> &tokens) {
//     return parseR(tokens, constants::opcode::OP, constants::funct3::SRA, constants::funct7::BIT30);
//   }}
//   {"JAL", [] (const std::vector<std::string> &tokens) {
//     return parseJ(tokens, constants::opcode::OP);
//   }},
//   {"JALR", [] (const std::vector<std::string> &tokens) {
//     return parseI(tokens, constants::opcode::OP, constants::funct3::SRA, constants::funct7::BIT30);
//   }}
// };

// std::variant<ParseResult, std::string> parse(const std::string line) {
//   std::vector<std::string> tokens;
//   std::istringstream linestream(line);
//   for (std::string token; linestream >> token; tokens.push_back(token));

//   try {
//     return parseFunctions.at(tokens[0])(tokens);
//   }
//   catch (std::out_of_range &e) {
//     if (tokens[0].back() == ':')
//       return ParseResult { Instruction::zero, tokens[0].substr(0, tokens[0].length() - 1) };
//     return "Unrecognized instruction: '" + tokens[0] + "'";
//   }
// }
