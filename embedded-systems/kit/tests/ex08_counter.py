SCENARIO = {"duration": 70000}
def check(tr, H):
    errs = []
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    ls = H.lines_with_time(tr)
    if len(ls) != 20: errs.append(f"expected exactly 20 'Product produced' lines, got {len(ls)} (first: {[l for _,l in ls][:3]}, last: {[l for _,l in ls][-2:]})")
    for i, (t, l) in enumerate(ls[:20]):
        w = f"Product produced : {i+1}"
        if l.strip() != w: errs.append(f"line {i+1} is {l!r}, expected {w!r}"); break
        if not H.approx(t, (i+1)*3000, 20): errs.append(f"line {i+1} at {t:.0f} ms, expected {(i+1)*3000}"); break
    errs += H.toggles_every(tr, 12, 500, start=61000, min_toggles=10)
    return errs
