SCENARIO = {"duration": 27000}
def check(tr, H):
    errs = []
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used; use stateEnteredAt + millis()")
    for p in (4, 5, 6): errs += H.mode_is(tr, p, "OUTPUT")
    red = [s for s in H.segments(tr, 6) if s[2] == 1]
    if len(red) < 2: return errs + [f"expected the red LED to be on at least twice in 27 s, found {len(red)}"]
    for t0, t1, _ in red[:2]:
        if not H.approx(t1 - t0, 5000, 10): errs.append(f"red ON for {t1-t0:.0f} ms from {t0:.0f}, expected 5000")
    green = [s for s in H.segments(tr, 4) if s[2] == 1]
    if not green: return errs + ["green LED never turned on"]
    g0, g1, _ = green[0]
    if not H.approx(g0, red[0][1], 10): errs.append(f"green turned on at {g0:.0f} ms, expected right after red ended at {red[0][1]:.0f}")
    if not H.approx(g1 - g0, 5000, 10): errs.append(f"green ON for {g1-g0:.0f} ms, expected 5000")
    errs += H.toggles_every(tr, 5, 100, start=g1 + 50, end=g1 + 2990, min_toggles=20)
    y = H.changes_between(tr, 5, g1 - 20, g1 + 3200)
    if y and not H.approx(y[-1][0] - g1, 3000, 110): errs.append(f"yellow phase lasted {y[-1][0]-g1:.0f} ms (last toggle), expected about 3000")
    if len(red) >= 2 and not H.approx(red[1][0] - g1, 3000, 10): errs.append(f"red came back {red[1][0]-g1:.0f} ms after green ended, expected 3000")
    errs += H.level_at(tr, 6, g0 + 2000, 0, what="red during green") + H.level_at(tr, 4, red[0][0] + 2000, 0, what="green during red") + H.level_at(tr, 4, g1 + 1000, 0, what="green during yellow")
    return errs
