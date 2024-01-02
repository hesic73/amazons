# Amazons
The **[Game of the Amazons](https://en.wikipedia.org/wiki/Game_of_the_Amazons)** is a two-player abstract strategy game invented in 1988 by Walter Zamkauskas of Argentina. The game is played by moving pieces and blocking the opponents from squares, and the last player able to move is the winner.


## Roadmap

- [x] Implement the Amazons game in C++ and export to Python using  [pybind11](https://github.com/pybind/pybind11).
- [x] Creating a custom [Pettingzoo](https://github.com/Farama-Foundation/PettingZoo) environment.
- [ ] Design a simple GUI.
- [ ] ~~Train AI agents to play Amazons. (This may not be achievable.)~~

## amazons_env

`amazons_env.raw_env` inherits from `pettingzoo.AECEnv`. As action masking is not implemented, it includes a `random_action` method to generate a random (valid) action. Note that  this method relies on brute force and can be computationally expensive.

```python
import amazons_env
env = amazons_env.env()
env.reset()

for agent in env.agent_iter():
    observation, reward, termination, truncation, info = env.last()

    if termination or truncation:
        action = None
    else:
        action = env.unwrapped.random_action()

    env.step(action)

env.close()
```

-  `agent` can take on values of either `"black_0"` or `"white_0"`.
- `observation` is structured as `{'observation': np.ndarray}`. The array is Boolean and has a shape of [N, N, 3], encoding the player's stones, the opponent's stones, and the arrows.
- `action` is expected to be a numpy array with a shape of [6,], representing $(x_0, y_0, x_1, y_1, x_2, y_2)$. Each coordinate should fall within the range [0, N).

For the moment,  `amazons_env.raw_env` has the following limitations:

- Lack of support for any `render_mode`.
- Absence of an action-masking mechanism.

## Requirements

- CMake

## Reference

- https://en.wikipedia.org/wiki/Game_of_the_Amazons

- https://ieeexplore.ieee.org/document/9534274
