## Process this file with automake to generate Makefile.in

SUBDIRS = src include doc

EXTRA_DIST = pgplot_libs

# WEB_SERVE and WEB_PATH must be defined before running this.
export: dist
	scp $(PACKAGE)-$(VERSION).tar.gz $(WEB_SERVE):$(WEB_PATH)/software/.
	ssh $(WEB_SERVE) "cd $(WEB_PATH)/software ; \rm -f $(PACKAGE).tar.gz ; ln -s $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE).tar.gz; \rm -fr $(PACKAGE); mkdir $(PACKAGE)"
	scp -r doc/html $(WEB_SERVE):$(WEB_PATH)/software/$(PACKAGE)/.
	ssh $(WEB_SERVE) "cd $(WEB_PATH)/software; sed -e 's%<\!-- subs version --><td>.*</td>%<\!-- subs version --><td>$(VERSION)</td>%' index.html > junk.html; mv junk.html index.html"
	ssh $(WEB_SERVE) "cd $(WEB_PATH)/software; sed -e 's%<\!-- subs date --><td>.*</td>%<\!-- subs date --><td>$(DATE)</td>%' index.html > junk.html; mv junk.html index.html"


## File of aliases

ALIASES = Subs

.PHONY : $(ALIASES)

DATE    = $(shell date)

progdir = @bindir@/@PACKAGE@

prog_DATA = $(ALIASES)

$(ALIASES):
	@echo '#' > $(ALIASES)
	@echo '# This file defines aliases for the $(PACKAGE) v$(VERSION) software.' >> $(ALIASES)
	@echo '# Source it to define them.' >> $(ALIASES)
	@echo '#' >> $(ALIASES)
	@echo 'alias tcorr     $(progdir)/tcorr'       >> $(ALIASES)
	@echo 'alias weekday   $(progdir)/weekday'     >> $(ALIASES)
	@echo 'alias gap       $(progdir)/gap'         >> $(ALIASES)
	@echo 'alias interp    $(progdir)/interp'      >> $(ALIASES)
	@echo ' ' >> $(ALIASES)
	@echo 'echo " "' >> $(ALIASES)
	@echo 'echo "Welcome to $(PACKAGE) version $(VERSION), built $(DATE)."' >> $(ALIASES)
	@echo 'echo " "' >> $(ALIASES)
	@echo 'echo "Commands available are: gap, interp, tcorr, weekday."' >> $(ALIASES)
	@echo 'echo " "' >> $(ALIASES)
	@echo 'echo "Point a browser at ${prefix}/html/$(PACKAGE)/index.html for help."' >> $(ALIASES)
	@echo 'echo " "' >> $(ALIASES)

