from pettingzoo import AECEnv
from pettingzoo.utils import agent_selector
from _amazons import Amazons
import numpy as np
import gymnasium.spaces as spaces
from typing import Dict


class AmazonsEnv(AECEnv):
    metadata = {
        "name": "amazons_env_v0",
    }

    def __init__(self, board_size: int = 10):
        assert 4 <= board_size <= 10
        self.board_size = board_size
        self._env = Amazons(board_size)
        self.agents = ["black_0", "white_0"]
        self.possible_agents = self.agents[:]

        self.observation_spaces = self._convert_to_dict(
            [
                spaces.Dict(
                    {
                        "observation": spaces.Box(
                            low=0, high=1, shape=(board_size, board_size, 3), dtype=bool
                        ),
                    }
                )
                for _ in range(self.num_agents)
            ]
        )

        self.action_spaces = self._convert_to_dict(
            [spaces.Box(
                low=0,
                high=1,
                shape=(board_size * board_size, 3),
                dtype=np.int32,
            ) for _ in range(self.num_agents)]
        )

        self._agent_selector = agent_selector(self.agents)

    def observation_space(self, agent: str):
        return self.observation_spaces[agent]

    def action_space(self, agent: str):
        return self.action_spaces[agent]

    def _int_to_name(self, ind: int) -> str:
        return self.possible_agents[ind]

    def _name_to_int(self, name: str) -> int:
        return self.possible_agents.index(name)

    def observe(self, agent: str) -> Dict[str, np.ndarray]:

        board_view = self._env.get_board_view()
        board = np.asarray(board_view)
        agent_id = self._name_to_int(agent)
        if agent_id == 0:
            player = int(Amazons.Cell.Black)
            opponent = int(Amazons.Cell.White)
        else:
            player = int(Amazons.Cell.White)
            opponent = int(Amazons.Cell.Black)

        layer1 = board == player
        layer2 = board == opponent
        layer3 = board == int(Amazons.Cell.Arrow)

        obs = np.stack([layer1, layer2, layer3], axis=-1)
        return {
            'observation': obs
        }

    def reset(self, seed=None, options=None):
        assert seed is None
        assert options is None

        self._env.reset()

        self.agents = self.possible_agents[:]
        self._agent_selector.reinit(self.agents)
        self.agent_selection = self._agent_selector.reset()
        self._cumulative_rewards = self._convert_to_dict(np.array([0.0, 0.0]))
        self.rewards = self._convert_to_dict(np.array([0.0, 0.0]))
        self.terminations = self._convert_to_dict(
            [False for _ in range(self.num_agents)]
        )
        self.truncations = self._convert_to_dict(
            [False for _ in range(self.num_agents)]
        )
        self.infos = self._convert_to_dict(
            [{} for _ in range(self.num_agents)])

        self._last_obs = self.observe(self.agents[0])

    def step(self, action: np.ndarray):
        if (
            self.terminations[self.agent_selection]
            or self.truncations[self.agent_selection]
        ):
            return self._was_dead_step(action)

        valid = self._env.step(action)
        if not valid:
            return
        next_player = self._agent_selector.next()

    def render(self):
        return super().render()

    def close(self):
        return super().close()

    def _convert_to_dict(self, list_of_list):
        return dict(zip(self.possible_agents, list_of_list))
