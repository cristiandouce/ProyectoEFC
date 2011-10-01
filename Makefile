all: master slave

master: master.asm
	avra master.asm

slave: slave.asm
	avra slave.asm

clean:
	rm *.hex *.cof *.HEX *.LST *.obj *.SDI