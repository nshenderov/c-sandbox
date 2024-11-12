CC = gcc
CFLAGS = -I$(INCLDIR) -ansi -pedantic-errors -Wall -Wextra
CFLAGS_REL = -DNDEBUG
CFLAGS_DBG = -g
DEPFLAGS = -MM -MT $@
MAKEDEPEND = $(CC) $(DEPFLAGS) $(CFLAGS) -c
MKDIR_P = @mkdir -p

INCLDIR = include
SRCDIR = src
DEPDIR = .d
OBJDIR = .o
OBJDIR_REL = $(OBJDIR)/release
OBJDIR_DBG = $(OBJDIR)/debug
TESTDIR = test
TESTEXEDIR = .out
TESTSEXEDIR_REL = $(TESTEXEDIR)/release
TESTSEXEDIR_DBG = $(TESTEXEDIR)/debug
LIBDIR = .bin
LIBDIR_REL = $(LIBDIR)/release
LIBDIR_DBG = $(LIBDIR)/debug
DIRS = $(DEPDIR) $(OBJDIR_REL) $(OBJDIR_DBG) $(LIBDIR_REL) \
							$(LIBDIR_DBG) $(TESTSEXEDIR_REL) $(TESTSEXEDIR_DBG)

SRCS = $(wildcard $(SRCDIR)/*.c)

OBJS_REL = $(patsubst $(SRCDIR)/%.c, $(OBJDIR_REL)/%.o, $(SRCS))
OBJS_DBG = $(patsubst $(SRCDIR)/%.c, $(OBJDIR_DBG)/%.o, $(SRCS))

TESTS = $(wildcard $(TESTDIR)/*_test.c)
TESTOBJS_REL = $(patsubst $(TESTDIR)/%_test.c, $(OBJDIR_REL)/%_test.o, $(TESTS))
TESTOBJS_DBG = $(patsubst $(TESTDIR)/%_test.c, $(OBJDIR_DBG)/%_test.o, $(TESTS))
TESTSEXE_REL = $(patsubst $(TESTDIR)/%_test.c, \
										$(TESTSEXEDIR_REL)/%_test.out, $(TESTS))
TESTSEXE_DBG = $(patsubst $(TESTDIR)/%_test.c, \
										$(TESTSEXEDIR_DBG)/%_test.out, $(TESTS))

LDFLAGS_DBG = -L$(LIBDIR_DBG) -Wl,-rpath=$(LIBDIR_DBG) \
					-Wl,-rpath=../$(LIBDIR_DBG) -Wl,-rpath=../../$(LIBDIR_DBG)
LDFLAGS_REL = -L$(LIBDIR_REL) -Wl,-rpath=$(LIBDIR_REL) \
					-Wl,-rpath=../$(LIBDIR_DBG) -Wl,-rpath=../../$(LIBDIR_DBG)
LDLIBS_REL = $(patsubst $(SRCDIR)/%.c, $(LIBDIR_REL)/lib%.so, $(SRCS))
LDLIBS_DBG = $(patsubst $(SRCDIR)/%.c, $(LIBDIR_DBG)/lib%.so, $(SRCS))


all: release debug test test_release

$(DIRS): ; $(MKDIR_P) $(DIRS)

release: $(DIRS)
release: CFLAGS += $(CFLAGS_REL) -fPIC
release: $(OBJS_REL)
release: $(LDLIBS_REL)

$(OBJDIR_REL)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$*.c

$(LIBDIR_REL)/lib%.so : OBJS = $(patsubst $(INCLDIR)/%.h, $(OBJDIR_REL)/%.o, \
										$(filter %.h, $(file < $(DEPDIR)/$*.d)))
$(LIBDIR_REL)/lib%.so : $(DEPDIR)/%.d $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS)


debug: $(DIRS)
debug: CFLAGS += $(CFLAGS_DBG) -fPIC
debug: $(OBJS_DBG)
debug: $(LDLIBS_DBG)

$(OBJDIR_DBG)/%.o: $(SRCDIR)/%.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$*.c

$(LIBDIR_DBG)/lib%.so : OBJS = $(patsubst $(INCLDIR)/%.h, $(OBJDIR_DBG)/%.o, \
										$(filter %.h, $(file < $(DEPDIR)/$*.d)))
$(LIBDIR_DBG)/lib%.so : $(DEPDIR)/%.d $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS)


test: debug
test: CFLAGS += $(CFLAGS_DBG)
test: $(TESTOBJS_DBG)
test: $(TESTSEXE_DBG)
$(OBJDIR_DBG)/%_test.o : $(TESTDIR)/%_test.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $(TESTDIR)/$*_test.c
$(TESTSEXEDIR_DBG)/%_test.out : $(OBJDIR_DBG)/%_test.o
	$(CC) $(CFLAGS) -o $@ $(OBJDIR_DBG)/$*_test.o $(LDFLAGS_DBG) -l$*

test_release: release
test_release: CFLAGS += $(CFLAGS_REL)
test_release: $(TESTOBJS_REL)
test_release: $(TESTSEXE_REL)
$(OBJDIR_REL)/%_test.o : $(TESTDIR)/%_test.c $(INCLDIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $(TESTDIR)/$*_test.c
$(TESTSEXEDIR_REL)/%_test.out : $(OBJDIR_REL)/%_test.o
	$(CC) $(CFLAGS) -o $@ $(OBJDIR_REL)/$*_test.o $(LDFLAGS_REL) -l$*


$(DEPDIR)/%.d : $(SRCDIR)/%.c
	$(MAKEDEPEND) $(OUTPUT_OPTION) $<


c: clean
clean:
	rm -f $(DEPDIR)/* $(OBJDIR_REL)/* $(OBJDIR_DBG)/* $(TESTSEXEDIR_REL)/*.out \
														$(TESTSEXEDIR_DBG)/*.out
cl: clean_libs
clean_libs:
	rm -f  $(LIBDIR_REL)/* $(LIBDIR_DBG)/*
ca: c cl


.PRECIOUS: $(DEPDIR)/%.d
.PHONY: all test debug release c clean cl clean_libs ca

-include $(wildcard $(DEPDIR)/*.d)