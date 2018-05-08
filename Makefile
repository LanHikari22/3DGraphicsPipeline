CC=g++
CFLAGS= -g -c -Wall
LDFLAGS= -lX11
SOURCES=main.cpp gcontext.cpp x11context.cpp mydrawing.cpp Shape.cpp Point.cpp  \
Line.cpp Triangle.cpp Circle.cpp Rectangle.cpp Polygon.cpp Image.cpp matrix.cpp \
ViewContext.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=view

all: $(SOURCES) $(EXECUTABLE) 

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d
