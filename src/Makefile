# MACROS
C11 	= cc -std=c11
CFLAGS	= -Wall -Werror -pedantic
P		= trove
H 		= $(P).h
OBJ		= trove.o utility.o search.o modify.o globals.o

# COMPILES
$(P) : $(OBJ)
	$(C11) $(CFLAGS) -o $(P) $(OBJ)

trove.o : trove.c modify.c utility.c globals.c $(H)
	$(C11) $(CFLAGS) -c trove.c

search.o : search.c utility.c $(H)
	$(C11) $(CFLAGS) -c search.c

modify.o : modify.c utility.c $(H)
	$(C11) $(CFLAGS) -c modify.c

utility.o : utility.c $(H)
	$(C11) $(CFLAGS) -c utility.c

globals.o : globals.c $(H)
	$(C11) $(CFLAGS) -c globals.c

# Cleaning
clean:
	rm -f *.o *~

# rake-c.o : rake-c.c $(H) utility.c file_transmitting.c quoting.c executing.c
# 	$(C11) $(CFLAGS) -c rake-c.c

# globals.o : globals.c $(H)
# 	$(C11) $(CFLAGS) -c globals.c

# utility.o : utility.c $(H)
# 	$(C11) $(CFLAGS) -c utility.c

# parsing.o : parsing.c $(H)
# 	$(C11) $(CFLAGS) -c parsing.c

# file_transmitting.o : file_transmitting.c $(H)
# 	$(C11) $(CFLAGS) -c file_transmitting.c

# quoting.o : quoting.c $(H)
# 	$(C11) $(CFLAGS) -c quoting.c

# executing.o: executing.c $(H)
# 	$(C11) $(CFLAGS) -c executing.c