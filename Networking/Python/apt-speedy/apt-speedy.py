import requests
import re
import subprocess
from concurrent.futures import ThreadPoolExecutor

# Fetch the HTML list of Ubuntu mirrors and extract URLs of up-to-date mirrors
response = requests.get('https://launchpad.net/ubuntu/+archivemirrors')
mirrors = re.findall(r'(f|ht)tp://[^\"]*', response.text)
#filtered_mirrors = [mirror for mirror in mirrors if 'statusUP' in response.text]
filtered_mirrors = [mirror for mirror in mirrors if 'statusUP' in response.text and 'United States' in response.text]

# Function to test mirror speed
def test_mirror_speed(mirror):
    try:
        result = subprocess.run(
            ['curl', '--max-time', '2', '-r', '0-102400', '-s', '-w', '%{speed_download}', '-o', '/dev/null', f'{mirror}/ls-lR.gz'],
            capture_output=True, text=True
        )
        speed_bps = float(result.stdout.strip())
        speed_kbps = speed_bps / 1024
        return mirror, speed_kbps
    except Exception as e:
        return mirror, 0

# Test each mirror with a 2-second timeout using ThreadPoolExecutor
print("Testing mirrors for speed...")
with ThreadPoolExecutor(max_workers=10) as executor:
    results = list(executor.map(test_mirror_speed, filtered_mirrors))

# Filter out mirrors with speeds of 0.0 KB/s
valid_results = [result for result in results if result[1] > 0]

# Sort mirrors by speed and get the top 5
sorted_results = sorted(valid_results, key=lambda x: x[1], reverse=True)[:5]

# Print the top 5 fastest mirrors
print("Top 5 fastest mirrors:")
for mirror, speed in sorted_results:
    print(f"{mirror} --> {speed:.2f} KB/s")