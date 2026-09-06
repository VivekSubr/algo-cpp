ALGOVIZ_REPO_ROOT ?= $(shell git rev-parse --show-toplevel)
ALGOVIZ_PYTHON ?= python3
ALGOVIZ_MODULE_PATH := $(ALGOVIZ_REPO_ROOT)/visualizer/src

.PHONY: visualize
visualize:
	@if [ -z "$(strip $(FILE))" ]; then \
		echo "usage: make visualize FILE=<source-file> [FUNCTION=<function>] [ALGOVIZ_ARGS='<options>']"; \
		exit 2; \
	fi
	@PYTHONPATH="$(ALGOVIZ_MODULE_PATH)$${PYTHONPATH:+:$${PYTHONPATH}}" \
		"$(ALGOVIZ_PYTHON)" -m algoviz "$(FILE)" \
		$(if $(strip $(FUNCTION)),"$(FUNCTION)",) $(ALGOVIZ_ARGS)
