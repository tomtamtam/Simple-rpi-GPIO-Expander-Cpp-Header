TARGET = bin/app

CXX = g++

CXXFLAGS = -Wall -O2

LIBS = -lwiringPi

SRC = src/main.cpp src/json/json.cpp src/gpio/gpio.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

