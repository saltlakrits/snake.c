CC=clang
OUT = snake
CFLAGS=-Isrc/
SRCDIR = src/
#OBJDIR = .obj/
CFILES = $(wildcard $(SRCDIR)*.c)
OBJS = $(CFILES:.c=.o)

LDLIBS = -lncursesw
#@mkdir -p .obj

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) $(LDLIBS)
	@printf "\n === Running program ===\n"
	#@./$(OUT)
	@rm -rf $(SRCDIR)*.o
