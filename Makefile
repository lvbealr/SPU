CXX                = g++

STACK_SRC          = stack.cpp privateStack.cpp hidePtr.cpp stackDump.cpp stackHash.cpp

ASSEMBLY_SRC       = assembler.cpp labels.cpp
ASSEMBLY_TARGET    = assembler

# DISASSEMBLY_SRC    = disassembler.cpp text_processing.cpp
# DISASSEMBLY_TARGET = disassmbler

PROCESSOR_SRC      = processor.cpp
PROCESSOR_TARGET   = processor

SUBMODULE_SRC      = colorPrint/colorPrint.cpp # TODO

BUILD_DIR    = build/
PROC_SRC_DIR = Processor/src/
ASM_SRC_DIR  = Assembler/src/
DASM_SRC_DIR = Disassembler/src/
STACK_DIR    = Stack/
CFLAGS       = -I customWarning/ -I colorPrint/ -I Stack/ -I Assembler/include/ -I Processor/include/ -I Disassembler/include/

DEFAULT_ASM_SRC      = asmExamples/
DEFAULT_ASM_FILE     = defaultAsm.asm

DEFAULT_EXE_SRC      = build/
DEFAULT_EXE_FILE     = SPU_code.txt

TARGET       = SPU
OBJECT       = $(patsubst %.cpp, %.o, $(SRC))
BUILD_OBJ    = $(addprefix $(BUILD_DIR), $(OBJECT))

GREEN_TEXT   			= \033[1;32m
YELLOW_TEXT  			= \033[1;33m
CYAN_TEXT         = \033[1;36m
DEFAULT_BOLD_TEXT = \033[1;37m
DEFAULT_TEXT 			= \033[0m

DED_FLAGS    = -D _DEBUG -ggdb2 -std=c++17 -O0 -Wall -Wextra -Weffc++                                     \
			    	   -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations                      \
				       -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported                       \
				       -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral            \
				       -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor  \
				       -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls  \
				       -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2         \
				       -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types                 \
				       -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef                      \
				       -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros                               \
				       -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing                         \
				       -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation        \
				       -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer         \
				       -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla

vpath %.o   $(BUILD_DIR)
vpath %.cpp $(PROC_SRC_DIR) $(ASM_SRC_DIR) $(DASM_SRC_DIR)

.PHONY: all

all:
	make clean
	clear
	make quickAssembler
	make quickProcessor
	clear
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "$(CYAN_TEXT)$(TARGET)$(GREEN_TEXT) COMPILED. $(DEFAULT_BOLD_TEXT)RUNNING... ($(DEFAULT_ASM_FILE))\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "$(DEFAULT_TEXT)"
	@$(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))

$(TARGET): $(BUILD_DIR) $(OBJECT)
	$(CXX)   $(BUILD_OBJ) -o $(TARGET) -D _NDEBUG
	@printf "$(GREEN_TEXT)$(TARGET) COMPILED$(DEFAULT_TEXT)\n"

$(BUILD_DIR):
	@mkdir -p build

$(OBJECT): %.o : %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $(addprefix $(BUILD_DIR), $@)

quickAssembler: $(addprefix $(ASM_SRC_DIR), $(ASSEMBLY_SRC))
	clear
	make $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET))
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "$(CYAN_TEXT)ASSEMBLER $(GREEN_TEXT)COMPILED. $(DEFAULT_BOLD_TEXT)TO RUN AN $(CYAN_TEXT)ASSEMBLER, $(DEFAULT_BOLD_TEXT)WRITE: $(GREEN_TEXT)build/assembler [file-PATH]\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
	@printf "$(DEFAULT_TEXT)\n"
	@printf "$(CYAN_TEXT)♦ HINT: $(DEFAULT_TEXT)do not type path to .asm file to assembly $(DEFAULT_EXE_SRC)$(DEFAULT_EXE_FILE)\n"
	@$(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET)) $(DEFAULT_ASM_SRC)$(DEFAULT_ASM_FILE)


quickProcessor: $(addprefix $(PROC_SRC_DIR), $(PROCESSOR_SRC))
	clear
	make $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) $(addprefix $(STACK_DIR), $(STACK_SRC)) \
	-o $(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "$(CYAN_TEXT)PROCESSOR $(GREEN_TEXT)COMPILED. $(DEFAULT_BOLD_TEXT)TO RUN A $(CYAN_TEXT)PROCESSOR, $(DEFAULT_BOLD_TEXT)WRITE: $(GREEN_TEXT)build/processor [file-PATH]\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░"
	@printf "$(DEFAULT_TEXT)\n"
	@printf "$(CYAN_TEXT)♦ HINT: $(DEFAULT_TEXT)do not type path to .bin file to run $(DEFAULT_EXT_SRC)$(DEFAULT_EXE_FILE)\n"

assembler: $(addprefix $(ASM_SRC_DIR), $(ASSEMBLY_SRC))
	clear
	make $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET))
	@printf "\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "ASSEMBLER COMPILED. $(DEFAULT_BOLD_TEXT)TO RUN AN ASSEMBLER, WRITE: $(GREEN_TEXT)build/assembler [file-PATH] $(YELLOW_TEXT)(build/assembler asmExamples/factorial.asm)\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n$(DEFAULT_TEXT)"
	@printf "$(CYAN_TEXT)♦ HINT: $(DEFAULT_TEXT)do not type any flags to start assembly with default settings\n"

disassembler: $(addprefix $(DASM_SRC_DIR), $(DISASSEMBLY_SRC))
	clear
	make $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(DISASSEMBLY_TARGET))
	@printf "$(GREEN_TEXT)$(DISASSEMBLY_TARGET) COMPILED$(DEFAULT_TEXT)\n"
	@printf "To run a disassemble, write:$(GREEN_TEXT) $(addprefix $(BUILD_DIR), $(DISASSEMBLY_TARGET)) [--input / --output] [file PATH] $(DEFAULT_TEXT)\n"
	@printf "Do not type any flags to start processor with default settings\n"

processor: $(addprefix $(PROC_SRC_DIR), $(PROCESSOR_SRC))
	clear
	make $(BUILD_DIR)
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) $(addprefix $(STACK_DIR), $(STACK_SRC)) \
	-o $(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))
	@printf "\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
	@printf "PROCESSOR COMPILED. $(DEFAULT_BOLD_TEXT)TO RUN A PROCESSOR, WRITE: $(GREEN_TEXT)build/processor [file-PATH] $(YELLOW_TEXT)(build/processor txtExamples/SPU_code.txt)\n"
	@printf "$(GREEN_TEXT)░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n$(DEFAULT_TEXT)"
	@printf "$(CYAN_TEXT)♦ HINT: $(DEFAULT_TEXT)do not type any flags to start assembly with default settings\n"

doxy:
	doxygen

clean:
	@printf "$(GREEN_TEXT)► $(CYAN_TEXT)build/ $(DEFAULT_BOLD_TEXT)and $(CYAN_TEXT)dumps/ $(DEFAULT_BOLD_TEXT)were removed!\n"
	@rm -rf build
	@rm -rf dumps