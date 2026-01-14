// GetUserHome should cache $HOME path, since it doesn't change, UNLESS:
// the user changed (priv escalation via account creation) OR idk maybe someone renames their acc?
// either way it's impropable, and caching will be a lot less noisy
