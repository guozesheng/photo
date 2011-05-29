# 2011.5.25

src = $(wildcard *.c)
obj = $(patsubst %.c, $(DIR)%.o, $(src))
target = main

CC = gcc
CFLAGS = -Wall -g -c
#LDFLAGS = -ljpeg -lfreetype
LDFLAGS = 

all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
$(DIR)%.o:%.c
	$(CC) -o $@ $(CFLAGS) $< 

run:
	./$(target)

clean:
	-rm -f $(target)
	-rm -f $(obj)
	-rm -f *~

.PHONY: clean
test:
	@echo $(src)
	@echo $(obj)
