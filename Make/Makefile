TARGET = mymake
OBJS = run.o synt.o lex.o mymain.o global.o

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $@ $^ -g

%.o: %.cpp
	g++ -c -o $@ $^ -g  

clean:
	rm $(TARGET) *.o -rf
