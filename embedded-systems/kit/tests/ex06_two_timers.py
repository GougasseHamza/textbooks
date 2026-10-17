SCENARIO = {"duration": 10000}
def check(tr, H):
    errs = H.toggles_every(tr, 12, 500) + H.toggles_every(tr, 11, 1500, min_toggles=3)
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used; this exercise must be non-blocking")
    return errs
