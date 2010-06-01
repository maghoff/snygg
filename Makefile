default: release

debug: .PHONY
	+make -C $@

release: .PHONY
	+make -C $@

install: .PHONY
	release/install.sh

bundle: .PHONY
	./create_deb.sh

.PHONY:

