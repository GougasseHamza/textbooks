SCENARIO = {"duration": 4000}
def check(tr, H):
    ch = H.pwm_changes(tr, 5)
    if len(ch) < 100: return [f"expected a long sequence of analogWrite values on pin 5, found {len(ch)}"]
    vals = [v for _, v in ch]
    up = list(range(0, 256, 5)); down = list(range(250, -1, -5))
    want = (up + down) * 3
    want = [w for i, w in enumerate(want) if i == 0 or w != want[i-1]]   # the trace only records changes
    for i, (g, w) in enumerate(zip(vals, want)):
        if g != w: return [f"analogWrite value #{i+1} is {g}, expected {w} (sequence must be 0,5,...,255,250,...,0)"]
    for (t0, _), (t1, _) in list(zip(ch, ch[1:]))[:100]:
        if not H.approx(t1 - t0, 30, 2): return [f"step at t={t0:.1f} lasts {t1-t0:.1f} ms, expected 30"]
    return []
