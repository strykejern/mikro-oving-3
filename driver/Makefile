###############################################################################
#
# Linux kjernemodul Makefile
#
###############################################################################

# navn på kjernemodul (navnet på C-fila men uten ".c")
MODULE = driver

ifneq ($(KERNELRELEASE),)
obj-m := ${MODULE}.o

else

# sti til linux-kildekoden
KERNELDIR := ../linux/
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

.PHONY : clean
clean:
	rm -rf *.o *.ko .*.cmd *.mod.* .tmp* 

