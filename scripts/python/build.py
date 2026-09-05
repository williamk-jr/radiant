import subprocess
import os


def main():
    env = os.environ.copy()
    subprocess.run(
        ["cmake", "-S", ".", "-B", "build", "-G", "Ninja", "-DBUILD_SHARED_LIBS=ON"], env=env)
    subprocess.run(["ninja", "-C", "build"], env=env)


if __name__ == "__main__":
    main()
