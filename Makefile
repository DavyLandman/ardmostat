update-libraries:
	git submodule update --init
	cd Arduino/libraries/ethercard
	git pull
	cd ../../../

