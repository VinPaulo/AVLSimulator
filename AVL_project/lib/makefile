ALLEGRO_PATH = C:/allegro
CC = gcc
CFLAGS = -Wall -Wextra -g3
INCLUDES = -I./include -I"$(ALLEGRO_PATH)/include"
LIBS = -L"$(ALLEGRO_PATH)/lib" -lallegro -allegro_primitives -allegro_font -allegro_ttf -allegro_main

SRCS = ../main.c ../avl-tree.c ../allegro-functions.c
OBJS = $(SRCS:.c=.o)
TARGET = ../output/main.exe

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del /F /Q $(OBJS) $(TARGET)