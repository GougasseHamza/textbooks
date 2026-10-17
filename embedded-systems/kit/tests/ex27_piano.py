H0 = __import__('checklib')
SCENARIO = {"duration": 7000, "events": H0.hold(1000, 2, 0) + H0.hold(2000, 2, 1) + H0.hold(3000, 3, 0) + H0.hold(4000, 3, 1) + H0.hold(5000, 4, 0) + H0.hold(6000, 4, 1)}
def check(tr, H):
    errs = []
    for p in (2, 3, 4): errs += H.mode_is(tr, p, "INPUT_PULLUP")
    tones = [(e["t"], e["freq"]) for e in tr["events"] if e["kind"] == "tone" and e["pin"] == 8]
    freqs = [f for _, f in tones if f]
    if freqs[:3] != [262, 294, 330]: errs.append(f"tone frequencies {freqs[:4]}, expected [262, 294, 330]")
    def freq_at(t):
        f = 0
        for tt, ff in tones:
            if tt <= t: f = ff
        return f
    for t, w in ((500, 0), (1500, 262), (2500, 0), (3500, 294), (4500, 0), (5500, 330), (6500, 0)):
        if freq_at(t) != w: errs.append(f"at t={t} ms the buzzer plays {freq_at(t)} Hz, expected {w}"); break
    return errs
