update-libraries: update-ethercard update-bootstrap update-arscons update-aeslib


init-submodules:
	git submodule update --init

update-ethercard: init-submodules
	cd Arduino/libraries/ethercard && git pull

update-aeslib: init-submodules
	cd Arduino/libraries/aeslib && git pull

update-bootstrap: init-submodules
	cd Server/libraries/bootstrap && git pull

update-arscons: init-submodules
	cd Arduino/arscons && git pull

start-db:
	pg_ctl -D /usr/local/var/postgres -l /usr/local/var/postgres/server.log start

stop-db:
	pg_ctl -D /usr/local/var/postgres stop -s -m fast
