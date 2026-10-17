H0 = __import__('checklib')
SCENARIO = {"duration": 10000, "events": H0.press(1000, 12) + H0.press(3000, 12) + H0.press(6000, 12) + H0.press(8000, 12)}
def check(tr, H):
    errs = H.mode_is(tr, 12, "INPUT_PULLUP") + H.mode_is(tr, 13, "OUTPUT")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    errs += H.level_at(tr, 13, 500, 0, what="LED at start") + H.level_at(tr, 13, 2000, 1, what="LED in ON state") + H.no_change(tr, 13, 1100, 2990)
    errs += H.toggles_every(tr, 13, 500, start=3100, end=5990, min_toggles=4)
    errs += H.level_at(tr, 13, 7000, 0, what="LED in OFF state") + H.no_change(tr, 13, 6100, 7990) + H.level_at(tr, 13, 9000, 1, what="LED after 4th press")
    return errs
