#
# Makefile for romzdb
# Zbigniew ROMANOWSKI
# e-mail: romz@wp.pl
#

.PHONY : romzdb clean


romzdb :
	cd build && \
	ln -s ../src/Makefile Makefile && \
	$(MAKE)



clean :
	cd build && $(MAKE) clean
	cd tests/build && $(MAKE) clean
	cd tests/googletest && $(MAKE) clean

test :
	cd tests/googletest && \
	$(MAKE) gtest.a && \
	cd ../build && \
	ln -s ../src/Makefile Makefile && \
	$(MAKE) && \
	./test.x

