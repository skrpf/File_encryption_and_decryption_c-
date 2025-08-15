CXX = g++
# --- THE FIX IS ON THIS LINE ---
CXXFLAGS = -std=c++17 -g -Wall -I. -Isrcapp/encryptDecrypt -Isrcapp/fileHandling -Isrcapp/processes

# --- OS Detection for executable extension ---
EXE =
ifeq ($(OS), Windows_NT)
    EXE = .exe
endif
# ---

MAIN_TARGET = encrypt_decrypt$(EXE)
CRYPTION_TARGET = cryption$(EXE)

MAIN_SRC = main.cpp \
           srcapp/processes/ProcessManagement.cpp \
           srcapp/fileHandling/IO.cpp \
           srcapp/fileHandling/ReadEnv.cpp \
           srcapp/encryptDecrypt/Cryption.cpp

CRYPTION_SRC = srcapp/encryptDecrypt/CryptionMain.cpp \
               srcapp/encryptDecrypt/Cryption.cpp \
               srcapp/fileHandling/IO.cpp \
               srcapp/fileHandling/ReadEnv.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
CRYPTION_OBJ = $(CRYPTION_SRC:.cpp=.o)

all: $(MAIN_TARGET) $(CRYPTION_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CRYPTION_TARGET): $(CRYPTION_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f $(MAIN_OBJ) $(CRYPTION_OBJ) $(MAIN_TARGET) $(CRYPTION_TARGET)

.PHONY: clean all