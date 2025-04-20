import subprocess
import json
import os
import re
from time import time
import logging

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')
logger = logging.getLogger(__name__)


BUILD_DIR = "build"
executables = [
    "prime1", "prime2", "prime3", "prime3a",
    "prime4", "prime4a", "prime5"
]

ranges = [
    (2, 10**8),
    (10**8 // 2, 10**8),
    (2, 10**8 // 2),
]

omp_targets = {"prime2", "prime4", "prime4a", "prime5"}
schedules = ["static", "dynamic", "guided", "auto"]


results_schedules = []
results_static = []


logger.info("🔍 Building")
subprocess.run(["make", BUILD_DIR], check=True)
logger.info("✅ Building complete")


def parse_output(output):
    prime_count = None
    time_taken = None
    performance = None

    for line in output.splitlines():
        if "Found" in line:
            match = re.search(r"Found (\d+) primes", line)
            if match:
                prime_count = int(match.group(1))
        elif "Time taken" in line:
            match = re.search(r"Time taken.*?([\d.]+) seconds", line)
            if match:
                time_taken = float(match.group(1))
        elif "Performance" in line:
            match = re.search(r"Performance: ([\d.]+) Mnums/sec", line)
            if match:
                performance = float(match.group(1))

    return prime_count, time_taken, performance

logger.info("🚀 Starting performance tests...")


for i in [3, 8]:
    for exe in executables:
        path = os.path.join(BUILD_DIR, exe)
        if not os.path.exists(path):
            logger.warning(f"Executable not found: {path}. Skipping.")
            continue
        
        if i == 8 and exe in {"prime1", "prime3", "prime3a"}:
            logger.warning(f"❌ Skipping {exe} for i=8 as it is not applicable.")
            continue

        logger.info(f"🔬 Testing executable: {exe}")
        for r in ranges:
            min_val, max_val = r
            range_str = f"[{min_val}, {max_val}]"
            logger.info(f"  🔢 Testing range: {range_str}")

            if exe in omp_targets:
                for sched in schedules if i == 8 else ["static"]:
                    logger.info(f"    ⚙️ Using OMP_SCHEDULE: {sched}")
                    env = os.environ.copy()
                    env["OMP_SCHEDULE"] = sched
                    cmd = [f"./{path}", str(min_val), str(max_val)]
                    logger.debug(f"    🔧 Running command: {' '.join(cmd)} with env OMP_SCHEDULE={sched}")

                    start_time = time()
                    try:
                        output = subprocess.check_output(cmd, env=env, stderr=subprocess.STDOUT, text=True)
                        run_time = time() - start_time
                        logger.debug(f"    ✅ Command finished successfully in {run_time:.2f}s.")
                    except subprocess.CalledProcessError as e:
                        run_time = time() - start_time
                        output = e.output
                        logger.error(f"    ❌ Command failed after {run_time:.2f}s! Error:\n{output}")
                    except FileNotFoundError:
                        logger.error(f"    ❌ Command failed: Executable '{path}' not found.")
                        output = "" # Ensure output is defined for parsing
                        run_time = time() - start_time


                    prime_count, time_taken, performance = parse_output(output)
                    logger.info(f"      📊 Result: Primes={prime_count}, Time={time_taken}s, Perf={performance} Mnums/sec")

                    p_to_save = results_static if i == 3 else results_schedules
                    p_to_save.append({
                        "executable": exe,
                        "range": [min_val, max_val],
                        "omp_schedule": sched,
                        "prime_count": prime_count,
                        "time_reported": time_taken,
                        "performance_mnums_sec": performance,
                        "wall_clock_time": run_time # Add wall clock time
                    })
            else:
                if i == 8:
                    continue
                logger.info(f"    ⚙️ No OMP schedule applicable.")
                cmd = [f"./{path}", str(min_val), str(max_val)]
                logger.debug(f"    🔧 Running command: {' '.join(cmd)}")

                start_time = time()
                try:
                    output = subprocess.check_output(cmd, stderr=subprocess.STDOUT, text=True)
                    run_time = time() - start_time
                    logger.debug(f"    ✅ Command finished successfully in {run_time:.2f}s.")
                except subprocess.CalledProcessError as e:
                    run_time = time() - start_time
                    output = e.output
                    logger.error(f"    ❌ Command failed after {run_time:.2f}s! Error:\n{output}")
                except FileNotFoundError:
                    logger.error(f"    ❌ Command failed: Executable '{path}' not found.")
                    output = "" # Ensure output is defined for parsing
                    run_time = time() - start_time


                prime_count, time_taken, performance = parse_output(output)
                logger.info(f"      📊 Result: Primes={prime_count}, Time={time_taken}s, Perf={performance} Mnums/sec")


                p_to_save = results_static if i == 3 else results_schedules
                p_to_save.append({
                    "executable": exe,
                    "range": [min_val, max_val],
                    "omp_schedule": None,
                    "prime_count": prime_count,
                    "time_reported": time_taken,
                    "performance_mnums_sec": performance,
                    "wall_clock_time": run_time # Add wall clock time
                })
                
    
    p_to_save = results_static if i == 3 else results_schedules

    results_file = "static_results.json" if i == 3 else "schedules_results.json"
    logger.info(f"💾 Saving results to {results_file}...")
    with open(results_file, "w") as f:
        json.dump(p_to_save, f, indent=4)
        
    results_schedules = []
    results_static = []

logger.info(f"✅ Tests complete. Results saved to {results_file}")