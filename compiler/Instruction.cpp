#include "Instruction.hpp"

#include <cstdint>
#include <iostream>
#include "constants.hpp"

const Instruction Instruction::zero;

// R type
Instruction::Instruction(const uint32_t opcode,
			 const uint32_t funct3,
			 const uint32_t funct7,
			 const uint32_t rd,
			 const uint32_t rs1,
			 const uint32_t rs2)
  : value((opcode << 0)
	  | (funct3 << 12)
	  | (rd << 7)
	  | (rs1 << 15)
	  | (rs2 << 20)
	  | (funct7 << 25)) {}

// I type
Instruction::Instruction(const uint32_t opcode,
			 const uint32_t funct3,
			 const uint32_t rd,
			 const uint32_t rs1,
			 const uint32_t imm)
  : value(opcode << 0
	  | funct3 << 12
	  | rd << 7
	  | rs1 << 15
	  | imm << 20) {}

// U type
Instruction::Instruction(const uint32_t opcode, const uint32_t rd, const uint32_t imm)
  : value(opcode << 0
	  | rd << 7
	  | (imm & 0xFFFFF000)) {} // truncate the immediate

uint8_t Instruction::getOpcode() const {
  return this->value.to_ulong() & constants::bitmasks::OPCODE;
}

Instruction& Instruction::setOpcode(const uint32_t opcode) {
  // first clear, then set
  this->value = (this->value.to_ulong() & ~constants::bitmasks::OPCODE) | (opcode & constants::bitmasks::OPCODE);
  return *this;
}

uint8_t Instruction::getRd() const {
  return (this->value.to_ulong() & constants::bitmasks::RD) >> 7;
}

Instruction& Instruction::setRd(const uint32_t rd) {
  this->value = (this->value.to_ulong() & ~constants::bitmasks::RD) | ((rd & 0x1F) << 7);
  return *this;
}

uint8_t Instruction::getFunct3() const {
  return (this->value.to_ulong() & constants::bitmasks::FUNCT3) >> 12;
}

Instruction& Instruction::setFunct3(const uint32_t funct3) {
  this->value = (this->value.to_ulong() & ~constants::bitmasks::FUNCT3) | ((funct3 & 0x7) << 12);
  return *this;
}

uint8_t Instruction::getRs1() const {
  return (this->value.to_ulong() & constants::bitmasks::RS1) >> 15;
}

Instruction& Instruction::setRs1(const uint32_t rs1) {
  this->value = (this->value.to_ulong() & ~constants::bitmasks::RS1) | ((rs1 & 0x1F) << 15);
  return *this;
}

uint8_t Instruction::getRs2() const {
  return (this->value.to_ulong() & constants::bitmasks::RS2) >> 20;
}

Instruction& Instruction::setRs2(const uint32_t rs2) {
  this->value = (this->value.to_ulong() & ~constants::bitmasks::RS2) | ((rs2 & 0x1F) << 20);
  return *this;
}

uint8_t Instruction::getFunct7() const {
  return (this->value.to_ulong() & constants::bitmasks::FUNCT7) >> 25;
}

Instruction& Instruction::setFunct7(const uint32_t funct7) {
  this->value = (this->value.to_ulong() & ~constants::bitmasks::FUNCT7) | ((funct7 & 0x7F) << 25);
  return *this;
}

uint16_t Instruction::getImmI() const {
  return (static_cast<uint16_t>(getFunct7()) << 5) | static_cast<uint16_t>(getRs2());
}

Instruction& Instruction::setImmI(const uint32_t imm) {
  setFunct7(imm >> 5);
  setRs2(imm & 0x1F);
  return *this;
}

uint16_t Instruction::getImmS() const {
  return (static_cast<uint16_t>(getFunct7()) << 5) | static_cast<uint16_t>(getRd());
}

Instruction& Instruction::setImmS(const uint32_t imm) {
  setFunct7(imm >> 5);
  setRd(imm & 0x1F);
  return *this;
}

uint16_t Instruction::getImmB() const {
  uint16_t b12 = this->value[31];
  uint16_t b11 = this->value[7];
  return (b12 << 12)
    | (b11 << 11)
    | ((static_cast<uint16_t>(getFunct7()) & 0x3F) << 5)
    | (static_cast<uint16_t>(getRd()) & 0x1E);
}

Instruction& Instruction::setImmB(const uint32_t imm) {
  uint32_t b12 = (imm & 0x1000) >> 12;
  uint32_t b11 = (imm & 0x800) >> 11;
  setFunct7((b12 << 7) | ((imm & 0x7E0) >> 5));
  setRd(((imm & 0x1E) << 1) | b11);
  return *this;
}

uint32_t Instruction::getImmU() const {
  return this->value.to_ulong() & ~(constants::bitmasks::OPCODE | constants::bitmasks::RD);
}

Instruction& Instruction::setImmU(const uint32_t imm) {
  this->value = (this->value.to_ulong() & (constants::bitmasks::OPCODE | constants::bitmasks::RD))
    | (imm & 0xFFFFF000);
  return *this;
}

uint32_t Instruction::getImmJ() const {
  uint32_t b20 = this->value[31];
  uint32_t b11 = this->value[20];
  uint32_t b19_12 = this->value.to_ulong() & (constants::bitmasks::RS1 | constants::bitmasks::FUNCT3);
  return (b20 << 20)
    | b19_12
    | (b11 << 11)
    | ((this->value.to_ulong() & 0x7FE00000) >> 20);
}

Instruction& Instruction::setImmJ(const uint32_t imm) {
  uint32_t b20 = imm & 0x10000 >> 20;
  uint32_t b11 = imm & 0x800 >> 11;
  this->value = (this->value.to_ulong() & (constants::bitmasks::OPCODE | constants::bitmasks::RD)) // clear
    | (b20 << 31)
    | (((imm & 0x7FE) >> 1) << 21)
    | (b11 << 20)
    | (imm & 0xFF000);
  return *this;
}
