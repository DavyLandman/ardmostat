update-libraries: update-ethercard update-bootstrap update-arscons update-aeslib


init-submodules:
	git submodule update --init

update-ethercard: init-submodules
	cd Arduino/libraries/ethercard
	git pull
	cd ../../../

update-aeslib: init-submodules
	cd Arduino/libraries/aeslib
	git pull
	cd ../../

update-bootstrap: init-submodules
	cd Server/libraries/bootstrap
	git pull
	cd ../../../

update-arscons: init-submodules
	cd Arduino/arscons
	git pull
	cd ../../

start-db:
	pg_ctl -D /usr/local/var/postgres -l /usr/local/var/postgres/server.log start

stop-db:
	pg_ctl -D /usr/local/var/postgres stop -s -m fast
