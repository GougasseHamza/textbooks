H0 = __import__('checklib')
SCENARIO = {"duration": 8000, "link": True, "events": H0.serial_in(5500, "\x03", board=1)}
def check(tr, H):
    errs = []
    a = H.bytes_sent(tr, 0); b = H.bytes_sent(tr, 1)
    if not a or a[0] != 15: errs.append(f"board A must send the byte 15 with Serial.write(); it sent {a[:4]}")
    if not b or b[0] != 1: errs.append(f"board B must answer the byte 1 to a value >= 10; it sent {b[:4]}")
    if 0 not in b: errs.append("board B never answered 0 (a value 3 was injected at 5.5 s)")
    errs += H.level_at(tr, 13, 1500, 1, board=0, what="A LED after ack 1")
    errs += H.level_at(tr, 13, 5600, 0, board=0, what="A LED after reply 0")
    return errs
