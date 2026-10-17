# ex07 Elapsed time every 2 s, as minutes:seconds

Every 2 s (without `delay()`), print the time elapsed since start as `m:ss`, e.g. `0:02`, `0:04`, ... `1:00`, `1:02`.
Seconds are zero-padded to two digits. Serial at 9600 baud.

Checked: lines `0:00` (optional), `0:02`, `0:04`, ..., `1:02` at the right times (tolerance 20 ms); no `delay()`.
