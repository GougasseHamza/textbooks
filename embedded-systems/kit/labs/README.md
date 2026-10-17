# Lab solutions (code)

Every sketch here was run in the simulator (`python3 labs/verify_labs.py` re-checks them all).
Tasks that are identical to an exercise are not duplicated; the table says where to look.
The written answers (post-lab questions, tables, measurements) are in the guide PDF, part H.

| Lab / assignment | Task | Where |
|---|---|---|
| Internal LED | Listing 1, Task 1 measurement | guide H.1; `solutions/ex01_blink.ino` (500/500 version) |
| | Task 2 (250 ms; 100/900) | `01_internal_led/task2_250ms.ino`, `task2_asymmetric.ino` |
| | Task 3 SOS | `solutions/ex02_sos.ino` |
| | Listing 2, Task 4 bonus | `01_internal_led/listing2_millis_blink.ino`, `task4_bonus.ino` |
| External LED | Listing 1 | `02_external_led/listing1.ino` |
| | Task 2 opposition, Task 3 fading | `solutions/ex04_opposition.ino`, `solutions/ex05_fade.ino` |
| Serial | Ex 1, Ex 2 | `solutions/ex13_serial_counter.ino`, `solutions/ex14_serial_command.ino` |
| | Ex 3, Ex 4 | `03_serial/ex3_print_vs_write.ino`, `ex4_read_parseint_peek.ino` |
| | Ex 5-6, Ex 7 | `03_serial/ex5_6_A.ino` + `_B`, `ex7_A.ino` + `_B` |
| | Ex 8, Ex 9-10 | `solutions/ex18_two_boards_*.ino`, `solutions/ex19_handshake_*.ino` |
| | Ex 11 final application | `03_serial/ex11_A.ino` + `_B` |
| Floating pin | Solution A, Solution B, bonus debounce | `04_floating_pin/solutionA_internal_pullup.ino`, `solutionB_external_pulldown.ino`, `solutions/ex10_debounce_counter.ino` |
| millis | Ex 1, Ex 2 | `05_millis/ex1_elapsed.ino`, `ex2_led12.ino` (and `solutions/ex07_elapsed.ino`) |
| | Ex 3, Ex 4, Ex 5 | `solutions/ex06_two_timers.ino`, `ex08_counter.ino`, `ex12_pushbutton_timer.ino` |
| | Ex 6, Ex 7, Ex 8 | `05_millis/ex6_debounce_counter_led.ino`, `ex7_three_leds.ino`, `solutions/ex25_industrial.ino` |
| Traffic light FSM | Listing 1 as given (measured), Task 2 log | `06_traffic_fsm/task2_logged.ino` |
| | Task 3 timing | change RED_TIME/GREEN_TIME/YELLOW_TIME to 8000/10000/2000 in `solutions/ex22_traffic_light.ino` (cycle 20 s) |
| | Task 4 non-blocking, pedestrian button, Task 5 NIGHT | `solutions/ex22_traffic_light.ino`, `solutions/ex23_pedestrian.ino`, `06_traffic_fsm/task5_night.ino` |
| Lecture-2 labs | Traffic light assignments V2 | `07_slide_labs/assignment_simple_traffic_light.ino`, `assignment_traffic_blinking_yellow.ino` |
| | Lab 2 smart-home lighting | `solutions/ex20_fsm_light.ino` |
| | Lab 3 three buttons | `07_slide_labs/three_buttons.ino` |
| | Lab 4 PIR alarm | `solutions/ex24_pir_alarm.ino` |
| | FSM lab (event / state based), non-blocking | `solutions/ex21_fsm_timeout.ino` |
| | Canvas "FSM State Based Reasoning" (fix the code) | `07_slide_labs/fsm_state_based_fixed.ino` |
| | "Advanced FSM" transition table | `07_slide_labs/advanced_fsm_table.ino` |
| | Piano lab | `solutions/ex27_piano.ino` |
