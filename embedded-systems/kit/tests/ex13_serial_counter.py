SCENARIO = {"duration": 16000}
def check(tr, H):
    ls = H.lines_with_time(tr)
    want = ["5", "6", "7", "8", "9", "10"]
    if len(ls) < 6: return [f"expected at least 6 lines, got {[l for _, l in ls]}"]
    for i, ((t, l), w) in enumerate(zip(ls, want)):
        if l.strip() != w: return [f"line {i+1} is {l!r}, expected {w!r}"]
        if not H.approx(t, i*3000, 25): return [f"line {w!r} at {t:.0f} ms, expected {i*3000}"]
    return []
