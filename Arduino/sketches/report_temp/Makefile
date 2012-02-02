report_temp.hex: report_temp.ino *.cpp *.h
	scons EXTRA_LIB=../../libraries

upload: 
	scons EXTRA_LIB=../../libraries upload
clean:
	rm -r build/
	rm *.hex
	rm *.elf
