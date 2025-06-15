# Nombre del ejecutable
EXEC = tarea3

# Archivos fuente
SRCS = main.c modules/processor.c modules/validator.c

# Archivos objeto (automáticamente derivados de los fuentes)
OBJS = $(SRCS:.c=.o)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./modules

# Regla principal
all: $(EXEC)

# Cómo construir el ejecutable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Compilar cada .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(EXEC) $(OBJS)