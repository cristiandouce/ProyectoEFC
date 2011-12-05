all: master slave

master: master.asm
	avra master.asm

slave: slave.asm
	avra slave.asm

clean:
	rm *.hex *.cof *.HEX *.LST *.obj *.SDI

clean_win:
	rm \*.hex \*.cof \*.HEX \*.LST \*.obj \*.SDI

program:
	avrdude -c usbtiny -p m88 -U flash:w:$(ROL).hex

hfuse:
	avrdude -c usbtiny -p m88 -U hfuse:w:0xD7:m

lfuse:
	avrdude -c usbtiny -p m88 -U lfuse:w:0xC7:m