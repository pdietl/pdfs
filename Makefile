obj-m := pdfs.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build

all: pdfs.ko mkfs-pdfs

pdfs.ko: pdfs.c pdfs.h
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) modules

mkfs-pdfs.o: pdfs.h

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) clean
