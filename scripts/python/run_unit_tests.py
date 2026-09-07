import subprocess
import os

import globals


def main():
    env = os.environ.copy()
    subprocess.run(globals.TESTS_PATH, env=env)


if __name__ == "__main__":
    main()
