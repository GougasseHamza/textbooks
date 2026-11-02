"""Exact numbers for OR_Essentials: basis enumeration with Fractions, tableau of a basis, sensitivity."""
from fractions import Fraction as F
from itertools import combinations
def inv(M):
    n=len(M); A=[list(map(F,r))+[F(int(i==j)) for j in range(n)] for i,r in enumerate(M)]
    for c in range(n):
        p=next((r for r in range(c,n) if A[r][c]!=0),None)
        if p is None: return None
        A[c],A[p]=A[p],A[c]; pv=A[c][c]; A[c]=[v/pv for v in A[c]]
        for r in range(n):
            if r!=c and A[r][c]!=0:
                f=A[r][c]; A[r]=[a-f*b for a,b in zip(A[r],A[c])]
    return [r[n:] for r in A]
def mv(M,v): return [sum(a*b for a,b in zip(r,v)) for r in M]
def col(M,j): return [r[j] for r in M]
def tableau(A,b,c,Bidx):
    m,n=len(A),len(A[0]); B=[[A[i][j] for j in Bidx] for i in range(m)]; Bi=inv(B)
    xB=mv(Bi,b); cB=[c[j] for j in Bidx]; y=[sum(cB[k]*Bi[k][i] for k in range(m)) for i in range(m)]
    N=[j for j in range(n) if j not in Bidx]
    r={j:c[j]-sum(y[i]*A[i][j] for i in range(m)) for j in N}
    Q={j:[-v for v in mv(Bi,col(A,j))] for j in N}   # displayed coefficients
    z0=sum(cB[k]*xB[k] for k in range(m))
    return dict(Bi=Bi,xB=xB,y=y,r=r,Q=Q,z0=z0,N=N)
def show(name,T,names,Bidx):
    print(f"--- {name}: basis {[names[j] for j in Bidx]}")
    for k,j in enumerate(Bidx):
        print(f"  {names[j]} = {T['xB'][k]} " + " ".join(f"{'+' if T['Q'][q][k]>=0 else '-'} {abs(T['Q'][q][k])}{names[q]}" for q in T['N']))
    print(f"  z = {T['z0']} " + " ".join(f"{'+' if T['r'][q]>=0 else '-'} {abs(T['r'][q])}{names[q]}" for q in T['N']))
    print("  B^-1 =",T['Bi']); print("  y =",T['y'])
def allbases(A,b,c,names):
    m,n=len(A),len(A[0]); rows=[]
    for Bidx in combinations(range(n),m):
        B=[[A[i][j] for j in Bidx] for i in range(m)]; Bi=inv(B)
        if Bi is None: rows.append((Bidx,"dependent",None,None)); continue
        xB=mv(Bi,b); x=[F(0)]*n
        for k,j in enumerate(Bidx): x[j]=xB[k]
        feas=all(v>=0 for v in x); z=sum(ci*xi for ci,xi in zip(c,x)) if feas else None
        rows.append((Bidx,x,feas,z))
    for Bidx,x,feas,z in rows:
        print("  ",[names[j] for j in Bidx], x if x!="dependent" else "dependent", feas, z)
F_=F
# ===== Running example: max 4x1+3x2 s.t. 2x1+x2<=10, x1+x2<=6, x2<=5
A=[[2,1,1,0,0],[1,1,0,1,0],[0,1,0,0,1]]; A=[[F(v) for v in r] for r in A]
b=[F(10),F(6),F(5)]; c=[F(4),F(3),F(0),F(0),F(0)]; names=["x1","x2","s1","s2","s3"]
print("=== RUNNING EXAMPLE, all bases"); allbases(A,b,c,names)
for Bidx in [(2,3,4),(0,3,4),(0,1,4)]: show("tableau",tableau(A,b,c,Bidx),names,Bidx)
T=tableau(A,b,c,(0,1,4))
# ranges on b
for i in range(3):
    g=col(T['Bi'],i); lo=max([-T['xB'][k]/g[k] for k in range(3) if g[k]>0],default="-inf"); hi=min([T['xB'][k]/(-g[k]) for k in range(3) if g[k]<0],default="+inf")
    print(f"  b{i+1} range: g={g} delta in [{lo},{hi}] slope y={T['y'][i]}")
# beyond: b1=14
b2=[F(14),F(6),F(5)]
print("  b1=14 old basis:",mv(T['Bi'],b2)); 
from itertools import combinations as C
best=None
for Bidx in C(range(5),3):
    B=[[A[i][j] for j in Bidx] for i in range(3)]; Bi=inv(B)
    if Bi is None: continue
    xB=mv(Bi,b2)
    if all(v>=0 for v in xB):
        z=sum(c[j]*xB[k] for k,j in enumerate(Bidx))
        if best is None or z>best[0]: best=(z,Bidx,xB)
print("  b1=14 optimum:",best, "tableau:"); show("b1=14",tableau(A,b2,c,best[1]),names,best[1])
# c ranges (basic): add eps*row to objective
for k,j in enumerate((0,1)):
    conds=[]
    for q in T['N']:
        rq=T['r'][q]; qk=T['Q'][q][k]   # r_q + eps*qk <= 0
        conds.append((rq,qk))
    lo=max([-rq/qk for rq,qk in conds if qk>0],default="-inf"); hi=min([-rq/qk for rq,qk in conds if qk<0],default="+inf")
    print(f"  c of {names[j]}: eps in [{lo},{hi}]")
# new activity a=(1,2,0)
a=[F(1),F(2),F(0)]; Ba=mv(T['Bi'],a); print("  B^-1 a =",Ba," y.a =",sum(y*ai for y,ai in zip(T['y'],a)))
A2=[r+[a[i]] for i,r in enumerate(A)]; c2=c+[F(6)]; n2=["x1","x2","s1","s2","s3","u"]
best=None
for Bidx in C(range(6),3):
    B=[[A2[i][j] for j in Bidx] for i in range(3)]; Bi=inv(B)
    if Bi is None: continue
    xB=mv(Bi,b)
    if all(v>=0 for v in xB):
        z=sum(c2[j]*xB[k] for k,j in enumerate(Bidx))
        if best is None or z>best[0]: best=(z,Bidx,xB)
print("  gamma=6 optimum:",best); show("gamma=6",tableau(A2,b,c2,best[1]),n2,best[1])
# ===== Reading example: max 3x1+2x2+x3 s.t. x1+x2+x3<=6, 2x1+x2<=8
A=[[1,1,1,1,0],[2,1,0,0,1]]; A=[[F(v) for v in r] for r in A]; b=[F(6),F(8)]; c=[F(3),F(2),F(1),F(0),F(0)]; names=["x1","x2","x3","s1","s2"]
print("=== READING EXAMPLE"); allbases(A,b,c,names)
for Bidx in [(3,4),(0,3),(0,2)]: show("tableau",tableau(A,b,c,Bidx),names,Bidx)
T=tableau(A,b,c,(0,2))
for i in range(2):
    g=col(T['Bi'],i); lo=max([-T['xB'][k]/g[k] for k in range(2) if g[k]>0],default="-inf"); hi=min([T['xB'][k]/(-g[k]) for k in range(2) if g[k]<0],default="+inf")
    print(f"  b{i+1} range: g={g} delta in [{lo},{hi}] slope y={T['y'][i]}")
for k,j in enumerate((0,2)):
    conds=[(T['r'][q],T['Q'][q][k]) for q in T['N']]
    lo=max([-rq/qk for rq,qk in conds if qk>0],default="-inf"); hi=min([-rq/qk for rq,qk in conds if qk<0],default="+inf")
    print(f"  c of {names[j]}: eps in [{lo},{hi}]")
a=[F(1),F(1)]; print("  new u a=(1,1) profit 5/2: B^-1a=",mv(T['Bi'],a)," r_u=",F(5,2)-sum(y*ai for y,ai in zip(T['y'],a)))
A2=[r+[a[i]] for i,r in enumerate(A)]; c2=c+[F(5,2)]; n2=names+["u"]
best=None
for Bidx in C(range(6),2):
    B=[[A2[i][j] for j in Bidx] for i in range(2)]; Bi=inv(B)
    if Bi is None: continue
    xB=mv(Bi,b)
    if all(v>=0 for v in xB):
        z=sum(c2[j]*xB[k] for k,j in enumerate(Bidx))
        if best is None or z>best[0]: best=(z,Bidx,xB)
print("  with u optimum:",best); show("with u",tableau(A2,b,c2,best[1]),n2,best[1])
# ===== Enumeration practice: A=[[1,2,1,0],[3,1,0,1]] b=(4,6) c=(2,3,0,0)
A=[[F(1),F(2),F(1),F(0)],[F(3),F(1),F(0),F(1)]]; b=[F(4),F(6)]; c=[F(2),F(3),F(0),F(0)]; names=["x1","x2","s1","s2"]
print("=== ENUMERATION PRACTICE"); allbases(A,b,c,names); show("opt",tableau(A,b,c,(0,1)),names,(0,1))
