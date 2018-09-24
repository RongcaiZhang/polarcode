PROGRAM=main
CFLAGS ?= -ffunction-sections -fdata-sections -Wl,--gc-sections
EXTRA_SRCS = common_cmd.cpp ctr.cpp gen_tx.cpp polar_sc.cpp polar_scl.cpp polar_ca_scl.cpp polar_aca_scl.cpp

all: $(PROGRAM).elf $(PROGRAM).dis System.map

LIBS = -lc -lcurses
INCS =

EXTRA_OBJS = $(EXTRA_SRCS:.cpp=.o)

$(PROGRAM).elf: $(PROGRAM).o $(EXTRA_OBJS) 
	g++ -O2  $(CFLAGS) -Wall $(PROGRAM).o  $(EXTRA_OBJS) -o $@ $(LIBS)
	cp $(PROGRAM).elf $(PROGRAM)

%.dis: %.elf
	objdump -d $^ > $@

%.o: %.c
	g++ -O2  -c $(INCS) -o $@ $^

System.map: $(PROGRAM).elf
	@nm $< | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort > System.map

clean:
	rm -f *.o *.elf *.bin *.vmem *.dis link.ld System.map
