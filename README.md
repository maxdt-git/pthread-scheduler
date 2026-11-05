*This activity has been created as part of the 42 curriculum by mde-nas.*

# Codexion

**Stack:** C · POSIX threads · mutexes · condition variables · custom priority queue

**Highlights**
- Dining-philosophers variant with fair resource arbitration (`fifo` or `edf` scheduling)
- Deadlock prevention (ordered acquisition), starvation prevention (per-resource priority queues), dedicated monitor thread for deadline detection

## Description

Codexion is a POSIX-threads simulation of a shared co-working hub where several **coders**
(threads) sit in a circle around a table of **USB dongles**. To compile quantum code, a
coder must simultaneously hold the dongle on their left and the dongle on their right —
there is exactly one dongle between each pair of neighboring coders. Once compiling is
done, the coder releases both dongles, debugs, refactors, and immediately tries to compile
again.

Every coder must compile regularly or they **burn out**. The simulation stops as soon as:
- a coder burns out (fails to start compiling before their deadline), or
- every coder has completed at least `number_of_compiles_required` compiles.

The project explores classic resource-sharing problems (a variant of the dining
philosophers problem) using mutexes, condition variables, and a custom priority queue to
implement fair dongle arbitration (`fifo` or `edf`), dongle cooldowns, and precise burnout
detection.

## Instructions

### Build

```sh
make
```

Produces the `codexion` binary at the project root. Rules `clean`, `fclean`, `re` are also
available. `make run` builds and runs the binary with a default argument set, and
`make valgrind` runs it under Valgrind with the same arguments.

### Run

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning |
|---|---|
| `number_of_coders` | Number of coder threads (and number of dongles) |
| `time_to_burnout` | Max delay (ms) since simulation start / last compile start before a coder burns out |
| `time_to_compile` | Time (ms) spent compiling while holding both dongles |
| `time_to_debug` | Time (ms) spent debugging |
| `time_to_refactor` | Time (ms) spent refactoring |
| `number_of_compiles_required` | Simulation stops once every coder has reached this many compiles |
| `dongle_cooldown` | Time (ms) a released dongle stays unavailable |
| `scheduler` | `fifo` or `edf` — arbitration policy when several coders request the same dongle |

Example:

```sh
./codexion 5 800 200 200 200 3 50 edf
```

Every state change is logged as `timestamp_in_ms coder_id message`, e.g.:

```
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
201 1 is debugging
```

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** a coder always tries to acquire its two
  dongles in a fixed, id-parity-based order (odd coders take their right dongle first,
  even coders take their left dongle first). This breaks the circular-wait condition that
  would otherwise let every coder hold one dongle while waiting for the second (the
  classic dining-philosophers deadlock).
- **Starvation prevention:** each dongle keeps its own request queue (a 2-slot priority
  queue/heap, since at most two coders ever contend for one dongle) ordered either by
  arrival order (`fifo`) or by burnout deadline (`edf`, with arrival order as a
  deterministic tie-breaker). A coder is only granted the dongle once it reaches the head
  of that queue, guaranteeing every waiting coder eventually gets served.
- **Cooldown handling:** a dongle records its release timestamp; it cannot be re-acquired
  until `dongle_cooldown` ms have elapsed, enforced via `pthread_cond_timedwait` on a
  computed deadline instead of busy-waiting.
- **Precise burnout detection:** a dedicated monitor thread periodically checks every
  coder's time elapsed since their last compile start against `time_to_burnout` and stops
  the simulation as soon as the deadline is exceeded, keeping the reported burnout time
  within the required tolerance.
- **Log serialization:** a single global mutex protects the logging function so that two
  state-change messages can never interleave, and no message is printed once the
  simulation has already been stopped.

## Thread synchronization mechanisms

- `pthread_mutex_t stop_mutex` protects the shared `stop` flag (read via `is_stopped`,
  set via `stop_simu`) so the monitor thread and coder threads always observe a consistent
  termination state.
- `pthread_mutex_t mutex` (in `t_env`) serializes all calls to `log_print`, preventing
  interleaved output between coders and the monitor.
- Each coder has its own `state_mutex` protecting `last_compile_start` and
  `number_compile_done`, which are written by the coder's own thread and read
  concurrently by the monitor thread — avoiding a race between a coder starting a new
  compile and the monitor computing elapsed time.
- Each dongle has its own `pthread_mutex_t mutex` and `pthread_cond_t cond`. The mutex
  protects the dongle's `available` flag, `released` timestamp, and its request queue; the
  condition variable is used both to wait for the dongle to become free (or its cooldown
  to expire, via `pthread_cond_timedwait`) and to wake up every waiting coder whenever the
  dongle is released or a new request is queued (`pthread_cond_broadcast`).
- Thread-safe coordination between coders and the monitor is therefore achieved without
  busy-waiting: coders block on their dongle's condition variable until they are both next
  in line and the dongle is ready, while the monitor only needs the lightweight
  `state_mutex` of each coder to read timing information safely.

## Resources

- `man pthread_mutex_lock`, `man pthread_cond_wait`, `man pthread_cond_timedwait`,
  `man gettimeofday`
- [POSIX Threads Programming (LLNL tutorial)](https://hpc-tutorials.llnl.gov/posix/)
- The Dining Philosophers problem (classic reference for deadlock/starvation avoidance in
  resource-sharing simulations)
- **AI usage:** AI assistance (Claude) was used to help complete this README (structuring the
  Description/Instructions/Blocking cases/Synchronization sections from the existing
  source code). All C code was written and understood by the
  author; AI-suggested explanations were checked against the actual implementation before
  being included here.
