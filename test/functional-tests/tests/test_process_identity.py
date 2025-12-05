# test_process_identity.py
from caps_utils import run_and_capture

def test_process_starts_as_root(cap_binary):
    """
    Ensure helper binary starts correctly and logs UID/process context.
    This validates L2 harness correctness.
    """
    proc = run_and_capture(cap_binary, None)

    # UID log must be present
    assert "uid=0" in proc.stdout.lower()

    # Binary executed (not loader failure)
    assert proc.returncode != 127

