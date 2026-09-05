import subprocess
import os

import globals


def main():
    env = os.environ.copy()
    if subprocess.run(globals.TESTS_PATH, env=env):
        subprocess.run(globals.DEMO_PATH, env=env)


if __name__ == "__main__":
    main()
