SCENARIO = {"duration": 4000, "events": __import__("checklib").press(1000, 3, 500) + __import__("checklib").press(2500, 3, 300)}
def check(tr, H):
    errs = H.mode_is(tr, 3, "INPUT_PULLUP") + H.mode_is(tr, 12, "OUTPUT")
    errs += H.level_at(tr, 12, 800, 0) + H.level_at(tr, 12, 1300, 1) + H.level_at(tr, 12, 2000, 0) + H.level_at(tr, 12, 2700, 1) + H.level_at(tr, 12, 3500, 0)
    return errs
