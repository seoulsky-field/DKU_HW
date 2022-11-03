/**
 * mycp.c		: cp command program
 * @author		: Kyoungmin Jeon
 * @email		: yeon2002.kj@dankook.ac.kr
 * @version		: 1.0
 * @date		: 2022. 11. 02.
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#define MAX_BUF 64

int main(int argc, char *argv[]) {
	int read_fd, write_fd, read_size, write_size, load_meta;
	char buf[MAX_BUF];
	struct stat metadata;
	mode_t cp_mode;
	
	if (argc != 3) {
		printf("USAGE: %s file_name target_file_name\n", argv[0]);
		exit(-1);
	}
	
	read_fd = open(argv[1], O_RDONLY);
	if (read_fd < 0) {
		printf("Cannot open %s file with errno %d\n", argv[1], errno);
		exit(-1);
	}
	
	load_meta = stat(argv[1], &metadata);
	if (load_meta == -1) {
		printf("Cannot load %s file's metadata with errno %d\n", argv[1], errno);
		exit(-1);
	}
	cp_mode = metadata.st_mode;
	
	write_fd = open(argv[2], O_RDWR | O_CREAT | O_EXCL, cp_mode);
	if (write_fd < 0) {
		if (errno == 17) {
			printf("The %s file already exists.\n", argv[2]);
		}else {
			printf("Cannot open %s file with errno %d\n", argv[2], errno);
		}
		exit(-1);
	}
	
	while(1) {
		read_size = read(read_fd, buf, MAX_BUF);
		
		if (read_size < 0) {
			printf("Cannot read from %s file, size = %d\n", argv[1], read_size);
			exit(-1);
		}else if (read_size == 0) {
			break;
		}
		
		write_size = write(write_fd, buf, read_size);
		
		if (write_size < 0) {
			printf("Cannot write to %s file, size = %d\n", argv[2], write_size);
			exit(-1);
		}
	}
	close(read_fd);
	close(write_fd);
	
	return 0;
}
