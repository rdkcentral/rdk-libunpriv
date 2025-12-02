from caps_utils import run_and_capture

def test_invalid_caps_fail_hard(cap_binary, assets_dir):
    cfg = os.path.join(assets_dir, "caps_invalid.json")
    proc = run_and_capture(cap_binary, cfg)

    assert proc.returncode != 0
    assert "failed" in proc.stderr.lower()
