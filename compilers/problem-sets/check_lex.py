"""check_lex.py: regex -> Thompson NFA (convention B) -> subset DFA -> minimal DFA,
brute-force language equivalence, and a longest-match scanner with read counts.
Regex syntax: letters/digits are symbols, | * + ? ( ) and 'e' is NOT epsilon; use '#' for epsilon."""
from itertools import product, combinations

# ---------- regex parsing ----------
class R:  # node: kind in sym, eps, cat, alt, star, plus, opt
    def __init__(s, k, a=None, b=None): s.k, s.a, s.b = k, a, b
def parse(src):
    pos = [0]
    def peek(): return src[pos[0]] if pos[0] < len(src) else None
    def eat(c): assert peek() == c, (src, pos[0], c); pos[0] += 1
    def alt():
        n = cat()
        while peek() == '|': eat('|'); n = R('alt', n, cat())
        return n
    def cat():
        n = None
        while peek() not in (None, '|', ')'):
            m = unary()
            n = m if n is None else R('cat', n, m)
        return n if n is not None else R('eps')
    def unary():
        n = atom()
        while peek() in ('*', '+', '?'):
            c = peek(); eat(c); n = R({'*': 'star', '+': 'plus', '?': 'opt'}[c], n)
        return n
    def atom():
        c = peek()
        if c == '(':
            eat('('); n = alt(); eat(')'); return n
        if c == '#': eat('#'); return R('eps')
        eat(c); return R('sym', c)
    n = alt(); assert pos[0] == len(src), src
    return n

def counts(n, c=None):
    c = c if c is not None else {'leaf': 0, 'alt': 0, 'star': 0, 'cat': 0, 'eps': 0}
    if n.k == 'sym': c['leaf'] += 1
    elif n.k == 'eps': c['eps'] += 1
    else:
        c[n.k if n.k in c else 'star'] += 0
        if n.k == 'alt': c['alt'] += 1
        if n.k == 'star': c['star'] += 1
        if n.k == 'cat': c['cat'] += 1
        if n.k in ('plus', 'opt'): c['star'] += 1  # counted as derived
        counts(n.a, c)
        if n.b is not None: counts(n.b, c)
    return c

# ---------- Thompson, convention B (concatenation merges the two states) ----------
class NFA:
    def __init__(s): s.edges = []; s.n = 0
    def new(s): s.n += 1; return s.n - 1
    def add(s, p, x, q): s.edges.append((p, x, q))
def thompson(n, N):
    """returns (start, end)"""
    if n.k == 'sym':
        s, t = N.new(), N.new(); N.add(s, n.a, t); return s, t
    if n.k == 'eps':
        s, t = N.new(), N.new(); N.add(s, '#', t); return s, t
    if n.k == 'cat':
        s1, t1 = thompson(n.a, N); s2, t2 = thompson(n.b, N)
        # merge t1 and s2
        N.edges = [(p if p != s2 else t1, x, q if q != s2 else t1) for p, x, q in N.edges]
        return s1, t2
    if n.k == 'alt':
        s, t = N.new(), N.new(); s1, t1 = thompson(n.a, N); s2, t2 = thompson(n.b, N)
        N.add(s, '#', s1); N.add(s, '#', s2); N.add(t1, '#', t); N.add(t2, '#', t); return s, t
    if n.k == 'star':
        s, t = N.new(), N.new(); s1, t1 = thompson(n.a, N)
        N.add(s, '#', s1); N.add(t1, '#', s1); N.add(t1, '#', t); N.add(s, '#', t); return s, t
    if n.k == 'plus':  # a+ = a a*  (expanded, as the book does)
        return thompson(R('cat', n.a, R('star', n.a)), N)
    if n.k == 'opt':
        return thompson(R('alt', n.a, R('eps')), N)
def build_nfa(src):
    N = NFA(); s, t = thompson(parse(src), N)
    states = sorted({p for p, _, _ in N.edges} | {q for _, _, q in N.edges} | {s, t})
    return N, s, t, states

def closure(S, edges):
    S = set(S); stack = list(S)
    while stack:
        p = stack.pop()
        for a, x, b in edges:
            if a == p and x == '#' and b not in S: S.add(b); stack.append(b)
    return frozenset(S)
def move(S, c, edges): return {b for a, x, b in edges if a in S and x == c}
def nfa_accepts(N, s, t, w):
    S = closure({s}, N.edges)
    for c in w: S = closure(move(S, c, N.edges), N.edges)
    return t in S

# ---------- DFA ----------
class DFA:
    def __init__(s, start, delta, acc, alphabet): s.start, s.delta, s.acc, s.alphabet = start, delta, acc, alphabet
    def states(s): return sorted({s.start} | {p for p, _ in s.delta} | set(s.delta.values()), key=str)
    def run(s, w):
        q = s.start
        for c in w:
            q = s.delta.get((q, c))
            if q is None: return False
        return q in s.acc
def subset(N, s, t, alphabet):
    start = closure({s}, N.edges); todo = [start]; seen = {start}; delta = {}; acc = set()
    while todo:
        S = todo.pop(0)
        if t in S: acc.add(S)
        for c in alphabet:
            T = closure(move(S, c, N.edges), N.edges)
            if not T: continue
            delta[(S, c)] = T
            if T not in seen: seen.add(T); todo.append(T)
    return DFA(start, delta, acc, alphabet)
def name(S): return ''.join(str(x) for x in sorted(S)) if S else 'dead'
def show_dfa(D, nm=name):
    for q in D.states():
        row = ' '.join(f"{c}->{nm(D.delta[(q,c)]) if (q,c) in D.delta else '-':>6}" for c in D.alphabet)
        print(f"  {nm(q):>10}{'*' if q in D.acc else ' '} {row}")
def minimize(D):
    """table filling with explicit dead state; returns (partition list, minimal DFA, marked pairs count)"""
    Q = D.states() + ['dead']
    def d(q, c): return 'dead' if q == 'dead' else D.delta.get((q, c), 'dead')
    marked = set()
    for p, q in combinations(Q, 2):
        if (p in D.acc) != (q in D.acc): marked.add(frozenset((p, q)))
    changed = True
    while changed:
        changed = False
        for p, q in combinations(Q, 2):
            pr = frozenset((p, q))
            if pr in marked: continue
            for c in D.alphabet:
                if frozenset((d(p, c), d(q, c))) in marked and d(p, c) != d(q, c):
                    marked.add(pr); changed = True; break
    blocks = []
    for q in Q:
        for b in blocks:
            if frozenset((q, b[0])) not in marked: b.append(q); break
        else: blocks.append([q])
    rep = {q: tuple(b) for b in blocks for q in b}
    delta = {}
    for b in blocks:
        if b[0] == 'dead' and all(x == 'dead' for x in b): continue
        for c in D.alphabet:
            tgt = rep[d(b[0], c)]
            if all(x == 'dead' for x in tgt): continue
            delta[(tuple(b), c)] = tgt
    M = DFA(rep[D.start], delta, {rep[q] for q in D.acc}, D.alphabet)
    return blocks, M
def all_strings(alphabet, n):
    for k in range(n + 1):
        for t in product(alphabet, repeat=k): yield ''.join(t)
def equiv(src1, src2, alphabet, n=8):
    N1, s1, t1, _ = build_nfa(src1); N2, s2, t2, _ = build_nfa(src2)
    for w in all_strings(alphabet, n):
        if nfa_accepts(N1, s1, t1, w) != nfa_accepts(N2, s2, t2, w): return w
    return None
def lang(src, alphabet, n):
    N, s, t, _ = build_nfa(src)
    return [w for w in all_strings(alphabet, n) if nfa_accepts(N, s, t, w)]

# ---------- longest-match scanner over a list of (name, regex) rules in priority order ----------
def scan(rules, text, alphabet):
    nfas = [(nm, build_nfa(r)) for nm, r in rules]
    out = []; i = 0; reads = 0
    while i < len(text):
        last_acc = None; j = i
        S = [closure({s}, N.edges) for nm, (N, s, t, _) in nfas]
        def accepting(S):
            for k, (nm, (N, s, t, _)) in enumerate(nfas):
                if t in S[k]: return nm
            return None
        a = accepting(S)
        if a: last_acc = (j, a)
        while j < len(text):
            c = text[j]; reads += 1
            S = [closure(move(S[k], c, N.edges), N.edges) for k, (nm, (N, s, t, _)) in enumerate(nfas)]
            j += 1
            if not any(S): break
            a = accepting(S)
            if a: last_acc = (j, a)
        if last_acc is None or last_acc[0] == i:
            out.append(('ERROR', text[i])); i += 1; continue
        pushback = j - last_acc[0]
        out.append((last_acc[1], text[i:last_acc[0]], pushback)); i = last_acc[0]
    return out, reads
