#include <ft_ssl.h>

void	hash_buffer(ssize_t r, t_params *params, char *buffer)
{
	int			i;
	static char	nbr_du_milieu[4][4] = {{7, 12, 17, 22},
		{5, 9, 14, 20}, {4, 11, 16, 23}, {6, 10, 15, 21}};

	i = 0;
	while (i <= r / 64 - 1)
	{
		compute_buffer(params, nbr_du_milieu, buffer + i * 16 * sizeof(uint));
		i++;
	}
}

int		padd_buffer(int original_file_size, int r, char *buffer)
{
	size_t	size;

	original_file_size += r;
	size = (r / 64 + 1) * 64;
	if (size - r <= 8)
		size += 64;
	((uint8_t*)buffer)[r] = 0x80;
	ft_bzero(buffer + r + 1, size - r - 8 - 1);
	*(uint64_t*)(buffer + size - 8) = original_file_size << 3;
	return (size);
}

int		ft_init(t_params *params, size_t *original_file_size,
		int *fd, char *filename)
{
	initialize_buffer(params->buffer);
	initialize_t(params->t);
	*original_file_size = 0;
	*fd = open(filename, O_RDONLY);
	if (*fd < 0)
	{
		ft_putstr_fd("Can't open file for reading\n", 2);
		return (0);
	}
	return (1);
}

void	print_result(uint buffer[4])
{
	int		i;
	char	buff[33];

	i = 0;
	while (i < 16)
	{
		buff[i * 2] = ((((char *)buffer)[i] & 0xf0) >> 4) + '0';
		buff[i * 2] > '9' ? buff[i * 2] = buff[i * 2] - '9' - 1 + 'a' : 0;
		buff[i * 2 + 1] = (((char *)buffer)[i] & 0xf) + '0';
		buff[i * 2 + 1] > '9' ?
			buff[i * 2 + 1] = buff[i * 2 + 1] - '9' - 1 + 'a' : 0;
		i++;
	}
	buff[32] = 0x0a;
	write(1, buff, 33);
}

int		read_file(char *filename)
{
	char		buffer[8192 + 64];
	int			fd;
	ssize_t		r;
	size_t		original_file_size;
	t_params	params;

	if (!ft_init(&params, &original_file_size, &fd, filename))
		return (0);
	while ((r = read(fd, buffer, 8192)) || original_file_size == 0)
	{
		if (r < 0)
		{
			close(fd);
			ft_putstr_fd("Read error\n", 2);
			return (0);
		}
		if (r < 8192)
			r = padd_buffer(original_file_size, r, buffer);
		hash_buffer(r, &params, buffer);
		original_file_size += r;
	}
	print_result(params.buffer);
	close(fd);
	return (1);
}
