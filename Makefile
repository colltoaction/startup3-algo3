.PHONY: test all

all: exacta.all

%.all: %
	@make -C $<
