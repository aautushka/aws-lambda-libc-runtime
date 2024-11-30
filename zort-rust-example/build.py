import os
import subprocess

TAG = 'rust-example'

script_dir = os.path.dirname(os.path.abspath(__file__))
dockerfile_path = os.path.join(script_dir, 'Dockerfile')
context_path = os.path.dirname(script_dir)

build_command = ['docker', 'build', '-t', TAG, '-f', dockerfile_path, context_path]
build_process = subprocess.run(build_command, check=True)

run_command = ['docker', 'run', '-it', 'rust-example']
subprocess.run(run_command, check=True)
