#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define COL_LIMIT 120
#define ROW_LIMIT 32
#define MOD 95

int main()
{
	char ch;
	struct termios newt, oldt;
	struct pollfd pfd;

	tcgetattr(0, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &newt);
	
	pfd.fd = 0;
	pfd.events = POLLIN;

	printf("\033[2J\033[?25l");
	int row[COL_LIMIT];

	srand(time(NULL));

	int trail_len = rand() % ROW_LIMIT + 2;
	for (int i = 1; i <= COL_LIMIT; i++)
	{
		row[i] = rand() % ROW_LIMIT + 1;
	}

	while(1)
	{
			//for (int col = 1; col <= COL_LIMIT; col = col + 2) {
			//		printf("\033[%d;%dH ", row, col);
			//}
			// trail length --> random
			// row start ---> random
			// sabu row_start to row_start + trail_len --> 1 -> " ", last-> bright, rest-> dark green 
			
		
			for (int col = 1; col <= COL_LIMIT; col = col + 2) {
					for (int r_offset = 0; r_offset < trail_len; r_offset++)
					{
							int rand_char = rand() % MOD + 32;
							//int rand_char = rand() % 2 + '0';
							printf("\033[%d;%dH\033[32m%c\033[0m", row[col] + r_offset, col, rand_char);
					}
			}	
			for (int col = 1; col <= COL_LIMIT; col = col + 2) {
					int rand_char = rand() % MOD + 32;
					//int rand_char = rand() % 2 + '0';
					printf("\033[%d;%dH\033[97m%c\033[0m", row[col] + trail_len, col, rand_char);
			}

			for (int i = 1; i <= COL_LIMIT; i++)
			{
					row[i]++;
					if (row[i] > ROW_LIMIT) {
							row[i] = rand() % ROW_LIMIT + 1;
							trail_len = rand() % ROW_LIMIT + 2;
					}
			}

			fflush(stdout);

			int ret = poll(&pfd, 1, 150);
			if (ret > 0 &&
							(pfd.revents & POLLIN) &&
							read(0, &ch, 1) == 1 &&
							ch == 'q') {
					break;
			}
			printf("\033[2J");
	}

	tcsetattr(0, TCSANOW, &oldt);

	printf("\033[2J\033[?25h\033[H");

	return 0;
}
