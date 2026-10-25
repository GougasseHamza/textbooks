#!/usr/bin/env python3
"""Run every midterm sketch in the simulator and check its key behaviour (python3 midterms/verify_midterms.py)."""
import os, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
import arduino_sim as A, checklib as H
D = os.path.dirname(os.path.abspath(__file__))
results = []
def run(name, sketch, duration, events=()):
    exe, err = A.build([os.path.join(D, sketch)])
    if not exe: results.append((name, ["compile error: " + err])); return None
    return A.run(exe, A.write_scenario(duration, events))
def rep(name, errs): results.append((name, errs))

# 2022-2023 ex5 lampe 3 modes : appuis a 1,0 s, 2,0 s, 4,0 s
tr = run("2023 ex5 lampe", "m2023_ex5_lampe3modes.ino", 5000, H.press(1000, 12) + H.press(2000, 12) + H.press(4000, 12))
rep("2023 ex5 lampe", H.level_at(tr, 13, 500, 0) + H.level_at(tr, 13, 1500, 1) + H.level_at(tr, 13, 2200, 1) + H.level_at(tr, 13, 2700, 0)
    + H.toggles_every(tr, 13, 500, start=2100, end=3990, min_toggles=2) + H.level_at(tr, 13, 4500, 0))
# 2022-2023 ex7 gel : main a 1 s pendant 3 s ; reservoir vide a 7 s ; maintenance refusee puis acceptee
ev = H.hold(0, 2, 1) + H.hold(0, 3, 1) + H.hold(0, 4, 1) + H.hold(1000, 2, 0) + H.hold(5000, 2, 1) + H.hold(7000, 3, 0) + H.press(8000, 4) + H.hold(9000, 3, 1) + H.press(9500, 4)
tr = run("2023 ex7 gel", "m2023_ex7_gel.ino", 11000, ev)
rep("2023 ex7 gel", H.level_at(tr, 9, 500, 1) + H.level_at(tr, 8, 1500, 1) + H.level_at(tr, 8, 2000, 0) + H.level_at(tr, 8, 3500, 0) + H.level_at(tr, 8, 4000, 1)
    + H.toggles_every(tr, 10, 500, start=7100, end=9400, min_toggles=3) + H.level_at(tr, 9, 8500, 0) + H.level_at(tr, 9, 10000, 1) + H.level_at(tr, 10, 10000, 0))
# 2022-2023 ex8 appel infirmier
ev = H.press(1000, 2) + H.press(65000, 3) + H.press(66000, 3)
tr = run("2023 ex8 appel", "m2023_ex8_appel_infirmier.ino", 67000, ev)
rep("2023 ex8 appel", H.toggles_every(tr, 8, 500, start=1100, end=60990, min_toggles=100) + H.level_at(tr, 10, 1100, 1) + H.level_at(tr, 10, 1500, 0) + H.level_at(tr, 10, 6100, 1)
    + H.level_at(tr, 10, 62000, 1) + H.toggles_every(tr, 8, 125, start=61100, end=64990, min_toggles=20)
    + H.level_at(tr, 9, 65500, 1) + H.level_at(tr, 10, 65500, 0) + H.level_at(tr, 8, 65500, 0) + H.level_at(tr, 9, 66500, 0))
# 2023-2024 ex6 passage a niveau corrige
tr = run("2024 ex6 PN corrige", "m2024_ex6_passage_niveau_corrige.ino", 11000, H.press(1000, 2))
rep("2024 ex6 PN corrige", H.toggles_every(tr, 8, 500, start=1100, end=8990, min_toggles=12) + H.level_at(tr, 9, 5000, 1) + H.level_at(tr, 9, 9500, 0) + H.level_at(tr, 8, 9500, 0))
# 2023-2024 ex7 afficheur : trace de l'original puis du corrige
ev = H.serial_in(500, "Q3\n") + H.serial_in(3000, "R12\n") + H.serial_in(6000, "A\n")
tr = run("2024 ex7 original", "m2024_ex7_afficheur_original.ino", 9000, ev)
rep("2024 ex7 original", [] if H.lines(tr) == ["Quai 3", "Commande inconnue", "Retard : 12 min", "Annule", "Commande inconnue", "Annule"] else ["lines: " + str(H.lines(tr))])
tr = run("2024 ex7 corrige", "m2024_ex7_afficheur_corrige.ino", 9000, ev)
rep("2024 ex7 corrige", [] if H.lines(tr) == ["Quai 3", "Retard : 12 min", "Annule"] else ["lines: " + str(H.lines(tr))])
# 2023-2024 ex8 passage a niveau FSM corrigee : train 1, second train pendant la reouverture
ev = H.press(1000, 2) + H.press(45000, 3) + H.press(46000, 2) + H.press(60000, 3)
tr = run("2024 ex8 PN fsm", "m2024_ex8_passage_niveau_fsm.ino", 70000, ev)
rep("2024 ex8 PN fsm", H.level_at(tr, 8, 2000, 1) + H.level_at(tr, 10, 5000, 0) + H.level_at(tr, 10, 9500, 1) + H.level_at(tr, 9, 9500, 0)
    + H.level_at(tr, 10, 48000, 1) + H.level_at(tr, 10, 55000, 1) + H.level_at(tr, 10, 67000, 0) + H.level_at(tr, 8, 67000, 0))
# 2023-2024 ex9 feu pieton + nuit
ev = H.press(3000, 2) + H.hold(0, 4, 1) + H.hold(25000, 4, 0) + H.hold(30000, 4, 1)
tr = run("2024 ex9 feu pieton", "m2024_ex9_feu_pieton_nuit.ino", 40000, ev)
rep("2024 ex9 feu pieton", H.level_at(tr, 8, 500, 1) + H.level_at(tr, 8, 9000, 1) + H.level_at(tr, 9, 10500, 1) + H.level_at(tr, 10, 14000, 1) + H.level_at(tr, 11, 14000, 1)
    + H.level_at(tr, 8, 21500, 1) + H.level_at(tr, 11, 21500, 0) + H.toggles_every(tr, 9, 500, start=25100, end=29990, min_toggles=6) + H.level_at(tr, 8, 27000, 0)
    + H.level_at(tr, 10, 30500, 1) + H.level_at(tr, 8, 38600, 1))
# 2024-2025 ex8 bouteilles
ev = H.press(1000, 2) + H.press(3000, 3) + H.press(8000, 2) + H.press(14000, 4)
tr = run("2025 ex8 bouteilles", "m2025_ex8_bouteilles.ino", 15000, ev)
rep("2025 ex8 bouteilles", H.level_at(tr, 8, 1200, 1) + H.level_at(tr, 8, 3200, 0) + H.level_at(tr, 9, 3200, 1) + H.level_at(tr, 9, 4700, 0) + H.level_at(tr, 10, 4700, 1)
    + H.level_at(tr, 10, 6700, 0) + H.level_at(tr, 8, 8200, 1) + H.level_at(tr, 11, 13200, 1) + H.level_at(tr, 8, 13200, 0) + H.level_at(tr, 11, 14300, 0))
# 2024-2025 ex9 arrosage
ev = H.hold(0, 2, 1) + H.hold(0, 3, 0) + H.hold(5000, 2, 0) + H.press(65200, 4, 1500) + H.hold(68000, 2, 1) + H.hold(70000, 3, 1) + H.hold(74000, 3, 0)
tr = run("2025 ex9 arrosage", "m2025_ex9_arrosage.ino", 76000, ev)
rep("2025 ex9 arrosage", H.level_at(tr, 7, 500, 1) + H.level_at(tr, 7, 5500, 0) + H.level_at(tr, 7, 60000, 0) + H.level_at(tr, 7, 66000, 1) + H.level_at(tr, 7, 67000, 0)
    + H.level_at(tr, 7, 68500, 1) + H.level_at(tr, 7, 70500, 0) + H.toggles_every(tr, 9, 250, start=70100, end=73990, min_toggles=10) + H.level_at(tr, 9, 74600, 0) + H.level_at(tr, 7, 74600, 1))
# Midterm 2 ex4c fcActive : activation avec rebonds -> une seule piece
ev = H.hold(0, 3, 0) + H.hold(1000, 3, 1) + H.hold(1003, 3, 0) + H.hold(1006, 3, 1) + H.hold(1012, 3, 0) + H.hold(1015, 3, 1) + H.hold(1500, 3, 0) + H.hold(1504, 3, 1) + H.hold(1508, 3, 0) + H.hold(3000, 3, 1) + H.hold(3600, 3, 0)
tr = run("2026 ex4 fcActive", "m2026_ex4_fcActive.ino", 4000, ev)
rep("2026 ex4 fcActive", [] if H.lines(tr) == ["Pieces: 1", "Pieces: 2"] else ["lines: " + str(H.lines(tr))])
tr = run("2026 ex5 A", "m2026_ex5_extraitA.ino", 500); rep("2026 ex5 A", [] if H.lines(tr) == ["1 1", "1 2", "1 3"] else ["lines: " + str(H.lines(tr))])
tr = run("2026 ex5 B", "m2026_ex5_extraitB.ino", 500); rep("2026 ex5 B", [] if H.lines(tr) == ["24", "2", "3"] else ["lines: " + str(H.lines(tr))])   # 24 on the host simulator (int = 4 bytes); 12 on the ATmega328P (int = 2 bytes)
ev = H.serial_in(500, "S\n") + H.serial_in(2000, "M\n")
tr = run("2026 ex5 C corrige", "m2026_ex5_extraitC_corrige.ino", 3000, ev)
rep("2026 ex5 C corrige", H.level_at(tr, 8, 1000, 0) + H.level_at(tr, 8, 1500, 0) + H.level_at(tr, 8, 2500, 1))
# Midterm 2 ex6 trois boutons
ev = H.hold(0, 2, 0) + H.hold(0, 4, 1) + H.hold(1000, 2, 1) + H.hold(1500, 2, 0) + H.hold(2000, 4, 0) + H.hold(2500, 4, 1) + H.press(3000, 9, 500, bounce=0)
tr = run("2026 ex6 trois boutons", "m2026_ex6_trois_boutons.ino", 4000, ev)
rep("2026 ex6 trois boutons", H.level_at(tr, 3, 1200, 1) + H.level_at(tr, 3, 1800, 0) + H.level_at(tr, 8, 2200, 1) + H.level_at(tr, 8, 2800, 0) + H.level_at(tr, 10, 3200, 1) + H.level_at(tr, 10, 3800, 0))
# Midterm 2 ex9 alarme de magasin
ev = H.hold(0, 2, 0) + H.hold(1000, 2, 1) + H.hold(1300, 2, 0)
tr = run("2026 ex9 magasin", "m2026_ex9_alarme_magasin.ino", 7000, ev)
rep("2026 ex9 magasin", H.level_at(tr, 11, 500, 1) + H.level_at(tr, 12, 1500, 1) + H.toggles_every(tr, 13, 300, start=1100, end=3990, min_toggles=6)
    + H.level_at(tr, 12, 4500, 0) + H.level_at(tr, 13, 4500, 0) + H.level_at(tr, 11, 6500, 1) + H.expect_lines(tr, ["State: STANDBY", "State: ALERT", "State: COOLDOWN", "State: STANDBY"]))
# Midterm 2 ex8d alarme corrigee : delai de sortie, PIR ignore, entree, sirene 3 min puis MEMO
ev = H.hold(0, 3, 0) + H.press(1000, 2) + H.hold(5000, 3, 1) + H.hold(5300, 3, 0) + H.hold(40000, 3, 1) + H.hold(40300, 3, 0) + H.press(245000, 2)
tr = run("2026 ex8 alarme corrigee", "m2026_ex8_alarme_corrigee.ino", 247000, ev)
rep("2026 ex8 alarme corrigee", H.level_at(tr, 10, 10000, 0) + H.level_at(tr, 10, 30000, 0) + H.level_at(tr, 9, 35000, 1) + H.level_at(tr, 10, 50000, 0)
    + H.level_at(tr, 10, 56000, 1) + H.level_at(tr, 10, 230000, 1) + H.level_at(tr, 10, 241000, 0) + H.level_at(tr, 8, 246000, 1))
# Midterm 2 ex10 ceinture
ev = H.hold(0, 2, 1) + H.hold(0, 3, 1) + H.hold(0, 4, 1) + H.hold(1000, 4, 0) + H.hold(1000, 2, 0) + H.hold(105000, 3, 0)
tr = run("2026 ex10 ceinture", "m2026_ex10_ceinture.ino", 107000, ev)
rep("2026 ex10 ceinture", H.toggles_every(tr, 8, 500, start=1100, end=10990, min_toggles=15) + H.level_at(tr, 9, 5000, 0)
    + H.toggles_every(tr, 9, 500, start=11100, end=100990, min_toggles=150) + H.level_at(tr, 8, 102000, 1) + H.level_at(tr, 9, 102000, 0) + H.level_at(tr, 8, 105500, 0))

ok = 0
for name, errs in results:
    print(f"[{name}] {'PASS' if not errs else 'FAIL'}"); ok += not errs
    for e in errs: print("   - " + e)
print(f"{ok}/{len(results)} midterm sketches OK")
