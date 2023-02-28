#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

char *opcode_to_str(opcode op)
{
    switch (op)
    {
    case MOV:
    	return "mov"; break;
    case CMP:
		return "cmp"; break;
    case ADD:
		return "add"; break;
    case SUB:
		return "sub"; break;
    case NOT:
		return "not"; break;
    case CLR:
		return "clr"; break;
    case LEA:
		return "lea"; break;
    case INC:
		return "inc"; break;
    case DEC:
		return "dec"; break;
    case JMP:
		return "jmp"; break;
    case BNE:
		return "bne"; break;
    case RED:
		return "red"; break;
    case PRN:
		return "prn"; break;
    case JSR:
		return "jsr"; break;
    case RTS:
		return "rts"; break;
    case STOP:
		return "stop"; break;
    case DATA:
		return ".data"; break;
    case STRING:
		return ".string"; break;
    case ENTRY:
		return ".entry"; break;
    case EXTERNAL:
		return ".external"; break;
    case OPCODE_SIZE:
		return "mov"; break;
    default:
        return "unknown opcode"; break;
    }
}

    bool opcode_in_group(opcode op, opcode group[], int count)
    {
		int i;
		for (i = 0; i < count; i++) {
			if (op == group[i])
				return True;
		}
		return False;
    }

