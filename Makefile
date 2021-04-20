SRCDIR=src
OBJDIR=obj

LEX=flex
YACC=bison
CC=clang

LIBS=-lfl
DFLAGS=-MMD -MP
CFLAGS=-Wall -Wextra -pedantic $(DEBUG_FLAGS)
DEBUG_FLAGS=-g -O0 -DDEBUG

MAINSUFFIX=_main.c
LEXER=lexer
LEXER_FLEX=$(addprefix $(SRCDIR)/, $(LEXER).flex)
LEXER_C=$(addprefix $(SRCDIR)/, $(LEXER)$(MAINSUFFIX))
LEXER_GEN_C=$(addprefix $(SRCDIR)/, $(LEXER).yy.c)
PARSER=parser
PARSER_Y=$(addprefix $(SRCDIR)/, $(PARSER).y)
PARSER_C=$(addprefix $(SRCDIR)/, $(PARSER)$(MAINSUFFIX))
PARSER_GEN_C=$(addprefix $(SRCDIR)/, $(PARSER).tab.c)
ANALYZER=analyzer
ANALYZER_C=$(addprefix $(SRCDIR)/, $(ANALYZER)$(MAINSUFFIX))

GENESRCS=$(LEXER_GEN_C) $(PARSER_GEN_C)
MAINSRCS=$(LEXER_C) $(PARSER_C) $(ANALYZER_C)

BASESRCS=$(wildcard $(SRCDIR)/*.c)
BASESRCS:=$(filter-out $(MAINSRCS), $(BASESRCS))
BASESRCS:=$(filter-out $(GENESRCS), $(BASESRCS))

SRCS=$(BASESRCS) $(GENESRCS) $(MAINSRCS)

BASEOBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(BASESRCS))
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
DEPS=$(OBJS:%.o=%.d)

LEXER_O=$(addprefix $(OBJDIR)/,$(notdir $(LEXER_C:.c=.o)))
LEXER_GEN_O=$(addprefix $(OBJDIR)/,$(notdir $(LEXER_GEN_C:.c=.o)))

PARSER_O=$(addprefix $(OBJDIR)/, $(notdir $(PARSER_C:.c=.o)))
PARSER_GEN_O=$(addprefix $(OBJDIR)/,$(notdir $(PARSER_GEN_C:.c=.o)))

EXECFILES=$(LEXER) $(PARSER)

TESTER=@python test/test.py
PREF=test/
SUFF=_tests
LEXER_TESTS=$(PREF)$(LEXER)$(SUFF)
PARSER_TESTS=$(PREF)$(PARSER)$(SUFF)
ANALYZER_TESTS=$(PREF)$(ANALYZER)$(SUFF)

all:
	$(eval $(call testdir,q,$(LEXER)))
	TEST=$(PREF)$(LEXER)
	@echo $(TEST)

$(LEXER_GEN_C): $(LEXER_FLEX)
	$(LEX) -o $@ $<

$(PARSER_GEN_C): $(SRCDIR)/$(PARSER).y
	$(YACC) -d -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $(DFLAGS) $< -o $@

$(OBJDIR):
	@mkdir -p $@

$(LEXER): $(PARSER_GEN_C) $(LEXER_GEN_O) $(LEXER_O) $(BASEOBJS)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@

$(PARSER): $(PARSER_GEN_O) $(PARSER_O) $(LEXER_GEN_O) $(BASEOBJS)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@

$(ANALYZER): $(ANALYZER_C) $(PARSER_GEN_O) $(LEXER_GEN_O) $(BASEOBJS)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@

test_lexer: $(LEXER)
	$(TESTER) $(LEXER) $(LEXER_TESTS)

test_parser: $(PARSER)
	$(TESTER) $(PARSER) $(PARSER_TESTS)

test_analyzer: $(ANALYZER)
	$(TESTER) $(ANALYZER) $(ANALYZER_TESTS)

test: test_lexer test_parser test_analyzer

clean:
	@rm -rfv $(OBJS) $(DEPS) $(GENESRCS) $(EXECFILES)

-include $(DEPS)
