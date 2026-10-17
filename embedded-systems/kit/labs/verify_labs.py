#!/usr/bin/env python3
"""Re-run every lab sketch in the simulator and check its key behaviour."""
import os, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
import arduino_sim as A, checklib as H
L = os.path.dirname(os.path.abspath(__file__))
S = os.path.join(L, "..", "solutions")
results = []
def run(name, sketches, duration, events=(), link=False):
    exe, err = A.build([os.path.join(L, s) if not s.startswith("solutions/") else os.path.join(L, "..", s) for s in sketches])
    if not exe: results.append((name, ["compile error: " + err])); return None
    return A.run(exe, A.write_scenario(duration, events, link=link))
def rep(name, errs): results.append((name, errs))

tr = run("01 task2_250ms", ["01_internal_led/task2_250ms.ino"], 3000); rep("01 task2_250ms", H.periodic(tr, 13, 250, 250))
tr = run("01 task2_asym", ["01_internal_led/task2_asymmetric.ino"], 4000); rep("01 task2_asym", H.periodic(tr, 13, 100, 900))
tr = run("01 listing2", ["01_internal_led/listing2_millis_blink.ino"], 6000); rep("01 listing2", H.toggles_every(tr, 13, 1000))
tr = run("01 task4", ["01_internal_led/task4_bonus.ino"], 11000)
ls = H.lines_with_time(tr); rep("01 task4", H.toggles_every(tr, 13, 250) + ([] if [l for _, l in ls][:2] == ["Blinks so far: 10", "Blinks so far: 20"] else [f"lines {[l for _, l in ls][:3]}"]))
tr = run("02 listing1", ["02_external_led/listing1.ino"], 4000); rep("02 listing1", H.periodic(tr, 5, 500, 500))
tr = run("03 ex3", ["03_serial/ex3_print_vs_write.ino"], 1000)
rep("03 ex3", [] if H.lines(tr)[0].startswith("print(48) -> 48   write(48) -> 0") and H.lines(tr)[3].endswith("A") else ["output: " + str(H.lines(tr)[:4])])
tr = run("03 ex4", ["03_serial/ex4_read_parseint_peek.ino"], 3000, H.serial_in(500, "25\n") + H.serial_in(1500, "A\n"))
rep("03 ex4", [] if any("read(): 50" in l for l in H.lines(tr)) and any("parseInt() of what follows: 5" in l for l in H.lines(tr)) and any("peek twice: 65 65" in l for l in H.lines(tr)) else ["output: " + str(H.lines(tr))])
tr = run("03 ex5_6", ["03_serial/ex5_6_A.ino", "03_serial/ex5_6_B.ino"], 10000, link=True); rep("03 ex5_6", [] if H.bytes_sent(tr, 0)[:4] == [5, 6, 7, 8] else [str(H.bytes_sent(tr, 0))])
tr = run("03 ex7", ["03_serial/ex7_A.ino", "03_serial/ex7_B.ino"], 12000, link=True)
rep("03 ex7", H.level_at(tr, 13, 2000, 1, board=1) + H.level_at(tr, 13, 7000, 0, board=1) + H.level_at(tr, 13, 11000, 1, board=1))
tr = run("03 ex11", ["03_serial/ex11_A.ino", "03_serial/ex11_B.ino"], 14000, link=True)
rep("03 ex11", H.level_at(tr, 13, 11000, 0, board=0) + H.level_at(tr, 13, 13000, 1, board=0) + ([] if H.bytes_sent(tr, 1)[:1] == [1] else ["B sent " + str(H.bytes_sent(tr, 1)[:3])]))
tr = run("04 solA", ["04_floating_pin/solutionA_internal_pullup.ino"], 3000, H.press(1000, 3, 500))
rep("04 solA", H.mode_is(tr, 3, "INPUT_PULLUP") + H.level_at(tr, 13, 1300, 1) + H.level_at(tr, 13, 2000, 0) + ([] if "Button Pressed" in H.lines(tr) else ["no 'Button Pressed' line"]))
tr = run("04 solB", ["04_floating_pin/solutionB_external_pulldown.ino"], 3000, H.hold(0, 3, 0) + H.hold(1000, 3, 1) + H.hold(1500, 3, 0))
rep("04 solB", H.mode_is(tr, 3, "INPUT") + H.level_at(tr, 13, 1300, 1) + H.level_at(tr, 13, 2000, 0))
tr = run("05 ex1", ["05_millis/ex1_elapsed.ino"], 7000); rep("05 ex1", [] if any(l.startswith("ms: 4000") and l.endswith("0:04") for l in H.lines(tr)) else ["lines: " + str(H.lines(tr)[:3])])
tr = run("05 ex2", ["05_millis/ex2_led12.ino"], 4000); rep("05 ex2", H.toggles_every(tr, 12, 500) + ([] if H.lines(tr)[:2] == ["LED ON", "LED OFF"] else ["lines " + str(H.lines(tr)[:2])]))
tr = run("05 ex6", ["05_millis/ex6_debounce_counter_led.ino"], 4000, H.press(500, 3) + H.press(1500, 3) + H.press(2500, 3, 1000))
rep("05 ex6", ([] if [l for l in H.lines(tr) if l.startswith("Presses")] == ["Presses: 1", "Presses: 2", "Presses: 3"] else ["lines " + str(H.lines(tr))]) + H.level_at(tr, 12, 3800, 1))
tr = run("05 ex7", ["05_millis/ex7_three_leds.ino"], 12000, H.press(6000, 3))
rep("05 ex7", H.toggles_every(tr, 12, 500, end=5990) + H.toggles_every(tr, 11, 1000, end=5990) + H.toggles_every(tr, 10, 2000, end=5990, min_toggles=2) + H.no_change(tr, 12, 6200, 12000) + ([] if len([l for l in H.lines(tr) if l == "System active"]) == 2 else ["System active count " + str(H.lines(tr))]))
tr = run("06 task2", ["06_traffic_fsm/task2_logged.ino"], 15000)
rep("06 task2", H.expect_lines(tr, ["RED + TIMEOUT -> GREEN", "GREEN + TIMEOUT -> YELLOW", "YELLOW + TIMEOUT -> RED", "RED + TIMEOUT -> GREEN"]))
tr = run("06 task5", ["06_traffic_fsm/task5_night.ino"], 20000, H.press(7000, 2) + H.press(15000, 2))
rep("06 task5", H.toggles_every(tr, 5, 500, start=7200, end=14990, min_toggles=8) + H.level_at(tr, 6, 15500, 1) + H.level_at(tr, 4, 6000, 1))
tr = run("07 simple TL", ["07_slide_labs/assignment_simple_traffic_light.ino"], 25000)
segs = [s for s in H.segments(tr, 6) if s[2] == 1]; rep("07 simple TL", [] if len(segs) >= 2 and H.approx(segs[1][0] - segs[0][0], 12000, 5) else [f"red period {segs}"])
tr = run("07 blinking TL", ["07_slide_labs/assignment_traffic_blinking_yellow.ino"], 14000); rep("07 blinking TL", H.toggles_every(tr, 5, 100, start=10050, end=12990, min_toggles=20))
tr = run("07 three buttons", ["07_slide_labs/three_buttons.ino"], 4000, H.hold(0, 2, 0) + H.hold(0, 3, 1) + H.hold(1000, 2, 1) + H.hold(1500, 2, 0) + H.hold(2000, 3, 0) + H.hold(2500, 3, 1) + H.press(3000, 4, 500, bounce=0))
rep("07 three buttons", H.level_at(tr, 10, 1200, 1) + H.level_at(tr, 10, 1800, 0) + H.level_at(tr, 11, 2200, 1) + H.level_at(tr, 11, 2800, 0) + H.level_at(tr, 12, 3200, 1) + H.level_at(tr, 12, 3800, 0))
tr = run("07 fsm fixed", ["07_slide_labs/fsm_state_based_fixed.ino"], 8000, H.press(1000, 12) + H.press(2000, 12) + H.press(5000, 12) + H.press(6000, 12) + H.press(6500, 12))
rep("07 fsm fixed", H.level_at(tr, 13, 1500, 1) + H.toggles_every(tr, 13, 500, start=2100, end=3990, min_toggles=2) + H.level_at(tr, 13, 4500, 0) + H.level_at(tr, 13, 5500, 1) + H.level_at(tr, 13, 7000, 0))
tr = run("07 advanced table", ["07_slide_labs/advanced_fsm_table.ino"], 8000, H.press(1000, 12) + H.press(2000, 12) + H.press(5000, 12) + H.press(6000, 12) + H.press(6500, 12))
rep("07 advanced table", H.level_at(tr, 13, 1500, 1) + H.toggles_every(tr, 13, 500, start=2100, end=3990, min_toggles=2) + H.level_at(tr, 13, 4500, 0) + H.level_at(tr, 13, 5500, 1) + H.level_at(tr, 13, 7000, 0))

ok = 0
for name, errs in results:
    print(f"[{name}] {'PASS' if not errs else 'FAIL'}"); ok += not errs
    for e in errs: print("   - " + e)
print(f"{ok}/{len(results)} lab sketches OK")
