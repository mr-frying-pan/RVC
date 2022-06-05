#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>
#include <utility> // std::pair

#include "Instruction.hpp"
#include "Parser.hpp"

/**
 * Compiles instruction into machine code.
 *
 * Does not do linking – if instruction has a label parameter it will
 * not be checked if it is defined, and will not be included in the
 * resulting instruction. This will have to be done by linker.
 *
 * @param node    parsed and checked assemblly instruction
 * @returns       compiled instruction and (optionally) a label referenced in it.
 *                If label is not referenced, then empty string is returned in its place.
 *                If instruction is a label itself, then Instruction::zero is produced as first member.
 */
std::pair<Instruction, std::string> compile(const ParseNode& node);

#endif
