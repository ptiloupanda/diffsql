buildir=
libdir=/usr/lib64/
include=-I. -I /usr/include/mysql-cppconn-8/jdbc/ -I../include -I/usr/include/mysql 
CC=g++
ldflags=-std=c++11 -D_FREETDS_LIBRARY_SOURCE -DUNIXODBC -D_REENTRANT -D_THREAD_SAFE -DDEBUG=1 -Wall -Wno-long-long -pthread -g -O2 -MT -DHAVE_CONFIG_H

all: check_diff
check_diff: check_diff.o 
	/bin/bash /usr/bin/libtool --tag=CC   --mode=link gcc -pthread -g -O2 -Wdeclaration-after-statement -no-install -lstdc++  -L/usr/lib -L$(buildir)/lib  -L$(libdir)/.libs -R $(libdir)/.libs  -o check_diff check_diff.o  $(libdir)/libsybdb.so -L/usr/lib/libmysqlpp.so -lmysqlcppconn -lmysqlclient -lcurl  -ltoolbox
	g++ -pthread -g -O2 -Wdeclaration-after-statement -o check_diff check_diff.o -L$(buildir)/lib -L/usr/lib   -L/usr/lib/libmysqlpp.so -lmysqlcppconn -lcurl -ltoolbox  -lmysqlclient -lstdc++  -L$(libdir)/.libs $(libdir)/libsybdb.so  -lrt -ldl -pthread -Wl,-rpath -Wl,$(libdir)/.libs -Wl,-rpath -Wl,$(libdir)/.libs

check_diff.o: check_diff.cpp
	$(CC) $(include)  $(ldflags) -MD -MP -MF ./.po/check_diff.Tpo -c -o check_diff.o check_diff.cpp
	mv -f ./.po/check_diff.Tpo ./.po/check_diff.Po

clean:
	rm *.o 
	rm check_diff
