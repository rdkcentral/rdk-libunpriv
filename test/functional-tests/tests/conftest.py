import os
import pytest

TEST_DIR = os.path.dirname(__file__)
CAPS_DIR = os.path.join(TEST_DIR, "assets")
BIN_PATH = os.path.join(TEST_DIR, "cap_test_process")

@pytest.fixture(scope="session")
def cap_binary():
    assert os.path.exists(BIN_PATH), "cap_test_process binary missing"
    return BIN_PATH

@pytest.fixture
def assets_dir():
    return CAPS_DIR
