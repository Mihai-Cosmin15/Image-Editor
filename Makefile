build:
	gcc *.c -o image_editor -lm -Wall -Wextra -std=c99

clean:
	rm -f image_editor