H0 = __import__('checklib')
SCENARIO = {"duration": 13000, "events": H0.hold(0, 2, 0) + H0.hold(1000, 2, 1) + H0.hold(1300, 2, 0) + H0.hold(5000, 2, 1) + H0.hold(5300, 2, 0) + H0.hold(8000, 2, 1) + H0.hold(8300, 2, 0)}
def check(tr, H):
    errs = H.mode_is(tr, 2, "INPUT") + H.mode_is(tr, 11, "OUTPUT")
    if any(e["kind"] == "delay" for e in tr["events"]): errs.append("delay() was used")
    errs += H.level_at(tr, 13, 500, 1, what="green in STANDBY") + H.level_at(tr, 12, 500, 0, what="red in STANDBY")
    errs += H.level_at(tr, 11, 1100, 1, what="buzzer right after motion") + H.toggles_every(tr, 11, 300, start=1050, end=3990, min_toggles=6)
    b = H.changes_between(tr, 11, 1000, 4500)
    if b and not (3690 <= b[-1][0] <= 4015): errs.append(f"buzzer last change at {b[-1][0]:.0f} ms, expected the alert to end at 4000")
    errs += H.level_at(tr, 11, 4500, 0, what="buzzer in COOLDOWN") + H.no_change(tr, 11, 4100, 7990)
    errs += H.level_at(tr, 12, 2000, 1, what="red in ALERT") + H.level_at(tr, 13, 2000, 0, what="green in ALERT") + H.level_at(tr, 13, 6500, 1, what="green back in STANDBY")
    errs += H.level_at(tr, 11, 8100, 1, what="buzzer after motion at 8 s")
    errs += H.expect_lines(tr, ["State: STANDBY", "State: ALERT", "State: COOLDOWN", "State: STANDBY", "State: ALERT", "State: COOLDOWN"])
    return errs
