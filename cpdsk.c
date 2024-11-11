#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
	#define DEBUG				printf("%s - %s : %d\n", __FILE__, __func__, __LINE__);
**/
#define STATUS_FINISH		1
//#define BLOCK_SIZE			1024
#define BLOCK_SIZE			16
typedef unsigned char 		uchar;

int 			getDevice(char *dev_path, char *out_path, int *dev, int *out)
{
	mode_t		mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (!dev_path || !out_path || !dev || !out)
		return (1);
	if ((*dev = open(dev_path, O_RDONLY, mode)) == -1)
	{
		printf("Can't open %s !\n", dev_path);
		return (1);
	}
	if ((*out = open(out_path, O_WRONLY | O_CREAT, mode)) == -1)
	{
		printf("Can't open %s !\n", out_path);
		close(*dev);
		return (1);
	}
	return (0);
}

uint 		my_strlen(const char *s)
{
	uint 		ret = 0;
	
	while (s[ret] != '\0')
		ret++;
	return (ret);
}

int 		say(const char *blah)
{
	uint 		slen;
	
	slen = my_strlen(blah);
	write(1, (const void *)blah, slen);
	return (1);
}

int 		my_abort(int *fd, int dummy)
{
	dummy = 707;
	close(fd[0]);
	close(fd[1]);
	return (say("Abort\n"));
}

int 		writeTo(int *fd, char *block, void *buffer, uint *bytes_read, uchar *done)
{
	write(1, (const void *)block, 4);
	if (*bytes_read == BLOCK_SIZE)
	{
		if (write(fd[1], buffer, BLOCK_SIZE) == -1)
			return (say("0x00\n"));
	}
	else
	{
		if (write(fd[1], buffer, *bytes_read) == -1)
			return (say("0x01\n"));
		*done = STATUS_FINISH;
	}
	return (0);
}

int 		main(int ac, char **av)
{
	unsigned long 		total_bytes = 0;
	uchar 				done = 0;
	uchar 				buffer[BLOCK_SIZE];
	uint 				bytes_write;
	uint 				bytes_read;
	uint 				bytes_toWrite;
	char 				block[4] = "BLK ";
	int 				fd[2];
	char 				*input;
	char 				*output;
	

	if (ac != 3)
	{
		printf("Usage : %s \"/dev/DEVICE_NAME\" \"output.raw\"\n", av[0]);
		return (0);
	}
	input = av[1];
	output = av[2];
	if (getDevice(input, output, &fd[0], &fd[1]))
		return (say("Abort.\n"));
	printf("Reading [%s]\n", input);
	while (done == 0)
	{
		bytes_read = read(fd[0], (void *)buffer, BLOCK_SIZE);
		if (bytes_read >= 0)
		{
			if (writeTo(fd, block, (void *)buffer, &bytes_read, &done))
				return (my_abort(fd, say("write error\n")));
			total_bytes += (unsigned long)bytes_read;
		}
		if (bytes_read == -1)
			return (my_abort(fd, say("read error\n")));
	}
	printf("\n\n----------------------\n");
	printf("BLOCK_SIZE : %u\n", BLOCK_SIZE);
	printf("Total size : %lu\n", total_bytes);
	printf("Input : %s\n", input);
	printf("Output: %s\n", output);
	printf("----------------------\n");
	my_abort(fd, say("DONE\n"));
	return (0);
}
