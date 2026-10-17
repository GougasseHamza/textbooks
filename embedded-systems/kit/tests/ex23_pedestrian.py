H0 = __import__('checklib')
SCENARIO = {"duration": 22000, "events": H0.press(7000, 2) + H0.press(12000, 2) + H0.press(17500, 2)}
def check(tr, H):
    errs = H.mode_is(tr, 2, "INPUT_PULLUP")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    green = [s for s in H.segments(tr, 4) if s[2] == 1]
    if len(green) < 2: return errs + [f"expected at least 2 green phases, found {len(green)}"]
    g0, g1, _ = green[0]
    if not H.approx(g0, 5000, 10): errs.append(f"first green at {g0:.0f} ms, expected 5000")
    if not (7000 <= g1 <= 7080): errs.append(f"green ended at {g1:.0f} ms, expected right after the pedestrian press at 7000")
    red = [s for s in H.segments(tr, 6) if s[2] == 1]
    if len(red) < 2: return errs + ["red did not come back"]
    r0, r1, _ = red[1]
    if not H.approx(r0 - g1, 3000, 10): errs.append(f"red came back {r0-g1:.0f} ms after green ended, expected 3000")
    if not (12000 <= r1 <= 12080): errs.append(f"red ended at {r1:.0f} ms, expected right after the press at 12000")
    g2, g3, _ = green[1]
    if not H.approx(g3 - g2, 5000, 10): errs.append(f"second green lasted {g3-g2:.0f} ms, expected 5000 (started at {g2:.0f})")
    # yellow after 2nd green: 17.0 .. 20.0 s, press at 17.5 ignored
    if len(red) < 3: return errs + ["third red phase missing"]
    if not H.approx(red[2][0] - g3, 3000, 10): errs.append(f"press during yellow should be ignored: red came back {red[2][0]-g3:.0f} ms after green ended, expected 3000")
    return errs
