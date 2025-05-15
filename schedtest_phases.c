#include "types.h"
#include "stat.h"
#include "user.h"

char *argv[] = {"dummywork", 0};

// Simple CPU delay loop (tuned for short waits)
void short_delay(int loops)
{
    for (volatile int i = 0; i < loops * 1000000; i++)
    {
        asm volatile(""); // Prevent optimization
    }
}

// Helper to wait for n children
void wait_for_all_children(int n)
{
    for (int i = 0; i < n; i++)
    {
        wait();
    }
}

// Phase 1: FCFS
void phase_one()
{
    printf(1, "\n--- Phase 1: FCFS with 3 normal processes ---\n");

    for (int i = 0; i < 3; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            printf(1, "Phase 1 - Normal Process %d\n", getpid());
            exec("dummywork", argv);
            printf(1, "exec failed (normal)\n");
            exit();
        }
    }

    wait_for_all_children(3);
}

// Phase 2: EDF with fork_rt and busy-loop delay
void phase_two()
{
    printf(1, "\n--- Phase 2: EDF with 1 RT parent and 4 RT children (delayed) ---\n");

    int parent_pid = fork_rt(200);
    if (parent_pid == 0)
    {
        printf(1, "RT parent %d started\n", getpid());

        int deadlines[] = {180, 160, 140, 120};
        for (int i = 0; i < 4; i++)
        {
            short_delay(1); // Small delay between child creation
            int child_pid = fork_rt(deadlines[i]);
            if (child_pid == 0)
            {
                printf(1, "  -> RT Child %d with deadline %d\n", getpid(), deadlines[i]);
                exec("dummywork", argv);
                printf(1, "exec failed (RT child)\n");
                exit();
            }
        }

        wait_for_all_children(4);
        printf(1, "RT parent %d done.\n", getpid());
        exit();
    }

    wait(); // Wait for RT parent
}

// Phase 3: Starvation + Aging
void phase_three()
{
    printf(1, "\n--- Phase 3: Starvation and Aging test ---\n");

    int pid = fork();
    if (pid == 0)
    {
        printf(1, "Phase 3 - Normal process %d\n", getpid());
        exec("dummywork", argv);
        printf(1, "exec failed (normal)\n");
        exit();
    }

    short_delay(3); // Short delay to allow starvation

    int deadlines[] = {100, 110, 120, 130, 140};
    for (int i = 0; i < 5; i++)
    {
        int rt_pid = fork_rt(deadlines[i]);
        if (rt_pid == 0)
        {
            printf(1, "  -> RT process %d with deadline %d\n", getpid(), deadlines[i]);
            exec("dummywork", argv);
            printf(1, "exec failed (RT)\n");
            exit();
        }
    }

    wait_for_all_children(6); // 1 normal + 5 RT
}

int main(void)
{
    // phase_one();
    // phase_two();
    phase_three();

    printf(1, "\nAll phases complete.\n");
    exit();
}
