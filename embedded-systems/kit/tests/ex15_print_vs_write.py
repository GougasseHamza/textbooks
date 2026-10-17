H0 = __import__('checklib')
SCENARIO = {"duration": 3000, "events": H0.serial_in(200, "A") + H0.serial_in(600, "0") + H0.serial_in(1000, "1") + H0.serial_in(1400, "2") + H0.serial_in(1800, "B")}
def check(tr, H):
    errs = H.expect_lines(tr, ["65 A", "48 0", "49 1", "50 2", "66 B"])
    bs = H.bytes_sent(tr)
    if not errs and bs[:4] != [54, 53, 32, 65]: errs.append(f"first bytes on the wire are {bs[:4]}, expected [54,53,32,65] ('6','5',' ', then raw 65)")
    return errs
