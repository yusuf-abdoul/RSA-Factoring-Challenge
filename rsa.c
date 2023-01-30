#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>

void factor(mpz_t *p, mpz_t n, mpz_t s)
{
	mpz_set(*p, s);

	if (mpz_divisible_p(n, s))
		return;

	if (mpz_divisible_ui_p(*p, 2U))
		mpz_sub_ui(*p, *p, 1U);

	while (!mpz_divisible_p(n, *p) && mpz_cmp_ui(*p, 1U) > 0)
		mpz_sub_ui(*p, *p, 2U);	
}

int is_prime(mpz_t op)
{
	mpz_t s, r;
	int yesOrNo = 0;
	
	mpz_init(s);
	mpz_init(r);

	mpz_sqrt(s, op);
	factor(&r, op, s);
	

	yesOrNo = (mpz_cmp_ui(r, 1) == 0);

	mpz_clear(s);
	mpz_clear(r);

	return yesOrNo;
}

int main(int ac, char **av)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t line_size = 0;
	ssize_t line_length = -1;
	mpz_t n, p, q, r;

	mpz_init(n);
	mpz_init(p);
	mpz_init(q);
	mpz_init(r);

	if (ac != 2)
	{
		fprintf(stderr, "Error: usage: factors <file>\n");
		exit(EXIT_FAILURE);
	}

	fp = fopen(av[1], "r");
	
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open file %s\n", av[1]);
		exit(EXIT_FAILURE);
	}

	while ((line_length = getline(&line, &line_size, fp)) != -1)
	{
		line[line_length - 1] = '\0';
		
		mpz_set_str(n, line, 10);

		while (1)
		{
			while (1)
			{
				mpz_sqrt(p, n);
				factor(&p, n, p);
				if (is_prime(p))
					break;
				mpz_sub_ui(p, p, 2U);
			}
			mpz_tdiv_q(q, n, p);
			if (is_prime(q))
				break;
			mpz_sub_ui(p, p, 2U);
		}


		mpz_out_str(stdout, 10, n);
		printf("=");
		mpz_out_str(stdout, 10, q);
		printf("*");
		mpz_out_str(stdout, 10, p);
		printf("\n");
	}

	free(line);
	fclose(fp);

	mpz_clear(n);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(r);

	return (0);
}
