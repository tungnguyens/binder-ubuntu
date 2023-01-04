####################################################################
# Custom: makefile
####################################################################
CXX_FLAGS += -std=c++14

# File *.cpp
SOURCE_FILES = \
	${SOURCE_PATTH}/server.cpp \

# File *.h
INCLUDES += \
	-I${SOURCE_PATTH}/.. \

CXX_SOURCE_FILES += $(filter %.cpp, $(SOURCE_FILES))
