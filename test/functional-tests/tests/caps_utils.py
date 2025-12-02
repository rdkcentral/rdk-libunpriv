import os
import shutil
import subprocess

# This is where rdk-libunpriv expects its config in production
CAPS_DIR = "/etc/security/caps"
CAPS_FILE = os.path.join(CAPS_DIR, "process-capabilities.json")


def run_and_capture(binary, src_config=None):
    """
    Run the cap_test_process helper with an optional test JSON.

    - If src_config is provided: it is copied to
      /etc/security/caps/process-capabilities.json for this run.
    - If src_config is None: we simulate "no config present" by removing
      process-capabilities.json (if it exists).
    - Any original process-capabilities.json is backed up and restored
      after the run.
    """

    os.makedirs(CAPS_DIR, exist_ok=True)

    backup_path = CAPS_FILE + ".bak"
    had_original = os.path.exists(CAPS_FILE)

    try:
        # Backup existing real config (if any)
        if had_original:
            shutil.copy(CAPS_FILE, backup_path)

        # Install test config or simulate missing config
        if src_config is not None:
            shutil.copy(src_config, CAPS_FILE)
        else:
            if os.path.exists(CAPS_FILE):
                os.remove(CAPS_FILE)

        # Run the real helper binary that calls drop_root_caps()
        proc = subprocess.run(
            [binary],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        return proc

    finally:
        # Restore original config if it existed
        if os.path.exists(backup_path):
            shutil.move(backup_path, CAPS_FILE)
        else:
            # No original config: don't leave test file lying around
            if os.path.exists(CAPS_FILE):
                os.remove(CAPS_FILE)

