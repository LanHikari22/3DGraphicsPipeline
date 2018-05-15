CC=g++
CFLAGS= -g -c -Wall -I include
LDFLAGS= -lX11
SOURCES= $(wildcard src/*.cpp)
OBJECTS= $(SOURCES:.cpp=.o) # TODO: change. always makes...
EXEC= orbit

all: $(SOURCES) $(EXEC) 

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXEC): $(OBJECTS)

	$(CC) $(notdir $(OBJECTS)) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $(notdir $@)
	$(CC) -MM $(CFLAGS) $< > $(notdir $*.d)

clean:
	rm -rf $(notdir $(OBJECTS)) $(EXEC) *.d
