##服务器端makefile############
all: server client
obj1 = src/wrap.o src/server.o
obj2 = src/wrap.o src/client.o
target1 = server
target2 = client
$(target1):$(obj1)
	gcc $(obj1) -o $(target1)
$(target2):$(obj2)
	gcc $(obj2) -o $(target2)
%.o:%.c
	gcc -c $<  -I ./include/ -o $@ -Wall
.PHONY:clean
clean:
	-rm $(obj1) $(obj2) $(target1) $(target2) -f



##all: server client
##obj1 = src/wrap.o src/server.o
##obj2 = src/wrap.o src/client.o

##target1 =server
##target2 =client

##$(target1):$(obj1)
##	gcc $(obj1) -o $(target1)
##$(target2):$(obj2)
##	gcc $(obj2) -o $(target2)
##%.o:%.c
##	gcc -c $< -I ./include/ -o  $@ -Wall
##.PHONY:clean
##clean:
##	-rm $(obj1) $(obj2) $(target1) $(target2) -f


