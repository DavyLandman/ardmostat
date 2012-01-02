update-libraries: update-ethercard update-bootstrap


init-submodules:
	git submodule update --init

update-ethercard: init-submodules
	cd Arduino/libraries/ethercard
	git pull
	cd ../../../

update-bootstrap: init-submodules
	cd Server/libraries/bootstrap
	git pull
	cd ../../../

