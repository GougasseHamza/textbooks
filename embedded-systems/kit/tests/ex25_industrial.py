H0 = __import__('checklib')
SCENARIO = {"duration": 32000, "events": H0.press(1000, 3) + H0.press(23000, 3) + H0.press(28000, 3)}
def check(tr, H):
    errs = H.mode_is(tr, 3, "INPUT_PULLUP")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    ls = H.lines_with_time(tr)
    prods = [(t, l) for t, l in ls if l.startswith("Product produced")]
    if len(prods) < 11: return errs + [f"expected 10 products then a restart, got {len(prods)} product lines: {[l for _, l in prods][:4]}"]
    for i in range(10):
        t, l = prods[i]
        if l.strip() != f"Product produced : {i+1}": errs.append(f"line {l!r}, expected 'Product produced : {i+1}'"); break
        if not (3000 + 2000*i <= t <= 3100 + 2000*i): errs.append(f"product {i+1} at {t:.0f} ms, expected about {3000+2000*i}"); break
    if prods[10][1].strip() != "Product produced : 1" or not (30000 <= prods[10][0] <= 30150): errs.append(f"after the restart at 28 s the first product should be 'Product produced : 1' at ~30000 ms, got {prods[10]}")
    errs += H.toggles_every(tr, 12, 500, start=1200, end=20990, min_toggles=10) + H.level_at(tr, 12, 22000, 0, what="green after 10 products") + H.no_change(tr, 12, 21200, 28000)
    red = [s for s in H.segments(tr, 11) if s[2] == 1]
    if not red: errs.append("red LED never turned on")
    else:
        t0, t1, _ = red[0]
        if not (21000 <= t0 <= 21100): errs.append(f"red turned on at {t0:.0f} ms, expected ~21000 (after the 10th product)")
        if not H.approx(t1 - t0, 5000, 40): errs.append(f"red stayed on {t1-t0:.0f} ms, expected 5000")
    errs += H.level_at(tr, 11, 27000, 0, what="red after finishing")
    return errs
