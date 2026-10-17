H0 = __import__('checklib')
SCENARIO = {"duration": 14000, "events": H0.press(1000, 3) + H0.press(3000, 3) + H0.press(8000, 3)}
def check(tr, H):
    errs = H.mode_is(tr, 3, "INPUT_PULLUP") + H.mode_is(tr, 12, "OUTPUT")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    on = [s for s in H.segments(tr, 12) if s[2] == 1]
    if len(on) != 2: return errs + [f"expected exactly 2 ON periods, found {len(on)}: {on}"]
    for (t0, t1, _), s in zip(on, (1000, 8000)):
        if not (s <= t0 <= s + 80): errs.append(f"LED turned on at {t0:.0f} ms, expected {s} (+ up to 80 ms of debounce latency)")
        if not H.approx(t1 - t0, 5000, 15): errs.append(f"LED stayed on {t1-t0:.0f} ms, expected 5000 (press at 3 s must be ignored)")
    errs += H.expect_lines(tr, ["Machine started", "Machine stopped", "Machine started", "Machine stopped"])
    return errs
