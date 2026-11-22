"""Keep temporary Git fixtures independent of the invoking commit hook."""

import os
import subprocess


def git_fixture_environment() -> dict[str, str]:
    """Remove Git's repository selectors before entering a disposable checkout.

    A different working directory alone does not override a hook's index or
    repository variables. Ask Git for its own list so fixtures also handle
    selectors added by newer Git versions. Production hooks retain their
    intentional repository and partial-index settings.
    """
    names = subprocess.run(
        ["git", "rev-parse", "--local-env-vars"],
        check=True,
        capture_output=True,
        text=True,
        timeout=15,
    ).stdout.splitlines()
    return {key: value for key, value in os.environ.items() if key not in names}
