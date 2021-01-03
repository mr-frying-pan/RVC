#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bitset>

class Instruction {
public:
  static const Instruction zero;
  Instruction() = default;
  // R type
  Instruction(const uint32_t opcode, const uint32_t funct3, const uint32_t funct7, const uint32_t rd, const uint32_t rs1, const uint32_t imm);
  // I type
  Instruction(const uint32_t opcode, const uint32_t funct3, const uint32_t rd, const uint32_t rs1, const uint32_t imm);
  // U type
  Instruction(const uint32_t opcode, const uint32_t rd, const uint32_t imm);

  std::bitset<32> value;

  uint8_t getOpcode() const;
  Instruction& setOpcode(const uint32_t opcode);
  uint8_t getRd() const;
  Instruction& setRd(const uint32_t rd);
  uint8_t getFunct3() const;
  Instruction& setFunct3(const uint32_t funct3);
  uint8_t getRs1() const;
  Instruction& setRs1(const uint32_t rs1);
  uint8_t getRs2() const;
  Instruction& setRs2(const uint32_t rs2);
  uint8_t getFunct7() const;
  Instruction& setFunct7(const uint32_t funct7);

  uint16_t getImmI() const;
  Instruction& setImmI(const uint32_t imm);
  uint16_t getImmS() const;
  Instruction& setImmS(const uint32_t imm);
  uint16_t getImmB() const;
  Instruction& setImmB(const uint32_t imm);
  uint32_t getImmU() const;
  Instruction& setImmU(const uint32_t imm);
  uint32_t getImmJ() const;
  Instruction& setImmJ(const uint32_t imm);
};
#endif

