SCENARIO = {"duration": 15000, "link": True}
def check(tr, H):
    errs = []
    bs = H.bytes_sent(tr, 0)
    if bs[:6] != [5, 6, 7, 8, 9, 10]: errs.append(f"board A sent bytes {bs[:8]}, expected [5,6,7,8,9,10,...] (use Serial.write)")
    ts = [t for t, _, _ in H.serial_chunks(tr, 0)]
    if len(ts) >= 3 and not (H.approx(ts[1]-ts[0], 2000, 20) and H.approx(ts[2]-ts[1], 2000, 20)): errs.append(f"board A send times {ts[:4]}, expected every 2000 ms")
    errs += H.mode_is(tr, 13, "OUTPUT", board=1)
    errs += H.level_at(tr, 13, 9500, 0, board=1, what="B LED before 10 arrives") + H.level_at(tr, 13, 10500, 1, board=1, what="B LED after 10 arrives") + H.level_at(tr, 13, 14500, 1, board=1, what="B LED")
    return errs
