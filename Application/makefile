#    File : makefile
#    Programmer : Mohammad Lotfi
#    Used : make
#    Design Pattern : none
#    Types of memory : none
#    Total Tread : Nothing
#    Site : https://www.mahsen.ir
#    Tel : +989124662703
#    Email : info@mahsen.ir
#    Last Update : 2025/3/7

# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -fdiagnostics-color=always -std=c++11 -g -pthread 
# Library
LIBRARY = -lsqlite3 -lcurl -ldl
# Build dir
BUILD_DIR = Build
# Target executable
TARGET = $(BUILD_DIR)/Application
# Source files
SRCS = *.cpp \
	Module/*.cpp \
	Core/*.cpp
# Source files www
SRCS_WWW = Web/www/
# Container
CONTAINER = 9bb713261435

all:
	clang-format -i -style=file  $(SRCS)
	clear
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBRARY_DIR) $(LIBRARY)
	chmod +x $(TARGET)

	docker start $(CONTAINER)
	docker cp "$(SRCS_WWW)" "$(CONTAINER):/"
	docker cp "$(TARGET)" "$(CONTAINER):/www/cgi-bin/"	

clean:
	del $(TARGET)
