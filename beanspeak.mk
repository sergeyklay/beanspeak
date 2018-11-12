clean: clean-dirs

TESTSDIR = $(top_srcdir)/tests

.PHONY: clean-dirs
clean-dirs:
	rm -rf $(top_srcdir)/include

.PHONY: investigation
investigation:
	@for i in `echo "$(wildcard $(TESTSDIR)/*.out)"`; do \
		echo "-- START $$i"; \
		cat $$i; \
		echo ;\
		echo "-- END"; \
	done;
	@for i in `echo "$(wildcard $(TESTSDIR)/*.mem)"`; do \
		echo "-- START $$i"; \
		cat $$i; \
		echo ;\
		echo "-- END"; \
	done;
