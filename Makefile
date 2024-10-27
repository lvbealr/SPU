CXX                = g++

STACK_SRC          = stack.cpp privateStack.cpp hidePtr.cpp stackDump.cpp stackHash.cpp

ASSEMBLY_SRC       = assembler.cpp labels.cpp
ASSEMBLY_TARGET    = assembly

# DISASSEMBLY_SRC    = disassembly.cpp text_processing.cpp
# DISASSEMBLY_TARGET = disassembly

PROCESSOR_SRC      = processor.cpp
PROCESSOR_TARGET   = processor

SUBMODULE_SRC      = ../colorPrint/colorPrint.cpp # TODO

BUILD_DIR    = ./build/
SRC_DIR      = ./src/
STACK_DIR    = ./Stack/
CFLAGS       = -I/include -I/colorPrint -I/customWarning -I/Stack

ASM_FILE     = defaultAsm.asm

TARGET       = SPU
OBJECT       = $(patsubst %.cpp, %.o, $(SRC))
BUILD_OBJ    = $(addprefix $(BUILD_DIR), $(OBJECT))

GREEN_TEXT   = \033[1;32m
YELLOW_TEXT  = \033[1;33m
DEFAULT_TEXT = \033[0m

DED_FLAGS    = -D _DEBUG -ggdb2 -std=c++17 -O0 -Wall -Wextra -Weffc++                                     \
			    -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations                     \
				-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported                      \
				-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral           \
				-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor \
				-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls \
				-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2        \
				-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types                \
				-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef                     \
				-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros                              \
				-Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing                        \
				-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation       \
				-fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer        \
				-Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla

vpath %.o   $(BUILD_DIR)
vpath %.cpp $(SRC_DIR)

.PHONY: clean all
all   :
	clear
	make quick_assembly
	make quick_processor
	@printf "$(GREEN_TEXT)$(TARGET) COMPILED $(DEFAULT_TEXT)\n"
	@$(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))

$(TARGET) :  $(BUILD_DIR) $(OBJECT)
	$(CXX)   $(BUILD_OBJ) -o $(TARGET) -D _NDEBUG
	@printf "$(GREEN_TEXT)$(TARGET) COMPILED$(DEFAULT_TEXT)\n"

$(BUILD_DIR) :
	mkdir -p build

$(OBJECT) : %.o : %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $(addprefix $(BUILD_DIR), $@)

quick_assembly : $(addprefix $(SRC_DIR), $(ASSEMBLY_SRC))
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET))
	$(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET)) $(ASM_FILE)

quick_processor : $(addprefix $(SRC_DIR), $(PROCESSOR_SRC))
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) $(addprefix $(STACK_DIR), $(STACK_SRC)) \
	-o $(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))

assembly : $(addprefix $(SRC_DIR), $(ASSEMBLY_SRC))
	clear
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET))
	@printf "$(GREEN_TEXT)$(ASSEMBLY_TARGET) COMPILED$(DEFAULT_TEXT)\n"
	@printf "To start an assembly write:$(GREEN_TEXT) $(addprefix $(BUILD_DIR), $(ASSEMBLY_TARGET)) [--input / --output] [file PATH] $(DEFAULT_TEXT)\n"
	@printf "Do not type any flags to start assembly with default settings\n"

disassembly : $(addprefix $(SRC_DIR), $(DISASSEMBLY_SRC))
	clear
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) -o $(addprefix $(BUILD_DIR), $(DISASSEMBLY_TARGET))
	@printf "$(GREEN_TEXT)$(DISASSEMBLY_TARGET) COMPILED$(DEFAULT_TEXT)\n"
	@printf "To start an processor write:$(GREEN_TEXT) $(addprefix $(BUILD_DIR), $(DISASSEMBLY_TARGET)) [--input / --output] [file PATH] $(DEFAULT_TEXT)\n"
	@printf "Do not type any flags to start processor with default settings\n"

processor : $(addprefix $(SRC_DIR), $(PROCESSOR_SRC))
	@$(CXX) $(CFLAGS) $^ $(SUBMODULE_SRC) $(addprefix $(STACK_DIR), $(STACK_SRC)) \
	-o $(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET))
	@printf "$(GREEN_TEXT)$(PROCESSOR_TARGET) COMPILED$(DEFAULT_TEXT)\n"
	@printf "To start an processor write:$(GREEN_TEXT) $(addprefix $(BUILD_DIR), $(PROCESSOR_TARGET)) [--input] [file PATH] $(DEFAULT_TEXT)\n"
	@printf "Do not type any flags to start processor with default settings\n"

doxy :
	doxygen