CXX=g++
CXX_FLAGS=-I ./ -std=gnu++20 -fno-rtti -fno-exceptions -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-red-zone -mno-sse -mno-sse2 -mcmodel=kernel -Wall -Wextra -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-PIC -ffunction-sections -fdata-sections
LD=ld
LD_FLAGS=-nostdlib -static -z max-page-size=0x1000 --gc-sections -T linker.lds
ASM=nasm
ASM_FLAGS=-f elf64
OUTPUT=larpkernel
ASM_SRC=$(shell find . -type f -name "*.asm")
CXX_SRC=$(shell find . -type f -name "*.cpp")
OBJ=$(patsubst %, %.o, $(CXX_SRC)) $(patsubst %.asm, %.nasm.o, $(ASM_SRC)) assets/zap.o

$(OUTPUT): $(OBJ)
	@echo "[LD] -> $@"
	@$(LD) $(LD_FLAGS) $(OBJ) -o $(@)

%.cpp.o: %.cpp
	@echo "[CXX] $<"
	@$(CXX) $(CXX_FLAGS) -c $< -o $@

%.nasm.o: %.asm
	@echo "[ASM] $<"
	@$(ASM) $(ASM_FLAGS) $< -o $@

assets/zap.o: assets/zap.psf
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@
	
clean:
	@rm -rf $(OBJ) $(OUTPUT)