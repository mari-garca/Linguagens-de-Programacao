# Laboratorio 4 - Questao 2: Sistema Meteorologico

CPP      = g++
LD       = g++
CPPFLAGS = -Wall

OBJ      = leiturasensor.o estacao.o sistemameteorologico.o main.o

BIN      = main

all: $(BIN)

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<

$(BIN): $(OBJ)
	$(LD) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)