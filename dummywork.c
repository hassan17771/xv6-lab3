#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int i;
    printf(1, "Process %d is doing some work...\n", getpid());
    for (i = 0; i < 50000000; i++)
    {
        for (int j = 0; j < 10; j++) {
        asm volatile(""); // prevent compiler from optimizing it away
        }
        if (i == 100000) {
            print_procinfo();
        }
    }
    printf(1, "Process %d finished work\n", getpid());
    exit();
}
