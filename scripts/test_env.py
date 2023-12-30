import amazons_env
env = amazons_env.env()
env.reset()

cnt = 0
for agent in env.agent_iter():
    observation, reward, termination, truncation, info = env.last()

    if termination or truncation:
        action = None
    else:
        # this is where you would insert your policy
        action = env.unwrapped.random_action()
        print(action)

    env.step(action)
    cnt += 1

print(cnt)
print(env.unwrapped.string_repr())
env.close()
