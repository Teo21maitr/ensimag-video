# Makefile racine — orchestre les builds C (pthreads) et C++ depuis un seul endroit.
# Les artefacts vont dans build/c/ et build/cxx/.
#
# Cibles principales :
#   make            → compile tout (C + C++)
#   make c          → compile uniquement la version C
#   make cpp        → compile uniquement la version C++
#   make hello10-c  → compile hello10 (C)
#   make hello10-cpp→ compile hello10 (C++)
#   make ensivideo  → compile le lecteur vidéo (C)
#   make test       → Coccinelle static analysis (C uniquement)
#   make check      → idem, mode verbeux
#   make clean      → supprime tout le dossier build/

BUILD_C   := build/c
BUILD_CXX := build/cxx

# --------------------------------------------------------------------------
.PHONY: all c cpp hello10-c hello10-cpp ensivideo test check clean

all: c cpp

# --- Version C/pthreads ---------------------------------------------------

$(BUILD_C)/Makefile:
	mkdir -p $(BUILD_C)
	cd $(BUILD_C) && cmake ../../C

c: $(BUILD_C)/Makefile
	$(MAKE) -C $(BUILD_C)

hello10-c: $(BUILD_C)/Makefile
	$(MAKE) -C $(BUILD_C) hello10

ensivideo: $(BUILD_C)/Makefile
	$(MAKE) -C $(BUILD_C) ensivideo

test: $(BUILD_C)/Makefile
	$(MAKE) -C $(BUILD_C) test

check: $(BUILD_C)/Makefile
	$(MAKE) -C $(BUILD_C) check

# --- Version C++ ------------------------------------------------------------

$(BUILD_CXX)/Makefile:
	mkdir -p $(BUILD_CXX)
	cd $(BUILD_CXX) && cmake ../../C++

cpp: $(BUILD_CXX)/Makefile
	$(MAKE) -C $(BUILD_CXX)

hello10-cpp: $(BUILD_CXX)/Makefile
	$(MAKE) -C $(BUILD_CXX) hello10

# --- Nettoyage --------------------------------------------------------------

clean:
	rm -rf build
