SCENARIO = {"duration": 5000}
def check(tr, H):
    errs = []
    pm = [e for e in tr["events"] if e["kind"] == "pinMode" and e["pin"] == 13]
    if not pm: return ["pin 13 never became an output: set bit 5 of DDRB"]
    if not any(e.get("via") == "register" for e in pm): errs.append("pinMode() was used; this exercise wants DDRB")
    src_ok = True
    errs += H.periodic(tr, 13, 500, 500)
    return errs
