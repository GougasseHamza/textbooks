#!/usr/bin/env python3
"""check.py -- grade a sketch against an exercise's automatic test (offline).

    python3 check.py ex07                 # checks exercises/ex07_*/sketch.ino (your file)
    python3 check.py ex07 my.ino          # checks a specific file
    python3 check.py ex18 a.ino b.ino     # two-board exercise
    python3 check.py ex07 --solution      # runs the reference solution instead
    python3 check.py all --solution       # verify every reference solution
    python3 check.py list                 # list exercises

Each test in tests/<id>.py defines SCENARIO (what happens to the board: button
presses, serial input, duration) and check(trace, H) returning a list of
failure messages (empty = PASS). H is the helper module checklib.
"""
import glob, importlib.util, os, sys, traceback
import arduino_sim as A
import checklib as H

KIT = os.path.dirname(os.path.abspath(__file__))

def find_exercise(eid):
    ds = sorted(glob.glob(os.path.join(KIT, "exercises", eid + "*")))
    ds = [d for d in ds if os.path.isdir(d)]
    if not ds: sys.exit(f"no exercise matching '{eid}' in exercises/")
    return os.path.basename(ds[0])

def load_test(name):
    path = os.path.join(KIT, "tests", name + ".py")
    if not os.path.exists(path): sys.exit(f"no test for {name}")
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec); spec.loader.exec_module(mod)
    return mod

def grade(name, sketches, verbose=True):
    test = load_test(name)
    exe, err = A.build(sketches)
    if not exe:
        print(f"[{name}] COMPILE ERROR\n{err}"); return False
    scn = test.SCENARIO
    path = A.write_scenario(scn.get("duration", 10000), scn.get("events", []), link=scn.get("link", False),
                            tick_us=scn.get("tick_us", 100), seed=scn.get("seed", 12345))
    try:
        trace = A.run(exe, path)
    except Exception as e:
        print(f"[{name}] RUN ERROR: {e}"); return False
    try:
        failures = test.check(trace, H) or []
    except Exception:
        failures = ["the test crashed while reading your trace (usually: no output at all)\n" + traceback.format_exc(limit=2)]
    warns = A.warnings(trace)
    if failures:
        print(f"[{name}] FAIL")
        for f in failures: print("   - " + f.replace("\n", "\n     "))
    else:
        print(f"[{name}] PASS")
    if warns and verbose:
        print("   simulator warnings:")
        for t, m in warns[:8]: print(f"     {t:8.1f} ms  {m}")
    if failures and verbose:
        print("   (run `python3 arduino_sim.py run <sketch> --wave` with the same stimuli to see the timeline)")
    return not failures

def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"): print(__doc__); return
    use_solution = "--solution" in args
    args = [a for a in args if a != "--solution"]
    if args[0] == "list":
        for d in sorted(glob.glob(os.path.join(KIT, "exercises", "ex*"))):
            first = open(os.path.join(d, "README.md")).readline().strip("# \n") if os.path.exists(os.path.join(d, "README.md")) else ""
            print(f"{os.path.basename(d):28s} {first}")
        return
    names = []
    if args[0] == "all":
        names = [os.path.basename(d) for d in sorted(glob.glob(os.path.join(KIT, "exercises", "ex*")))]
    else:
        names = [find_exercise(args[0])]
    ok = 0
    for name in names:
        test = load_test(name)
        nboards = 2 if test.SCENARIO.get("link") else 1
        if use_solution:
            base = os.path.join(KIT, "solutions", name)
            sketches = [base + ".ino"] if nboards == 1 else [base + "_A.ino", base + "_B.ino"]
        elif len(args) > 1:
            sketches = args[1:1 + nboards]
        else:
            d = os.path.join(KIT, "exercises", name)
            sketches = [os.path.join(d, "sketch.ino")] if nboards == 1 else [os.path.join(d, "sketch_A.ino"), os.path.join(d, "sketch_B.ino")]
        missing = [s for s in sketches if not os.path.exists(s)]
        if missing:
            print(f"[{name}] missing file(s): {', '.join(missing)}  (copy starter.ino to sketch.ino and fill it in)"); continue
        if grade(name, sketches, verbose=(len(names) == 1)): ok += 1
    if len(names) > 1: print(f"\n{ok}/{len(names)} passed")

if __name__ == "__main__":
    main()
