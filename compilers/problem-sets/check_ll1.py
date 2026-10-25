"""Recompute FIRST/FOLLOW/LL(1) tables and run the stack parser for the mock's grammars."""
EPS='eps'
def ff(G,start):
    N=list(G); first={A:set() for A in N}; follow={A:set() for A in N}; follow[start].add('$')
    def first_of(seq):
        out=set()
        for X in seq:
            if X not in G: out.add(X); return out
            out|=first[X]-{EPS}
            if EPS not in first[X]: return out
        out.add(EPS); return out
    ch=True
    while ch:
        ch=False
        for A,prods in G.items():
            for p in prods:
                f=first_of(p) if p!=[EPS] else {EPS}
                if not f<=first[A]: first[A]|=f; ch=True
    ch=True
    while ch:
        ch=False
        for A,prods in G.items():
            for p in prods:
                if p==[EPS]: continue
                for i,X in enumerate(p):
                    if X in G:
                        f=first_of(p[i+1:]) if i+1<len(p) else {EPS}
                        add=(f-{EPS})|(follow[A] if EPS in f else set())
                        if not add<=follow[X]: follow[X]|=add; ch=True
    table={}
    for A,prods in G.items():
        for k,p in enumerate(prods):
            sel=first_of(p) if p!=[EPS] else {EPS}
            toks=(sel-{EPS})|(follow[A] if EPS in sel else set())
            for t in toks:
                table.setdefault((A,t),[]).append((A,p))
    conflicts={k:v for k,v in table.items() if len(v)>1}
    return first,follow,table,conflicts
def parse(G,start,table,inp):
    stack=['$',start]; inp=inp+['$']; steps=0
    while True:
        top=stack[-1]; a=inp[0]; steps+=1
        if top=='$' and a=='$': return True,steps
        if top not in G:
            if top==a: stack.pop(); inp=inp[1:]
            else: return False,(top,a)
        else:
            e=table.get((top,a))
            if not e: return False,(top,a)
            stack.pop(); p=e[0][1]
            if p!=[EPS]: stack+=list(reversed(p))
def show(name,G,start,inputs=()):
    f,fo,t,c=ff(G,start); print("==",name); 
    for A in G: print(f"  {A}: FIRST={sorted(f[A])} FOLLOW={sorted(fo[A])}")
    print("  conflicts:",{k:[' '.join(p) for _,p in v] for k,v in c.items()} or "none")
    for s in inputs: print("  parse",s,"->",parse(G,start,t,s.split()))
show("1.5",{'S':[['A','x'],['B','x']],'A':[['a'],[EPS]],'B':[['b'],[EPS]]},'S')
show("3 (fixed)",{'Block':[['begin','Stmts','end']],'Stmts':[['Stmt','Stmts'],[EPS]],'Stmt':[['Block'],['Simple']],'Simple':[['id',';']]},'Block',["begin id ; begin end end"])
show("3 (original)",{'Block':[['begin','Stmts','end']],'Stmts':[['Stmts','Stmt'],[EPS]],'Stmt':[['Block'],['Simple']],'Simple':[['id',';']]},'Block')
G4={'S':[['ID',':=','E',';','S'],['PRINT','E',';','S'],[EPS]],'E':[['T','Ep']],'Ep':[['+','T','Ep'],[EPS]],'T':[['ID'],['NUM'],['(','E',')']]}
show("4",G4,'S',["PRINT ( ID ) + NUM ;","ID := ( NUM + ID ) ; PRINT NUM ;","ID := NUM NUM ;","ID := + ;"])
show("5",{'S':[['P','Q','R']],'P':[['(','S',')'],['x','S','y'],['z']],'Q':[['x','Q'],[EPS]],'R':[['w','R'],[EPS]]},'S',["( z w ) x w"])
show("1.4d",{'E':[['T','Ep']],'Ep':[['-','T','Ep'],[EPS]],'T':[['(','E',')'],['NUM']]},'E',["NUM - NUM - NUM"])
