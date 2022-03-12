import para
import fns
from spins import Spins
from init import init
S = Spins()
# print(S.A)
init(S)
print(S.A)
print(S.energy())
fns.advance(S)
fns.display()