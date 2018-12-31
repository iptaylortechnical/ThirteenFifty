import json
import subprocess
import re

'''
  Test: TFTP
  Suggested Icon: server
  Description:
    Performs a TFTP download and records the time required.

  Metrics:
    NAME             UNITS    DECIMALS    DESCRIPTION
    _____________    _____    ________    ___________________________
    download_time    ms       2           Total time for the download
    download_speed   kB/s     1           Rate of the download
    download_size    B        0           Total bytes downloaded

  Test instance template:
    NAME             TYPE                 DESCRIPTION
    _________        ________             _____________________________________
    target           hostname             Hostname or IP address of TFTP server
    filename         string               Name of the file to be downloaded
    port             integer              Optional: Port number to connect on. Default 69.
    window_size      integer              Optional: The desired window size (RFC 7440)
    block_size       integer              Optional: The desired block size (RFC 2348)
    timeout          integer              Optional: receive timeout in secs. Default 1.
'''

def parse_stats(result):
  total = re.search(r'{{total_bytes:(.+)}}', result).group(1)
  kbps = re.search(r'{{kbps:(.+)}}', result).group(1)
  time_total = re.search(r'{{total_time:(.+)}}', result).group(1)

  return total, kbps, time_total

def parse_error(output):
  print 'nope'


def run(test_config):
  config = json.loads(test_config)

  try:
    target = config['target']
    filename = config['filename']
  except:
    # handle missing target / filename
    print 'nope'

  result = ''

  cmd = ['./bin/ntftp']

  if 'port' in config:
    cmd.append('-p')
    cmd.append(config['port'])

  if 'window_size' in config:
    cmd.append('-w')
    cmd.append(config['window_size'])

  if 'block_size' in config:
    cmd.append('-b')
    cmd.append(config['block_size'])

  if 'timeout' in config:
    cmd.append('-t')
    cmd.append(config['timeout'])

  cmd.append(target)
  cmd.append(filename)

  try:
    result = subprocess.check_output(cmd)
  except subprocess.CalledProcessError as e:
    return parse_error(e.output)
  print result
  total, kbps, time = parse_stats(result)

  print total
  print kbps
  print time

  

run('{"target":"10.250.10.189", "filename":"test3", "block_size":"1024", "window_size":"4"}')