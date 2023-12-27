from amazons_env import AmazonsEnv
import numpy as np
env = AmazonsEnv()
obs: np.ndarray = env.observe("white_0")['observation']
print(obs.shape)
print(obs[..., 0].astype(np.int32))
print(obs[..., 1].astype(np.int32))
print(obs[..., 2].astype(np.int32))
