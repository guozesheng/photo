# 2011.5.28

#DIR = ./out/
#TAR_DIR = ./out/

src = $(wildcard *.c)
obj = $(patsubst %.c, $(DIR)%.o, $(src))
target = $(TAR_DIR)main

CC = gcc
#CC = arm-linux-gcc
#CFLAGS = -Wall -g -c -I../jpeg/ -I../freetype/include
CFLAGS = -Wall -g -c 
#LDFLAGS = -L../jpeg/.libs/ -ljpeg -L../freetype/objs/.libs -lfreetype
LDFLAGS = -ljpeg -lfreetype -lpthread

all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
$(DIR)%.o:%.c
	$(CC) -o $@ $(CFLAGS) $< 

clean:
	-rm -f $(target)
	-rm -f $(obj)
	-rm -f *~

.PHONY: clean
test:
	@echo $(src)
	@echo $(obj)
run:
	./main
