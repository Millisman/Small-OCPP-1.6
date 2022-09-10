include Makefile.inc



LIBS += ws16_gsm
LIBS += ocpp
# LIBS += utils mcu
LIBS += port
LIBS += utils
# LIBS += app_gsm
# LIBS += app_gui
# LIBS += app_radio
# LIBS += app_store
# LIBS += app_ocpp

# LIBS += utils mcu
# LIBS += utils mcu
CXXFLAGS = $(PRJCXXFLAGS) -I.

SRCDIR = .
DEPDIR = .dep
OBJDIR = obj

SOURCE = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(SOURCE:.c=.o))
DEPS = $(patsubst $(OBJDIR)%, $(DEPDIR)%, $(OBJS:.o=.d))
LIBA = $(patsubst %, $(OBJDIR)/lib%.a, $(LIBS))
LIBL = -L$(OBJDIR) $(patsubst %, -l%, $(LIBS))

TARGET 	= $(notdir $(CURDIR)).hex
ELF 	= $(notdir $(CURDIR)).elf

all: $(TARGET)

upload: $(TARGET)
	@echo [UPLOAD] $<
# 	stty -F /dev/ttyUSB0 115200
# 	echo -e "reboot\\r\\n" > /dev/ttyUSB0
# 	sleep 0.8
	$(AVRDUDE) $(DUDEFLAGS) -U flash:w:$<
#	echo -e "reboot\\r\\n\\nreboot\\r\\n" > /dev/ttyUSB0

fuses:
	@echo [FUSE] Settings fuses: $(FUSES)
	@$(AVRDUDE) $(DUDEFLAGS) $(FUSES)

clean:
	-@for lib in $(LIBS); do (cd $$lib; $(MAKE) clean); done
	-@rm -rf obj
	-@rm -rf .dep
	-@rm $(TARGET)

$(DEPDIR)/%.d: %.c
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s,$*.o:,$(OBJDIR)/$*.o $@: ,g" $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo [GCC] $(notdir $<)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.a: force_look
	@cd $(patsubst lib%, %, $(basename $(notdir $@)) ; $(MAKE) $(MFLAGS))

force_look:
	@true

$(OBJDIR)/$(ELF): $(LIBA) $(OBJS)
	@echo [LINK] $(notdir $@)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(LIBL) -o $@
	@echo
	@$(SIZE) $(SIZEFLAGS) $@

%.hex: $(OBJDIR)/%.elf
	@echo [HEX] $@
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

-include $(DEPS)
