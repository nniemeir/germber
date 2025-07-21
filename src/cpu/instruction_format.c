#include <core/bus.h>
#include <cpu/cpu.h>
#include <cpu/instructions.h>

char *inst_lookup[] = {
    "<NONE>", "NOP",     "LD",     "INC",    "DEC",    "RLCA",  "ADD",
    "RRCA",   "STOP",    "RLA",    "JR",     "RRA",    "DAA",   "CPL",
    "SCF",    "CCF",     "HALT",   "ADC",    "SUB",    "SBC",   "AND",
    "XOR",    "OR",      "CP",     "POP",    "JP",     "PUSH",  "RET",
    "CB",     "CALL",    "RETI",   "LDH",    "JPHL",   "DI",    "EI",
    "RST",    "IN_ERR",  "IN_RLC", "IN_RRC", "IN_RL",  "IN_RR", "IN_SLA",
    "IN_SRA", "IN_SWAP", "IN_SRL", "IN_BIT", "IN_RES", "IN_SET"};

char *inst_name(in_type t) { return inst_lookup[t]; }

static char *rt_lookup[] = {"<NONE>", "A",  "F",  "B",  "C",  "D",  "E", "H",
                            "L",      "AF", "BC", "DE", "HL", "SP", "PC"};

void inst_to_str(cpu_context *cpu, char *str) {
  instruction *inst = cpu->cur_inst;
  snprintf(str, MAX_INST_STR_LEN,"%s ", inst_name(inst->type));

  switch (inst->mode) {
  case AM_IMP:
    return;

  case AM_R_D16:
  case AM_R_A16:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,$%04X", inst_name(inst->type), rt_lookup[inst->reg_1],
            cpu->fetched_data);
    return;

  case AM_R:
    snprintf(str, MAX_INST_STR_LEN,"%s %s", inst_name(inst->type), rt_lookup[inst->reg_1]);
    return;

  case AM_R_R:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,%s", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_MR_R:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s),%s", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_MR:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s)", inst_name(inst->type), rt_lookup[inst->reg_1]);
    return;

  case AM_R_MR:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,(%s)", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_R_D8:
  case AM_R_A8:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,$%02X", inst_name(inst->type), rt_lookup[inst->reg_1],
            cpu->fetched_data & 0xFF);
    return;

  case AM_R_HLI:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,(%s+)", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_R_HLD:
    snprintf(str, MAX_INST_STR_LEN,"%s %s,(%s-)", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_HLI_R:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s+),%s", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_HLD_R:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s-),%s", inst_name(inst->type), rt_lookup[inst->reg_1],
            rt_lookup[inst->reg_2]);
    return;

  case AM_A8_R:
    snprintf(str, MAX_INST_STR_LEN,"%s $%02X,%s", inst_name(inst->type),
            bus_read(cpu->regs.pc - 1), rt_lookup[inst->reg_2]);

    return;

  case AM_HL_SPR:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s),SP+%d", inst_name(inst->type), rt_lookup[inst->reg_1],
            cpu->fetched_data & 0xFF);
    return;

  case AM_D8:
    snprintf(str, MAX_INST_STR_LEN,"%s $%02X", inst_name(inst->type), cpu->fetched_data & 0xFF);
    return;

  case AM_D16:
    snprintf(str, MAX_INST_STR_LEN,"%s $%04X", inst_name(inst->type), cpu->fetched_data);
    return;

  case AM_MR_D8:
    snprintf(str, MAX_INST_STR_LEN,"%s (%s),$%02X", inst_name(inst->type), rt_lookup[inst->reg_1],
            cpu->fetched_data & 0xFF);
    return;

  case AM_A16_R:
    snprintf(str, MAX_INST_STR_LEN,"%s ($%04X),%s", inst_name(inst->type), cpu->fetched_data,
            rt_lookup[inst->reg_2]);
    return;

  default:
    fprintf(stderr, "INVALID AM: %d\n", inst->mode);
    NO_IMPL
  }
}
