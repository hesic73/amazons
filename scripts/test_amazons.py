from _amazons import Amazons
import numpy as np

a = Amazons(10)
view = a.get_board_view()
print()
print(id(view))
print(np.asarray(view).ctypes.data)
print(np.asarray(view).ctypes.data)

action = np.array([0, 3, 0, 4, 0, 3], dtype=np.uint32)
assert a.step(action)
# print(a.string_repr())
# print(a.done)
