#include <ft_ssl.h>

uint	cshift(uint nbr, uint s)
{
	return (uint)(((nbr) << (s)) |
		(((nbr) & ((-1u) << (32 - (s)))) >> (32 - (s))));
}

void	compute_buffer(t_params_md5 *params, char nbr_du_milieu[4][4], void *buffer)
{
	uint	buffer_save[4];

	params->x = buffer;
	ft_memcpy(buffer_save, params->buffer, sizeof(params->buffer));
	stage1(params, nbr_du_milieu);
	stage2(params, nbr_du_milieu);
	stage3(params, nbr_du_milieu);
	stage4(params, nbr_du_milieu);
	params->buffer[0] = (uint)params->buffer[0] + (uint)buffer_save[0];
	params->buffer[1] = (uint)params->buffer[1] + (uint)buffer_save[1];
	params->buffer[2] = (uint)params->buffer[2] + (uint)buffer_save[2];
	params->buffer[3] = (uint)params->buffer[3] + (uint)buffer_save[3];
}

int		main(int argc, char **argv)
{
	t_opt	opt;

	if (argc == 1)
	{
		ft_putstr_fd("Usage : not like this\n", 2);
		return (1);
	}
	if (!parse_options(argc, argv, &opt))
		return (2);
	return (88);
}
