SCENARIO = {"duration": 16000}
def check(tr, H):
    errs = H.mode_is(tr, 13, "OUTPUT")
    segs = [s for s in H.segments(tr, 13) if s[2] == 1]
    want = [200]*3 + [600]*3 + [200]*3
    if len(segs) < 18: return errs + [f"expected at least 18 flashes in 16 s, found {len(segs)}"]
    for rep in range(2):
        for i, w in enumerate(want):
            t0, t1, _ = segs[rep*9 + i]
            if not H.approx(t1 - t0, w): errs.append(f"flash {rep*9+i+1} lasts {t1-t0:.0f} ms, expected {w}"); break
        # gap after the word
        end_word = segs[rep*9 + 8][1]; next_start = segs[rep*9 + 9][0]
        if next_start - end_word < 995: errs.append(f"pause after word {rep+1} is {next_start-end_word:.0f} ms, expected >= 1000")
    return errs
