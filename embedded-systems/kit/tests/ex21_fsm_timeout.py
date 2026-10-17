H0 = __import__('checklib')
SCENARIO = {"duration": 8000, "events": H0.press(1000, 12) + H0.press(2000, 12) + H0.press(5000, 12) + H0.press(6000, 12) + H0.press(6500, 12)}
def check(tr, H):
    errs = H.mode_is(tr, 12, "INPUT_PULLUP") + H.mode_is(tr, 13, "OUTPUT")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    errs += H.level_at(tr, 13, 1500, 1, what="LED in ACTIVE") + H.toggles_every(tr, 13, 100, start=2150, end=3990, min_toggles=8)
    ch = H.changes_between(tr, 13, 2100, 5000)
    if ch and not (3990 <= ch[-1][0] <= 4120): errs.append(f"last LED change of the blinking phase at {ch[-1][0]:.0f} ms; the 2 s timeout should end it near 4060 ms")
    errs += H.level_at(tr, 13, 4500, 0, what="LED after the timeout") + H.level_at(tr, 13, 5500, 1, what="LED after press at 5 s")
    errs += H.toggles_every(tr, 13, 100, start=6150, end=6490, min_toggles=2) + H.level_at(tr, 13, 7000, 0, what="LED after press at 6.5 s") + H.no_change(tr, 13, 6650, 8000)
    errs += H.expect_lines(tr, ["IDLE -> ACTIVE", "ACTIVE -> BLINKING", "BLINKING -> IDLE", "IDLE -> ACTIVE", "ACTIVE -> BLINKING", "BLINKING -> IDLE"])
    return errs
