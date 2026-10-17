SCENARIO = {"duration": 4000}
def check(tr, H):
    errs = H.mode_is(tr, 5, "OUTPUT") + H.mode_is(tr, 13, "OUTPUT")
    errs += H.periodic(tr, 5, 500, 500) + H.periodic(tr, 13, 500, 500)
    for t in range(600, 4000, 125):
        a, b = H.pin_level_at(tr, 5, t), H.pin_level_at(tr, 13, t)
        if a == b: errs.append(f"at t={t} ms both LEDs are {'HIGH' if a else 'LOW'}"); break
    return errs
