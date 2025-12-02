from caps_utils import run_and_capture

def test_drop_root_missing_config(cap_binary):
    """
    Behavior when no capability configuration is present.

    Expected:
    - Library should fail gracefully
    - No crash
    - Non-zero return code
    """
    proc = run_and_capture(cap_binary, None)

    assert proc.returncode != 0
