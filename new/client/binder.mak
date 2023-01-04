####################################################################
# Custom: makefile
####################################################################

CXX_FLAGS += -O0 -g -std=c++11

LIBS += -pthread -lbinder

C_DEFS += -DHAVE_PTHREADS -DHAVE_SYS_UIO_H -DHAVE_ENDIAN_H -DHAVE_ANDROID_OS=1

LD_FLAGS += -L../../libs

INCLUDES += -I../../libs/include \
	-I../..
