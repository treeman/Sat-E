export CC = gcc

export CFLAGS = -Wno-unused-value -Wall -I./src -I../7days/src

export LIBS = -L../7days/lib -l7days -lX11 -lGL -lXrandr -lfreetype -lsndfile -lopenal -lpthread\
 -lsfml-graphics-s -lsfml-audio -lsfml-window-s -lsfml-system-s -llua

export SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*cpp)

export OBJDIR = obj
OBJ = $(patsubst %,$(OBJDIR)/%,$(SRC:.cpp=.o))

EXE = bin/Sat-E
DEBUG_EXE = bin/Sat-E-debug

.PHONY: all clean remake debug

all: $(SRC) $(EXE)

debug: $(SRC) $(DEBUG_EXE)

$(EXE): .depend $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

$(DEBUG_EXE): .depend $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS) -pg

$(OBJDIR)/%.o: %.cpp
	@(mkdir -p $(@D))
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm $(EXE) $(DEBUG_EXE) $(OBJDIR)/* .depend -rf

remake: clean all

# Generate dependency information for all source files
.depend: $(SRC)
	rm -f ./.depend
# Generate dependency information
	$(CC) $(CFLAGS) -MM $^ >> ./.depend
# Set correct directory for object files
	sed -i 's|\(^.*:\)|$(OBJDIR)/$(SRCDIR)/\1|g' ./.depend

include .depend

