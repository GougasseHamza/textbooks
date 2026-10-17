"""checklib.py -- assertion helpers shared by the tests in tests/.
Every helper returns a list of failure strings (empty = fine)."""
import arduino_sim as A

serial_text = A.serial_text
serial_lines = A.serial_lines
serial_chunks = A.serial_chunks
pin_changes = A.pin_changes
pin_level_at = A.pin_level_at
pwm_changes = A.pwm_changes
pin_modes = A.pin_modes
intervals = A.intervals

def press(t, pin, ms=150, board=0, bounce=5):
    """scenario events for a button press (active LOW, pull-up) with bounce"""
    return [{"t": t, "board": board, "kind": "pin", "pin": pin, "value": 0, "bounce": bounce},
            {"t": t + ms, "board": board, "kind": "pin", "pin": pin, "value": 1, "bounce": bounce}]

def hold(t, pin, value, board=0):
    return [{"t": t, "board": board, "kind": "pin", "pin": pin, "value": value}]

def serial_in(t, text, board=0):
    return [{"t": t, "board": board, "kind": "serial", "text": text}]

def mode_is(trace, pin, mode, board=0):
    m = pin_modes(trace, board).get(pin)
    if m is None: return [f"pin {pin} was never configured with pinMode()"]
    if m != mode: return [f"pin {pin} configured as {m}, expected {mode}"]
    return []

def segments(trace, pin, board=0, start=0):
    """[(t0, t1, level)] for an output pin, from the first change after `start`"""
    ch = [c for c in pin_changes(trace, pin, board) if c[0] >= start]
    return intervals(ch, trace["end_ms"])

def periodic(trace, pin, on_ms, off_ms, tol=5, board=0, min_cycles=3, start=0):
    """pin must alternate HIGH for on_ms and LOW for off_ms"""
    segs = segments(trace, pin, board, start)
    segs = segs[1:-1] if len(segs) > 2 else []   # drop partial first/last
    if len(segs) < 2 * min_cycles:
        return [f"pin {pin}: expected at least {min_cycles} full cycles, found {len(segs)} level changes between {start} and {trace['end_ms']:.0f} ms"]
    errs = []
    for t0, t1, v in segs:
        want = on_ms if v == 1 else off_ms
        if abs((t1 - t0) - want) > tol:
            errs.append(f"pin {pin}: {'HIGH' if v else 'LOW'} for {t1 - t0:.1f} ms at t={t0:.1f}, expected {want} ms")
            if len(errs) >= 3: break
    return errs

def toggles_every(trace, pin, period, tol=5, board=0, min_toggles=4, start=0, end=None):
    """pin must toggle every `period` ms between start and end"""
    ch = [c for c in pin_changes(trace, pin, board) if c[0] >= start and (end is None or c[0] <= end)]
    if len(ch) < min_toggles + 1:
        return [f"pin {pin}: expected at least {min_toggles} toggles, found {max(0, len(ch) - 1)}"]
    errs = []
    for (t0, _), (t1, _) in zip(ch[1:], ch[2:]):
        if abs((t1 - t0) - period) > tol:
            errs.append(f"pin {pin}: toggle interval {t1 - t0:.1f} ms at t={t0:.1f}, expected {period} ms")
            if len(errs) >= 3: break
    return errs

def constant(trace, pin, level, t0, t1, board=0):
    """pin must stay at `level` during [t0, t1] (level None = any, but no change)"""
    segs = segments(trace, pin, board)
    for a, b, v in segs:
        if b <= t0 or a >= t1: continue
        if level is not None and v != level:
            return [f"pin {pin} was {'HIGH' if v else 'LOW'} at t={max(a, t0):.1f} ms, expected {'HIGH' if level else 'LOW'} during [{t0}, {t1}]"]
    if not segs: return [f"pin {pin} was never driven"]
    return []

def no_change(trace, pin, t0, t1, board=0):
    ch = [c for c in pin_changes(trace, pin, board) if t0 < c[0] < t1]
    return [f"pin {pin} changed {len(ch)} time(s) during [{t0}, {t1}] ms, expected none (first at {ch[0][0]:.1f})"] if ch else []

def level_at(trace, pin, t, level, board=0, what=None):
    v = pin_level_at(trace, pin, t, board)
    if v != level:
        return [f"{what or f'pin {pin}'} at t={t} ms is {'HIGH' if v == 1 else 'LOW' if v == 0 else 'undefined'}, expected {'HIGH' if level else 'LOW'}"]
    return []

def changes_between(trace, pin, t0, t1, board=0):
    return [c for c in pin_changes(trace, pin, board) if t0 <= c[0] <= t1]

def lines(trace, board=0):
    return [l for l in serial_lines(trace, board) if l != ""]

def lines_with_time(trace, board=0):
    """[(t_first_byte, line)] -- reconstruct lines from write chunks"""
    out = []; cur = ""; t0 = None
    for t, text, _ in serial_chunks(trace, board):
        for ch in text:
            if ch == "\r": continue
            if t0 is None: t0 = t
            if ch == "\n":
                out.append((t0, cur)); cur = ""; t0 = None
            else: cur += ch
    if cur: out.append((t0, cur))
    return out

def expect_lines(trace, expected, board=0, prefix_ok=False):
    got = lines(trace, board)
    for i, e in enumerate(expected):
        if i >= len(got): return [f"serial output stopped after {len(got)} line(s); expected line {i + 1}: {e!r}\n   got: {got[:8]}"]
        g = got[i].strip()
        if (g.startswith(e) if prefix_ok else g == e): continue
        return [f"serial line {i + 1} is {got[i]!r}, expected {e!r}\n   all lines: {got[:12]}"]
    return []

def line_times(trace, matcher, board=0):
    """times of lines for which matcher(line) is True"""
    return [t for t, l in lines_with_time(trace, board) if matcher(l)]

def approx(got, want, tol=5):
    return abs(got - want) <= tol

def bytes_sent(trace, board=0):
    out = []
    for _, _, bs in serial_chunks(trace, board): out += bs
    return out

def pin_defined(trace, pin, board=0):
    return bool(pin_changes(trace, pin, board))
