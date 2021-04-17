CC=gcc

SRCS = solar.c vec.c ray.c sphere.c view.c light.c pngsetup.c renderable.c camera.c plane.c
OBJS =  $(SRCS:.c=.o)  
DEP = vec.h sphere.h ray.h view.h light.h pngsetup.h renderable.h camera.h plane.h

OPTS = -lpng -lX11 -lm  -O2


solar : $(OBJS) $(DEP)
	@echo solar build
	$(CC) -o solar $(OBJS) $(OPTS)

list :
	@echo SRCS: $(SRCS)
	@echo OBJS: $(OBJS)
	@echo DEPS: $(DEP)


$(SRCS) :
	@echo source thing
	$(CC) -c $<                                                                                                                                                      

clean :
	rm -f *.o
