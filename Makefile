

CC=gcc
OUTNAME=ascii-art
OPTIONS=-Wall -Wextra -ggdb -g

SRC=$(addprefix src/, $(addsuffix .c, ascii util))
HDR=$(addprefix src/, $(addsuffix .h, util))

LIBS=SDL2 SDL2_image


$(OUTNAME) : $(SRC) $(HDR) Makefile
	$(CC) src/*.c $(addprefix -l, $(LIBS)) -o $(OUTNAME) $(OPTIONS)

clean :
	@if [ -f $(OUTNAME) ]; then\
	    rm $(OUTNAME);\
            echo "Removed $(OUTNAME)";\
	fi

