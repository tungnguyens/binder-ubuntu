SUBDIRS := driver/ashmem driver/binder libs servicemanager test_client test_server

.PHONY: all clean

all:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
		echo; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir -f Makefile clean; \
		echo; \
	done
