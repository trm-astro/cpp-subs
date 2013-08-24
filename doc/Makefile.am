## Process this file with automake to generate Makefile.in
##

EXTRA_DIST = sphinx

install-data-local: 
	cd sphinx; python genrst.py
	cd sphinx; make html

#uninstall-html:
#	rm -r $(prefix)/html/$(PACKAGE)

#clean-html:
#	-rm -r html
