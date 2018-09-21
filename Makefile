
CC=gcc

OUTNAME=ascii-art
OPTIONS=-Wall -Wextra -ggdb -g -Og

_SRC=ascii util
SRC=$(addprefix src/, $(addsuffix .c, $(_SRC)))
_HDR=util
HDR=$(addprefix src/, $(addsuffix .h, $(_HDR)))

_LIBS=SDL2 SDL2_image
LIBS=$(addprefix -l, $(_LIBS))


$(OUTNAME) : $(SRC) $(HDR) Makefile
	$(CC) $(SRC) $(LIBS) -o $(OUTNAME) $(OPTIONS)

clean :
	rm $(OUTNAME);

