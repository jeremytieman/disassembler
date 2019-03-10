#include "..\inc\Disassembler8080.h"

#include <sstream>

namespace CodexMachina
{
  inline void printByte(std::stringstream& ss, unsigned char byte)
  {
    ss << ' ' << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(byte);
  }

  void Disassembler8080::disassembleImpl(std::ostream& ostream, const bool verbose)
  {
    const auto width = addressPrintingWidth(_addressBusSize);
    std::size_t memOffset = 0;

    while (!_eof)
    {
      const auto& buffer = nextBuffer();

      for (size_t i = 0; i < buffer.size(); ++i)
      {
        std::stringstream ss{ std::ios::out };
        ss << std::setfill('0') << std::setw(width) << std::hex << memOffset + i;

        switch (const auto op = buffer[i]; op)
        {
        case 0x00:
          printByte(ss, buffer[i]);
          ss << std::setfill(' ') << std::setw(6) << ' ' << " NOP";
          break;
        case 0x01:
        {
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          const int instWidth = verbose ? 16 : 0;
          ss << std::setfill(' ') << std::setw(instWidth) << std::left << " LXI B";
          if (verbose) { ss << "; Load B <- byte 2 and C <- byte 1"; }
          break;
        }
        case 0x02:
        {
          printByte(ss, buffer[i]);
          const int instWidth = verbose ? 16 : 0;
          ss << std::setfill(' ') << std::setw(6) << ' ' << std::setw(instWidth) << std::left << " STAX B";
          if (verbose) { ss << "; Store register A -> (BC)"; }
          break;
        }
        case 0x03:
        {
          printByte(ss, buffer[i]);
          const int instWidth = verbose ? 16 : 0;
          ss << std::setfill(' ') << std::setw(6) << ' ' << std::setw(instWidth) << std::left << " INX B";
          if (verbose) { ss << "; Increment registers B and C"; }
          break;
        }
        case 0x04:
          printByte(ss, buffer[i]);
          ss << "       INR B ; Increment register B";
          break;
        case 0x05:
          printByte(ss, buffer[i]);
          ss << "       DCR B ; Decrement register B";
          break;
        case 0x06:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI B ; Move byte 1 into register B";
          break;
        case 0x07:
          printByte(ss, buffer[i]);
          ss << "       RCL ; Set Carry bit to bit 7 of register A and rotate register A left";
          break;
        case 0x08:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0x09:
          printByte(ss, buffer[i]);
          ss << "       DAD B ; tmp = ((H << 8) & L) + ((B << 8) & C,\n";
          ss << "                    ; H = ((tmp & 0x80) >> 8), L = (tmp & 0x8)";
          break;
        case 0x0A:
          printByte(ss, buffer[i]);
          ss << "       LDAX B ; Load register A <- (BC)";
          break;
        case 0x0B:
          printByte(ss, buffer[i]);
          ss << "       DCX B ; Decrement registers B and C";
          break;
        case 0x0C:
          printByte(ss, buffer[i]);
          ss << "       INR C ; Increment register C";
          break;
        case 0x0D:
          printByte(ss, buffer[i]);
          ss << "       DCR C ; Decrement register C";
          break;
        case 0x0E:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI C ; Move byte 1 into register C";
          break;
        case 0x0F:
          printByte(ss, buffer[i]);
          ss << "       RRC ; Set Carry bit to bit 0 of register A and rotate register A right";
          break;
        case 0x10:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0x11:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " LXI D ; Load register D <- byte 2 and register E <- byte 1";
          memOffset += 2;
          break;
        case 0x12:
          printByte(ss, buffer[i]);
          ss << "       STAX D ; Store register A -> (DE)";
          break;
        case 0x13:
          printByte(ss, buffer[i]);
          ss << "       INX D ; Increment registers D and E";
          break;
        case 0x14:
          printByte(ss, buffer[i]);
          ss << "       INR D ; Increment register D";
          break;
        case 0x15:
          printByte(ss, buffer[i]);
          ss << "       DCR D ; Decrement register D";
          break;
        case 0x16:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI D ; Move byte 1 into register D";
          break;
        case 0x17:
          printByte(ss, buffer[i]);
          ss << "       RAL ; Set tmp = Carry bit, set Carry bit to bit 7 of register A,\n";
          ss << "                  ; rotate register A left, and set bit 0 of register A to tmp";
          break;
        case 0x18:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0x19:
          printByte(ss, buffer[i]);
          ss << "       DAD D ; tmp = ((H << 8) & L) + ((D << 8) & E,\n";
          ss << "                    ; H = ((tmp & 0x80) >> 8), L = (tmp & 0x8)";
          break;
        case 0x1A:
          printByte(ss, buffer[i]);
          ss << "       LDAX D ; Load register A <- (DE)";
          break;
        case 0x1B:
          printByte(ss, buffer[i]);
          ss << "       DCX D ; Decrement registers D and E";
          break;
        case 0x1C:
          printByte(ss, buffer[i]);
          ss << "       INR E ; Increment register E";
          break;
        case 0x1D:
          printByte(ss, buffer[i]);
          ss << "       DCR E ; Decrement register E";
          break;
        case 0x1E:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI E ; Move byte 1 into register E";
          break;
        case 0x1F:
          printByte(ss, buffer[i]);
          ss << "       RAR ; Set tmp = Carry bit, set Carry bit to bit 0 of register A,\n";
          ss << "                  ; rotate register A right, and set bit 7 of register A to tmp";
          break;
        case 0x20:
          printByte(ss, buffer[i]);
          ss << "       NOP";
          break;
        case 0x21:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " LXI H ; Load register H <- byte 2 and register L <- byte 1";
          break;
        case 0x22:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " SHLD ; addr = ((byte 2 << 8) & byte 1), Store L -> (addr) and H -> (addr + 1)";
          break;
        case 0x23:
          printByte(ss, buffer[i]);
          ss << "       INX H ; Increment registers H and L";
          break;
        case 0x24:
          printByte(ss, buffer[i]);
          ss << "       INR H ; Increment register H";
          break;
        case 0x25:
          printByte(ss, buffer[i]);
          ss << "       DCR H ; Decrement register H";
          break;
        case 0x26:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI H ; Move byte 1 into register H";
          break;
        case 0x27:
          printByte(ss, buffer[i]);
          ss << "       DAA ; Decimal Adjust Accumulator,\n";
          ss << "                  ; if (A & 0b1111) > 9, or the Auxiliary\n";
          ss << "                  ; Carry flag is set, A += 6, if this addition\n";
          ss << "                  ; causes a carry, set the Auxiliary carry flag,\n";
          ss << "                  ; if ((A & 0b11110000) >> 4) > 9 or the \n";
          ss << "                  ; Carry flag is set, A += (6 << 4), if this\n";
          ss << "                  ; addition causes a carry, set the Carry flag";
          break;
        case 0x28:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid Instruction";
          break;
        case 0x29:
          printByte(ss, buffer[i]);
          ss << "       DAD D ; tmp = ((H << 8) & L) + ((H << 8) & L,\n";
          ss << "                    ; H = ((tmp & 0x80) >> 8), L = (tmp & 0x8)";
          break;
        case 0x2A:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " LHLD ; addr = ((byte 2 << 8) & byte 1), Load L <- (addr) and H <- (addr + 1)";
          break;
        case 0x2B:
          printByte(ss, buffer[i]);
          ss << "       DCX H ; Decrement registers H and L";
          break;
        case 0x2C:
          printByte(ss, buffer[i]);
          ss << "       INR L ; Increment register L";
          break;
        case 0x2D:
          printByte(ss, buffer[i]);
          ss << "       DCR L ; Decrement register L";
          break;
        case 0x2E:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI L ; Move byte 1 into register L";
          break;
        case 0x2F:
          printByte(ss, buffer[i]);
          ss << "       CMA ; Complement Accumulator - flip all bits in register A";
          break;
        case 0x30:
          printByte(ss, buffer[i]);
          ss << "       NOP";
          break;
        case 0x31:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " LXI SP ; SP = ((byte 2 << 8) & byte 1)";
          break;
        case 0x32:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " STA ; ((byte 2 << 8) & byte 1) <- A";
          break;
        case 0x33:
          printByte(ss, buffer[i]);
          ss << "       INX SP ; Increment register SP";
          break;
        case 0x34:
          printByte(ss, buffer[i]);
          ss << "       INR M ; Increment (HL)";
          break;
        case 0x35:
          printByte(ss, buffer[i]);
          ss << "       DCR M ; Decrement (HL)";
          break;
        case 0x36:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI M ; Move byte 1 into (HL)";
          break;
        case 0x37:
          printByte(ss, buffer[i]);
          ss << "       STC ; Set Carry bit to 1";
          break;
        case 0x38:
          printByte(ss, buffer[i]);
          ss << "       NOP";
          break;
        case 0x39:
          printByte(ss, buffer[i]);
          ss << "       DAD D ; tmp = ((H << 8) & L) + SP,\n";
          ss << "                    ; H = ((tmp & 0x80) >> 8), L = (tmp & 0x8)";
          break;
        case 0x3A:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " LDA ; Load A <- ((byte 2 << 8) & byte 1)";
          break;
        case 0x3B:
          printByte(ss, buffer[i]);
          ss << "       DCX SP ; Decrement register SP";
          break;
        case 0x3C:
          printByte(ss, buffer[i]);
          ss << "       INR A ; Increment register A";
          break;
        case 0x3D:
          printByte(ss, buffer[i]);
          ss << "       DCR A ; Decrement register A";
          break;
        case 0x3E:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    MVI A ; Move byte 1 into register A";
          break;
        case 0x3F:
          printByte(ss, buffer[i]);
          ss << "       CMC ; Complement Carry bit - flip Carry bit";
          break;
        case 0x40:
          printByte(ss, buffer[i]);
          ss << "       MOV B, B ; B = B";
          break;
        case 0x41:
          printByte(ss, buffer[i]);
          ss << "       MOV B, C ; B = C";
          break;
        case 0x42:
          printByte(ss, buffer[i]);
          ss << "       MOV B, D ; B = D";
          break;
        case 0x43:
          printByte(ss, buffer[i]);
          ss << "       MOV B, E ; B = E";
          break;
        case 0x44:
          printByte(ss, buffer[i]);
          ss << "       MOV B, H ; B = H";
          break;
        case 0x45:
          printByte(ss, buffer[i]);
          ss << "       MOV B, L ; B = L";
          break;
        case 0x46:
          printByte(ss, buffer[i]);
          ss << "       MOV B, M ; Move into register B <- (HL)";
          break;
        case 0x47:
          printByte(ss, buffer[i]);
          ss << "       MOV B, A ; B = A";
          break;
        case 0x48:
          printByte(ss, buffer[i]);
          ss << "       MOV C, B ; C = B";
          break;
        case 0x49:
          printByte(ss, buffer[i]);
          ss << "       MOV C, C ; C = C";
          break;
        case 0x4A:
          printByte(ss, buffer[i]);
          ss << "       MOV C, D ; C = D";
          break;
        case 0x4B:
          printByte(ss, buffer[i]);
          ss << "       MOV C, E ; C = E";
          break;
        case 0x4C:
          printByte(ss, buffer[i]);
          ss << "       MOV C, H ; C = H";
          break;
        case 0x4D:
          printByte(ss, buffer[i]);
          ss << "       MOV C, L ; C = L";
          break;
        case 0x4E:
          printByte(ss, buffer[i]);
          ss << "       MOV C, M ; Move into register C <- (HL)";
          break;
        case 0x4F:
          printByte(ss, buffer[i]);
          ss << "       MOV C, A ; C = A";
          break;
        case 0x50:
          printByte(ss, buffer[i]);
          ss << "       MOV D, B ; D = B";
          break;
        case 0x51:
          printByte(ss, buffer[i]);
          ss << "       MOV D, C ; D = C";
          break;
        case 0x52:
          printByte(ss, buffer[i]);
          ss << "       MOV D, D ; D = D";
          break;
        case 0x53:
          printByte(ss, buffer[i]);
          ss << "       MOV D, E ; D = E";
          break;
        case 0x54:
          printByte(ss, buffer[i]);
          ss << "       MOV D, H ; D = H";
          break;
        case 0x55:
          printByte(ss, buffer[i]);
          ss << "       MOV D, L ; D = L";
          break;
        case 0x56:
          printByte(ss, buffer[i]);
          ss << "       MOV D, M ; Move into register D <- (HL)";
          break;
        case 0x57:
          printByte(ss, buffer[i]);
          ss << "       MOV D, A ; D = A";
          break;
        case 0x58:
          printByte(ss, buffer[i]);
          ss << "       MOV E, B ; E = B";
          break;
        case 0x59:
          printByte(ss, buffer[i]);
          ss << "       MOV E, C ; E = C";
          break;
        case 0x5A:
          printByte(ss, buffer[i]);
          ss << "       MOV E, D ; E = D";
          break;
        case 0x5B:
          printByte(ss, buffer[i]);
          ss << "       MOV E, E ; E = E";
          break;
        case 0x5C:
          printByte(ss, buffer[i]);
          ss << "       MOV E, H ; E = H";
          break;
        case 0x5D:
          printByte(ss, buffer[i]);
          ss << "       MOV E, L ; E = L";
          break;
        case 0x5E:
          printByte(ss, buffer[i]);
          ss << "       MOV E, M ; Move into register E <- (HL)";
          break;
        case 0x5F:
          printByte(ss, buffer[i]);
          ss << "       MOV E, A ; E = A";
          break;
        case 0x60:
          printByte(ss, buffer[i]);
          ss << "       MOV H, B ; H = B";
          break;
        case 0x61:
          printByte(ss, buffer[i]);
          ss << "       MOV H, C ; H = C";
          break;
        case 0x62:
          printByte(ss, buffer[i]);
          ss << "       MOV H, D ; H = D";
          break;
        case 0x63:
          printByte(ss, buffer[i]);
          ss << "       MOV H, E ; H = E";
          break;
        case 0x64:
          printByte(ss, buffer[i]);
          ss << "       MOV H, H ; H = H";
          break;
        case 0x65:
          printByte(ss, buffer[i]);
          ss << "       MOV H, L ; H = L";
          break;
        case 0x66:
          printByte(ss, buffer[i]);
          ss << "       MOV H, M ; Move into register H <- (HL)";
          break;
        case 0x67:
          printByte(ss, buffer[i]);
          ss << "       MOV H, A ; H = A";
          break;
        case 0x68:
          printByte(ss, buffer[i]);
          ss << "       MOV L, B ; L = B";
          break;
        case 0x69:
          printByte(ss, buffer[i]);
          ss << "       MOV L, C ; L = C";
          break;
        case 0x6A:
          printByte(ss, buffer[i]);
          ss << "       MOV L, D ; L = D";
          break;
        case 0x6B:
          printByte(ss, buffer[i]);
          ss << "       MOV L, E ; L = E";
          break;
        case 0x6C:
          printByte(ss, buffer[i]);
          ss << "       MOV L, H ; L = H";
          break;
        case 0x6D:
          printByte(ss, buffer[i]);
          ss << "       MOV L, L ; L = L";
          break;
        case 0x6E:
          printByte(ss, buffer[i]);
          ss << "       MOV L, M ; Move into register L <- (HL)";
          break;
        case 0x6F:
          printByte(ss, buffer[i]);
          ss << "       MOV L, A ; L = A";
          break;
        case 0x70:
          printByte(ss, buffer[i]);
          ss << "       MOV M, B ; Move into (HL) <- register B";
          break;
        case 0x71:
          printByte(ss, buffer[i]);
          ss << "       MOV M, C ; Move into (HL) <- register C";
          break;
        case 0x72:
          printByte(ss, buffer[i]);
          ss << "       MOV M, D ; Move into (HL) <- register D";
          break;
        case 0x73:
          printByte(ss, buffer[i]);
          ss << "       MOV M, E ; Move into (HL) <- register E";
          break;
        case 0x74:
          printByte(ss, buffer[i]);
          ss << "       MOV M, H ; Move into (HL) <- register H";
          break;
        case 0x75:
          printByte(ss, buffer[i]);
          ss << "       MOV M, L ; Move into (HL) <- register L";
          break;
        case 0x76:
          printByte(ss, buffer[i]);
          ss << "       HLT ; Halt";
          break;
        case 0x77:
          printByte(ss, buffer[i]);
          ss << "       MOV M, A ; Move into (HL) <- register A";
          break;
        case 0x78:
          printByte(ss, buffer[i]);
          ss << "       MOV A, B ; A = B";
          break;
        case 0x79:
          printByte(ss, buffer[i]);
          ss << "       MOV A, C ; A = C";
          break;
        case 0x7A:
          printByte(ss, buffer[i]);
          ss << "       MOV A, D ; A = D";
          break;
        case 0x7B:
          printByte(ss, buffer[i]);
          ss << "       MOV A, E ; A = E";
          break;
        case 0x7C:
          printByte(ss, buffer[i]);
          ss << "       MOV A, H ; A = H";
          break;
        case 0x7D:
          printByte(ss, buffer[i]);
          ss << "       MOV A, L ; A = L";
          break;
        case 0x7E:
          printByte(ss, buffer[i]);
          ss << "       MOV A, M ; Move into register A <- (HL)";
          break;
        case 0x7F:
          printByte(ss, buffer[i]);
          ss << "       MOV A, A ; A = A";
          break;
        case 0x80:
          printByte(ss, buffer[i]);
          ss << "       ADD B ; Add register B to register A";
          break;
        case 0x81:
          printByte(ss, buffer[i]);
          ss << "       ADD C ; Add register C to register A";
          break;
        case 0x82:
          printByte(ss, buffer[i]);
          ss << "       ADD D ; Add register D to register A";
          break;
        case 0x83:
          printByte(ss, buffer[i]);
          ss << "       ADD E ; Add register E to register A";
          break;
        case 0x84:
          printByte(ss, buffer[i]);
          ss << "       ADD H ; Add register H to register A";
          break;
        case 0x85:
          printByte(ss, buffer[i]);
          ss << "       ADD L ; Add register L to register A";
          break;
        case 0x86:
          printByte(ss, buffer[i]);
          ss << "       ADD M ; Add (HL) to register A";
          break;
        case 0x87:
          printByte(ss, buffer[i]);
          ss << "       ADD A ; Add register A to register A";
          break;
        case 0x88:
          printByte(ss, buffer[i]);
          ss << "       ADD B ; Add register B to register A with Carry bit";
          break;
        case 0x89:
          printByte(ss, buffer[i]);
          ss << "       ADD C ; Add register C to register A with Carry bit";
          break;
        case 0x8A:
          printByte(ss, buffer[i]);
          ss << "       ADD D ; Add register D to register A with Carry bit";
          break;
        case 0x8B:
          printByte(ss, buffer[i]);
          ss << "       ADD E ; Add register E to register A with Carry bit";
          break;
        case 0x8C:
          printByte(ss, buffer[i]);
          ss << "       ADD H ; Add register H to register A with Carry bit";
          break;
        case 0x8D:
          printByte(ss, buffer[i]);
          ss << "       ADD L ; Add register L to register A with Carry bit";
          break;
        case 0x8E:
          printByte(ss, buffer[i]);
          ss << "       ADD M ; Add (HL) to register A with Carry bit";
          break;
        case 0x8F:
          printByte(ss, buffer[i]);
          ss << "       ADD A ; Add register A to register A with Carry bit";
          break;
        case 0x90:
          printByte(ss, buffer[i]);
          ss << "       SUB B ; Subtract register B from register A";
          break;
        case 0x91:
          printByte(ss, buffer[i]);
          ss << "       SUB C ; Subtract register C from register A";
          break;
        case 0x92:
          printByte(ss, buffer[i]);
          ss << "       SUB D ; Subtract register D from register A";
          break;
        case 0x93:
          printByte(ss, buffer[i]);
          ss << "       SUB E ; Subtract register E from register A";
          break;
        case 0x94:
          printByte(ss, buffer[i]);
          ss << "       SUB H ; Subtract register H from register A";
          break;
        case 0x95:
          printByte(ss, buffer[i]);
          ss << "       SUB L ; Subtract register L from register A";
          break;
        case 0x96:
          printByte(ss, buffer[i]);
          ss << "       SUB M ; Subtract (HL) from register A";
          break;
        case 0x97:
          printByte(ss, buffer[i]);
          ss << "       SUB A ; Subtract register A from register A";
          break;
        case 0x98:
          printByte(ss, buffer[i]);
          ss << "       SBB B ; Subtract register B and Carry bit from register A";
          break;
        case 0x99:
          printByte(ss, buffer[i]);
          ss << "       SBB C ; Subtract register C and Carry bit from register A";
          break;
        case 0x9A:
          printByte(ss, buffer[i]);
          ss << "       SBB D ; Subtract register D and Carry bit from register A";
          break;
        case 0x9B:
          printByte(ss, buffer[i]);
          ss << "       SBB E ; Subtract register E and Carry bit from register A";
          break;
        case 0x9C:
          printByte(ss, buffer[i]);
          ss << "       SBB H ; Subtract register H and Carry bit from register A";
          break;
        case 0x9D:
          printByte(ss, buffer[i]);
          ss << "       SBB L ; Subtract register L and Carry bit from register A";
          break;
        case 0x9E:
          printByte(ss, buffer[i]);
          ss << "       SBB M ; Subtract (HL) and Carry bit from register A";
          break;
        case 0x9F:
          printByte(ss, buffer[i]);
          ss << "       SBB A ; Subtract register A and Carry bit from register A";
          break;
        case 0xA0:
          printByte(ss, buffer[i]);
          ss << "       ANA B ; register A &= register B";
          break;
        case 0xA1:
          printByte(ss, buffer[i]);
          ss << "       ANA C ; register A &= register C";
          break;
        case 0xA2:
          printByte(ss, buffer[i]);
          ss << "       ANA D ; register A &= register D";
          break;
        case 0xA3:
          printByte(ss, buffer[i]);
          ss << "       ANA E ; register A &= register E";
          break;
        case 0xA4:
          printByte(ss, buffer[i]);
          ss << "       ANA H ; register A &= register H";
          break;
        case 0xA5:
          printByte(ss, buffer[i]);
          ss << "       ANA L ; register A &= register L";
          break;
        case 0xA6:
          printByte(ss, buffer[i]);
          ss << "       ANA M ; register A &= (HL)";
          break;
        case 0xA7:
          printByte(ss, buffer[i]);
          ss << "       ANA A ; register A &= register A";
          break;
        case 0xA8:
          printByte(ss, buffer[i]);
          ss << "       XRA B ; register A ^= register B";
          break;
        case 0xA9:
          printByte(ss, buffer[i]);
          ss << "       XRA C ; register A ^= register C";
          break;
        case 0xAA:
          printByte(ss, buffer[i]);
          ss << "       XRA D ; register A ^= register D";
          break;
        case 0xAB:
          printByte(ss, buffer[i]);
          ss << "       XRA E ; register A ^= register E";
          break;
        case 0xAC:
          printByte(ss, buffer[i]);
          ss << "       XRA H ; register A ^= register H";
          break;
        case 0xAD:
          printByte(ss, buffer[i]);
          ss << "       XRA L ; register A ^= register L";
          break;
        case 0xAE:
          printByte(ss, buffer[i]);
          ss << "       XRA M ; register A ^= (HL)";
          break;
        case 0xAF:
          printByte(ss, buffer[i]);
          ss << "       XRA A ; register A ^= register A";
          break;
        case 0xB0:
          printByte(ss, buffer[i]);
          ss << "       ORA B ; register A |= register B";
          break;
        case 0xB1:
          printByte(ss, buffer[i]);
          ss << "       ORA C ; register A |= register C";
          break;
        case 0xB2:
          printByte(ss, buffer[i]);
          ss << "       ORA D ; register A |= register D";
          break;
        case 0xB3:
          printByte(ss, buffer[i]);
          ss << "       ORA E ; register A |= register E";
          break;
        case 0xB4:
          printByte(ss, buffer[i]);
          ss << "       ORA H ; register A |= register H";
          break;
        case 0xB5:
          printByte(ss, buffer[i]);
          ss << "       ORA L ; register A |= register L";
          break;
        case 0xB6:
          printByte(ss, buffer[i]);
          ss << "       ORA M ; register A |= (HL)";
          break;
        case 0xB7:
          printByte(ss, buffer[i]);
          ss << "       ORA A ; register A |= register A";
          break;
        case 0xB8:
          printByte(ss, buffer[i]);
          ss << "       CMP B ; Compare register A to register B";
          break;
        case 0xB9:
          printByte(ss, buffer[i]);
          ss << "       CMP C ; Compare register A to register C";
          break;
        case 0xBA:
          printByte(ss, buffer[i]);
          ss << "       CMP D ; Compare register A to register D";
          break;
        case 0xBB:
          printByte(ss, buffer[i]);
          ss << "       CMP E ; Compare register A to register E";
          break;
        case 0xBC:
          printByte(ss, buffer[i]);
          ss << "       CMP H ; Compare register A to register H";
          break;
        case 0xBD:
          printByte(ss, buffer[i]);
          ss << "       CMP L ; Compare register A to register L";
          break;
        case 0xBE:
          printByte(ss, buffer[i]);
          ss << "       CMP M ; Compare register A to (HL)";
          break;
        case 0xBF:
          printByte(ss, buffer[i]);
          ss << "       CMP A ; Compare register A to register A";
          break;
        case 0xC0:
          printByte(ss, buffer[i]);
          ss << "       RNZ ; If Zero bit is not set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xC1:
          printByte(ss, buffer[i]);
          ss << "       POP B ; register C = (SP), register B = (SP + 1), SP += 2";
          break;
        case 0xC2:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JNZ ; If Zero bit is not set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xC3:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JMP ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xC4:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CNZ ; If Zero bit is not set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xC5:
          printByte(ss, buffer[i]);
          ss << "       PUSH B ; (SP - 1) = register B, (SP - 2) = register C, SP -= 2";
          break;
        case 0xC6:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    ADI ; register A += byte 1";
          break;
        case 0xC7:
          printByte(ss, buffer[i]);
          ss << "       RST 0 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $0";
          break;
        case 0xC8:
          printByte(ss, buffer[i]);
          ss << "       RZ ; If Zero bit is set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xC9:
          printByte(ss, buffer[i]);
          ss << "       RET ; PC = ((SP + 1) << 8) & (SP), SP += 2";
          break;
        case 0xCA:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JZ ; If Zero bit is set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xCB:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0xCC:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CZ ; If Zero bit is set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xCD:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CALL ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xCE:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    ACI ; A += byte 1 + Carry bit";
          break;
        case 0xCF:
          printByte(ss, buffer[i]);
          ss << "        RST 1 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $8";
          break;
        case 0xD0:
          printByte(ss, buffer[i]);
          ss << "       RNC ; If Carry bit is not set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xD1:
          printByte(ss, buffer[i]);
          ss << "       POP D ; register E = (SP), register D = (SP + 1), SP += 2";
          break;
        case 0xD2:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JNC ; If Carry bit is not set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xD3:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    OUT ; Store A register to output device byte 1";
          break;
        case 0xD4:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CNC ; If Carry bit is not set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xD5:
          printByte(ss, buffer[i]);
          ss << "       PUSH D ; (SP - 1) = register D, (SP - 2) = register E, SP -= 2";
          break;
        case 0xD6:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    SUI ; A -= byte 1";
          break;
        case 0xD7:
          printByte(ss, buffer[i]);
          ss << "       RST 2 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $10";
          break;
        case 0xD8:
          printByte(ss, buffer[i]);
          ss << "       RC ; If Carry bit is set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xD9:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0xDA:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JC ; If Carry bit is set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xDB:
          printByte(ss, buffer[i]);
          ss << "       IN ; Load A register with value from device byte 1";
          break;
        case 0xDC:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CC ; If Carry bit is set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xDD:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0xDE:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    SBI ; register A -= (data + Carry bit)";
          break;
        case 0xDF:
          printByte(ss, buffer[i]);
          ss << "       RST 3 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $18";
          break;
        case 0xE0:
          printByte(ss, buffer[i]);
          ss << "       RPO ; If Parity bit is not set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xE1:
          printByte(ss, buffer[i]);
          ss << "       POP H ; register L = (SP), register H = (SP + 1); SP += 2";
          break;
        case 0xE2:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JPO ; If Parity bit is not set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xE3:
          printByte(ss, buffer[i]);
          ss << "       XTHL ; tmp = L, L = (SP), (SP) = tmp,\n";
          ss << "                  ; tmp = H, H = (SP + 1), (SP + 1) = tmp";
          break;
        case 0xE4:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CPO ; If Parity bit is not set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xE5:
          printByte(ss, buffer[i]);
          ss << "       PUSH H ; (SP - 1) = register H, (SP - 2) = register L; SP -= 2";
          break;
        case 0xE6:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    ANI ; register A &= byte 1";
          break;
        case 0xE7:
          printByte(ss, buffer[i]);
          ss << "       RST 4 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $20";
          break;
        case 0xE8:
          printByte(ss, buffer[i]);
          ss << "       RPE ; If Parity bit is set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xE9:
          printByte(ss, buffer[i]);
          ss << "       PCHL ; PC = (H << 8) & L";
          break;
        case 0xEA:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JPE ; If Parity bit is set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xEB:
          printByte(ss, buffer[i]);
          ss << "       XCHL ; tmp = register H, register H = registerD, register D = tmp,\n";
          ss << "                  ; tmp = register L, register L = register E, register E = tmp";
          break;
        case 0xEC:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CPE ; If Parity bit is set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xED:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0xEE:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    XRI ; register A ^= byte 1";
          break;
        case 0xEF:
          printByte(ss, buffer[i]);
          ss << "       RST 5 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $28";
          break;
        case 0xF0:
          printByte(ss, buffer[i]);
          ss << "       RP ; If Sign bit is not set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xF1:
          printByte(ss, buffer[i]);
          ss << "       POP PSW ; register Flags = (SP), register A = (SP + 1), SP += 2";
          break;
        case 0xF2:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JP ; If Sign bit is not set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xF3:
          printByte(ss, buffer[i]);
          ss << "       DI ; Disable interrupts";
          break;
        case 0xF4:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CP ; If Sign bit is not set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xF5:
          printByte(ss, buffer[i]);
          ss << "       PUSH PSW ; (SP - 1) = register A, (SP - 2) = register Flags, SP -= 2";
          break;
        case 0xF6:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    ORI ; register A |= byte 1";
          break;
        case 0xF7:
          printByte(ss, buffer[i]);
          ss << "       RST 6 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $30";
          break;
        case 0xF8:
          printByte(ss, buffer[i]);
          ss << "       RM ; If Sign bit is set, PC = ((SP + 1) << 8) & (SP),\n";
          ss << "                  ; SP += 2";
          break;
        case 0xF9:
          printByte(ss, buffer[i]);
          ss << "       SPHL ; SP = ((H << 8) & L)";
          break;
        case 0xFA:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " JM ; If Sign bit is set, PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xFB:
          printByte(ss, buffer[i]);
          ss << "       EI ; Enable Interrupts";
          break;
        case 0xFC:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << " CM ; If Sign bit is set, (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8, SP -= 2,\n";
          ss << "                  ; PC = ((byte 2 << 8) & byte 1)";
          break;
        case 0xFD:
          printByte(ss, buffer[i]);
          ss << "       ; Invalid instruction";
          break;
        case 0xFE:
          printByte(ss, buffer[i++]);
          printByte(ss, buffer[i]);
          ss << "    CPI ; Compare register A to byte 1";
          break;
        case 0xFF:
          printByte(ss, buffer[i]);
          ss << "       RST 7 ; (SP - 1) = ((PC & 0x80) >> 8),\n";
          ss << "                  ; (SP - 2) = PC & 0x8,\n";
          ss << "                  ; PC = $38";
          break;
        }

        ss << "\n";
        ostream << ss.str();
      }

      memOffset += buffer.size();
    }
  }
} // namespace CodexMachina