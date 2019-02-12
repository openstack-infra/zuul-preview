#!/usr/bin/python3

import json
import os
import argparse
import uuid

parser = argparse.ArgumentParser()
parser.add_argument('builds', default='100')
args = parser.parse_args()

if os.path.exists('./api/build'):
    for build in os.listdir('./api/build'):
        print('http://localhost:8000 artifact.%s.tenant' % build)
else:
    os.makedirs('./api/build')

    json_out = json.dumps({'log_url': 'https://something.test/builds/prefixes/abcdefghijklmnop09123456789/feedbeef/'})

    for i in range(0, int(args.builds)):
        build = uuid.uuid4().hex
        bp = os.path.join('./api/build', build)
        with open(bp, 'w') as js:
            js.write(json_out)
        print('https://localhost:8000 artifact.%s.tenant' % build)
