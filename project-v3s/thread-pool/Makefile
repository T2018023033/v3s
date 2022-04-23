objs := $(patsubst %.c, %.o, $(wildcard *.c))  

all: $(objs)

%.o:%.c
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	-@rm -f *.o 
