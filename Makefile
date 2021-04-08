CC=gcc

SRCS = solar.c vec.c ray.c sphere.c view.c  phong.c
OBJS =  $(SRCS:.c=.o)  
DEP = vec.h sphere.h ray.h view.h phong.h light.h

OPTS = -lpng -lX11 -lm -O2


solar : $(OBJS) $(DEP)
	$(CC) -o solar $(OBJS) $(OPTS)

list :
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo DEPS: $(DEP)


$(SRCS) :
	$(CC) -c $<                                                                                                                                                      

clean :
	rm *.o
