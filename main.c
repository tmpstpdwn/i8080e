#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ROM_SIZE 2048

int dissassemble_8080(uint8_t *buff, size_t pc) {
  int size = 1;
  uint8_t instr = buff[pc];
  switch (instr) {
    case 0x00: printf("NOP\n"); break;
    case 0x01: printf("LXI B, 0x%x 0x%x", buff[pc + 2], buff[pc + 1]); size = 3; break;
    case 0x02: printf("STAX B\n"); break;
    case 0x03: printf("INX B\n"); break;
    case 0x04: printf("INR B\n"); break;
    case 0x05: printf("DCR B\n"); break;
    case 0x06: printf("MVI B, 0x%x\n", buff[pc + 1]); size = 2; break;
    case 0x07: printf("RLC\n"); break;
    case 0x08: printf("NOP\n"); break;
    case 0x09: printf("DAD B\n"); break;
    case 0x0A: printf("LDAX B\n"); break;
    case 0x0B: printf("DCX B\n"); break;
    default: break;
  }
  return size;
}

int main(int argc, char** argv) {
  if (argc == 1) return 1;

  int return_code = 0;

  uint8_t *rom_buff = NULL;
  size_t pc = 0;

  // File buffering.
  FILE *fp = fopen(argv[1], "rb");

  if (!fp) {
    printf("File opening failed.\n");
    return_code = 1;
    goto end;
  }

  fseek(fp, 0, SEEK_END);
  int fsize = ftell(fp);
  rewind(fp);

  if (fsize <= 0) {
    printf("File size determination failed.\n");
    return_code = 1;
    goto end;
  }

  rom_buff = malloc(fsize);

  if (rom_buff == NULL || fread(rom_buff, 1, fsize, fp) != fsize) {
    printf("Loading rom to memory failed.\n");
    return_code = 1;
    goto end;
  }

  // Dissassembly of the entire rome.
  while (pc < fsize) {
    int size = dissassemble_8080(rom_buff, pc);
    pc += size;
  }

end:
  if (!fp) fclose(fp);
  free(rom_buff);

  return return_code;
}
