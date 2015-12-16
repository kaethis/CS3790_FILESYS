filesys:
	g++ ./filesys.cpp ./driver.cpp -lncurses -std=c++11 -o filesys
clean:
	rm filesys
archive:
	tar cvf ../CS3790_4374851_FILESYS.tar .

