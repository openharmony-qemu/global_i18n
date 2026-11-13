#!/usr/bin/env python3
"""Mirror configured official refs without changing a QEMU development branch."""
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile


def git(directory, *args, capture=False):
    return subprocess.run(['git', '-C', str(directory), *args], check=True,
                          text=True, stdout=subprocess.PIPE if capture else None)


def main():
    config = json.loads(Path('UPSTREAM.json').read_text())
    upstream = config['url']
    if not upstream.startswith(('https://gitcode.com/', 'https://github.com/', 'https://gitlab.com/')):
        raise ValueError('unexpected official upstream URL')
    revision = config['baseline_revision']
    if not re.fullmatch(r'[0-9a-f]{40}', revision):
        raise ValueError('exact upstream baseline commit required')
    repository = os.environ['GITHUB_REPOSITORY']
    if not re.fullmatch(r'openharmony-qemu/[A-Za-z0-9_.-]+', repository):
        raise ValueError('sync is limited to this QEMU organization')
    with tempfile.TemporaryDirectory(prefix='qemu-upstream-') as temporary:
        root = Path(temporary)
        git(root, 'init', '--bare')
        git(root, 'remote', 'add', 'upstream', upstream)
        # The token is passed in the environment and never printed. Git's
        # configured credential helper returns it only for this GitHub host.
        git(root, 'config', 'credential.https://github.com.helper',
            '!f() { echo username=x-access-token; echo "password=$GH_TOKEN"; }; f')
        git(root, 'remote', 'add', 'origin', 'https://github.com/' + repository + '.git')
        refspecs = []
        for branch in config['branches']:
            git(root, 'check-ref-format', 'refs/heads/' + branch)
            target = 'refs/heads/upstream/' + branch
            # Keep full ancestry. A shallow source cannot seed a new mirror.
            git(root, 'fetch', '--no-tags', 'upstream',
                '+refs/heads/' + branch + ':' + target)
            refspecs.append('+' + target + ':' + target)
        present = subprocess.run(['git', '-C', str(root), 'cat-file', '-e', revision + '^{commit}'])
        if present.returncode:
            git(root, 'fetch', '--no-tags', 'upstream', revision)
        baseline = 'refs/heads/upstream/baseline-7.0'
        git(root, 'update-ref', baseline, revision)
        # All destination refs are explicitly inside upstream/. No mirror push,
        # ref deletion or force update of OpenHarmony-*-Release-qemu is allowed.
        git(root, 'push', '--atomic', 'origin', *refspecs, '+' + baseline + ':' + baseline)
        print('Mirrored official upstream; QEMU development refs were preserved.')


if __name__ == '__main__':
    main()
