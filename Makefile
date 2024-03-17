obj-m := kernal.o

KERNAL_DIR=/usr/src/linux-headers-6.6.9-amd64

all:
	$(MAKE) -C $(KERNAL_DIR) M=${PWD} modules

clean:
	rm -rf *.o *.ko *.mod.* .symvers *.order *~
