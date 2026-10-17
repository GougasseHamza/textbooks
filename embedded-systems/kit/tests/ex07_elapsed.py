SCENARIO = {"duration": 63000}
def check(tr, H):
    ls = H.lines_with_time(tr)
    ls = [(t, l) for t, l in ls if l.strip() != "0:00"]
    if any(e["kind"] == "delay" for e in tr["events"]): return ["delay() was used; use millis()"]
    want = [(s, f"{s//60}:{s%60:02d}") for s in range(2, 63, 2)]
    if len(ls) < len(want): return [f"expected {len(want)} lines, got {len(ls)}: {[l for _, l in ls][:6]}"]
    for (t, l), (s, w) in zip(ls, want):
        if l.strip() != w: return [f"line {l!r} at t={t:.0f} ms, expected {w!r}"]
        if not H.approx(t, s*1000, 20): return [f"line {w!r} printed at t={t:.0f} ms, expected at {s*1000} ms"]
    return []
