report_tempv2.hex: report_tempv2.ino *.c *.h
	scons EXTRA_LIB=../../libraries

upload: 
	scons EXTRA_LIB=../../libraries upload
clean:
	rm -r build/
	rm *.hex
	rm *.elf
