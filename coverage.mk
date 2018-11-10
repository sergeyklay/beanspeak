OUTCOV=coverage.info
DIRCOV=coverage

.PHONY: clean-coverage
clean-coverage:
	-rm -fr coverage
	-rm -f coverage.info

.PHONY: coverage-initial
coverage-initial: clean-coverage
	@$(LCOV) --directory . --zerocounters
	@$(LCOV) --directory . --capture --compat-libtool --initial --base-directory=. --output-file coverage.info

.PHONY: coverage-capture
coverage-capture:
	@$(LCOV) --no-checksum --directory . --capture --compat-libtool --output-file coverage.info
	@$(LCOV) \
		--remove coverage.info "/usr*" \
		--remove coverage.info "${HOME}/.phpenv/*" \
		--compat-libtool \
		--output-file coverage.info

.PHONY: coverage-html
coverage-html: coverage-capture
	@$(GENHTML) --legend --output-directory coverage --title "Beanspeak code coverage" coverage.info
