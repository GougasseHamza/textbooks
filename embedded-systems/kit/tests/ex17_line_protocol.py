H0 = __import__('checklib')
SCENARIO = {"duration": 6000, "events": H0.serial_in(1000, "ON\n") + H0.serial_in(2000, "BLINK\n") + H0.serial_in(4000, "OFF\n") + H0.serial_in(5000, "foo\n")}
def check(tr, H):
    errs = H.level_at(tr, 13, 1500, 1, what="LED after ON") + H.toggles_every(tr, 13, 250, start=2100, min_toggles=5)
    errs += H.level_at(tr, 13, 4500, 0, what="LED after OFF") + H.no_change(tr, 13, 4300, 6000)
    ls = [l for l in H.lines(tr) if "unknown" in l]
    if ls != ["unknown: foo"]: errs.append(f"expected one line 'unknown: foo', got {ls}")
    return errs
