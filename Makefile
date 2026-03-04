PYTHON ?= python3
VENV_DIR ?= .venv
VENV_BIN := $(VENV_DIR)/bin
PIP := $(VENV_BIN)/pip
MKDOCS := $(VENV_BIN)/mkdocs
DEPS_STAMP := $(VENV_DIR)/.deps-installed

.PHONY: help venv install build run clean-site clean-venv reinstall

.DEFAULT_GOAL := help

help:
	@echo "Targets disponiveis:"
	@echo "  make venv        - cria o ambiente virtual em .venv"
	@echo "  make install     - instala/atualiza dependencias no .venv"
	@echo "  make build       - gera o site MkDocs em ./site"
	@echo "  make run         - sobe servidor local do MkDocs"
	@echo "  make clean-site  - remove a pasta ./site"
	@echo "  make clean-venv  - remove a pasta ./.venv"
	@echo "  make reinstall   - recria .venv e reinstala dependencias"

venv: $(VENV_BIN)/python

$(VENV_BIN)/python:
	$(PYTHON) -m venv $(VENV_DIR)

$(DEPS_STAMP): requirements.txt | venv
	$(PIP) install --upgrade pip
	$(PIP) install -r requirements.txt
	@touch $(DEPS_STAMP)

install: $(DEPS_STAMP)

build: install
	$(MKDOCS) build --clean

run: install
	$(MKDOCS) serve -a 127.0.0.1:8000

clean-site:
	rm -rf site

clean-venv:
	rm -rf $(VENV_DIR)

reinstall: clean-venv install
