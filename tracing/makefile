# 컴파일러 정의
CC=gcc

# 컴파일 옵션
CFLAGS=-c -Wall

# wiringPi 라이브러리 플래그
LDFLAGS=-lwiringPi

# 소스 파일, 오브젝트 파일, 실행 파일
SOURCES=YB_Pcb_Car.c tracking.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=linetracer

# make
all: $(SOURCES) $(EXECUTABLE)

# 실행 파일 빌드
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# 오브젝트 파일 빌드
.c.o:
	$(CC) $(CFLAGS) $< -o $@

# make clean: 생성된 파일 모두 삭제
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
