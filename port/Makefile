include ../Makefile.inc

SHAREDINC = -I..

SRCDIR = .
DEPDIR = .dep
OBJDIR = obj
OUTDIR = ../obj

CXXFLAGS = $(PRJCXXFLAGS) $(SHAREDINC)

SOURCE = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(SOURCE:.c=.o))
DEPS = $(patsubst $(OBJDIR)%, $(DEPDIR)%, $(OBJS:.o=.d))

TARGET = $(OUTDIR)/lib$(notdir $(CURDIR)).a

all: $(OUTDIR) $(TARGET)

clean:
	-@rm $(TARGET)
	-@rm -rf .dep
	-@rm -rf obj

$(OUTDIR):
	@mkdir $@

$(TARGET): $(OBJS)
	@echo [LIB] $(notdir $@)
	@$(AR) rcs $@ $^

$(DEPDIR)/%.d: %.c
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s,$*.o:,$(OBJDIR)/$*.o $@: ,g" $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo [C++] $(notdir $<)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

-include $(DEPS)
