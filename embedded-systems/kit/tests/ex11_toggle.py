H0 = __import__('checklib')
SCENARIO = {"duration": 7000, "events": H0.press(500, 3) + H0.press(1500, 3) + H0.press(2500, 3) + H0.press(4000, 3, 1500)}
def check(tr, H):
    errs = H.mode_is(tr, 3, "INPUT_PULLUP") + H.mode_is(tr, 12, "OUTPUT")
    errs += H.level_at(tr, 12, 1200, 1, what="LED after press 1") + H.level_at(tr, 12, 2200, 0, what="LED after press 2") + H.level_at(tr, 12, 3500, 1, what="LED after press 3")
    errs += H.level_at(tr, 12, 6500, 0, what="LED after the long press")
    ch = H.changes_between(tr, 12, 4000, 6500)
    if len(ch) != 1: errs.append(f"LED changed {len(ch)} times during the 1.5 s hold, expected exactly 1")
    return errs
