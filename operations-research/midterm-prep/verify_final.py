from fractions import Fraction as F
import itertools
def inv(M):
    n=len(M); A=[[F(x) for x in r]+[F(int(i==j)) for j in range(n)] for i,r in enumerate(M)]
    for c in range(n):
        p=next(r for r in range(c,n) if A[r][c]!=0); A[c],A[p]=A[p],A[c]
        pv=A[c][c]; A[c]=[x/pv for x in A[c]]
        for r in range(n):
            if r!=c and A[r][c]!=0:
                f=A[r][c]; A[r]=[a-f*b for a,b in zip(A[r],A[c])]
    return [r[n:] for r in A]
def mv(M,v): return [sum(a*b for a,b in zip(r,v)) for r in M]
def tab(A,b,c,Bi):
    m=len(A); n=len(A[0]); B=[[A[i][j] for j in Bi] for i in range(m)]; Binv=inv(B)
    xB=mv(Binv,b); cB=[c[j] for j in Bi]; y=mv(list(map(list,zip(*Binv))),cB)
    N=[j for j in range(n) if j not in Bi]
    r={j:c[j]-sum(y[i]*A[i][j] for i in range(m)) for j in N}
    Q={j:[-v for v in mv(Binv,[A[i][j] for i in range(m)])] for j in N}
    z=sum(a*b for a,b in zip(cB,xB)); return Binv,xB,y,r,Q,z,N
def show(A,b,c,Bi,names,title):
    Binv,xB,y,r,Q,z,N=tab(A,b,c,Bi)
    print('==',title,'basis',[names[j] for j in Bi])
    for k,j in enumerate(Bi): print('  ',names[j],'=',xB[k],' '.join(f'{Q[q][k]:+}{names[q]}' for q in N))
    print('   z =',z,' '.join(f'{r[q]:+}{names[q]}' for q in N)); print('   y',[str(v) for v in y]); print('   Binv',[[str(v) for v in row] for row in Binv])
    return Binv,xB,y,r,Q,z,N
def best(A,b,c,names):
    out=None
    for Bi in itertools.combinations(range(len(A[0])),len(A)):
        try: Binv,xB,y,r,Q,z,N=tab(A,b,c,list(Bi))
        except StopIteration: continue
        if all(v>=0 for v in xB) and all(v<=0 for v in r.values()): out=list(Bi); print('   optimal basis',[names[j] for j in Bi],'x=',[str(v) for v in xB],'z=',z,'y=',[str(v) for v in y])
    return out
A=[[2,3,1,1,0,0],[4,1,2,0,1,0],[3,4,2,0,0,1]]; b=[5,11,8]; c=[5,4,3,0,0,0]
nm=['x1','x2','x3','s1','s2','s3']
show(A,b,c,[3,4,5],nm,'start')
show(A,b,c,[0,4,5],nm,'after pivot 1 (x1 in, s1 out)')
show(A,b,c,[0,2,4],nm,'optimal')
print('--- all feasible bases'); 
for Bi in itertools.combinations(range(6),3):
    try: Binv,xB,y,r,Q,z,N=tab(A,b,c,list(Bi))
    except StopIteration: continue
    if all(v>=0 for v in xB): print('  ',[nm[j] for j in Bi],[str(v) for v in xB],'z=',z)
print('--- resource 1 = 6 (delta=1)'); best(A,[6,11,8],c,nm)
print('--- resource 3 = 10 (delta=2 endpoint) and 11'); best(A,[5,11,10],c,nm); best(A,[5,11,11],c,nm)
print('--- c1=6 endpoint'); best(A,b,[6,4,3,0,0,0],nm)
print('--- c1=4.5 endpoint'); best(A,b,[F(9,2),4,3,0,0,0],nm)
print('--- c2=7 break-even'); best(A,b,[5,7,3,0,0,0],nm)
print('--- new product a=(1,1,1) gamma=3')
A2=[r+[v] for r,v in zip(A,[1,1,1])]; nm2=nm+['u']; c2=c+[3]
show(A2,b,c2,[0,2,4],nm2,'old basis with u column'); Bo=best(A2,b,c2,nm2); show(A2,b,c2,Bo,nm2,'new optimum')
print('--- degenerate b3=15/2'); show(A,[5,11,F(15,2)],c,[0,2,4],nm,'b3=7.5 old basis'); best(A,[5,11,F(15,2)],c,nm)
print('--- bounds: max x1+x2+x3 over P'); best(A,b,[1,1,1,0,0,0],nm)
print('--- sigma: profits (4.5,4,3.5)'); best(A,b,[F(9,2),4,F(7,2),0,0,0],nm)
print('--- min and max of v over the box: corners')
import itertools as it
vals=[]
for e in it.product([F(-1,2),F(1,2)],repeat=3):
    cc=[5+e[0],4+e[1],3+e[2],0,0,0]
    bestz=max(tab(A,b,cc,list(Bi))[5] for Bi in [(0,2,4),(0,4,5),(1,2,4),(1,4,5),(2,3,4),(3,4,5)])
    vals.append((bestz,e))
print('   min',min(vals),'max',max(vals))
