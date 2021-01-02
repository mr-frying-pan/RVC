#include "Instruction.hpp"

#include <cstdint>
#include <iostream>
#include "constants.hpp"

const Instruction Instruction::zero;

uint32_t Instruction::getValue() const {
  return this->value.to_ulong();
}

IType::IType(const uint8_t opcode, const uint8_t funct3, const uint8_t rd, const uint8_t rs1, const uint16_t imm) {
  uint32_t sh_opcode = static_cast<uint32_t>(opcode) << 0,
    sh_funct3 = static_cast<uint32_t>(funct3) << 12,
    sh_rd = static_cast<uint32_t>(rd) << 7,
    sh_rs1 = static_cast<uint32_t>(rs1) << 15,
    sh_imm = static_cast<uint32_t>(imm) << 20;
  this->value |= sh_opcode | sh_funct3 | sh_rd | sh_rs1 | sh_imm;
  
  using std::cout;
  using std::endl;
  cout << "IType args: " << endl
       << "\topcode: " << (int)opcode << endl
       << "\tfunct3: " << (int)funct3 << endl
       << "\trd:     " << (int)rd << endl
       << "\trs1:    " << (int)rs1 << endl
       << "\timm:    " << (int)imm << endl;
  cout << "IType value: " << this->value.to_string() << endl;
  cout << "IType at:" << endl
       << "\t20: " << this->value[20] << endl
       << "\t15: " << this->value[15] << endl
       << "\t12: " << this->value[12] << endl
       << "\t7:  " << this->value[7]  << endl
       << "\t0:  " << this->value[0]  << endl;
  
}
