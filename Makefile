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
	avrdude -c usbtiny -p m88 flash:w:$(MICRO).hex