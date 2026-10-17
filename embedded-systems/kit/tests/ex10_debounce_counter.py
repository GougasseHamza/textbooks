H0 = __import__('checklib')
SCENARIO = {"duration": 9000, "events": H0.press(500, 3) + H0.press(1200, 3) + H0.press(2000, 3, 80) + H0.press(3000, 3) + H0.press(4000, 3) + H0.press(5500, 3, 2000)}
def check(tr, H):
    errs = H.mode_is(tr, 3, "INPUT_PULLUP")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used; debounce with millis()")
    ls = [l.strip() for l in H.lines(tr) if l.strip().startswith("count=")]
    want = [f"count={i}" for i in range(1, 7)]
    if ls != want: errs.append(f"counter lines {ls}, expected {want} (6 presses, the last one held for 2 s)")
    return errs
