H0 = __import__('checklib')
SCENARIO = {"duration": 5000, "events": H0.serial_in(1000, "1\n") + H0.serial_in(2000, "0\n") + H0.serial_in(3000, "x\n") + H0.serial_in(4000, "1")}
def check(tr, H):
    errs = H.mode_is(tr, 13, "OUTPUT")
    errs += H.level_at(tr, 13, 1500, 1) + H.level_at(tr, 13, 2500, 0) + H.level_at(tr, 13, 3500, 0) + H.level_at(tr, 13, 4500, 1)
    return errs
