# Copyright 2026 dywoq - Apache License 2.0
# https://github.com/dywoq/dynamicus

OS   ?= posix
ARCH ?= amd64

MAJOR_VERSION := 1
MINOR_VERSION := 0

# Dynamically choose the right compiler and linker, depending on OS and ARCH.
# Every OS-ARCH pair must configure the following environment variables (some 
# are marked with "optional" tag):
#
# 	COMPILER_CXX
# 	AR
#	PLATFORM_DEPENDENT_CXX_FLAGS (optional)
#	PLATFORM_DEPENDENT_SOURCES (optional)
COMPILER_CXX :=
AR :=
PLATFORM_DEPENDENT_CXX_FLAGS :=
PLATFORM_DEPENDENT_SOURCES :=
ifeq ($(ARCH),amd64)
	ifeq ($(OS),posix)
		COMPILER_CXX := x86_64-linux-gnu-g++
		AR := ar 
        	PLATFORM_DEPENDENT_CXX_FLAGS := -DOS_LINUX -DARCH_AMD64
        	PLATFORM_DEPENDENT_SOURCES := Dynamicus.Posix.cxx
    	else ifeq ($(OS),windows)
            	COMPILER_CXX := x86_64-w64-mingw32-g++
            	AR := ar
            	PLATFORM_DEPENDENT_CXX_FLAGS := -DOS_WINDOWS -DARCH_AMD64
            	PLATFORM_DEPENDENT_SOURCES := Dynamicus.Windows.cxx
        else
		$(error Unsupported OS: $(OS))
        endif
else
	$(error Unsupported architecture: $(ARCH))
endif

# General C++ compiler and linker flags
GENERAL_CXX_FLAGS 	:= --std=c++17 -Wall -Werror

# Configure build mode and validate it.
BUILD_MODE ?= debug
ifeq ($(BUILD_MODE), debug)
	GENERAL_CXX_FLAGS += -g -DDEBUG
else ifeq ($(BUILD_MODE), release)
	GENERAL_CXX_FLAGS += -O2 -DRELEASE
else
$(error "Invalid build mode: $(BUILD_MODE). Expected: release debug")
endif

# Target and directories
BUILD_DIR 	:= .build/$(OS)-$(ARCH)
OBJS_DIR 	:= .objs/$(OS)-$(ARCH)
TARGET		:= $(BUILD_DIR)/libDynamicus.a

# Convert the sources to object files

SOURCES		:= Metadata.cxx
OBJECTS		:= $(patsubst %.cxx,$(OBJS_DIR)/%.o,$(PLATFORM_DEPENDENT_SOURCES) $(SOURCES))

# General C++ compiler and linker flags
GENERAL_CXX_FLAGS 	:= --std=c++17 -Wall -Werror \
			   -DMAJOR_VERSION=$(MAJOR_VERSION) \
			   -DMINOR_VERSION=$(MINOR_VERSION)

# Configure build mode and validate it.
BUILD_MODE ?= debug
ifeq ($(BUILD_MODE), debug)
	GENERAL_CXX_FLAGS += -g -DDEBUG
else ifeq ($(BUILD_MODE), release)
	GENERAL_CXX_FLAGS += -O2 -DRELEASE
else
$(error "Invalid build mode: $(BUILD_MODE). Expected: release debug")
endif

# Archive file
BASE_ARCHIVE_FILE := dynamicus-$(MAJOR_VERSION).$(MINOR_VERSION)-$(BUILD_MODE)
ZIP_ARCHIVE_FILE := $(BASE_ARCHIVE_FILE).zip

# Recipes
all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR) 
	@echo "* AR 	$(TARGET)"
	@rm -fr $(TARGET)
	@$(AR) rcs $(TARGET) $(OBJECTS)

$(OBJS_DIR)/%.o: %.cxx | $(OBJS_DIR)
	@echo "* CXX	$<"
	@$(COMPILER_CXX) $< -o $@ -c $(GENERAL_CXX_FLAGS) $(PLATFORM_DEPENDENT_CXX_FLAGS)

# Create directories and delete them.
$(BUILD_DIR):
	@echo "* MKDIR	$(BUILD_DIR)"
	@mkdir -p $(BUILD_DIR)
$(OBJS_DIR):
	@echo "* MKDIR	$(OBJS_DIR)"
	@mkdir -p $(OBJS_DIR)
clean:
	@echo "* RM	$(BUILD_DIR) $(OBJS_DIR)"
	@rm -fr $(BUILD_DIR) $(OBJS_DIR) 

pack_zip: $(TARGET)
	@echo "* ZIP	$(ZIP_ARCHIVE_FILE)"
	@rm -fr $(ZIP_ARCHIVE_FILE)
# Temporarily create the .pack.zip directory to put the library and header file
# into $(ZIP_ARCHIVE_FILE).
	@mkdir .pack.zip/lib -p
	@mkdir .pack.zip/include -p
	@cp Dynamicus.hxx .pack.zip/include
	@cp $(TARGET) .pack.zip/lib/
	@cp -r README.md .pack.zip/
	@tar -rvf $(ZIP_ARCHIVE_FILE) --transform 's|.pack.zip/include|include/|' .pack.zip/include
	@tar -rvf $(ZIP_ARCHIVE_FILE) --transform 's|.pack.zip/lib|lib/|' .pack.zip/lib
	@tar -rvf $(ZIP_ARCHIVE_FILE) --transform 's|.pack.zip/README.md|README.md|' .pack.zip/README.md
	@rm .pack.zip -fr

