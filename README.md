*This project has been created as part of the 42 curriculum by alehamad.*

# Philosophers

The dining philosophers problem, solved with threads and mutexes in C.

![language](https://img.shields.io/badge/language-C-000000?style=flat-square)
![school](https://img.shields.io/badge/school-42-000000?style=flat-square)
![threads](https://img.shields.io/badge/threads-pthread-000000?style=flat-square)

## Overview

A number of philosophers sit around a round table with a bowl of spaghetti in the middle and **one fork between each pair of neighbours**. A philosopher needs **two forks** to eat. They eat, sleep and think, in that order, and die if they go too long without eating. The program simulates the table and must keep everyone alive as long as possible, and report a death within 10 ms when it happens.

Each philosopher is a **thread**, each fork is a **mutex**.

The goal of the project is to understand:

- how to create and join **threads** with `pthread_create` / `pthread_join`
- how to protect shared data with **mutexes**, and what a **data race** is
- how a **deadlock** happens when every philosopher holds one fork and waits for the other, and how to avoid it
- how to measure and wait for time precisely with `gettimeofday` and `usleep`
- how a **monitor** can watch other threads without slowing them down

## Features

| Feature | Details |
|---|---|
| One thread per philosopher | Every philosopher runs the same routine: take forks, eat, sleep, think |
| One mutex per fork | Philosopher `i` uses fork `i` and fork `(i + 1) % n` |
| No deadlock | Forks are always taken in the same global order (lowest address first), so a waiting cycle cannot form |
| Staggered start | Even philosophers start 1 ms later, so neighbours do not all reach for the same fork at once |
| Thinking time | Computed as `time_to_die - (time_to_eat + time_to_sleep) - 20` ms (never negative), so a philosopher does not grab forks back too early and starve its neighbours |
| Monitor | The main thread checks every philosopher's last meal in a loop and prints `died` as soon as `time_to_die` is exceeded |
| Meal limit | With the optional 5th argument, the simulation stops once every philosopher has eaten that many times |
| One philosopher | Takes the only fork, waits `time_to_die`, then dies |
| Clean output | A print mutex keeps lines from mixing, and nothing is printed after a death |
| Clean exit | Every thread is joined, every mutex destroyed, every allocation freed |

### Output

Every state change prints one line, with the time in ms since the start:

```
<timestamp> <id> has taken a fork
<timestamp> <id> is eating
<timestamp> <id> is sleeping
<timestamp> <id> is thinking
<timestamp> <id> died
```

### Input validation

| Case | Behaviour |
|---|---|
| Not 4 or 5 arguments | Exit status `1` |
| A character other than a digit (`-800`, `abc`, `+5`) | Exit status `1` |
| A value of `0` | Exit status `1` |

## Build

### Requirements

- Linux or macOS
- `cc` / `gcc` / `clang` with `pthread`
- `make`

### Compile

```bash
git clone <repository-url> philosophers
cd philosophers
make
```

This builds `philo` at the root of the repository, with `-Wall -Wextra -Werror -pthread`.

### Make rules

| Rule | Action |
|---|---|
| `make` / `make all` | Build `philo` |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the binary |
| `make re` | `fclean` then `all` |

## Usage

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Meaning |
|---|---|
| `number_of_philosophers` | Number of philosophers, and of forks |
| `time_to_die` | Time in ms a philosopher can live without starting a meal |
| `time_to_eat` | Time in ms spent eating, holding two forks |
| `time_to_sleep` | Time in ms spent sleeping |
| `number_of_times_each_philosopher_must_eat` | Optional: stop once everyone has eaten this many times |

## How it works

```
 main ──► verif_arg ──► init_all (forks, mutexes, philosophers)
                              │
                              ▼
                         ft_philo
                     ┌────────┴─────────────────────────┐
                     ▼                                  ▼
        pthread_create × n                         monitor (main thread)
   ┌──────────────────────────┐            ┌───────────────────────────┐
   │ routine_philo            │            │ for each philosopher:     │
   │   take forks (ordered)   │            │   now - last_meal > die ? │
   │   eat   → last_meal      │  meal mtx  │     → print "died", stop  │
   │   sleep                  │ ◄────────► │ everyone ate enough ?     │
   │   think                  │            │     → stop                │
   │   until data->dead       │            │ usleep(10)                │
   └──────────────────────────┘            └───────────────────────────┘
                     │                                  │
                     └──────────── my_free_data ────────┘
                         join threads, destroy mutexes, free
```

### 1. Data — `src/philo.h`

```c
typedef struct s_philo
{
    int             id;
    int             eat_count;
    long long       last_meal;    // ms, protected by meal
    pthread_t       thread;
    pthread_mutex_t *left;        // fork i
    pthread_mutex_t *right;       // fork (i + 1) % n
    pthread_mutex_t meal;         // protects last_meal and eat_count
    t_data          *data;
}   t_philo;
```

`t_data` holds the arguments, the array of forks, the array of philosophers, the `dead` flag and three shared mutexes: `print`, `death` and `time`. Counters (`mutex_count`, `meal_count`, `threads_count`…) record how many of each were really created, so `my_free_data` only destroys and joins what exists, even after a failure halfway through the init.

### 2. Init — `src/init.c`, `src/parsing.c`

`verif_arg` checks that every argument only contains digits, `ft_atoi` converts them, `verif_data` rejects zero values. Then the forks, the shared mutexes and the philosophers are created, in that order.

### 3. Routine — `src/routine.c`, `src/utils.c`

Each philosopher loops until the `dead` flag is set:

1. **Take the forks**: the fork with the lowest address first, then the other one. Because every philosopher follows the same order, there is always one who can take both forks, so the table cannot deadlock. The `dead` flag is checked between each step so nobody waits on a fork after the end.
2. **Eat**: update `last_meal` and `eat_count` under the `meal` mutex, print, wait `time_to_eat`, release both forks.
3. **Sleep**, then **think** for the computed thinking time.

### 4. Precise waiting — `src/time_sleep.c`

`usleep` alone can oversleep by several ms. `my_usleep` loops on `gettimeofday` and sleeps in small steps (1 ms, then 100 µs near the end) until the exact duration has passed.

### 5. Monitor — `src/monitoring.c`

The main thread checks, in a loop, whether `now - last_meal > time_to_die` for any philosopher. If so, `ft_slayer` takes the print mutex, sets `dead`, prints `died` and returns: since `write_philo` checks `dead` under the same print mutex, no line can appear after the death. With a meal limit, it also stops once every philosopher has eaten enough.

### 6. Clean up — `src/free_destroy.c`

`my_free_data` joins every created thread, destroys every initialised mutex, and frees the arrays.

## Project structure

```
.
├── Makefile
└── src/
    ├── philo.h              # structures, states, prototypes
    ├── main.c               # arguments, init, simulation, clean up
    ├── parsing.c            # digit check, ft_atoi, value checks
    ├── init.c               # forks, mutexes, philosophers
    ├── philo.c              # thread creation, one-philosopher case, printing
    ├── routine.c            # eat / sleep / think loop
    ├── utils.c              # ordered fork taking, eating
    ├── monitoring.c         # death and meal-limit checks
    ├── time_sleep.c         # my_get_time, my_usleep
    └── free_destroy.c       # join, destroy, free
```

## Examples

```console
$ ./philo 1 800 200 200
0 1 has taken a fork
800 1 died

$ ./philo 4 310 200 100
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
...
311 1 died

$ ./philo 5 800 200 200 7
...                          # stops by itself once everyone has eaten 7 times
$ echo $?
0
```

## Testing

| Test | Expected | Result |
|---|---|---|
| `1 800 200 200` | One death at 800 ms | dies at `800` |
| `4 310 200 100` | One death | dies at `311` |
| `2 310 200 100` | One death | dies at `311` |
| `4 200 210 100` | One death | dies at `201` |
| `5 800 200 200` | Nobody dies | no death |
| `4 410 200 200` | Nobody dies | no death |
| `5 610 200 200` | Nobody dies | no death |
| `199 610 200 200` | Nobody dies | no death |
| `5 800 200 200 7` | Stops after 7 meals each | stops, every philosopher ate at least 7 times |

Every death is printed less than 10 ms after `time_to_die`, and nothing is printed after it.

- `valgrind --leak-check=full`: no leak, no error, with and without a meal limit, and with one philosopher
- `valgrind --tool=helgrind`: no data race and no lock order problem reported

Under **WSL**, the system clock sometimes jumps forward by about 1.8 s when Windows resyncs it. Since the timestamps come from `gettimeofday`, a philosopher can then be reported dead in a case where nobody should die. This comes from the environment, not from the simulation: on a native Linux machine the clock does not jump.

## Resources

- `man 3` pages: `pthread_create`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_lock`, `gettimeofday`, `usleep`
- [Dining philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/)

## Author

| Login | Work |
|---|---|
| **alehamad** | Everything |
