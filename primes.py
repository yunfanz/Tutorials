import numpy as np, pylab as plt
def is_prime(n):
  if n == 2 or n == 3: return True
  if n < 2 or n%2 == 0: return False
  if n < 9: return True
  if n%3 == 0: return False
  r = int(n**0.5)
  f = 5
  while f <= r:
    #print '\t',f
    if n%f == 0: return False
    if n%(f+2) == 0: return False
    f +=6
  return True    

n = 2
p1 = 2; p2 = 3
twins = []
while n < 1000000000:
	#if n % max(n/10,1) == 0: print n
	if is_prime(n): 
		p2 = n
		if p2-p1 == 2:
			twins.append(p2)
			#print p1, p2
		p1 = p2
	n += 1
plt.figure()
plt.hist(twins, bins='auto')
#plt.show()
plt.savefig('primes.png')