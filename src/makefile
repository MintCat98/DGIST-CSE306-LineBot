# 컴파일러 및 컴파일 옵션 정의
CXX = g++
CC = gcc
CXXFLAGS = `pkg-config --cflags opencv4 zbar`
CFLAGS = `pkg-config --cflags opencv4 zbar`
LDFLAGS = `pkg-config --libs opencv4 zbar` -lpthread -lm -lwiringPi

# 타겟 실행 파일 이름
TARGET = runfile

# 소스 파일 및 오브젝트 파일 정의
CXXSRCS = qr_recognition.cpp
CSRCS = main.c servercomm.c car_tracking.c car_control.c
CXXOBJS = $(CXXSRCS:.cpp=.o)
COBJS = $(CSRCS:.c=.o)
OBJS = $(CXXOBJS) $(COBJS)

# 기본 타겟
all: $(TARGET)

# 타겟 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# C++ 소스 파일 컴파일 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# C 소스 파일 컴파일 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
