objdir := .obj
objects = $(addprefix $(objdir)/,ast.o generator.o optimize.o parser.o scanner.o main.o)

all: $(objdir) | $(objects)
	$(CC) -g -o tpc $(objects)

$(objdir)/%.o : %.c
	$(CC) -g -c $(CFLAGS) $< -o $@

$(objdir):
	@-mkdir $(objdir)

clean:
	@rm -fR .obj
	@rm -f tpc
	@rm -f full.c

full:
	rm full.c
	echo "/* tpc.h */" >> full.c
	cat tpc.h >> full.c
	echo "/* ast.c */" >> full.c
	cat ast.c >> full.c
	echo "/* scanner.c */" >> full.c
	cat scanner.c >> full.c
	echo "/* parser.c */" >> full.c
	cat parser.c >> full.c
	echo "/* optimize.c */" >> full.c
	cat optimize.c >> full.c
	echo "/* generator.c */" >> full.c
	cat generator.c >> full.c
	gsed -i '/^.*tpc\.h.*$$/d' full.c
