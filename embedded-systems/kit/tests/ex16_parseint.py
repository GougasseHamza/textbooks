H0 = __import__('checklib')
SCENARIO = {"duration": 6000, "events": H0.serial_in(500, "25\n") + H0.serial_in(2000, "125\n") + H0.serial_in(3500, "7\n")}
def check(tr, H):
    return H.expect_lines(tr, ["got 25", "got 125", "got 7"]) + (["extra lines printed: " + str(H.lines(tr)[3:])] if len(H.lines(tr)) > 3 else [])
