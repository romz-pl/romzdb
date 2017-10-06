#
# Makefile for romzdb
# Zbigniew ROMANOWSKI
# e-mail: romz@wp.pl
#

.PHONY : romzdb clean


romzdb :
	cd build && $(MAKE)



clean :
	cd build && $(MAKE) clean

test :
	cd tests/googletest && $(MAKE) gtest.a && \
	cd tests/build && $(MAKE) && ./test.x

