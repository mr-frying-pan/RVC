#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace constants {
  namespace opcode {
    const uint8_t OP_IMM = 0b0010011;
    const uint8_t LUI    = 0b0110111;
    const uint8_t AUIPC  = 0b0010111;
    const uint8_t OP     = 0b0110011;
  };

  namespace funct3 {
    const uint8_t ADDI  = 0b000;
    const uint8_t SLTI  = 0b010;
    const uint8_t SLTIU = 0b011;
    const uint8_t XORI  = 0b100;
    const uint8_t ORI   = 0b110;
    const uint8_t ANDI  = 0b111;
    const uint8_t SLLI  = 0b001;
    const uint8_t SRLI  = 0b101;
    const uint8_t SRAI  = 0b101;
    const uint8_t ADD   = 0b000;
    const uint8_t SLT   = 0b010;
    const uint8_t SLTU  = 0b011;
    const uint8_t AND   = 0b111;
    const uint8_t OR    = 0b110;
    const uint8_t XOR   = 0b100;
    const uint8_t SLL   = 0b001;
    const uint8_t SRL   = 0b101;
    const uint8_t SRA   = 0b101;
    const uint8_t SUB   = 0b000;
  };

  namespace funct7 {
    const uint8_t ZERO  = 0b000000;
    const uint8_t BIT30 = 0b010000;
  };

  namespace bitmasks {
    const uint32_t OPCODE = 0b00000000000000000000000001111111;
    const uint32_t RD     = 0b00000000000000000000111110000000;
    const uint32_t FUNCT3 = 0b00000000000000000111000000000000;
    const uint32_t RS1    = 0b00000000000011111000000000000000;
    const uint32_t RS2    = 0b00000001111100000000000000000000;
    const uint32_t FUNCT7 = 0b11111110000000000000000000000000;
  };
};

#endif
