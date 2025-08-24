
# --------------------------------
# adapted from KOS-included Makefile
#
# --------------------------------
#  Nehe02 example adapted from KGL
# (c)2013-2014 Josh Pearson
# (c)2001 Benoit Miller
# --------------------------------   

TARGET = aquarius.elf
CDI  = aquarius.cdi
FLYCAST = ~/flycast/build/flycast
OBJS = main.o 
DISK = ~/mkdcdisc/builddir/mkdcdisc
OBJDUMP = sh-elf-objdump -d -S 

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean: rm-elf
	-rm -f $(OBJS) *.asm

rm-elf:
	-rm -f $(TARGET) *.cdi

disk:
	$(DISK) -e $(TARGET) -o aquarius.cdi

test:
	$(FLYCAST) $(TARGET)

objdump:
	$(OBJDUMP) $(TARGET) > objdump.asm

$(TARGET): $(OBJS)
	kos-c++ -o $(TARGET) $(OBJS) -L$(KOS_BASE)/lib -lKGL

run:
	$(FLYCAST) $(CDI)

dist: $(TARGET)
	-rm -f $(OBJS)
	$(KOS_STRIP) $(TARGET)
