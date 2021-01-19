all :
	$(MAKE) -C build

clean ::
	$(MAKE) -C build clean
	rm -rf html
	rm -rf rtf
	rm -f tags

rebuild :: clean all

test : all
	$(MAKE) -C build test

document ::
	doxygen

tags ::
	ctags --recurse=yes include source
