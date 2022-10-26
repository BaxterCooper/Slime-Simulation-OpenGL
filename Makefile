TARGET := main

SRC_DIR := ./src
INCLUDE_DIR := ./include
LIB_DIR := ./lib
BUILD_DIR := ./build

SRCS := $(wildcard ${SRC_DIR}/*.cpp)
OBJS := $(SRCS:${SRC_DIR}/%=$(BUILD_DIR)/%.o)

INC_FLAGS := -I$(INCLUDE_DIR)
LIB_FLAGS := -L${LIB_DIR}
CPPFLAGS := -std=c++17 $(INC_FLAGS) ${LIB_FLAGS}

ifeq ($(OS), Windows_NT)
	OSFLAG := -lglfw3 -lopengl32 -lglu32 -lgdi32
else
	OSFLAG := -lglfw3 -ldl -pthread
endif

COMPILE:
	$(CXX) $(CPPFLAGS) $(SRCS) ${OSFLAG} -o ${TARGET}