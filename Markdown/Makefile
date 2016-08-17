TARGET = markdown
VAR = 	
OBJS = abstract.o    config.o main.o mark_list.o text_viewer.o abstract_list.o header.o num_list.o word_list.o

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^ -g
%.o: %.cpp
		g++ -c -o $@ $< -g -Wall 

clean:
	rm $(TARGET) *.o -rf
