"""Exact verification of the numeric answers in OR_Midterm_Exercises (basis enumeration with Fractions)."""
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
def matvec(M,v): return [sum(a*b for a,b in zip(r,v)) for r in M]
def col(M,j): return [r[j] for r in M]

def bases(A,b):
    m,n=len(A),len(A[0]); out=[]
    for Bidx in combinations(range(n),m):
        B=[[A[i][j] for j in Bidx] for i in range(m)]; Bi=inv(B)
        if Bi is None: out.append((Bidx,None,None)); continue
        xB=matvec(Bi,b); x=[F(0)]*n
        for k,j in enumerate(Bidx): x[j]=xB[k]
        out.append((Bidx,x,Bi))
    return out
def best(A,b,c):
    bb=None
    for Bidx,x,Bi in bases(A,b):
        if x is not None and all(v>=0 for v in x):
            z=sum(ci*xi for ci,xi in zip(c,x))
            if bb is None or z>bb[0]: bb=(z,x,Bidx)
    return bb
def slackform(A,b,c):  # Ax<=b -> [A I]
    m=len(A); return [list(map(F,r))+[F(int(i==j)) for j in range(m)] for i,r in enumerate(A)], list(map(F,b)), list(map(F,c))+[F(0)]*m
def dual_of_basis(A,c,Bidx):
    B=[[A[i][j] for j in Bidx] for i in range(len(A))]; Bi=inv(B); cB=[c[j] for j in Bidx]
    return [sum(cB[k]*Bi[k][i] for k in range(len(Bidx))) for i in range(len(A))]
ok=True
def check(name,cond,detail=""):
    global ok; ok&=bool(cond); print(("OK  " if cond else "FAIL"),name,detail)

# Ex3
A=[[1,0,1,2],[0,1,1,2]]; b=[4,2]; c=[3,2,4,7]
A=[[F(v) for v in r] for r in A]; b=[F(v) for v in b]; c=[F(v) for v in c]
tab={Bidx:x for Bidx,x,_ in bases(A,b)}
check("ex3 table",tab[(0,1)]==[4,2,0,0] and tab[(0,2)]==[2,0,2,0] and tab[(0,3)]==[2,0,0,1] and tab[(1,2)]==[0,-2,4,0] and tab[(1,3)]==[0,-2,0,2] and tab[(2,3)] is None)
check("ex3 opt 16",best(A,b,c)[0]==16)
# Ex4
Ai,bi,ci=slackform([[1,2],[3,1],[1,0]],[10,12,4],[3,2]); z,x,_=best(Ai,bi,ci)
check("ex4 opt (14/5,18/5) 78/5",(x[0],x[1],z)==(F(14,5),F(18,5),F(78,5)))
# Ex5
Ai,bi,ci=slackform([[2,1,1],[1,3,2],[1,1,2]],[10,15,12],[5,4,3]); z,x,Bidx=best(Ai,bi,ci)
check("ex5 (3,4,0,0,0,5) z=31",x==[3,4,0,0,0,5] and z==31)
# Ex6 primal and dual
Ai,bi,ci=slackform([[2,1,1],[1,2,3]],[14,16],[6,5,4]); z,x,Bidx=best(Ai,bi,ci)
check("ex6 primal (4,6,0) 54",x[:3]==[4,6,0] and z==54, str(dual_of_basis(Ai,ci,Bidx)))
check("ex6 dual (7/3,4/3)",dual_of_basis(Ai,ci,Bidx)==[F(7,3),F(4,3)])
# Ex7
Ai,bi,ci=slackform([[1,2,1],[3,1,2]],[12,16],[4,3,2]); Bi=inv([[1,2],[3,1]])
check("ex7 Binv",Bi==[[F(-1,5),F(2,5)],[F(3,5),F(-1,5)]]); 
dB=[-v for v in matvec(Bi,[1,2])]; check("ex7 dB (-3/5,-1/5)",dB==[F(-3,5),F(-1,5)])
check("ex7 y=(1,1)",dual_of_basis(Ai,ci,(0,1))==[1,1]); z,x,_=best(Ai,bi,ci); check("ex7 opt 28",z==28 and x[:2]==[4,4])
# Ex8: constraints: budget, staff, xC-xA<=0, xB+xD=1, xE-xB-xC<=0, x<=1 (5)
# build as equality with slacks: handle equality row by no slack. Use general form: rows with slack except eq.
rows=[([4,6,5,3,2],14,'le'),([2,3,2,2,1],7,'le'),([-1,0,1,0,0],0,'le'),([0,1,0,1,0],1,'eq'),([0,-1,-1,0,1],0,'le')]
for k in range(5):
    r=[0]*5; r[k]=1; rows.append((r,1,'le'))
ns=sum(1 for r in rows if r[2]=='le'); A8=[]; s=0
for r,rhs,t in rows:
    row=list(map(F,r))+[F(0)]*ns
    if t=='le': row[5+s]=F(1); s+=1
    A8.append(row)
b8=[F(r[1]) for r in rows]; c8=[F(v) for v in [8,12,11,6,5]]+[F(0)]*ns
z,x,Bidx=best(A8,b8,c8); check("ex8 opt 30 at (1,0,1,1,1)",z==30 and x[:5]==[1,0,1,1,1])
# uniqueness: count optimal BFS
opts=[x for _,x,_ in bases(A8,b8) if x is not None and all(v>=0 for v in x) and sum(ci*xi for ci,xi in zip(c8,x))==30]
check("ex8 unique among BFS",len({tuple(o[:5]) for o in opts})==1)
# Ex9 beta=30
Ai,bi,ci=slackform([[2,1],[1,3],[5,5]],[10,15,30],[5,4]); z,x,_=best(Ai,bi,ci); check("ex9 (4,2,0,5,0) 28",x==[4,2,0,5,0] and z==28)
# Ex10 p=10
Ai,bi,ci=slackform([[2,1,1,1],[1,2,1,2],[1,1,2,2]],[18,18,16],[6,5,3,10]); z,x,Bidx=best(Ai,bi,ci)
check("ex10 p=10 opt 260/3 at (20/3,0,0,14/3,0,2,0)",z==F(260,3) and x==[F(20,3),0,0,F(14,3),0,2,0], str(x))
check("ex10 dual (2/3,0,14/3)",dual_of_basis(Ai,ci,Bidx)==[F(2,3),0,F(14,3)])
Bi=inv([[2,1,0],[1,2,0],[1,1,1]]); check("ex10 Binv a=(0,1,1)",matvec(Bi,[1,2,2])==[0,1,1])
# original p: reduced cost p-5
# Ex11
Ai,bi,ci=slackform([[1,2],[3,1],[0,1]],[20,25,9],[4,7]); z,x,Bidx=best(Ai,bi,ci); check("ex11 base (6,7,0,0,2) 73",x==[6,7,0,0,2] and z==73)
Bi=inv([[1,2,0],[3,1,0],[0,1,1]]); check("ex11 Binv col1 (-1/5,3/5,-3/5)",col(Bi,0)==[F(-1,5),F(3,5),F(-3,5)])
for d,exp in [(2,F(399,5)),(5,F(253,3))]:
    Ai,bi,ci=slackform([[1,2],[3,1],[0,1]],[20+d,25,9],[4,7]); z,x,Bidx=best(Ai,bi,ci); check(f"ex11 delta={d} z={exp}",z==exp,str(x)+" y="+str(dual_of_basis(Ai,ci,Bidx)))
# Ex12
for g,exp,xe in [(0,39,[6,3,0]),(2,F(105,2),[F(15,2),0,0])]:
    Ai,bi,ci=slackform([[1,1,1],[2,1,3]],[9,15],[5+g,3,4]); z,x,Bidx=best(Ai,bi,ci); check(f"ex12 gamma={g}",z==exp and x[:3]==xe, "y="+str(dual_of_basis(Ai,ci,Bidx)))
# Ex15: min eps  -> max -eps ; vars a,b free -> a=a1-a2 etc. 3 vars: (a+,a-,b+,b-,eps), 6 rows <=
rows=[]; ts=[0,1,2]; ys=[1,4,2]
for t,y in zip(ts,ys):
    rows.append(([1,-1,t,-t,-1],y))     # a+bt-eps<=y
    rows.append(([-1,1,-t,t,-1],-y))    # -a-bt-eps<=-y
A15=[list(map(F,r))+[F(int(i==j)) for j in range(6)] for i,(r,_) in enumerate(rows)]; b15=[F(r[1]) for r in rows]; c15=[F(0)]*4+[F(-1)]+[F(0)]*6
z,x,Bidx=best(A15,b15,c15); check("ex15 Emin=5/4, a=9/4, b=1/2",-z==F(5,4) and x[0]-x[1]==F(9,4) and x[2]-x[3]==F(1,2), str(x[:5]))
# Ex17
for d,g,exp in [(0,0,28),(5,0,33),(0,2,36),(5,2,39),(20,-3,48)]:
    Ai,bi,ci=slackform([[1,2],[3,1]],[12+d,16],[4+g,3]); z,x,Bidx=best(Ai,bi,ci); check(f"ex17 ({d},{g}) -> {exp}",z==exp,str(x))
check("ex17 formula",all(28+4*g+d-F(g*d,5)==e for d,g,e in [(0,0,28),(5,0,33),(0,2,36),(5,2,39)]))
# Ex18
for p,exp in [(0,44),(F(13,5),44),(3,50),(5,80),(6,96)]:
    Ai,bi,ci=slackform([[2,1,1],[1,3,1]],[16,18],[4,5,p]); z,x,Bidx=best(Ai,bi,ci); check(f"ex18 p={p} z={exp}",z==exp,str(x[:3]))
Bi=inv([[2,1],[1,3]]); check("ex18 Binv(1,1)=(2/5,1/5)",matvec(Bi,[1,1])==[F(2,5),F(1,5)])
# Ex13 samples
for al in [F(1,2),1]:
    Ai,bi,ci=slackform([[-1,1],[0,1]],[-1,al],[al-1,1]); z,x,_=best(Ai,bi,ci); check(f"ex13 alpha={al} z={al*al+al-1}",z==al*al+al-1,str(x[:2]))
print("ALL OK" if ok else "SOME FAILED")
