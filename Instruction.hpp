#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <bitset>

class Instruction {
protected:
  std::bitset<32> value;
public:
  uint32_t getValue() const;
};

class IType : public Instruction {
protected:
  IType(const uint8_t opcode, const uint8_t funct3, const uint8_t rd, const uint8_t rs1, const uint16_t imm);
};

class ADDI : public IType {
public:
  ADDI(const uint8_t rd, const uint8_t rs1, const uint16_t imm);
};
#endif

