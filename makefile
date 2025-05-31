
#-------------------------------------------------------------------------------

APP           := XGame
SERVER_NAME   := Comm
TARGET_NAME   := $(SERVER_NAME)
TARGET        := lib$(SERVER_NAME).a
TARSFILE_DIR  := /home/tarsproto/$(APP)/$(SERVER_NAME)
CFLAGS        += -lm
CXXFLAGS      += -lm

CONFIG      :=
#EXTRA_LIB 	:= libpcre.a

RELEASE_ICE += *.h

INCLUDE   += -I/usr/local/cpp_modules/wbl/include
LIB       += -L/usr/local/cpp_modules/wbl/lib -lwbl

INCLUDE   += -I/usr/local/cpp_modules/pcre/include
LIB       += -L/usr/local/cpp_modules/pcre/lib -lpcre

#-------------------------------------------------------------------------------

publish:
	rm -rvf $(TARSFILE_DIR)
	mkdir -vp $(TARSFILE_DIR)
	@make cleanall
	@make all 
	cp -rf *.h $(TARSFILE_DIR)
	cp -rf *.hpp $(TARSFILE_DIR)
	cp -rf *.tars $(TARSFILE_DIR)
	cp -rf ${TARGET} $(TARSFILE_DIR)
	echo "INCLUDE += -I$(TARSFILE_DIR)"  >  $(TARSFILE_DIR)/$(TARGET_NAME).mk;
	echo "REMOTE_OBJ += $(TARSFILE_DIR)/$(TARGET) "  >> $(TARSFILE_DIR)/$(TARGET_NAME).mk;

include /usr/local/tars/cpp/makefile/makefile.tars

#-------------------------------------------------------------------------------
