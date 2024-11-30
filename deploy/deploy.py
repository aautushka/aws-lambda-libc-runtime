import os
import subprocess
import sys

script_dir = os.path.dirname(os.path.abspath(__file__))
dockerfile_path = os.path.join(script_dir, 'Dockerfile')
context_path = os.path.join(script_dir, '..')

build_command = ['docker', 'build', '-t', 'deploy', '-f', dockerfile_path, context_path]
build_process = subprocess.run(build_command)

if build_process.returncode != 0:
    print("Docker build failed", file=sys.stderr)
    sys.exit(1)

run_command = ['docker', 'run', '-v', f"{os.path.expanduser('~')}/.aws:/root/.aws", '-it', 'deploy']
subprocess.run(run_command)