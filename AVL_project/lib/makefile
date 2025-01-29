CC = gcc
CFLAGS = -Wall -Wextra -g3
INCLUDES = -I./include -I"$(ALLEGRO_PATH)/include"
LIBS = -L"$(ALLEGRO_PATH)/lib" -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
ALLEGRO_PATH = ../allegro

SRCS = ../main.c ../avl-tree.c ../allegro-functions.c
OBJS = $(SRCS:.c=.o)
TARGET = ../output/main.exe

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del /F /Q $(OBJS) $(TARGET)